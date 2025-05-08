#pragma once

#include "Util.h"
#include "PondOS.h"

// Implemented from RFC 9110 and RFC 9112

#define HTTP_ERROR_OK "200"
#define HTTP_ERROR_BAD_REQUEST "400"

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

struct HTTPField {
	const char* name;
	Range value;

	bool has_value() {
		return value != Range{ 0, 0 };
	}

	void reset() {
		value.reset();
	}

	void reset(const char* newName) {
		name = newName;
		value.reset();
	}
};

#define HTTP_VERIFY_RETURN(cond, errCode, ret) if(!(cond)) { errored = true; errorCode = errCode; return ret; }

struct HTTPReader {
	char* src;
	U32 length;
	U32 pos;
	const char* errorCode;
	B8 errored;
	B8 headerDone;
	B8 readComplete;
	B8 chunkedEncoding;
	U32 contentOffset;
	U32 contentLength;
	U32 nextChunkLength;

	void init(char* source, U32 sourceLength) {
		src = source;
		length = sourceLength;
		pos = 0;
		errorCode = HTTP_ERROR_OK;
		errored = false;
		headerDone = false;
		readComplete = false;
		chunkedEncoding = false;
		contentOffset = 0;
		contentLength = 0;
		nextChunkLength = 0;
	}

	void set_read_data(void* source, U32 sourceLength) {
		src = reinterpret_cast<char*>(source);
		length = sourceLength;
		pos = 0;
	}

	B32 is_crlf(U32 pos) {
		if (pos >= (length - 1)) {
			return false;
		}
		return src[pos] == '\r' && src[pos + 1] == '\n';
	}

	B32 consume_crlf() {
		if (pos < (length - 1) && src[pos] == '\r' && src[pos + 1] == '\n') {
			pos += 2;
			return true;
		} else {
			return false;
		}
	}

	void skip_whitespace(U32 end) {
		while (pos < end && is_whitespace(src[pos])) {
			pos++;
		}
	}

	U32 skip_whitespace_reverse(I32 endPos) {
		while (endPos >= 0 && is_whitespace(src[endPos])) {
			endPos--;
		}
		return endPos;
	}

