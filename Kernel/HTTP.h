#pragma once

#include "Util.h"
#include "PondOS.h"

// Implemented from RFC 9110 and RFC 9112

const char* HTTP_ERROR_OK = "200";
const char* HTTP_ERROR_BAD_REQUEST = "400";

enum HTTPRequestMethod {
	HTTP_METHOD_GET,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_CONNECT,
	HTTP_METHOD_OPTIONS,
	HTTP_METHOD_TRACE
};

#ifdef _DEBUG
#define HTTP_VERIFY_RETURN(cond, errCode, ret) if(!(cond)) { errored = true; errorCode = errCode; __debugbreak(); return ret; }
#else
#define HTTP_VERIFY_RETURN(cond, errCode, ret) if(!(cond)) { errored = true; errorCode = errCode; return ret; }
#endif
struct HTTPReader {
	const char* src;
	u32 length;
	u32 pos;
	b32 errored;
	const char* errorCode;
	b32 headerDone;

	void init(const char* source, u32 sourceLength) {
		src = source;
		length = sourceLength;
		pos = 0;
		errored = false;
		errorCode = HTTP_ERROR_OK;
		headerDone = false;
	}

	void set_read_data(const char* source, u32 sourceLength) {
		src = source;
		length = sourceLength;
		pos = 0;
	}

	b32 is_crlf(u32 pos) {
		if (pos >= (length - 1)) {
			return false;
		}
		return src[pos] == '\r' && src[pos + 1] == '\n';
	}

	b32 consume_crlf() {
		if (pos < (length - 1) && src[pos] == '\r' && src[pos + 1] == '\n') {
			pos += 2;
			return true;
		} else {
			return false;
		}
	}

	void skip_whitespace(u32 end) {
		while (pos < end && is_whitespace(src[pos])) {
			pos++;
		}
	}

	u32 skip_whitespace_reverse(i32 endPos) {
		while (endPos >= 0 && is_whitespace(src[endPos])) {
			endPos--;
		}
		return endPos;
	}

	b32 get_line_length(u32* lengthOut) {
		u32 lineLength = 0;
		while (!is_crlf(pos + lineLength) && i32(pos + lineLength) < (i32(length) - 1)) {
			lineLength++;
		}
		if (!is_crlf(pos + lineLength)) {
			return false;
		}
		if (lengthOut) {
			*lengthOut = lineLength;
		}
		return true;
	}

	b32 src_begin_match(const char* cmpNullTerminated) {
		u32 cmpPos = 0;
		while (cmpPos + pos < length && cmpNullTerminated[cmpPos] != '\0' && src[pos + cmpPos] == cmpNullTerminated[cmpPos]) {
			cmpPos++;
		}
		if (cmpNullTerminated[cmpPos] == '\0') {
			pos += cmpPos;
			return true;
		} else {
			return false;
		}
	}

	b32 parse_verify_http_version() {
		// Accept versions in the 1.x range
		if (!src_begin_match("HTTP/1.")) {
			return false;
		}
		if (pos >= length || !is_digit(src[pos])) {
			return false;
		}
		pos++;
		return true;
	}

	b32 read_request_header(HTTPRequestMethod* requestMethod, Range* requestTarget) {
		u32 lineEnd = 0;
		if (!get_line_length(&lineEnd)) {
			return false;
		}
		lineEnd += pos;

		if (src_begin_match("HEAD ")) {
			*requestMethod = HTTP_METHOD_HEAD;
		} else if (src_begin_match("POST ")) {
			*requestMethod = HTTP_METHOD_POST;
		} else if (src_begin_match("GET ")) {
			*requestMethod = HTTP_METHOD_GET;
		} else if (src_begin_match("CONNECT ")) {
			*requestMethod = HTTP_METHOD_CONNECT;
		} else if (src_begin_match("PUT ")) {
			*requestMethod = HTTP_METHOD_PUT;
		} else if (src_begin_match("DELETE ")) {
			*requestMethod = HTTP_METHOD_DELETE;
		} else if (src_begin_match("OPTIONS ")) {
			*requestMethod = HTTP_METHOD_OPTIONS;
		} else if (src_begin_match("TRACE ")) {
			*requestMethod = HTTP_METHOD_TRACE;
		} else {
			errorCode = HTTP_ERROR_BAD_REQUEST;
			errored = true;
			return false;
		}

		HTTP_VERIFY_RETURN(!is_whitespace(src[pos]), HTTP_ERROR_BAD_REQUEST, false);
		(*requestTarget).start = pos;
		while (pos < lineEnd && src[pos] != ' ') {
			pos++;
		}
		(*requestTarget).end = pos;

		HTTP_VERIFY_RETURN(src[pos] == ' ', HTTP_ERROR_BAD_REQUEST, false);
		pos++;

		if (!parse_verify_http_version()) {
			errorCode = HTTP_ERROR_BAD_REQUEST;
			errored = true;
			return false;
		}
		return consume_crlf();
	}

