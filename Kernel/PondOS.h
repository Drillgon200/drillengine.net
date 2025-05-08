#pragma once
#include "Util.h"

#define TCP_CONNECTION_BLOCK_INVALID_IDX 0xFFFF
#define TCP_NOTIFY_HALF_CLOSED 0b1
#define TCP_NOTIFY_TIMED_OUT 0b10
#define TCP_NOTIFY_OPENED 0b100
#define TCP_NOTIFY_DATA 0b1000
#define TCP_NOTIFY_CLOSED 0b10000
#define TCP_NOTIFY_RESET 0b100000
#define TCP_NOTIFY_SEND_BUFFER_AVAILABLE 0b1000000
#define TCP_NOTIFY_MASK 0b1111111
// TCP_CLOSE_FREE_FLAG should not be used until TCP_NOTIFY_CLOSED is received if you want a graceful close
#define TCP_CLOSE_FREE_FLAG 0x80000000

#define USER_PROGRAM_DISK_OFFSET (512 * 1024)
#define DISK_PROTECTED_AREA_END (1024 * 1024)

enum DiskError : U32 {
	DISK_ERROR_SUCCESS = 0,
	DISK_ERROR_COMMAND_TYPE_INVALID = 1,
	DISK_ERROR_WOULD_BLOCK = 2,
	DISK_ERROR_OPERATION_FAILED = 3,
	DISK_ERROR_DATA_PTR_INVALID = 4,
	DISK_ERROR_DISK_AREA_OUT_OF_RANGE = 5,
	DISK_ERROR_TIMEOUT = 6
};

enum DiskCmdType : U32 {
	DISK_CMD_TYPE_READ = 0,
	DISK_CMD_TYPE_WRITE = 1,
	DISK_CMD_TYPE_FLUSH = 2
};

enum SyscallNum : U64 {
	SYSCALL_TIME = 0,
	SYSCALL_VIRTUAL_MAP = 1,
	SYSCALL_DISK_COMMAND = 2,
	SYSCALL_DISK_COMMAND_BLOCKING = 3,
	SYSCALL_GET_COMPLETED_DISK_COMMAND = 4,
	SYSCALL_TCP_OPEN = 5,
	SYSCALL_TCP_CLOSE = 6,
	SYSCALL_TCP_SEND = 7,
	SYSCALL_TCP_RECEIVE = 8,
	SYSCALL_TCP_ABORT = 9,
	SYSCALL_TCP_CANNIBALIZE = 10,
	SYSCALL_TCP_READ = 11,
	SYSCALL_UDP_SEND = 12,
	SYSCALL_DNS_LOOKUP_BLOCKING = 13,
	SYSCALL_TCP_PORT_CONTROL = 14,
	SYSCALL_SHUTDOWN = 15,
	SYSCALL_DEBUG_PRINT = 16,
	SYSCALL_SET_KERNEL_PROGRAM_WRITE_ENABLE = 17
};

#define TCP_PORT_ENABLE_PORT_80_BIT (1 << 0)
#define TCP_PORT_ENABLE_PORT_443_bit (1 << 1)
#define TCP_PORT_ENABLE_USER_PORT_SHIFT 2
#define TCP_PORT_ENABLE_USER_PORT_MASK 0b1111111111

enum SyscallShutdownCode {
	SYSCALL_SHUTDOWN_CODE_HLT = 0,
	SYSCALL_SHUTDOWN_CODE_RESTART = 1,
	SYSCALL_SHUTDOWN_CODE_DEEP_SLEEP = 2
};

#define DNS_LOOKUP_SYSCALL_RESULT_IP_MASK 0xFFFFFFFF
#define DNS_LOOKUP_SYSCALL_RESULT_RESULT_CODE_SHIFT 32
#define DNS_LOOKUP_SYSCALL_RESULT_RESULT_CODE_MASK 0b11
#define DNS_LOOKUP_SYSCALL_RESULT_TTL_SHIFT 34

#define DNS_LOOKUP_RESULT_NO_LOOKUP 0
#define DNS_LOOKUP_RESULT_LOOKUP_PENDING 1
#define DNS_LOOKUP_RESULT_SUCCESS 2
#define DNS_LOOKUP_RESULT_FAILURE 3

#pragma pack(push, 1)
struct SyscallDiskCommandArgs {
	U32 type;
	U32 sectorCount;
	void* data;
	U64 diskSectorOffset;
	U64 userData;
};
struct SyscallDiskCommandBlockingArgs {
	DiskCmdType type;
	U32 sectorCount;
	void* data;
	U64 diskSectorOffset;
};
struct DiskCmdCompleted {
	U64 userData;
	DiskError error;
	U32 reserved;
};
#define TCP_OPEN_SYSCALL_ARG_REMOTE_IP_SHIFT 0
#define TCP_OPEN_SYSCALL_ARG_REMOTE_PORT_SHIFT 32
#define TCP_OPEN_SYSCALL_ARG_LOCAL_PORT_SHIFT 48
struct SyscallTCPSendArgs {
	U64 bufferAddress;
	U32 bufferSize;
	U32 blockIndex;
};
struct SyscallTCPReceiveArgs {
	U64 bufferAddress;
	U32 bufferSize;
	U32 blockIndex;
};
struct SyscallTCPCannibalizeArgs {
	U32 blockIndex;
	U32 remoteIP;
	U16 remotePort;
	U16 localPort;
};
struct SyscallDNSLookupArgs {
	char name[255];
	char alwaysZero; // Set to zero no matter what
};
#pragma pack(pop)

typedef U64(__vectorcall *SyscallProc)(SyscallNum num, U64 args);

SyscallProc g_syscallProc;

void print(const char* str) {
	g_syscallProc(SYSCALL_DEBUG_PRINT, reinterpret_cast<U64>(str));
}

void print_num(U64 num) {
	char buffer[64];
	buffer[63] = '\0';
	buffer[62] = '\n';
	char* ptr = &buffer[62];
	if (!num) {
		*(--ptr) = '0';
	}
	while (num) {
		*(--ptr) = num % 10 + '0';
		num /= 10;
	}
	print(ptr);
}

I64 current_time_millis() {
	return g_syscallProc(SYSCALL_TIME, 0);
}

B32 dns_lookup(U32* ipOut, U32* ttlOut, StrA domain) {
	SyscallDNSLookupArgs dnsLookupArgs;
	if (domain.length > ARRAY_COUNT(dnsLookupArgs.name)) {
		return false;
	}
	dnsLookupArgs.alwaysZero = 0;
	memcpy(dnsLookupArgs.name, domain.str, domain.length);
	if (domain.length < ARRAY_COUNT(dnsLookupArgs.name)) {
		dnsLookupArgs.name[domain.length] = 0;
	}
	U64 lookupResult = g_syscallProc(SYSCALL_DNS_LOOKUP_BLOCKING, U64(&dnsLookupArgs));
	U32 ip = lookupResult & DNS_LOOKUP_SYSCALL_RESULT_IP_MASK;
	U32 dnsLookupResultCode = lookupResult >> DNS_LOOKUP_SYSCALL_RESULT_RESULT_CODE_SHIFT & DNS_LOOKUP_SYSCALL_RESULT_RESULT_CODE_MASK;
	U32 ttlMillis = lookupResult >> DNS_LOOKUP_SYSCALL_RESULT_TTL_SHIFT;
	if (ipOut) {
		*ipOut = ip;
	}
	if (ttlOut) {
		*ttlOut = ttlMillis;
	}
	return dnsLookupResultCode == DNS_LOOKUP_RESULT_SUCCESS;
}