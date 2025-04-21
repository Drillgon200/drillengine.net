#include "Util.h"
#include "PondOS.h"
#include "HTTP.h"
#include "TLS.h"
#include "ACME.h"

const char* serverData = R"(
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
)";

ACMEState acmeState;
TLSClient testClient;
u16 currentConnectionPort;

void handle_client_connection_notification(u16 connectionIdx, u32 tcpFlags) {

}

__declspec(dllexport) void __vectorcall server_main(SyscallProc syscallProc) {
	g_syscallProc = syscallProc;
	syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("Hello from user space!\n"));
	currentConnectionPort = 4001;
	syscallProc(SYSCALL_TCP_PORT_CONTROL, TCP_PORT_ENABLE_PORT_80_BIT | TCP_PORT_ENABLE_PORT_443_bit | ((currentConnectionPort - 4000) << TCP_PORT_ENABLE_USER_PORT_SHIFT));

	ec_init();

	
	testClient.connect("google.com", currentConnectionPort);

	HTTPServer server;
	server.init();
	server.serve_resource(HTTPResource{"/", serverData, u32(strlen(serverData)) });

	while (true) {

		// Pump TCP message queue
		while (true) {
			u64 readResult = syscallProc(SYSCALL_TCP_READ, 0);
			u32 flags = readResult & TCP_NOTIFY_MASK;
			u16 localPort = (readResult >> 16) & 0xFFFF;
			u32 connectionIdx = readResult >> 32;
			if (connectionIdx == TCP_CONNECTION_BLOCK_INVALID_IDX) {
				break;
			}
			if (localPort == currentConnectionPort) {
				handle_client_connection_notification(connectionIdx, flags);
				continue;
			}
			if (flags & TCP_NOTIFY_OPENED) {
				syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("Opening connection.\n"));
				server.accept_new_connection(connectionIdx);
			}
			u32 httpServerConnectionIdx = U32_MAX;
			for (u32 i = 0; i < server.connectionCount; i++) {
				if (server.connections[i].tcpConnectionIdx == connectionIdx) {
					httpServerConnectionIdx = i;
					break;
				}
			}
			syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("After open connection.\n"));
			if (flags & (TCP_NOTIFY_HALF_CLOSED | TCP_NOTIFY_TIMED_OUT | TCP_NOTIFY_CLOSED)) {
				syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("Closing connection.\n"));
				if (httpServerConnectionIdx != U32_MAX) {
					server.close_client(server.connections[httpServerConnectionIdx]);
				}
				syscallProc(SYSCALL_TCP_CLOSE, connectionIdx);
			}
			if (flags & TCP_NOTIFY_DATA) {
				if (httpServerConnectionIdx != U32_MAX) {
					server.receive_data(server.connections[httpServerConnectionIdx]);
				}
			}
			if (flags & TCP_NOTIFY_SEND_BUFFER_AVAILABLE) {
				if (httpServerConnectionIdx != U32_MAX) {
					server.send_remaining_data(server.connections[httpServerConnectionIdx]);
				}
			}
			if (flags & TCP_NOTIFY_RESET) {
				syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("Connection reset.\n"));
			}
			if (flags & TCP_NOTIFY_CLOSED) {
				syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("Freeing connection.\n"));
				syscallProc(SYSCALL_TCP_CLOSE, TCP_CLOSE_FREE_FLAG | connectionIdx);
			}
			syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<u64>("End of connection processing.\n"));
		}

		server.take_requests();

		// Pump completed disk command message queue
		while (true) {
			DiskCmdCompleted completed{};
			syscallProc(SYSCALL_GET_COMPLETED_DISK_COMMAND, reinterpret_cast<u64>(&completed));
			if (completed.error == DISK_ERROR_WOULD_BLOCK) {
				break;
			}
			// Handle completed disk command
		}

		syscallProc(SYSCALL_SHUTDOWN, SYSCALL_SHUTDOWN_CODE_HLT);
	}
}

void _DllMainCRTStartup() {}