	b32 read_response_header(Range* responseCode, Range* reasonPhrase) {
		u32 lineEnd = 0;
		if (!get_line_length(&lineEnd)) {
			return false;
		}
		lineEnd += pos;
		if (!parse_verify_http_version()) {
			errorCode = HTTP_ERROR_BAD_REQUEST;
			errored = true;
			return false;
		}
		HTTP_VERIFY_RETURN(src[pos] == ' ', HTTP_ERROR_BAD_REQUEST, false);
		pos++;
		HTTP_VERIFY_RETURN(pos + 3 < lineEnd, HTTP_ERROR_BAD_REQUEST, false);
		HTTP_VERIFY_RETURN(is_digit(src[pos]) && is_digit(src[pos + 1]) && is_digit(src[pos + 2]), HTTP_ERROR_BAD_REQUEST, false);
		*responseCode = Range{ pos, pos + 3 };
		pos += 3;

		HTTP_VERIFY_RETURN(src[pos] == ' ', HTTP_ERROR_BAD_REQUEST, false);
		pos++;
		*reasonPhrase = Range{ pos, lineEnd };
		pos = lineEnd;
		return consume_crlf();
	}

	b32 check_response_is(const char* httpcode) {
		Range responseCode;
		Range reason;
		if (read_response_header(&responseCode, &reason)) {
			// x can match anything, so I can do something like check_response_is("2xx") to see if it's a general ok response
			if ((httpcode[0] == 'x' || src[responseCode.start] == httpcode[0]) &&
				(httpcode[1] == 'x' || src[responseCode.start] == httpcode[1]) &&
				(httpcode[2] == 'x' || src[responseCode.start] == httpcode[2])) {
				return true;
			}
		}
		return false;
	}

	b32 read_header_field(Range* fieldName, Range* fieldValue) {
		if (consume_crlf()) {
			headerDone = true;
			return false;
		}
		u32 lineEnd = 0;
		if (!get_line_length(&lineEnd)) {
			return false;
		}
		lineEnd += pos;

		(*fieldName).start = pos;
		HTTP_VERIFY_RETURN(src[pos] != ':', HTTP_ERROR_BAD_REQUEST, false);
		while (pos < lineEnd && src[pos] != ':') {
			pos++;
		}
		HTTP_VERIFY_RETURN(src[pos] == ':', HTTP_ERROR_BAD_REQUEST, false);
		HTTP_VERIFY_RETURN(!is_whitespace(src[pos - 1]), HTTP_ERROR_BAD_REQUEST, false);
		(*fieldName).end = pos;
		pos++;

		skip_whitespace(lineEnd);
		(*fieldValue).start = pos;
		(*fieldValue).end = max(skip_whitespace_reverse(lineEnd - 1) + 1, pos);

		pos = lineEnd + 2;
		return true;
	}
};
#undef HTTP_VERIFY_RETURN

struct HTTPField {
	const char* name;
	Range value;

	b32 has_value() {
		return value != Range{ 0, 0 };
	}

	void reset() {
		value.reset();
	}
};

#define HTTP_READ_FAILED -1
#define HTTP_READ_NO_CONTENT_LENGTH -2

