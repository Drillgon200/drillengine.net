#include "Util.h"
#include "PondOS.h"
#include "HTTP.h"
#include "TLS.h"
#include "ACME.h"
#include "CommandMode.h"
#include "Blog.h"

const char penguin[]{ R"(
             . --- .
           /        \
          |  O  _  O |
          |  ./   \. |
          /  `-._.-'  \
        .' /         \ `.
    .-~.-~/           \~-.~-.
.-~ ~    |             |    ~ ~-.
`- .     |             |     . -'
     ~ - |             | - ~
         \             /
       ___\           /___
       ~;_  >- . . -<  _i~
          `'         `'
)"};

const char BLOG_PAGE_HEADER[] {
R"_html(
<!DOCTYPE html>
<html lang="en">
<head>
	<title>DrillEngine</title>
	<meta charset="utf-8" name="viewport" content="width=device-width, initial-scale=1">
	<style>
		* {
			margin: 0;
			font-family: -apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif;
			color: #C9D1D9;
		}
		header {
			text-align: center;
			font-size: 30px;
		}
		body {
			background-color: #121212;
			font-size: 14px;
		}
		.content {
			background-color: #282828;
			padding: 32px;
			padding-top: 20px;
			width: 600px;
			margin-left: auto;
			margin-right: auto;
			margin-top: 20px;
			line-height: 22px;
		}
			.content a:link {
				color: #0077AA;
			}
			.content a:visited {
				color: #685098;
			}
			.content hr {
				color: #3F3F3F;
				margin-top: 10px;
				margin-bottom: 10px
			}
		.comments {

		}
			.comments h4 {
				font-size: 16px;
				padding-bottom: 5px;
			}
			.comments p {
				margin-left: 10px;
				white-space: pre-wrap;
			}
		.text_input {
			padding: 5px;
			border: none;
			box-sizing: border-box;
			margin-bottom: 10px;
			resize: none;
			width: 100%;
			background-color: #202020;
			font-size: 14px;
			outline: none;
			display: inline;
		}
			.text_input:focus {
				outline: 2px solid #3F3F3F;
			}
		.button_input {
			margin-left: 2px;
			color: #C9D1D9;
			background-color: #505050;
			border-color: #404040;
			padding: 4px;
			padding-bottom: 6px;
		}
			.button_input:hover {
				background-color: #404040;
			}
		h1, h2, h3 {
			text-align: center;
			line-height: 1.2;
			margin-bottom: 10px;
			margin-top: 10px;
		}
		p {
			margin-bottom: 5px;
		}
	</style>
	<script>
		function update_textarea_size(element) {
			element.style.height = "auto";
			element.style.height = element.scrollHeight + "px";
		}
		function get_comments() {
			fetch("/api/GetComments", {
				method: "POST",
				body: window.location.pathname
			})
			.then(res => res.json())
			.then(res => {
				var commentsElement = document.getElementById("comments");
				commentsElement.textContent = "";
				for (var i = 0; i < res.length; i += 2) {
					commentsElement.appendChild(document.createElement("hr"));
					var nameElement = document.createElement("h4");
					nameElement.textContent = res[i];
					commentsElement.appendChild(nameElement);
					var contentElement = document.createElement("p");
					contentElement.textContent = res[i + 1];
					commentsElement.appendChild(contentElement);
				}
			});
		}
		function post_comment() {
			var name = document.getElementById("comment_name");
			var content = document.getElementById("comment_content");
			fetch("/api/NewComment", {
				method: "POST",
				body: JSON.stringify({
					post: window.location.pathname,
					name: name.value,
					content: content.value
				})
			}).then(res => {
				content.value = "";
				content.oninput();
				get_comments();
			});
		}
		function on_load() {
			get_comments();
		}
	</script>
</head>
<body onload="on_load()">
	<header>
		<h2 style="background-color: #6F1010; margin-top: 0; margin-bottom: 0; padding-top: 10px; padding-bottom: 20px"><a href="/" style="text-decoration: none; padding: 5px">DrillEngine Blog</a></h2>
		<div style="background-color: #3F1818; margin-left: auto; margin-right: auto; font-size: 15px;">
			<h2 style="display: inline-block"><a href="/MainPage.html" style="text-decoration: none; padding: 10px">Home</a></h2>
			<h2 style="display: inline-block"><a href="/About.html" style="text-decoration: none; padding: 10px">About</a></h2>
			<h2 style="display: inline-block"><a href="/Posts.html" style="text-decoration: none; padding: 10px">Posts</a></h2>
		</div>
	</header>
	<div class="content">
)_html"
};

