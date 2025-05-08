#pragma once

#include "Util.h"
#include "TLS.h"

char COMMAND_MODE_MAGIC[]{"OS_INIT_CMD_MODE"};
const U32 COMMAND_MODE_MAGIC_LENGTH = 16;

const Byte AUTH_PUBLIC_KEY[SECP256R1_KEY_SIZE + SECP256R1_KEY_SIZE]{ 0xe0, 0x75, 0xc0, 0x16, 0xe8, 0x46, 0xf0, 0xaf, 0x19, 0x7f, 0x01, 0x97, 0x4c, 0x93, 0xf7, 0x34, 0xb4, 0xe0, 0xae, 0xc9, 0xdc, 0xd5, 0x6e, 0x7f, 0x14, 0x76, 0x09, 0x17, 0xaf, 0xe6, 0x37, 0x11, 0x17, 0x3f, 0xe0, 0x1b, 0x4b, 0x52, 0xdf, 0x56, 0x89, 0x7e, 0x70, 0xc3, 0xb7, 0xf2, 0x34, 0xcd, 0xc0, 0xda, 0x4a, 0xa9, 0x53, 0x75, 0xa7, 0x5f, 0xbd, 0xd7, 0x9e, 0x88, 0x48, 0x57, 0x78, 0x76 };

const U32 SIGN_REQUEST_DATA_LENGTH = 16;

struct CommandModeState {
	enum {
		REQUEST_AUTH,
		WAIT_AUTH,
		DISK_WRITE,
		WAIT_CMD
	} state;
	U8 signedData[SIGN_REQUEST_DATA_LENGTH];
	U64 diskWriteSectorOffset;
	U32 diskWriteSectorCount;
};
#pragma pack(push, 1)
enum CommandModeCmdType : U16 {
	COMMAND_MODE_CMD_TYPE_REQUEST_SIGNATURE = 0,
	COMMAND_MODE_CMD_TYPE_RESPONSE = 1,
	COMMAND_MODE_CMD_TYPE_CURRENT_UPTIME = 2,
	COMMAND_MODE_CMD_TYPE_WRITE_DISK = 3,
	COMMAND_MODE_CMD_TYPE_KEEPALIVE = 4,
	COMMAND_MODE_CMD_TYPE_ACME_REQUEST_CERT = 5,
	COMMAND_MODE_CMD_TYPE_GET_CERT_DATA = 6,
	COMMAND_MODE_CMD_TYPE_SET_CERT_DATA = 7
};
struct CommandModeCmdHeader {
	CommandModeCmdType type;
	U16 cmdLength;
};
struct CommandModeCmdRequestSignature {
	CommandModeCmdHeader header;
	U8 data[SIGN_REQUEST_DATA_LENGTH];
};
struct CommandModeCmdResponse {
	CommandModeCmdHeader header;
	enum Code : U16 {
		OK,
		PROGRESS,
		FAILURE
	} responseCode;
	char data[1];
};
struct CommandModeCmdCurrentUptime {
	CommandModeCmdHeader header;
};
struct CommandModeCmdWriteDisk {
	CommandModeCmdHeader header;
	U64 sectorOffset;
	U32 sectorCount;
};
struct CommandModeCmdCertData {
	CommandModeCmdHeader header;
	SECP256R1Key acmeAccountSigningKey;
	char acmeAccountURL[256];
	U32 rsaPublicKeyLength;
	U32 rsaPrivateKeyLength;
	U32 rsaModulusLength;
	U32 certLength;
	U64 rsaPublicKey[64];
	U64 rsaPrivateKey[64];
	U64 rsaModulus[64];
	U8 certData[1];
};
#pragma pack(pop)

B32 start_acme_cert_request();
X509Certificate& get_server_cert();
ACMEAccount& get_server_acme_account();

void fail_command_mode_client(TLSConnection& client, StrA responseString) {
	CommandModeCmdResponse response{};
	response.header.type = COMMAND_MODE_CMD_TYPE_RESPONSE;
	response.header.cmdLength = OFFSET_OF(CommandModeCmdResponse, data[responseString.length]);
	response.responseCode = CommandModeCmdResponse::FAILURE;
	client.write_bytes(&response, OFFSET_OF(CommandModeCmdResponse, data[0]));
	client.write_bytes(responseString.str, responseString.length);
	client.error_alert(TLS_ALERT_CLOSE_NOTIFY);
}