/*
i32 http_read_full_content(TLSClient& connection, char** responseData, HTTPField* fields = nullptr, u32 numFields = 0) {
	HTTPReader http;
	http.init(connection.receiveBuffer, connection.receiveBufferPos);
	u32 lastReceiveSize = 0;
	i32 contentLength = HTTP_READ_NO_CONTENT_LENGTH;
	while (true) {
		connection.receive_more_than(lastReceiveSize);
		lastReceiveSize = http.length = connection.receiveBufferPos;
		if (http.pos == 0 && http.get_line_length(nullptr) && !http.check_response_is("xxx")) {
			return HTTP_READ_FAILED;
		}
		if (!http.headerDone) {
			Range fieldName;
			Range fieldValue;
			while (!http.errored && http.read_header_field(&fieldName, &fieldValue)) {
				if (fieldName.data_range(http.src) == "Content-Length") {
					contentLength = fieldValue.data_range(http.src).parse_uint(I32_MAX);
				} else if (fieldName.data_range(http.src) == "Transfer-Encoding") {
					//TODO deal with chunked format
					return HTTP_READ_FAILED;
				}
				for (u32 i = 0; i < numFields; i++) {
					if (fieldName.data_range(http.src) == fields[i].name) {
						if (fields[i].has_value()) {
							// Duplicate field, illegal (we're not dealing with any comma separated lists)
							return HTTP_READ_FAILED;
						}
						fields[i].value = fieldValue;
					}
				}
			}
		}
		if (http.headerDone && (connection.receiveBufferCap >= contentLength || contentLength == HTTP_READ_NO_CONTENT_LENGTH)) {
			*responseData = connection.receiveBuffer + http.pos;
			break;
		}
	}
	return contentLength;
}
*/

struct HTTPResource {
	const char* dataDirectory;
	const void* dataToServe;
	u32 dataSize;
};

struct HTTPConnection {
	static constexpr u32 receiveBufferCap = 1024;
	u8 receiveBuffer[receiveBufferCap];
	u32 receivePos;
	HTTPReader httpReader;
	HTTPRequestMethod requestMethod;
	Range requestTarget;
	u32 tcpConnectionIdx;
};

// Not a very good HTTP server implementation, but I'm only using it for certificate renewal, so I don't think it needs to be good (unlike the main server)
struct HTTPServer {
	static constexpr u32 resourceCap = 16;
	HTTPResource resources[resourceCap];
	u32 resourceCount;
	static constexpr u32 connectionCap = 16;
	HTTPConnection connections[connectionCap];
	u32 connectionCount;
	u32 responseCount;
	b32 open;

	void init() {
		open = false;
		resourceCount = 0;
		responseCount = 0;
		connectionCount = 0;
		for (u32 i = 0; i < connectionCap; i++) {
			connections[i].tcpConnectionIdx = TCP_CONNECTION_BLOCK_INVALID_IDX;
		}
		open = true;
	}

	void close() {
		open = false;
	}

	void close_client(HTTPConnection& client) {
		if (client.tcpConnectionIdx == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			return;
		}
		g_syscallProc(SYSCALL_TCP_CLOSE, client.tcpConnectionIdx);
		client.tcpConnectionIdx = TCP_CONNECTION_BLOCK_INVALID_IDX;
		connectionCount--;
	}

	void close_connections() {
		for (u32 i = 0; i < connectionCap; i++) {
			close_client(connections[i]);
		}
		connectionCount = 0;
	}

	void serve_resource(HTTPResource resource) {
		if (resourceCount >= resourceCap) {
			return;
		}
		resources[resourceCount++] = resource;
	}

	void clear_resources() {
		resourceCount = 0;
	}