const char BLOG_PAGE_FOOTER[]{
R"_html(
	</div>
	<div class="content">
		<h2 style="margin-top: 0">Comments</h2>
		<hr />
		<div style="padding:2px">
			<input id="comment_name" class="text_input" style="width:auto" type="text" id="comment_name" placeholder="Your name" />
			<textarea id="comment_content" class="text_input" rows=3 placeholder="Write a comment" oninput="update_textarea_size(this)"></textarea>
			<button class="button_input" onclick="post_comment()">Post Comment</button>
		</div>
		<div id="comments" class="comments">
		</div>
	</div>
</body>

</html>
)_html"
};

#define SAVED_IMAGE_MEMORY_OFFSET 0x15000000000
#define SAVED_IMAGE_MEMORY_CAP 1024 * 1024 * 20
U32 savedImageSize;

X509Certificate cert;

ACMEAgent acmeAgent;
TLSConnection testClient;
U16 currentConnectionPort;
U32 currentPortControlMask;

const U32 MAX_CLIENTS = 16;
struct Client {
	TLSConnection tlsClient;
	HTTPReader httpReader;
	HTTPRequestMethod requestMethod;
	Range requestTarget;
	CommandModeState commandModeState;
	B8 commandMode;
	B8 imageWriteMode;

	void init(U32 connectionIdx) {
		tlsClient.accept_connection_from_client(connectionIdx, &cert);
		httpReader.init(reinterpret_cast<char*>(tlsClient.receiveBuffer), 0);
		commandMode = false;
		imageWriteMode = false;
		commandModeState.state = CommandModeState::REQUEST_AUTH;
	}
};

Client clients[MAX_CLIENTS];

B32 start_acme_cert_request() {
	if (acmeAgent.state != ACME_STATE_IDLE) {
		return false;
	}
	B32 success = acmeAgent.request_cert(&cert, "acme-v02.api.letsencrypt.org"a, 0, "/directory", 443, currentConnectionPort);
	if (success) {
		currentPortControlMask |= TCP_PORT_ENABLE_PORT_80_BIT;
		g_syscallProc(SYSCALL_TCP_PORT_CONTROL, currentPortControlMask);
	}
	return success;
}

X509Certificate& get_server_cert() {
	return cert;
}
ACMEAccount& get_server_acme_account() {
	return acmeAgent.currentAccount;
}

// Some message about our outbound connection
void handle_client_connection_notification(U16 connectionIdx, U32 tcpFlags) {
	if (testClient.state == TLS_CONNECTION_STATE_CLOSED) {
		g_syscallProc(SYSCALL_TCP_CLOSE, connectionIdx);
		return;
	}
	testClient.receive_data();
	if (testClient.receiveBufferUserReadPos != testClient.receiveBufferUserDataEnd) {
		char stringBuf[256];
		stringBuf[255] = 0;
		U32 stringBufPos = 0;
		for (U32 i = testClient.receiveBufferUserReadPos; i < testClient.receiveBufferUserDataEnd; i++) {
			stringBuf[stringBufPos++] = static_cast<char>(testClient.receiveBuffer[i]);
			if (stringBufPos == 255) {
				print(stringBuf);
				stringBufPos = 0;
			}
		}
		stringBuf[stringBufPos] = 0;
		print(stringBuf);
		testClient.skip_received_bytes(testClient.receiveBufferUserDataEnd - testClient.receiveBufferUserReadPos);
	}
	testClient.send_data();
}