void send_response_to_command_mode_client(TLSConnection& client, CommandModeCmdResponse::Code responseCode, StrA responseString) {
	CommandModeCmdResponse response{};
	response.header.type = COMMAND_MODE_CMD_TYPE_RESPONSE;
	response.header.cmdLength = OFFSET_OF(CommandModeCmdResponse, data[responseString.length]);
	response.responseCode = responseCode;
	client.write_bytes(&response, OFFSET_OF(CommandModeCmdResponse, data[0]));
	client.write_bytes(responseString.str, responseString.length);
}

void handle_command_mode_client(TLSConnection& client, CommandModeState& state) {
	switch (state.state) {
	case CommandModeState::REQUEST_AUTH: {
		CommandModeCmdRequestSignature signRequest;
		signRequest.header.type = COMMAND_MODE_CMD_TYPE_REQUEST_SIGNATURE;
		signRequest.header.cmdLength = sizeof(CommandModeCmdRequestSignature);
		Keccak random; random.make_secure_random();
		random.squeeze(signRequest.data, sizeof(signRequest.data));
		memcpy(state.signedData, signRequest.data, SIGN_REQUEST_DATA_LENGTH);
		client.write_bytes(&signRequest, sizeof(signRequest));
		state.state = CommandModeState::WAIT_AUTH;
	} break;
	case CommandModeState::WAIT_AUTH: {
		const U32 SIGNATURE_SIZE = 32 + 32;
		if (client.received_user_data_size() >= SIGNATURE_SIZE) {
			if (ecdsa_verify_secpr1_sha256(client.receiveBuffer + client.receiveBufferUserReadPos, SIGNATURE_SIZE, state.signedData, SIGN_REQUEST_DATA_LENGTH, AUTH_PUBLIC_KEY)) {
				client.skip_received_bytes(SIGNATURE_SIZE);
				CommandModeCmdResponse response{};
				response.header.type = COMMAND_MODE_CMD_TYPE_RESPONSE;
				response.header.cmdLength = OFFSET_OF(CommandModeCmdResponse, data[0]);
				response.responseCode = CommandModeCmdResponse::OK;
				client.write_bytes(&response, OFFSET_OF(CommandModeCmdResponse, data[0]));
				state.state = CommandModeState::WAIT_CMD;
				goto waitCmd;
			} else {
				fail_command_mode_client(client, "Bad auth signature"a);
			}
		}
	} break;
	case CommandModeState::DISK_WRITE: {
	diskWrite:;
		// Try to write half the receive buffer at once for efficiency
		while (state.diskWriteSectorCount) {
			U32 sectorsToWriteNext = sizeof(client.receiveBuffer) / 512 / 2;
			sectorsToWriteNext = min(sectorsToWriteNext, state.diskWriteSectorCount);
			if (client.received_user_data_size() < sectorsToWriteNext * 512) {
				break;
			}
			SyscallDiskCommandBlockingArgs diskCmdArgs;
			diskCmdArgs.type = DISK_CMD_TYPE_WRITE;
			diskCmdArgs.sectorCount = sectorsToWriteNext;
			diskCmdArgs.data = client.receiveBuffer + client.receiveBufferUserReadPos;
			diskCmdArgs.diskSectorOffset = state.diskWriteSectorOffset;
			g_syscallProc(SYSCALL_SET_KERNEL_PROGRAM_WRITE_ENABLE, 1);
			DiskError result = DiskError(g_syscallProc(SYSCALL_DISK_COMMAND_BLOCKING, U64(&diskCmdArgs)));
			g_syscallProc(SYSCALL_SET_KERNEL_PROGRAM_WRITE_ENABLE, 0);
			if (result != DISK_ERROR_SUCCESS) {
				StrA responseString = "Disk write failed"a;
				CommandModeCmdResponse response{};
				response.header.type = COMMAND_MODE_CMD_TYPE_RESPONSE;
				response.header.cmdLength = OFFSET_OF(CommandModeCmdResponse, data[responseString.length]);
				response.responseCode = CommandModeCmdResponse::FAILURE;
				client.write_bytes(&response, OFFSET_OF(CommandModeCmdResponse, data[0]));
				client.write_bytes(responseString.str, responseString.length);
			} else {
				char stringBuf[64]{};
				char* numOffset = strcpy(stringBuf, "Wrote sectors: ");
				I32 writtenLength = ascii_base10_encode_u32(numOffset, stringBuf + 64 - numOffset, sectorsToWriteNext);
				numOffset[writtenLength] = 0;
				U32 totalLength = writtenLength + (numOffset - stringBuf);
				print(stringBuf);
				print("\n");
				CommandModeCmdResponse response{};
				response.header.type = COMMAND_MODE_CMD_TYPE_RESPONSE;
				response.header.cmdLength = OFFSET_OF(CommandModeCmdResponse, data[totalLength]);
				response.responseCode = CommandModeCmdResponse::PROGRESS;
				client.write_bytes(&response, OFFSET_OF(CommandModeCmdResponse, data[0]));
				client.write_bytes(stringBuf, totalLength);
			}
			state.diskWriteSectorCount -= sectorsToWriteNext;
			state.diskWriteSectorOffset += sectorsToWriteNext;
			client.skip_received_bytes(sectorsToWriteNext * 512);
		}
		if (state.diskWriteSectorCount) {
			// Still needs more data
			break;
		}
		CommandModeCmdResponse response{};
		response.header.type = COMMAND_MODE_CMD_TYPE_RESPONSE;
		response.header.cmdLength = OFFSET_OF(CommandModeCmdResponse, data[0]);
		response.responseCode = CommandModeCmdResponse::OK;
		client.write_bytes(&response, OFFSET_OF(CommandModeCmdResponse, data[0]));
		state.state = CommandModeState::WAIT_CMD;
		goto waitCmd;
	} break;
	case CommandModeState::WAIT_CMD: {
	waitCmd:;
		bool processedCommand = true;
		while (processedCommand) {
			processedCommand = false;
			CommandModeCmdHeader* cmdHeader = (CommandModeCmdHeader*)&client.receiveBuffer[client.receiveBufferUserReadPos];
			if (client.received_user_data_size() >= sizeof(CommandModeCmdHeader) && client.received_user_data_size() >= cmdHeader->cmdLength && cmdHeader->cmdLength >= sizeof(CommandModeCmdHeader)) {
				client.skip_received_bytes(cmdHeader->cmdLength);
				processedCommand = true;
				switch (cmdHeader->type) {
				case COMMAND_MODE_CMD_TYPE_CURRENT_UPTIME: {
					if (cmdHeader->cmdLength != sizeof(CommandModeCmdCurrentUptime)) {
						fail_command_mode_client(client, "Uptime command length invalid"a);
						break;
					}
					char timeBuffer[64];
					I32 encodedSize = ascii_base10_encode_u64(timeBuffer, sizeof(timeBuffer), current_time_millis());
					CommandModeCmdResponse response{};
					response.header.type = COMMAND_MODE_CMD_TYPE_RESPONSE;
					response.header.cmdLength = OFFSET_OF(CommandModeCmdResponse, data[encodedSize]);
					response.responseCode = CommandModeCmdResponse::OK;
					client.write_bytes(&response, OFFSET_OF(CommandModeCmdResponse, data[0]));
					client.write_bytes(timeBuffer, encodedSize);
				} break;
				case COMMAND_MODE_CMD_TYPE_WRITE_DISK: {
					if (cmdHeader->cmdLength != sizeof(CommandModeCmdWriteDisk)) {
						fail_command_mode_client(client, "Write disk command length invalid"a);
						break;
					}
					CommandModeCmdWriteDisk* diskWriteCmd = (CommandModeCmdWriteDisk*)cmdHeader;
					state.diskWriteSectorOffset = diskWriteCmd->sectorOffset;
					state.diskWriteSectorCount = diskWriteCmd->sectorCount;
					state.state = CommandModeState::DISK_WRITE;
					goto diskWrite;
				} break;
				case COMMAND_MODE_CMD_TYPE_KEEPALIVE: {
				} break;
				case COMMAND_MODE_CMD_TYPE_ACME_REQUEST_CERT: {
					if (start_acme_cert_request()) {
						send_response_to_command_mode_client(client, CommandModeCmdResponse::PROGRESS, "Started ACME Cert Request"a);
					} else {
						send_response_to_command_mode_client(client, CommandModeCmdResponse::FAILURE, "Failed to start ACME Cert Request"a);
					}
				} break;
				case COMMAND_MODE_CMD_TYPE_GET_CERT_DATA: {
					X509Certificate& serverCert = get_server_cert();
					ACMEAccount& acmeAccount = get_server_acme_account();
					CommandModeCmdCertData certData{};
					certData.header.type = COMMAND_MODE_CMD_TYPE_SET_CERT_DATA;
					certData.header.cmdLength = OFFSET_OF(CommandModeCmdCertData, certData[serverCert.certLength]);
					certData.acmeAccountSigningKey = acmeAccount.signingKey;
					static_assert(sizeof(certData.acmeAccountURL) == sizeof(acmeAccount.accountURL), "Wrong account url size");
					memcpy(certData.acmeAccountURL, acmeAccount.accountURL, sizeof(certData.acmeAccountURL));
					certData.rsaPublicKeyLength = serverCert.rsaPublicKey.size;
					certData.rsaPrivateKeyLength = serverCert.rsaPrivateKey.size;
					certData.rsaModulusLength = serverCert.rsaModulus.size;
					certData.certLength = serverCert.certLength;
					memcpy(certData.rsaPublicKey, serverCert.rsaPublicKey.data, certData.rsaPublicKeyLength * sizeof(certData.rsaPublicKey[0]));
					memcpy(certData.rsaPrivateKey, serverCert.rsaPrivateKey.data, certData.rsaPrivateKeyLength * sizeof(certData.rsaPrivateKey[0]));
					memcpy(certData.rsaModulus, serverCert.rsaModulus.data, certData.rsaModulusLength * sizeof(certData.rsaModulus[0]));
					client.write_bytes(&certData, OFFSET_OF(CommandModeCmdCertData, certData[0]));
					client.write_bytes(serverCert.cert, serverCert.certLength);
				} break;
				case COMMAND_MODE_CMD_TYPE_SET_CERT_DATA: {
					if (cmdHeader->cmdLength < OFFSET_OF(CommandModeCmdCertData, certData[0])) {
						fail_command_mode_client(client, "Set cert data command length invalid"a);
						break;
					}
					CommandModeCmdCertData* certData = (CommandModeCmdCertData*)cmdHeader;
					if (cmdHeader->cmdLength != OFFSET_OF(CommandModeCmdCertData, certData[certData->certLength])) {
						fail_command_mode_client(client, "Set cert data command cert length invalid"a);
						break;
					}
					print("Setting cert data\n");
					X509Certificate& serverCert = get_server_cert();
					ACMEAccount& acmeAccount = get_server_acme_account();
					memcpy(acmeAccount.accountURL, certData->acmeAccountURL, sizeof(certData->acmeAccountURL));
					acmeAccount.accountURLLength = strlen(certData->acmeAccountURL);
					acmeAccount.signingKey = certData->acmeAccountSigningKey;
					BigInteger::assign(serverCert.rsaPublicKey, certData->rsaPublicKey, certData->rsaPublicKeyLength);
					BigInteger::assign(serverCert.rsaPrivateKey, certData->rsaPrivateKey, certData->rsaPrivateKeyLength);
					BigInteger::assign(serverCert.rsaModulus, certData->rsaModulus, certData->rsaModulusLength);
					memcpy(serverCert.cert, certData->certData, certData->certLength);
					serverCert.certLength = certData->certLength;
					send_response_to_command_mode_client(client, CommandModeCmdResponse::OK, "Updated certificate and ACME data"a);
				} break;
				default: {
					fail_command_mode_client(client, "Invalid command type"a);
				} break;
				}
			}
		}
	} break;
	}
}