	void accept_new_connection(u32 tcpConnectionIdx) {
		for (u32 i = 0; i < connectionCap; i++) {
			HTTPConnection& client = connections[i];
			if (client.tcpConnectionIdx != TCP_CONNECTION_BLOCK_INVALID_IDX) {
				continue;
			}
			client.receivePos = 0;
			client.tcpConnectionIdx = tcpConnectionIdx;
			client.httpReader.init(reinterpret_cast<char*>(&client.receiveBuffer[0]), 0);
			connectionCount++;
			g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("Accepted new connection!\n"));
			break;
		}
	}

	void receive_data(HTTPConnection& connection) {
		if (connection.tcpConnectionIdx == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			return;
		}
		print("Trying to receive data.\n");
		SyscallTCPReceiveArgs receiveArgs;
		receiveArgs.bufferAddress = reinterpret_cast<u64>(&connection.receiveBuffer[connection.receivePos]);
		receiveArgs.bufferSize = HTTPConnection::receiveBufferCap - connection.receivePos;
		receiveArgs.blockIndex = connection.tcpConnectionIdx;
		i64 received = g_syscallProc(SYSCALL_TCP_RECEIVE, reinterpret_cast<u64>(&receiveArgs));
		if (received >= 0) {
			connection.receivePos += received;
			print("Read data from tcp.\n");
			print_num(received);
		} else { // received < 0
			close_client(connection);
		}
	}

	void send_data_blocking(HTTPConnection& client, const void* vdata, u32 dataLength) {
		if (client.tcpConnectionIdx == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			return;
		}
		const char* data = reinterpret_cast<const char*>(vdata);
		while (dataLength > 0 && client.tcpConnectionIdx != TCP_CONNECTION_BLOCK_INVALID_IDX) {
			SyscallTCPSendArgs sendArgs;
			sendArgs.bufferAddress = reinterpret_cast<u64>(data);
			sendArgs.bufferSize = dataLength;
			sendArgs.blockIndex = client.tcpConnectionIdx;
			i64 sent = g_syscallProc(SYSCALL_TCP_SEND, reinterpret_cast<u64>(&sendArgs));
			if (sent >= 0) {
				data += sent;
				dataLength -= sent;
			} else { // sent < 0
				close_client(client);
			}
			if (dataLength > 0 && client.tcpConnectionIdx != TCP_CONNECTION_BLOCK_INVALID_IDX) {
				g_syscallProc(SYSCALL_SHUTDOWN, SYSCALL_SHUTDOWN_CODE_HLT);
			}
		}
	}

	void send_remaining_data(HTTPConnection& connection) {
		if (connection.tcpConnectionIdx == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			return;
		}
	}

	void take_requests() {
		for (u32 i = 0; i < connectionCap; i++) {
			HTTPConnection& client = connections[i];
			if (client.tcpConnectionIdx == TCP_CONNECTION_BLOCK_INVALID_IDX) {
				continue;
			}

			client.httpReader.length = client.receivePos;
			const char* response = "";
			if (client.httpReader.pos == 0 && !client.httpReader.read_request_header(&client.requestMethod, &client.requestTarget)) {
				if (client.receivePos == HTTPConnection::receiveBufferCap) {
					response = "HTTP/1.1 413 Payload Too Large\r\n\r\n";
					g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("HTTP client payload too large\n"));
					send_data_blocking(client, response, strlen(response));
					close_client(client);
				} else if (client.httpReader.errored) {
					response = "HTTP/1.1 400 Bad Request\r\n\r\n";
					g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("HTTP client bad request\n"));
					send_data_blocking(client, response, strlen(response));
					close_client(client);
				}
				continue;
			}

			Range dummy;
			while (!client.httpReader.errored && client.httpReader.read_header_field(&dummy, &dummy));


			if (!client.httpReader.headerDone) {
				if (client.httpReader.errored) {
					response = "HTTP/1.1 400 Bad Request\r\n\r\n";
					g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("HTTP client bad request\n"));
					send_data_blocking(client, response, strlen(response));
					close_client(client);
				} else if (client.receivePos == HTTPConnection::receiveBufferCap) {
					response = "HTTP/1.1 413 Payload Too Large\r\n\r\n";
					g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("HTTP client payload too large\n"));
					send_data_blocking(client, response, strlen(response));
					close_client(client);
				}
				continue;
			}

			if (client.requestTarget.length() > 1 && client.httpReader.src[client.requestTarget.end - 1] == '/') {
				client.requestTarget.end--;
			}

			if (client.requestMethod != HTTP_METHOD_GET && client.requestMethod != HTTP_METHOD_HEAD) {
				response = "HTTP/1.1 501 Not Implemented\r\n\r\n";
				g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("HTTP response not implemented\n"));
				send_data_blocking(client, response, strlen(response));
				close_client(client);
				continue;
			}
			for (u32 j = 0; j < resourceCount; j++) {
				HTTPResource& resource = resources[j];
				if (client.requestTarget.data_range(client.httpReader.src) != resource.dataDirectory) {
					continue;
				}
				g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("HTTP serving response\n"));
				char data[1024];
				response = "HTTP/1.1 200 OK\r\nContent-Length: ";
				u32 responseLength = strlen(response);
				memcpy(data, response, responseLength);
				i32 numberEncodedLength = ascii_base10_encode_u32(data + responseLength, 1024 - responseLength, resource.dataSize);
				memcpy(&data[responseLength + numberEncodedLength], "\r\n\r\n", 4);
				send_data_blocking(client, data, responseLength + numberEncodedLength + 4);
				if (client.requestMethod == HTTP_METHOD_GET) {
					send_data_blocking(client, resource.dataToServe, resource.dataSize);
					responseCount++;
				}
				close_client(client);
				g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("Served data to client\n"));
				goto foundResource;
			}
			g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("HTTP serving not found\n"));
			response = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
			send_data_blocking(client, response, strlen(response));
		foundResource:;
			memmove(client.receiveBuffer, client.receiveBuffer + client.httpReader.pos, client.receivePos - client.httpReader.pos);
			client.receivePos -= client.httpReader.pos;
			client.httpReader.init(reinterpret_cast<char*>(&client.receiveBuffer[0]), 0);
		}
	}
};