void handle_server_request_notification(U16 connectionIdx, U32 tcpFlags) {
	U32 clientIdx = U32_MAX;
	for (U32 i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].tlsClient.tcpConnection == connectionIdx) {
			clientIdx = i;
			break;
		} else if (clients[i].tlsClient.state == TLS_CONNECTION_STATE_CLOSED) {
			clientIdx = i;
		}
	}
	if (clientIdx == U32_MAX) {
		g_syscallProc(SYSCALL_TCP_CLOSE, connectionIdx);
		return;
	}
	Client& client = clients[clientIdx];
	if (tcpFlags & TCP_NOTIFY_OPENED) {
		print("Opening connection.\n");
		client.init(connectionIdx);
	}

	bool couldReceiveMoreData = client.tlsClient.receive_data();
	HTTPReader& http = client.httpReader;
	if (client.tlsClient.state == TLS_CONNECTION_STATE_CONNECTED) {
		if (http.pos == 0 && client.tlsClient.receiveBufferUserDataEnd >= COMMAND_MODE_MAGIC_LENGTH && memcmp(client.tlsClient.receiveBuffer, COMMAND_MODE_MAGIC, COMMAND_MODE_MAGIC_LENGTH) == 0) {
			client.tlsClient.skip_received_bytes(COMMAND_MODE_MAGIC_LENGTH);
			client.commandMode = true;
			print("Entering command mode\n");
		}
		if (client.commandMode) {
		handleCommandData:;
			handle_command_mode_client(client.tlsClient, client.commandModeState);
			if (couldReceiveMoreData) {
				couldReceiveMoreData = client.tlsClient.receive_data();
				goto handleCommandData;
			}
		} else {
			if (client.tlsClient.receiveBufferUserDataEnd != http.length) {
				http.length = client.tlsClient.receiveBufferUserDataEnd;
				const char* response = "";
				if (http.pos == 0 && !http.read_request_header(&client.requestMethod, &client.requestTarget)) {
					if (client.tlsClient.receiveBufferDataEnd == sizeof(TLSConnection::receiveBuffer)) {
						client.tlsClient.write_str("HTTP/1.1 413 Payload Too Large\r\n\r\n");
					} else {
						client.tlsClient.write_str("HTTP/1.1 400 Bad Request\r\n\r\n");
					}
					client.tlsClient.error_alert(TLS_ALERT_CLOSE_NOTIFY);
				}
				if (http.pos != 0) {
					Range fieldName;
					Range fieldValue;
					while (!http.errored && http.read_header_field(&fieldName, &fieldValue)) {
						if (fieldName.data_range(http.src) == "Content-Length") {
							http.contentLength = fieldValue.data_range(http.src).parse_uint(I32_MAX);
						} else if (fieldName.data_range(http.src) == "Connection") {
						}
					}
					if (http.headerDone && http.length - http.contentOffset >= http.contentLength) {
						http.readComplete = true;
					}
				}
			}
			if (client.imageWriteMode || http.headerDone && client.requestMethod == HTTP_METHOD_POST && StrA{ http.src + client.requestTarget.start, client.requestTarget.length() } == "/api/UploadImage"a) {
				if (!client.imageWriteMode) {
					if (client.httpReader.chunkedEncoding || client.httpReader.contentLength == 0 || client.httpReader.contentLength > SAVED_IMAGE_MEMORY_CAP) {
						client.tlsClient.write_str("HTTP/1.1 400 Bad Request\r\n\r\n");
						client.tlsClient.error_alert(TLS_ALERT_CLOSE_NOTIFY);
					} else {
						client.imageWriteMode = true;
						client.tlsClient.skip_received_bytes(client.httpReader.contentOffset);
						savedImageSize = 0;
					}
				}
				if (client.imageWriteMode) {
					while (client.tlsClient.received_user_data_size() && savedImageSize < client.httpReader.contentLength) {
						U32 dataToCopy = min(client.tlsClient.received_user_data_size(), SAVED_IMAGE_MEMORY_CAP - savedImageSize);
						memcpy(((Byte*)SAVED_IMAGE_MEMORY_OFFSET) + savedImageSize, client.tlsClient.receiveBuffer + client.tlsClient.receiveBufferUserReadPos, dataToCopy);
						client.tlsClient.skip_received_bytes(dataToCopy);
						savedImageSize += dataToCopy;
						client.tlsClient.receive_data();
					}
					if (savedImageSize >= client.httpReader.contentLength) {
						print("Uploaded image\n");
						client.tlsClient.write_str("HTTP/1.1 200 OK\r\n\r\n");
						client.tlsClient.error_alert(TLS_ALERT_CLOSE_NOTIFY);
					}
				}
			} else if (http.readComplete) {
				StrA requestPath = StrA{ http.src + client.requestTarget.start, client.requestTarget.length() };
				if (requestPath == "/"a) {
					requestPath = "/MainPage.html"a;
				}
				if (client.requestMethod == HTTP_METHOD_POST) {
					if (requestPath == "/api/NewComment"a) {
						StrA postCommentedOn{};
						StrA commenterName{};
						StrA commentData{};

						JSONReader json; json.init(http.src + http.contentOffset, http.contentLength);
						json.begin_object();
						StrA fieldName;
						while (json.read_field_name(&fieldName)) {
							StrA fieldValue{};
							if (fieldName == "post"a) { postCommentedOn = json.consume_string_field(); }
							else if (fieldName == "name"a) { commenterName = json.consume_string_field(); }
							else if (fieldName == "content"a) { commentData = json.consume_string_field(); }
							else { json.skip_value(); }
						}
						json.end_object();

						if (!json.errored && !postCommentedOn.is_empty() && !commenterName.is_empty() && !commentData.is_empty()) {
							if (postCommentedOn == "/"a) {
								postCommentedOn = "/MainPage.html"a;
							}
							BlogEntry* entry = get_blog_entry(postCommentedOn);
							if (entry) {
								blog_add_comment(entry, commenterName, commentData);
								client.tlsClient.write_str("HTTP/1.1 200 OK\r\n\r\n");
							} else {
								client.tlsClient.write_str("HTTP/1.1 404 Not Found\r\n\r\n");
							}
						} else {
							client.tlsClient.write_str("HTTP/1.1 400 Bad Request\r\n\r\n");
						}
					} else if (requestPath == "/api/GetComments"a) {
						StrA postCommentedOn = StrA{ http.src + http.contentOffset, http.contentLength };
						if (postCommentedOn == "/"a) {
							postCommentedOn = "/MainPage.html"a;
						}
						BlogEntry* entry = get_blog_entry(postCommentedOn);
						if (entry) {
							client.tlsClient.write_str("[");
							for (BlogComment* comment = entry->firstComment; comment; comment = comment->nextComment) {
								client.tlsClient.write_str("\"")
									.write_bytes(comment->username.str, comment->username.length)
									.write_str("\",\"")
									.write_bytes(comment->content.str, comment->content.length)
									.write_str(comment->nextComment ? "\"," : "\"");
							}
							client.tlsClient.write_str("]");
						} else {
							client.tlsClient.write_str("HTTP/1.1 404 Not Found\r\n\r\n");
						}
					} else {
						client.tlsClient.write_str("HTTP/1.1 405 Method Not Allowed\r\n\r\n");
					}
				} else if (client.requestMethod == HTTP_METHOD_GET) {
					if (BlogEntry* entry = get_blog_entry(requestPath)) {
						char header[1024];
						char* headerPtr = header;
						headerPtr = strcpy(headerPtr, "HTTP/1.1 200 OK\r\nContent-Length: ");
						headerPtr += ascii_base10_encode_u32(headerPtr, sizeof(header) - (headerPtr - header), sizeof(BLOG_PAGE_HEADER) + entry->content.length + sizeof(BLOG_PAGE_FOOTER));
						headerPtr = strcpy(headerPtr, "\r\n\r\n");
						client.tlsClient.write_bytes(header, headerPtr - header);
						client.tlsClient.queue_buffer_to_send(BLOG_PAGE_HEADER, sizeof(BLOG_PAGE_HEADER));
						client.tlsClient.queue_buffer_to_send(entry->content.str, entry->content.length);
						client.tlsClient.queue_buffer_to_send(BLOG_PAGE_FOOTER, sizeof(BLOG_PAGE_FOOTER));
					} else if (requestPath == "/api/UploadedImage"a) {
						char header[1024];
						char* headerPtr = header;
						headerPtr = strcpy(headerPtr, "HTTP/1.1 200 OK\r\nContent-Length: ");
						headerPtr += ascii_base10_encode_u32(headerPtr, sizeof(header) - (headerPtr - header), savedImageSize);
						headerPtr = strcpy(headerPtr, "\r\n\r\n");
						client.tlsClient.write_bytes(header, headerPtr - header);
						client.tlsClient.queue_buffer_to_send((void*)SAVED_IMAGE_MEMORY_OFFSET, savedImageSize);
					} else {
						client.tlsClient.write_str("HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found");
					}
				} else {
					client.tlsClient.write_str("HTTP/1.1 405 Method Not Allowed\r\n\r\n");
				}
				client.tlsClient.error_alert(TLS_ALERT_CLOSE_NOTIFY);
			}
			if (!http.readComplete && client.tlsClient.receiveBufferDataEnd == sizeof(TLSConnection::receiveBuffer)) {
				client.tlsClient.write_str("HTTP/1.1 413 Payload Too Large\r\n\r\n");
				client.tlsClient.error_alert(TLS_ALERT_CLOSE_NOTIFY);
			}
		}
	}
	client.tlsClient.send_data();
	if (client.tlsClient.state == TLS_CONNECTION_STATE_CLOSED) {
		// Should not be necessary, check the TLS implementation to make sure
		if (client.tlsClient.tcpConnection != TCP_CONNECTION_BLOCK_INVALID_IDX) {
			g_syscallProc(SYSCALL_TCP_CLOSE, client.tlsClient.tcpConnection);
		}
		client.tlsClient.tcpConnection = TCP_CONNECTION_BLOCK_INVALID_IDX;
	}
	if (tcpFlags & (TCP_NOTIFY_HALF_CLOSED | TCP_NOTIFY_TIMED_OUT | TCP_NOTIFY_CLOSED)) {
		client.tlsClient.close_no_alert();
	}
}