	B32 get_line_length(U32* lengthOut) {
		U32 lineLength = 0;
		while (!is_crlf(pos + lineLength) && I32(pos + lineLength) < (I32(length) - 1)) {
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

	B32 src_begin_match(const char* cmpNullTerminated) {
		U32 cmpPos = 0;
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

	B32 parse_verify_http_version() {
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

	B32 read_request_header(HTTPRequestMethod* requestMethod, Range* requestTarget) {
		U32 lineEnd = 0;
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

	B32 read_response_header(Range* responseCode, Range* reasonPhrase) {
		U32 lineEnd = 0;
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

	B32 check_response_is(const char* httpcode) {
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

	B32 read_header_field(Range* fieldName, Range* fieldValue) {
		if (consume_crlf()) {
			contentOffset = pos;
			headerDone = true;
			return false;
		}
		U32 lineEnd = 0;
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

	void try_read_more_data(HTTPField* fields = nullptr, U32 numFields = 0) {
		if (errored || readComplete) {
			return;
		}
		if (pos == 0 && get_line_length(nullptr) && !check_response_is("xxx")) {
			errorCode = HTTP_ERROR_BAD_REQUEST;
			errored = true;
			return;
		}
		if (pos == 0) {
			// Haven't yet parsed the first line
			return;
		}
		if (!headerDone) {
			Range fieldName;
			Range fieldValue;
			while (!errored && read_header_field(&fieldName, &fieldValue)) {
				if (fieldName.data_range(src) == "Content-Length") {
					contentLength = fieldValue.data_range(src).parse_uint(I32_MAX);
				} else if (fieldName.data_range(src) == "Transfer-Encoding") {
					chunkedEncoding = true;
					nextChunkLength = 0;
				}
				for (U32 i = 0; i < numFields; i++) {
					if (fieldName.data_range(src) == fields[i].name) {
						if (fields[i].has_value()) {
							// Duplicate field, illegal (we're not dealing with any comma separated lists)
							errorCode = HTTP_ERROR_BAD_REQUEST;
							errored = true;
							return;
						}
						fields[i].value = fieldValue;
					}
				}
			}
		}
		if (headerDone) {
			if (chunkedEncoding) {
				bool shouldParseNext = true;
				while (shouldParseNext) {
					shouldParseNext = false;
					if (nextChunkLength == 0) {
						// Need to read a chunk length
						U32 lineLength = 0;
						if (get_line_length(&lineLength)) {
							nextChunkLength = 0;
							const char* lengthStr = src + pos;
							while (*lengthStr != '\r' && *lengthStr != ';') { // Extensions start with semicolon, ignore them
								char c = *lengthStr++;
								if (is_hex_digit(c)) {
									if (nextChunkLength > U32_MAX >> 4) {
										// Potential overflow
										errorCode = HTTP_ERROR_BAD_REQUEST;
										errored = true;
										return;
									}
									nextChunkLength <<= 4;
									nextChunkLength += c > '9' ? (c | 0b100000) - 'a' + 10 : c - '0';
								} else {
									errorCode = HTTP_ERROR_BAD_REQUEST;
									errored = true;
									return;
								}
							}
							pos += lineLength;
							consume_crlf();
							if (nextChunkLength == 0) {
								readComplete = true;
							} else {
								shouldParseNext = true;
							}
						}
					} else {
						// Need to read nextChunkLength bytes before the next chunk length
						if (length - pos >= nextChunkLength + 2) { // + 2 for the CRLF after the chunk
							memmove(src + contentOffset + contentLength, src + pos, nextChunkLength);
							contentLength += nextChunkLength;
							pos += nextChunkLength;
							nextChunkLength = 0;
							consume_crlf();
							shouldParseNext = true;
						}
					}
				}
			} else {
				if (length - contentOffset >= contentLength) {
					readComplete = true;
				}
			}
		}
	}
};
#undef HTTP_VERIFY_RETURN

struct HTTPResource {
	const char* dataDirectory;
	const void* dataToServe;
	U32 dataDirectorySize;
	U32 dataSize;
};

struct HTTPConnection {
	static constexpr U32 receiveBufferCap = 1024;
	Byte receiveBuffer[receiveBufferCap];
	U32 receivePos;
	U32 sendLength;
	const void* sendResource;
	HTTPReader httpReader;
	HTTPRequestMethod requestMethod;
	Range requestTarget;
	U32 tcpConnection;
};

// Not a very good HTTP server implementation, but I'm only using it for certificate renewal, so I don't think it needs to be good (unlike the main server)
struct HTTPServer {
	static constexpr U32 resourceCap = 16;
	HTTPResource resources[resourceCap];
	U32 resourceCount;
	static constexpr U32 connectionCap = 16;
	HTTPConnection connections[connectionCap];
	U32 connectionCount;
	U32 responseCount;
	B32 open;

	void init() {
		open = false;
		resourceCount = 0;
		responseCount = 0;
		connectionCount = 0;
		for (U32 i = 0; i < connectionCap; i++) {
			connections[i].tcpConnection = TCP_CONNECTION_BLOCK_INVALID_IDX;
		}
		open = true;
	}

	void close() {
		close_connections();
		open = false;
	}

	void close_client(HTTPConnection& client) {
		if (client.tcpConnection == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			return;
		}
		g_syscallProc(SYSCALL_TCP_CLOSE, client.tcpConnection);
		client.tcpConnection = TCP_CONNECTION_BLOCK_INVALID_IDX;
		connectionCount--;
	}

	void close_connections() {
		for (U32 i = 0; i < connectionCap; i++) {
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
		for (U32 i = 0; i < connectionCount; i++) {
			connections[i].sendLength = 0;
			connections[i].sendResource = nullptr;
		}
	}

	bool accept_new_connection(U32 tcpConnection) {
		if (!open) {
			return false;
		}
		for (U32 i = 0; i < connectionCap; i++) {
			HTTPConnection& client = connections[i];
			if (client.tcpConnection != TCP_CONNECTION_BLOCK_INVALID_IDX) {
				continue;
			}
			client.receivePos = 0;
			client.sendLength = 0;
			client.sendResource = nullptr;
			client.tcpConnection = tcpConnection;
			client.httpReader.init(reinterpret_cast<char*>(&client.receiveBuffer[0]), 0);
			connectionCount++;
			return true;
		}
		return false;
	}

	void receive_data(HTTPConnection& connection) {
		if (connection.tcpConnection == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			return;
		}
		SyscallTCPReceiveArgs recvArgs;
		recvArgs.bufferAddress = U64(&connection.receiveBuffer[connection.receivePos]);
		recvArgs.bufferSize = HTTPConnection::receiveBufferCap - connection.receivePos;
		recvArgs.blockIndex = connection.tcpConnection;
		I64 dataInBuffer = I64(g_syscallProc(SYSCALL_TCP_RECEIVE, U64(&recvArgs)));
		if (dataInBuffer >= 0) {
			connection.receivePos += dataInBuffer;
		} else { // dataInBuffer < 0
			close_client(connection);
		}
	}

	void try_send_remaining_data(HTTPConnection& client) {
		if (client.tcpConnection == TCP_CONNECTION_BLOCK_INVALID_IDX || client.sendLength == 0) {
			return;
		}
		const char* data = reinterpret_cast<const char*>(client.sendResource);
		SyscallTCPSendArgs sendArgs;
		sendArgs.bufferAddress = U64(data);
		sendArgs.bufferSize = client.sendLength;
		sendArgs.blockIndex = client.tcpConnection;
		I64 dataAccepted = I64(g_syscallProc(SYSCALL_TCP_SEND, U64(&sendArgs)));
		if (dataAccepted >= 0) {
			data += dataAccepted;
			client.sendResource = data;
			client.sendLength -= dataAccepted;
		} else { // sent < 0
			close_client(client);
		}
	}

	void try_send_data(HTTPConnection& client, const void* vdata, U32 dataLength) {
		client.sendResource = vdata;
		client.sendLength = dataLength;
		try_send_remaining_data(client);
	}

	void take_requests() {
		for (U32 i = 0; i < connectionCap; i++) {
			HTTPConnection& client = connections[i];
			if (client.tcpConnection == TCP_CONNECTION_BLOCK_INVALID_IDX) {
				continue;
			}

			client.httpReader.length = client.receivePos;
			const char* response = "";
			if (client.httpReader.pos == 0 && !client.httpReader.read_request_header(&client.requestMethod, &client.requestTarget)) {
				if (client.receivePos == HTTPConnection::receiveBufferCap) {
					response = "HTTP/1.1 413 Payload Too Large\r\n\r\n";
					print("HTTP client payload too large\n");
					try_send_data(client, response, strlen(response));
					close_client(client);
				} else if (client.httpReader.errored) {
					response = "HTTP/1.1 400 Bad Request\r\n\r\n";
					print("HTTP client bad request\n");
					try_send_data(client, response, strlen(response));
					close_client(client);
				}
				continue;
			}

			Range dummy;
			while (!client.httpReader.errored && client.httpReader.read_header_field(&dummy, &dummy));


			if (!client.httpReader.headerDone) {
				if (client.httpReader.errored) {
					response = "HTTP/1.1 400 Bad Request\r\n\r\n";
					print("HTTP client bad request\n");
					try_send_data(client, response, strlen(response));
					close_client(client);
				} else if (client.receivePos == HTTPConnection::receiveBufferCap) {
					response = "HTTP/1.1 413 Payload Too Large\r\n\r\n";
					print("HTTP client payload too large\n");
					try_send_data(client, response, strlen(response));
					close_client(client);
				}
				continue;
			}

			if (client.requestTarget.length() > 1 && client.httpReader.src[client.requestTarget.end - 1] == '/') {
				client.requestTarget.end--;
			}

			if (client.requestMethod != HTTP_METHOD_GET && client.requestMethod != HTTP_METHOD_HEAD) {
				response = "HTTP/1.1 501 Not Implemented\r\n\r\n";
				print("HTTP response not implemented\n");
				try_send_data(client, response, strlen(response));
				close_client(client);
				continue;
			}
			for (U32 j = 0; j < resourceCount; j++) {
				HTTPResource& resource = resources[j];
				if (client.requestTarget.data_range(client.httpReader.src) != DataRange{ resource.dataDirectory, 0, resource.dataDirectorySize }) {
					continue;
				}
				print("HTTP serving response\n");
				char data[1024];
				response = "HTTP/1.1 200 OK\r\nContent-Length: ";
				U32 responseLength = strlen(response);
				memcpy(data, response, responseLength);
				I32 numberEncodedLength = ascii_base10_encode_u32(data + responseLength, 1024 - responseLength, resource.dataSize);
				memcpy(&data[responseLength + numberEncodedLength], "\r\n\r\n", 4);
				try_send_data(client, data, responseLength + numberEncodedLength + 4);
				if (client.sendLength != 0) {
					close_client(client);
					continue;
				}
				if (client.requestMethod == HTTP_METHOD_GET) {
					try_send_data(client, resource.dataToServe, resource.dataSize);
					responseCount++;
				}
				close_client(client);
				print("Served data to client\n");
				goto foundResource;
			}
			print("HTTP serving not found\n");
			response = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
			try_send_data(client, response, strlen(response));
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
	U32 length;
	U32 pos;

#define URL_VERIFY(cond) if (!(cond)) { return false; }

	bool match_char(char c) {
		if (pos < length && src[pos] == c) {
			pos++;
			return true;
		}
		return false;
	}

	bool match_string(const char* str) {
		// match char until null character
		U32 prevPos = pos;
		while (pos < length && str[0] != '\0' && str[0] == src[pos]) {
			pos++, str++;
		}
		if (str[0] != '\0') {
			pos = prevPos;
			return false;
		}
		return true;
	}

	bool match_digit() {
		// '0'-'9'
		if (pos < length && is_digit(src[pos])) {
			pos++;
			return true;
		}
		return false;
	}

	bool match_digits() {
		// 1*digit
		URL_VERIFY(match_digit());
		while(match_digit());
		return true;
	}

	bool match_alpha() {
		// 'a'-'z' | 'A'-'Z'
		if (pos < length && is_alpha(src[pos])) {
			pos++;
			return true;
		}
		return false;
	}

	bool match_alphadigit() {
		// alpha | digit
		if (pos < length && is_alphanumeric(src[pos])) {
			pos++;
			return true;
		}
		return false;
	}

	bool match_hexdigit() {
		// digit | 'A'-'F' | 'a'-'f'
		if (pos < length && is_hex_digit(src[pos])) {
			pos++;
			return true;
		}
		return false;
	}

	bool match_safe() {
		// "$" | "-" | "_" | "." | "+"
		return
			match_char('$') ||
			match_char('-') || 
			match_char('_') || 
			match_char('.') || 
			match_char('+');
	}

	bool match_extra() {
		// "!" | "*" | "'" | "(" | ")" | ","
		return
			match_char('!') ||
			match_char('*') || 
			match_char('\'') || 
			match_char('(') || 
			match_char(')') ||
			match_char(',');
	}

	bool match_unreserved() {
		// alpha | digit | safe | extra
		return match_alpha() || match_digit() || match_safe() || match_extra();
	}

	bool match_escape() {
		// "%" hex hex
		URL_VERIFY(match_char('%'));
		URL_VERIFY(match_hexdigit());
		URL_VERIFY(match_hexdigit());
		return true;
	}

	bool match_uchar() {
		// unreserved | escape
		return match_unreserved() || match_escape();
	}

	bool match_domainlabel() {
		// alphadigit | alphadigit *[ alphadigit | "-" ] alphadigit
		URL_VERIFY(match_alphadigit());
		U32 prevPos = pos;
		while (match_alphadigit() || match_char('-'));
		if (src[pos - 1] == '-') {
			// The rule is either match one alphadigit or alphadigits with either alphadigits or dashes in the middle. If the end character was a dash, go back to the first part of the rule
			pos = prevPos;
		}
		return true;
	}

	bool match_toplabel() {
		// alpha | alpha *[ alphadigit | "-" ] alphadigit
		// toplabel is the same as domainlabel, but it can't start with a number
		URL_VERIFY(match_alpha());
		U32 prevPos = pos;
		while (match_alphadigit() || match_char('-'));
		if (src[pos - 1] == '-') {
			// Same as above
			pos = prevPos;
		}
		return true;
	}

	bool match_hostname() {
		// *[ domainlabel "." ] toplabel
		U32 prevPos = pos;
		U32 prevLabel = pos;
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

	bool match_hostnumber() {
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

	bool match_host() {
		// hostname | hostnumber
		return match_hostname() || match_hostnumber();
	}

	bool parse_hostport() {
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

	bool match_hsegment() {
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

	bool match_hpath() {
		// hsegment *[ "/" hsegment ]
		match_hsegment();
		while (match_char('/')) {
			match_hsegment();
		}
		return true;
	}

	bool match_search() {
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

	bool parse(const char* url, U32 urlLength) {
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