// RFC 1738 section 5, httpurl
// I'm hoping an https url is the same format as httpurl (couldn't find anything related to https in the RFC).
// 
// Relevent backus-naur form for an https URL, from the RFC:
// 
// httpsurl    = "https://" hostport [ "/" hpath [ "?" search ]]
// hostport    = host [ ":" port ]
// hpath       = hsegment *[ "/" hsegment ]
// search      = *[ uchar | ";" | ":" | "@" | "&" | "=" ]
// hsegment    = *[ uchar | ";" | ":" | "@" | "&" | "=" ]
// host        = hostname | hostnumber
// hostname    = *[ domainlabel "." ] toplabel
// domainlabel = alphadigit | alphadigit *[ alphadigit | "-" ] alphadigit
// toplabel    = alpha | alpha *[ alphadigit | "-" ] alphadigit
// hostnumber  = digits "." digits "." digits "." digits
// port        = digits
// uchar       = unreserved | escape
// unreserved  = alpha | digit | safe | extra
// alphadigit  = alpha | digit
// alpha       = 'a'-'z' | 'A'-'Z' (full english alphabet, didn't feel like typing out all characters)
// digit       = '0'-'9' (all 10 digits, again didn't want to type it all)
// safe        = "$" | "-" | "_" | "." | "+"
// escape      = "%" hex hex
// extra       = "!" | "*" | "'" | "(" | ")" | ","
// hex         = digit | 'A'-'F' | 'a'-'f'
// digits      = 1*digit
struct HTTPURL {
	Range hostName;
	Range hostPort;
	Range path;
	Range search;

	const char* src;
	u32 length;
	u32 pos;

#define URL_VERIFY(cond) if (!(cond)) { return false; }

	b32 match_char(char c) {
		if (pos < length && src[pos] == c) {
			pos++;
			return true;
		}
		return false;
	}

	b32 match_string(const char* str) {
		// match char until null character
		u32 prevPos = pos;
		while (pos < length && str[0] != '\0' && str[0] == src[pos]) {
			pos++, str++;
		}
		if (str[0] != '\0') {
			pos = prevPos;
			return false;
		}
		return true;
	}

	b32 match_digit() {
		// '0'-'9'
		if (pos < length && is_digit(src[pos])) {
			pos++;
			return true;
		}
		return false;
	}

	b32 match_digits() {
		// 1*digit
		URL_VERIFY(match_digit());
		while (match_digit());
		return true;
	}

	b32 match_alpha() {
		// 'a'-'z' | 'A'-'Z'
		if (pos < length && is_alpha(src[pos])) {
			pos++;
			return true;
		}
		return false;
	}

	b32 match_alphadigit() {
		// alpha | digit
		if (pos < length && is_alphanumeric(src[pos])) {
			pos++;
			return true;
		}
		return false;
	}

	b32 match_hexdigit() {
		// digit | 'A'-'F' | 'a'-'f'
		if (pos < length && is_hex_digit(src[pos])) {
			pos++;
			return true;
		}
		return false;
	}

	b32 match_safe() {
		// "$" | "-" | "_" | "." | "+"
		return
			match_char('$') ||
			match_char('-') ||
			match_char('_') ||
			match_char('.') ||
			match_char('+');
	}