void run_test_https_get_request() {
	testClient.connect_to_server("perfectmotherfuckingwebsite.com"a, 443, currentConnectionPort);
	testClient.write_str("GET / HTTP/1.1\r\n");
	testClient.write_str("Host: perfectmotherfuckingwebsite.com\r\n");
	testClient.write_str("Connection: close\r\n\r\n");
	testClient.send_data();
}

__declspec(dllexport) void __vectorcall server_main(SyscallProc syscallProc) {
	g_syscallProc = syscallProc;
	print("Hello from user space!\n");
	currentConnectionPort = 4001;
	syscallProc(SYSCALL_TCP_PORT_CONTROL, currentPortControlMask = TCP_PORT_ENABLE_PORT_443_bit | currentConnectionPort - 4000 << TCP_PORT_ENABLE_USER_PORT_SHIFT);

	ec_init();
	cert.init();
	// This is just a test certificate, not valid
	const char* certData = "\x30\x82\x04\xfb\x30\x82\x03\xe3\xa0\x03\x02\x01\x02\x02\x12\x04\xa8\x0d\x2a\x76\xf9\xaa\xae\x6f\xc8\xe8\x61\x55\xcd\xf7\xd5\x02\x2d\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x0b\x05\x00\x30\x32\x31\x0b\x30\x09\x06\x03\x55\x04\x06\x13\x02\x55\x53\x31\x16\x30\x14\x06\x03\x55\x04\x0a\x13\x0d\x4c\x65\x74\x27\x73\x20\x45\x6e\x63\x72\x79\x70\x74\x31\x0b\x30\x09\x06\x03\x55\x04\x03\x13\x02\x52\x33\x30\x1e\x17\x0d\x32\x34\x30\x31\x31\x36\x30\x37\x31\x34\x34\x33\x5a\x17\x0d\x32\x34\x30\x34\x31\x35\x30\x37\x31\x34\x34\x32\x5a\x30\x22\x31\x20\x30\x1e\x06\x03\x55\x04\x03\x13\x17\x77\x65\x62\x74\x65\x73\x74\x2e\x64\x72\x69\x6c\x6c\x65\x6e\x67\x69\x6e\x65\x2e\x6e\x65\x74\x30\x82\x01\x22\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x01\x05\x00\x03\x82\x01\x0f\x00\x30\x82\x01\x0a\x02\x82\x01\x01\x00\xc3\x61\x0a\x29\x7e\x54\x43\x29\x3b\x3e\xbc\xae\xdc\x5f\x9e\x99\x83\xee\xcc\x60\x58\xb2\x64\x4e\x2c\xd6\xe8\xe7\xe0\xe1\x0a\x72\x2e\xc1\xc2\x5a\x70\x25\x7a\x14\xad\x22\x72\x5a\xf3\xf5\xd7\x11\x52\xb7\x04\x48\xd6\x38\x7a\xae\xd2\x87\xb6\x53\xb8\x03\xee\x01\xc0\x1b\xa5\x5c\xf6\x54\xc9\x1d\xc8\xd0\xd5\x7d\x0d\xae\x93\x9b\x2e\xf3\x52\x8e\x94\x49\x66\xaa\x49\xea\xcb\x36\xe1\x77\x1b\x2f\x2b\xcf\x8b\x33\x78\xa7\x77\x63\x4b\x8c\x13\x38\xf6\xc1\x66\x34\xc3\xa2\xfb\x78\x9b\x2d\x9e\x5f\x02\xbc\xa5\xc7\x9f\xc5\x7d\x9a\xfa\x7e\x2b\xd8\x98\x01\x4b\x6f\x7c\xa6\x02\xc4\x74\x2b\x6c\x31\x88\x9f\xc6\xaa\x46\x57\x95\x57\x7a\x7f\x73\xd8\x5f\xc2\xd7\xfb\x0a\x58\xb0\xda\x47\x84\xce\xa8\x15\x41\x52\x92\x0b\xd2\x2f\x82\xac\xac\x53\xc8\xc9\x2d\x65\xcb\x55\x12\xe8\x74\xc9\xd5\x10\x57\xa1\xdc\x65\xff\x9b\x2b\x00\xfa\x40\xda\x12\xc3\x1b\x5c\xf5\x4c\xa3\x36\x23\x00\x54\x99\x4a\x19\xd8\x10\x95\x12\x83\x01\x29\x35\xbb\xc6\xdb\xa5\xd8\x02\xaf\x2b\xae\x8a\xc7\xbe\x78\xb4\x0e\xcb\x17\x3f\x2f\x56\x6a\xdc\x33\x2d\x2c\x28\xf0\xfd\x93\x15\xef\xbf\x02\x03\x01\x00\x01\xa3\x82\x02\x19\x30\x82\x02\x15\x30\x0e\x06\x03\x55\x1d\x0f\x01\x01\xff\x04\x04\x03\x02\x05\xa0\x30\x1d\x06\x03\x55\x1d\x25\x04\x16\x30\x14\x06\x08\x2b\x06\x01\x05\x05\x07\x03\x01\x06\x08\x2b\x06\x01\x05\x05\x07\x03\x02\x30\x0c\x06\x03\x55\x1d\x13\x01\x01\xff\x04\x02\x30\x00\x30\x1d\x06\x03\x55\x1d\x0e\x04\x16\x04\x14\x0b\x34\x74\x9a\xde\x61\x8f\x94\xd5\xa2\xa8\x41\x3e\x9d\x1a\x00\xe6\x49\xb3\x05\x30\x1f\x06\x03\x55\x1d\x23\x04\x18\x30\x16\x80\x14\x14\x2e\xb3\x17\xb7\x58\x56\xcb\xae\x50\x09\x40\xe6\x1f\xaf\x9d\x8b\x14\xc2\xc6\x30\x55\x06\x08\x2b\x06\x01\x05\x05\x07\x01\x01\x04\x49\x30\x47\x30\x21\x06\x08\x2b\x06\x01\x05\x05\x07\x30\x01\x86\x15\x68\x74\x74\x70\x3a\x2f\x2f\x72\x33\x2e\x6f\x2e\x6c\x65\x6e\x63\x72\x2e\x6f\x72\x67\x30\x22\x06\x08\x2b\x06\x01\x05\x05\x07\x30\x02\x86\x16\x68\x74\x74\x70\x3a\x2f\x2f\x72\x33\x2e\x69\x2e\x6c\x65\x6e\x63\x72\x2e\x6f\x72\x67\x2f\x30\x22\x06\x03\x55\x1d\x11\x04\x1b\x30\x19\x82\x17\x77\x65\x62\x74\x65\x73\x74\x2e\x64\x72\x69\x6c\x6c\x65\x6e\x67\x69\x6e\x65\x2e\x6e\x65\x74\x30\x13\x06\x03\x55\x1d\x20\x04\x0c\x30\x0a\x30\x08\x06\x06\x67\x81\x0c\x01\x02\x01\x30\x82\x01\x04\x06\x0a\x2b\x06\x01\x04\x01\xd6\x79\x02\x04\x02\x04\x81\xf5\x04\x81\xf2\x00\xf0\x00\x76\x00\xa2\xe2\xbf\xd6\x1e\xde\x2f\x2f\x07\xa0\xd6\x4e\x6d\x37\xa7\xdc\x65\x43\xb0\xc6\xb5\x2e\xa2\xda\xb7\x8a\xf8\x9a\x6d\xf5\x17\xd8\x00\x00\x01\x8d\x11\x56\x46\xf5\x00\x00\x04\x03\x00\x47\x30\x45\x02\x21\x00\xac\x9f\xc5\x06\x3b\x71\x88\x42\x3c\xf6\x47\x26\x36\x9c\x37\x76\x68\x36\x6c\x47\xf3\x3d\x6f\xd5\x21\xb4\xf6\x3c\x79\x7d\x2d\xdb\x02\x20\x3e\x5b\x84\x93\xef\x8c\xbc\x6f\xf9\x41\x85\xec\x3f\x97\xf4\xe4\xdc\xfe\x55\x02\xbd\xf1\xab\xd4\x04\x58\x34\x2f\xec\x82\x6b\x58\x00\x76\x00\x76\xff\x88\x3f\x0a\xb6\xfb\x95\x51\xc2\x61\xcc\xf5\x87\xba\x34\xb4\xa4\xcd\xbb\x29\xdc\x68\x42\x0a\x9f\xe6\x67\x4c\x5a\x3a\x74\x00\x00\x01\x8d\x11\x56\x47\x1f\x00\x00\x04\x03\x00\x47\x30\x45\x02\x20\x77\x3a\x19\x51\x60\xe9\x99\xb0\xed\x26\x41\xde\x48\x48\x78\xd3\xde\x87\xce\x06\x51\xb4\x4d\x6b\xac\xe9\x2b\xff\xf5\x2a\xad\x14\x02\x21\x00\x86\xa8\x06\x64\x60\x20\x1c\xbe\x59\xbb\x95\x0d\x61\x5f\x6c\xd1\x5f\xdb\x12\x90\xc3\xf1\x7d\x9b\x4a\xa4\x44\x4d\x6d\xdd\x33\x4a\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x0b\x05\x00\x03\x82\x01\x01\x00\x2e\x1e\x6a\xc0\x31\x70\x84\x55\x57\x35\x14\xdf\xaa\x9d\x00\xc2\x43\x0c\x77\x26\x8a\xf9\x73\x61\x56\x58\xae\x2d\xdc\xf7\x34\xc7\xe9\x3b\xed\x34\xed\xbf\x56\x2d\xd6\x57\xa7\xbe\x64\xf9\x98\x9d\x03\x9c\x1b\xe1\x07\x4d\x4c\x26\x5d\xc4\x59\xa0\xc4\x92\x70\x2a\x53\x16\x8a\xda\xe1\x80\xd6\x3e\xc5\x82\x8f\x03\xc6\x8f\x75\x7a\xb4\xda\x7a\x19\x17\xb5\x98\xaa\xa8\x10\x04\x93\x5a\xce\xa3\x07\xf6\x49\xb1\xec\xdf\x91\x78\x83\xf2\x0a\xbf\xdf\x24\xa5\x0a\x23\xc8\x4c\xae\x06\xf7\x9a\xc4\xd6\xb8\xfd\xe7\xcc\x01\xfe\xce\xd5\x8b\xb8\x39\x44\xbf\x32\x16\x83\x02\x7e\xfc\xd9\xe0\x2f\x5a\x16\xa6\x38\xfc\x09\x6a\x9e\x5e\x01\x03\xe8\x3b\xb4\xd6\x9a\x06\x74\x1b\x09\xcb\x9f\x9d\xcb\x99\x63\xc6\x47\x7b\x55\x48\x98\xdd\xa1\xf6\x15\xeb\x19\xea\x46\x8f\x58\x2d\x37\xc3\x88\xb9\x47\x0b\xc0\xb1\xf8\xff\xf8\x3e\x2f\x90\x68\x62\x13\x0a\x43\xe0\x05\x3e\x2d\x35\x98\xd0\xe6\x6d\xf0\xc6\x7e\x86\xaa\x5b\xd6\x04\xdc\xb3\xd2\xd9\xf0\x24\x2e\xec\x08\x91\xac\xd4\xc5\xe5\xae\x20\x0b\xe6\xe1\x61\x64\x75\xd4\x54\x81\x9b\x72\x5d\xcb\x2a\x9c\x9c\xad\xc4\x6a";
	const U32 certDataLen = 1279;
	memcpy(cert.cert, certData, certDataLen);
	cert.certLength = certDataLen;
	BigInteger::assign_hex(cert.rsaPublicKey, "010001");
	BigInteger::assign_hex(cert.rsaPrivateKey, "31EA6F65B57442AA5C8C0539A96CB680E30F95E08198A5FEC0FEF10718D175D14137D8773192A99371BE4CB806FF0680D300961423C2CF392DE965401351D1B359C8D6611F3FFF17E06FD6333A552139F2EF5066C3E4D2D96BC4144203AD61E115D0D2F00601C70333D06832D3FC0A282DD94C64FE4453E353382E2487A1B59A79CB1A4DC4EDB39A66429C585C0A570AF0F614B882DF10DEBA26E66003920E04AD808C102D9A45A0DD5FD8E70D1954631C99CCC63A0E4BD600829575AF3D6FC32862C8B907EC8D9CBF8340FAE1231B684DDC0F3A540B09201BE73A73A5DAB4C165B77FF1C1867A300E6AAB80C7654117ECA0718DBC2328412A3C2F7F580ACCC9");
	BigInteger::assign_hex(cert.rsaModulus, "C3610A297E5443293B3EBCAEDC5F9E9983EECC6058B2644E2CD6E8E7E0E10A722EC1C25A70257A14AD22725AF3F5D71152B70448D6387AAED287B653B803EE01C01BA55CF654C91DC8D0D57D0DAE939B2EF3528E944966AA49EACB36E1771B2F2BCF8B3378A777634B8C1338F6C16634C3A2FB789B2D9E5F02BCA5C79FC57D9AFA7E2BD898014B6F7CA602C4742B6C31889FC6AA465795577A7F73D85FC2D7FB0A58B0DA4784CEA8154152920BD22F82ACAC53C8C92D65CB5512E874C9D51057A1DC65FF9B2B00FA40DA12C31B5CF54CA336230054994A19D810951283012935BBC6DBA5D802AF2BAE8AC7BE78B40ECB173F2F566ADC332D2C28F0FD9315EFBF");

	acmeAgent.init("acme-v02.api.letsencrypt.org", "webserver.drillengine.net");
	ACMEAccount& account = acmeAgent.currentAccount;
	account = {};

	//run_test_get_request();
	//run_test_https_get_request();

	if (!disk_deserialize_blog()) {
		syscallProc(SYSCALL_SHUTDOWN, SYSCALL_SHUTDOWN_CODE_DEEP_SLEEP);
	}
	init_blog();

	SyscallVirtualMapArgs mapArgs;
	mapArgs.address = SAVED_IMAGE_MEMORY_OFFSET;
	mapArgs.length = SAVED_IMAGE_MEMORY_CAP;
	mapArgs.pageFlags = PAGE_PRESENT | PAGE_WRITE | PAGE_EXECUTE_DISABLE;
	if (g_syscallProc(SYSCALL_VIRTUAL_MAP, U64(&mapArgs)) != SYSCALL_VIRTUAL_MAP_SUCCESS) {
		print("Saved image memory map failed\n");
		syscallProc(SYSCALL_SHUTDOWN, SYSCALL_SHUTDOWN_CODE_DEEP_SLEEP);
	}

	while (true) {
		// Pump TCP message queue
		while (true) {
			U64 readResult = syscallProc(SYSCALL_TCP_READ, 0);
			U32 flags = readResult & TCP_NOTIFY_MASK;
			U16 localPort = (readResult >> 16) & 0xFFFF;
			U32 connectionIdx = readResult >> 32;
			if (connectionIdx == TCP_CONNECTION_BLOCK_INVALID_IDX) {
				break;
			}
			if (localPort == currentConnectionPort) {
				if (connectionIdx != acmeAgent.client.tcpConnection) {
					handle_client_connection_notification(connectionIdx, flags);
				}
			} else if (localPort == 443) {
				handle_server_request_notification(connectionIdx, flags);
			} else if (localPort == 80 && acmeAgent.httpServer.open) {
				if (flags & TCP_NOTIFY_OPENED) {
					acmeAgent.accept_http_server_connection(connectionIdx);
				}
			} else {
				g_syscallProc(SYSCALL_TCP_CLOSE, connectionIdx);
			}
			if (flags & TCP_NOTIFY_RESET) {
				print("Connection reset.\n");
			}
			if (flags & (TCP_NOTIFY_HALF_CLOSED | TCP_NOTIFY_TIMED_OUT | TCP_NOTIFY_CLOSED)) {
				//print("Closing connection.\n");
				g_syscallProc(SYSCALL_TCP_CLOSE, connectionIdx);
			}
			if (flags & TCP_NOTIFY_CLOSED) {
				print("Freeing connection.\n");
				syscallProc(SYSCALL_TCP_CLOSE, TCP_CLOSE_FREE_FLAG | connectionIdx);
			}
			//print("End of connection processing.\n");
		}

		blog_write_dirty_data();
		
		// Pump completed disk command message queue
		while (true) {
			DiskCmdCompleted completed{};
			syscallProc(SYSCALL_GET_COMPLETED_DISK_COMMAND, reinterpret_cast<U64>(&completed));
			if (completed.error == DISK_ERROR_WOULD_BLOCK) {
				break;
			}
			// Handle completed disk command
		}

		acmeAgent.do_network_io();
		if (acmeAgent.state == ACME_STATE_FAILED || acmeAgent.state == ACME_STATE_CERT_READY) {
			bool success = acmeAgent.state == ACME_STATE_CERT_READY;
			acmeAgent.state = ACME_STATE_IDLE;
			currentPortControlMask &= ~TCP_PORT_ENABLE_PORT_80_BIT;
			syscallProc(SYSCALL_TCP_PORT_CONTROL, currentPortControlMask);
			print(success ? "ACME success\n" : "ACME fail\n");
			for (U32 i = 0; i < MAX_CLIENTS; i++) {
				if (clients[i].tlsClient.tcpConnection != TCP_CONNECTION_BLOCK_INVALID_IDX && clients[i].commandMode && clients[i].commandModeState.state == CommandModeState::WAIT_CMD) {
					send_response_to_command_mode_client(clients[i].tlsClient, success ? CommandModeCmdResponse::OK : CommandModeCmdResponse::FAILURE, success ? "ACME success\n"a : "ACME fail\n"a);
					clients[i].tlsClient.send_data();
				}
			}
		}

		syscallProc(SYSCALL_SHUTDOWN, SYSCALL_SHUTDOWN_CODE_HLT);
	}
}

// Irrelevant for this OS, but MSVC won't compile without it
void _DllMainCRTStartup() {}