	b32 match_extra() {
		// "!" | "*" | "'" | "(" | ")" | ","
		return
			match_char('!') ||
			match_char('*') ||
			match_char('\'') ||
			match_char('(') ||
			match_char(')') ||
			match_char(',');
	}

	b32 match_unreserved() {
		// alpha | digit | safe | extra
		return match_alpha() || match_digit() || match_safe() || match_extra();
	}

	b32 match_escape() {
		// "%" hex hex
		URL_VERIFY(match_char('%'));
		URL_VERIFY(match_hexdigit());
		URL_VERIFY(match_hexdigit());
		return true;
	}

	b32 match_uchar() {
		// unreserved | escape
		return match_unreserved() || match_escape();
	}

	b32 match_domainlabel() {
		// alphadigit | alphadigit *[ alphadigit | "-" ] alphadigit
		URL_VERIFY(match_alphadigit());
		u32 prevPos = pos;
		while (match_alphadigit() || match_char('-'));
		if (src[pos - 1] == '-') {
			// The rule is either match one alphadigit or alphadigits with either alphadigits or dashes in the middle. If the end character was a dash, go back to the first part of the rule
			pos = prevPos;
		}
		return true;
	}

	b32 match_toplabel() {
		// alpha | alpha *[ alphadigit | "-" ] alphadigit
		// toplabel is the same as domainlabel, but it can't start with a number
		URL_VERIFY(match_alpha());
		u32 prevPos = pos;
		while (match_alphadigit() || match_char('-'));
		if (src[pos - 1] == '-') {
			// Same as above
			pos = prevPos;
		}
		return true;
	}

	b32 match_hostname() {
		// *[ domainlabel "." ] toplabel
		u32 prevPos = pos;
		u32 prevLabel = pos;
		while (match_domainlabel()) {
			if (!match_char('.')) {
				pos = prevLabel;
				break;
			}
			prevLabel = pos;
		}
		if (!match_toplabel()) {
			pos = prevPos;
			return false;
		}
		return true;
	}

	b32 match_hostnumber() {
		// digits "." digits "." digits "." digits
		URL_VERIFY(match_digits());
		URL_VERIFY(match_char('.'));
		URL_VERIFY(match_digits());
		URL_VERIFY(match_char('.'));
		URL_VERIFY(match_digits());
		URL_VERIFY(match_char('.'));
		URL_VERIFY(match_digits());
		return true;
	}

	b32 match_host() {
		// hostname | hostnumber
		return match_hostname() || match_hostnumber();
	}

	b32 parse_hostport() {
		// host [ ":" port ]
		hostName.start = pos;
		URL_VERIFY(match_host());
		hostName.end = pos;
		if (match_char(':')) {
			hostPort.start = pos;
			URL_VERIFY(match_digits());
			hostPort.end = pos;
		} else {
			hostPort = Range{ 0, 0 };
		}
		return true;
	}

	b32 match_hsegment() {
		// *[ uchar | ";" | ":" | "@" | "&" | "=" ]
		while (
			match_char(';') ||
			match_char(':') ||
			match_char('@') ||
			match_char('&') ||
			match_char('=') ||
			match_uchar()
			);
		return true;
	}

	b32 match_hpath() {
		// hsegment *[ "/" hsegment ]
		match_hsegment();
		while (match_char('/')) {
			match_hsegment();
		}
		return true;
	}

	b32 match_search() {
		// Same as hsegment
		// *[ uchar | ";" | ":" | "@" | "&" | "=" ]
		while (
			match_char(';') ||
			match_char(':') ||
			match_char('@') ||
			match_char('&') ||
			match_char('=') ||
			match_uchar()
			);
		return true;
	}

	b32 parse(const char* url, u32 urlLength) {
		src = url;
		length = urlLength;
		pos = 0;

		// "https://" hostport [ "/" hpath [ "?" search ]]
		URL_VERIFY(match_string("http://") || match_string("https://"));
		URL_VERIFY(parse_hostport());
		if (match_char('/')) {
			path.start = pos - 1;
			URL_VERIFY(match_hpath());
			path.end = pos;
			if (match_char('?')) {
				search.start = pos;
				URL_VERIFY(match_search());
				search.end = pos;
			} else {
				search = Range{ 0, 0 };
			}
		} else {
			path = Range{ 0, 0 };
			search = Range{ 0, 0 };
		}
		return true;
	}
};