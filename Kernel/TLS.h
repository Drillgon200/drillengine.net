#pragma once
#include "CryptoHash.h"
#include "CryptoSignature.h"
#include "AES.h"
#include "RSA.h"
#include "ECC.h"
#include "ASN1.h"
#include "X509.h"
#include "PondOS.h"

#define TLS_DEBUG_LOG_LEVEL 1
#define TLS_DEBUG_LOG(str, level) if((level) <= TLS_DEBUG_LOG_LEVEL) { print(str); }
#define TLS_DEBUG_LOG_NUM(n, level) if((level) <= TLS_DEBUG_LOG_LEVEL) { print_num(n); }

// SHA256 of "HelloRetryRequest"
const Byte tlsHelloRetryRequestMagic[]{ 0xCF, 0x21, 0xAD, 0x74, 0xE5, 0x9A, 0x61, 0x11, 0xBE, 0x1D, 0x8C, 0x02, 0x1E, 0x65, 0xB8, 0x91, 0xC2, 0xA2, 0x11, 0x16, 0x7A, 0xBB, 0x8C, 0x5E, 0x07, 0x9E, 0x09, 0xE2, 0xC8, 0xA8, 0x33, 0x9C };
const Byte tlsTLS12NegotiationMagic[]{ 0x44, 0x4F, 0x57, 0x4E, 0x47, 0x52, 0x44, 0x01 };
const Byte tlsTLS11NegotiationMagic[]{ 0x44, 0x4F, 0x57, 0x4E, 0x47, 0x52, 0x44, 0x00 };

// RFC 8446 Section 4.2
enum TLSExtensionType : U16 {
	TLS_EXTENSION_TYPE_SERVER_NAME = 0,
	TLS_EXTENSION_TYPE_MAX_FRAGMENT_LENGTH = 1,
	TLS_EXTENSION_TYPE_STATUS_REQUEST = 5,
	TLS_EXTENSION_TYPE_SUPPORTED_GROUPS = 10,
	TLS_EXTENSION_TYPE_SIGNATURE_ALGORITHMS = 13,
	TLS_EXTENSION_TYPE_USE_SRTP = 14,
	TLS_EXTENSION_TYPE_HEARTBEAT = 15,
	TLS_EXTENSION_TYPE_APPLICATION_LAYER_PROTOCOL_NEGOTIATION = 16,
	TLS_EXTENSION_TYPE_SIGNED_CERTIFICATE_TIMESTAMP = 18,
	TLS_EXTENSION_TYPE_CLIENT_CERTIFICATE_TYPE = 19,
	TLS_EXTENSION_TYPE_SERVER_CERTIFICATE_TYPE = 20,
	TLS_EXTENSION_TYPE_PADDING = 21,
	TLS_EXTENSION_TYPE_PRE_SHARED_KEY = 41,
	TLS_EXTENSION_TYPE_EARLY_DATA = 42,
	TLS_EXTENSION_TYPE_SUPPORTED_VERSIONS = 43,
	TLS_EXTENSION_TYPE_COOKIE = 44,
	TLS_EXTENSION_TYPE_PSK_KEY_EXCHANGE_MODES = 45,
	TLS_EXTENSION_TYPE_CERTIFICATE_AUTHORITIES = 47,
	TLS_EXTENSION_TYPE_OID_FILTERS = 48,
	TLS_EXTENSION_TYPE_POST_HANDSHAKE_AUTH = 49,
	TLS_EXTENSION_TYPE_SIGNATURE_ALGORITHMS_CERT = 50,
	TLS_EXTENSION_TYPE_KEY_SHARE = 51
};

// RFC 8446 Appendix B.1
enum TLSContentType : U8 {
	TLS_CONTENT_TYPE_INVALID = 0,
	TLS_CONTENT_TYPE_CHANGE_CIPHER_SPEC = 20,
	TLS_CONTENT_TYPE_ALERT = 21,
	TLS_CONTENT_TYPE_HANDSHAKE = 22,
	TLS_CONTENT_TYPE_APPLICATION_DATA = 23,
	TLS_CONTENT_TYPE_HEARTBEAT = 24
};

// RFC 8446 Appendix B.2
enum TLSAlertDescription : U8 {
	TLS_ALERT_CLOSE_NOTIFY = 0,
	TLS_ALERT_UNEXPECTED_MESSAGE = 10,
	TLS_ALERT_BAD_RECORD_MAC = 20,
	TLS_ALERT_DECRYPTION_FAILED_RESERVED = 21,
	TLS_ALERT_RECORD_OVERFLOW = 22,
	TLS_ALERT_DECOMPRESSION_FAILURE_RESERVED = 30,
	TLS_ALERT_HANDSHAKE_FAILURE = 40,
	TLS_ALERT_NO_CERTIFICATE_RESERVED = 41,
	TLS_ALERT_BAD_CERTIFICATE = 42,
	TLS_ALERT_UNSUPPORTED_CERTIFICATE = 43,
	TLS_ALERT_CERTIFICATE_REVOKED = 44,
	TLS_ALERT_CERTIFICATE_EXPIRED = 45,
	TLS_ALERT_CERTIFICATE_UNKNOWN = 46,
	TLS_ALERT_ILLEGAL_PARAMETER = 47,
	TLS_ALERT_UNKNOWN_CA = 48,
	TLS_ALERT_ACCESS_DENIED = 49,
	TLS_ALERT_DECODE_ERROR = 50,
	TLS_ALERT_DECRYPT_ERROR = 51,
	TLS_ALERT_EXPORT_RESTRICTION_RESERVED = 60,
	TLS_ALERT_PROTOCOL_VERSION = 70,
	TLS_ALERT_INSUFFICIENT_SECURITY = 71,
	TLS_ALERT_INTERNAL_ERROR = 80,
	TLS_ALERT_INAPPROPRIATE_FALLBACK = 86,
	TLS_ALERT_USER_CANCELED = 90,
	TLS_ALERT_NO_RENEGOTIATION_RESERVED = 100,
	TLS_ALERT_MISSING_EXTENSION = 109,
	TLS_ALERT_UNSUPPORTED_EXTENSION = 110,
	TLS_ALERT_CERTIFICATE_UNOBTAINABLE_RESERVED = 111,
	TLS_ALERT_UNRECOGNIZED_NAME = 112,
	TLS_ALERT_BAD_CERTIFICATE_STATUS_RESPONSE = 113,
	TLS_ALERT_BAD_CERTIFICATE_HASH_VALUE_RESERVED = 114,
	TLS_ALERT_UNKNOWN_PSK_IDENTITY = 115,
	TLS_ALERT_CERTIFICATE_REQUIRED = 116,
	TLS_ALERT_NO_APPLICATION_PROTOCOL = 120
};

enum TLSAlertLevel : U8 {
	TLS_ALERT_LEVEL_WARNING = 1,
	TLS_ALERT_LEVEL_FATAL = 2
};

// TFC 8446 Section 4.6.3
enum TLSKeyUpdateRequest : U8 {
	TLS_KEY_UPDATE_NOT_REQUESTED = 0,
	TLS_KEY_UPDATE_REQUESTED = 1
};

// RFC 8446 Appendix B.3
enum TLSHandshakeType : U8 {
	TLS_HANDSHAKE_TYPE_HELLO_REQUEST_RESERVED = 0,
	TLS_HANDSHAKE_TYPE_CLIENT_HELLO = 1,
	TLS_HANDSHAKE_TYPE_SERVER_HELLO = 2,
	TLS_HANDSHAKE_TYPE_HELLO_VERIFY_REQUEST_RESERVED = 3,
	TLS_HANDSHAKE_TYPE_NEW_SESSION_TICKET = 4,
	TLS_HANDSHAKE_TYPE_END_OF_EARLY_DATA = 5,
	TLS_HANDSHAKE_TYPE_HELLO_RETRY_REQUEST_RESERVED = 6,
	TLS_HANDSHAKE_TYPE_ENCRYPTED_EXTENSIONS = 8,
	TLS_HANDSHAKE_TYPE_CERTIFICATE = 11,
	TLS_HANDSHAKE_TYPE_SERVER_KEY_EXCHANGE_RESERVED = 12,
	TLS_HANDSHAKE_TYPE_CERTIFICATE_REQUEST = 13,
	TLS_HANDSHAKE_TYPE_SERVER_HELLO_DONE_RESERVED = 14,
	TLS_HANDSHAKE_TYPE_CERTIFICATE_VERIFY = 15,
	TLS_HANDSHAKE_TYPE_CLIENT_KEY_EXCHANGE_RESERVED = 16,
	TLS_HANDSHAKE_TYPE_FINISHED = 20,
	TLS_HANDSHAKE_TYPE_CERTIFICATE_URL_RESERVED = 21,
	TLS_HANDSHAKE_TYPE_CERTIFICATE_STATUS_RESERVED = 22,
	TLS_HANDSHAKE_TYPE_SUPPLEMENTAL_DATA_RESERVED = 23,
	TLS_HANDSHAKE_TYPE_KEY_UPDATE = 24,
	TLS_HANDSHAKE_TYPE_MESSAGE_HASH = 254
};

// RFC 8446 Section 4.4.2
enum TLSCertificateType : U8 {
	TLS_CERTIFICATE_TYPE_X509 = 0,
	TLS_CERTIFICATE_TYPE_RAW_PUBLIC_KEY = 2
};

// RFC 8446 Appendix B.4
enum TLSCipherSuite {
	TLS_AES_128_GCM_SHA256 = 0x1301,
	TLS_AES_256_GCM_SHA384 = 0x1302,
	TLS_CHACHA20_POLY1305_SHA256 = 0x1303,
	TLS_AES_128_CCM_SHA256 = 0x1304,
	TLS_AES_128_CCM_8_SHA256 = 0x1305
};

// RFC 6066 section 3
enum TLSNameType : U8 {
	TLS_NAME_TYPE_HOST_NAME = 0
};

enum TLSNamedGroup : U16 {
	TLS_NAMED_GROUP_SECP256R1 = 0x0017,
	TLS_NAMED_GROUP_SECP384R1 = 0x0018,
	TLS_NAMED_GROUP_SECP521R1 = 0x0019,

	TLS_NAMED_GROUP_X25519 = 0x001D,
	TLS_NAMED_GROUP_X448 = 0x001E,

	TLS_NAMED_GROUP_FFDHE2048 = 0x0100,
	TLS_NAMED_GROUP_FFDHE3072 = 0x0101,
	TLS_NAMED_GROUP_FFDHE4096 = 0x0102,
	TLS_NAMED_GROUP_FFDHE6144 = 0x0103,
	TLS_NAMED_GROUP_FFDHE8192 = 0x0104

	/* FFDHE Private Use = 0x01FC..0x01FF */
	/* ECDHE Private Use = 0xFE00..0xFEFF */
};

// RFC 8446 Section 4.2.3
enum TLSSignatureScheme : U16 {
	// RSASSA-PKCS1-v1_5 algorithms
	TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA256 = 0x0401,
	TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA384 = 0x0501,
	TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA512 = 0x0601,

	// ECDSA algorithms
	TLS_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256 = 0x0403,
	TLS_SIGNATURE_SCHEME_ECDSA_SECP384R1_SHA384 = 0x0503,
	TLS_SIGNATURE_SCHEME_ECDSA_SECP521R1_SHA512 = 0x0603,

	// RSASSA-PSS algorithms with public key OID rsaEncryption
	TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256 = 0x0804,
	TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA384 = 0x0805,
	TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA512 = 0x0806,

	// EdDSA algorithms
	TLS_SIGNATURE_SCHEME_ED25519 = 0x0807,
	TLS_SIGNATURE_SCHEME_ED448 = 0x0808,

	// RSASSA-PSS algorithms with public key OID RSASSA-PSS
	TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA256 = 0x0809,
	TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA384 = 0x080a,
	TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA512 = 0x080b,

	// Legacy algorithms
	TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA1 = 0x0201,
	TLS_SIGNATURE_SCHEME_ECDSA_SHA1 = 0x0203

	// reserved for private use = 0xFE00..0xFFFF
};

// RFC 8446 Appendix A.1
enum TLSConnectionState {
	TLS_CONNECTION_STATE_CLOSED,
	TLS_CONNECTION_STATE_START,
	TLS_CLIENT_STATE_WAIT_SERVER_HELLO,
	TLS_CLIENT_STATE_WAIT_ENCRYPTED_EXTENSIONS,
	TLS_CLIENT_STATE_WAIT_CERTIFICATE_CERTIFICATE_REQUEST,
	TLS_SERVER_STATE_RECEIVED_CLIENT_HELLO,
	TLS_SERVER_STATE_NEGOTIATED, // Not necessary, but implemented to follow the RFC state diagram
	TLS_SERVER_STATE_WAIT_END_OF_EARLY_DATA, // Not relevant, since we're not doing 0-RTT
	TLS_SERVER_STATE_WAIT_FLIGHT2, // Not necessary, but implemented to follow the RFC state diagram
	TLS_CONNECTION_STATE_WAIT_CERTIFICATE, // Not used in server mode, since we don't ask for client auth
	TLS_CONNECTION_STATE_WAIT_CERTIFICATE_VERIFY, // Not used in server mode, since we don't ask for client auth
	TLS_CONNECTION_STATE_WAIT_FINISHED,
	TLS_CONNECTION_STATE_CONNECTED,
	TLS_CONNECTION_STATE_ALERT_PENDING,
	TLS_CONNECTION_STATE_CLOSE_PENDING
};

#define TLS_PROTOCOL_VERSION_1_3 0x0304
#define TLS_PROTOCOL_VERSION_1_2 0x0303
#define TLS_MAX_RECORD_DATA_LENGTH (1 << 14)
#define TLS_MAX_RECORD_AUTH_TAG_LENGTH 256
#define TLS_MAX_RECORD_LENGTH (TLS_MAX_RECORD_DATA_LENGTH + TLS_MAX_RECORD_AUTH_TAG_LENGTH)

#pragma pack(push, 1)
struct TLSRecordHeader {
	TLSContentType contentType;
	U16 protocolVersion;
	U16 length;
};
#pragma pack(pop)

struct TLSSendEntry {
	// Null if should send from internal send buffer
	const Byte* resource;
	U32 size;
};

// I'm a bit conflicted as to whether this macro is a good idea or not, since it can make control flow less obvious
// I eventually decided yes because it removes a lot of error checking noise
// I think that makes the code nicer to read and nicer to write
#define TLS_VERIFY_ALERT_RETURN(toCheck, alert) if (!(toCheck)){ error_alert(TLS_ALERT_##alert); return; } static_assert(true, "")

struct TLSConnection {
	Byte receiveBuffer[32 * 1024];
	Byte sendBuffer[16 * 1024];
	Byte sendRecord[sizeof(TLSRecordHeader) + TLS_MAX_RECORD_LENGTH];
	U32 sendBufferPos; // The current send position in the send buffer
	U32 sendBufferDataEnd; // The end of data to be sent in the send buffer
	U32 sendRecordPos; // The current buffer position to send from
	U32 sendRecordDataEnd; // The end of data to send in the send record
	U32 receiveBufferUserReadPos; // The start of data the user should be able to read
	U32 receiveBufferUserDataEnd; // The end of data the user should be able to read
	U32 receiveBufferDataEnd; // The end of meaningful data in the receive buffer (also contains partial records to be decrypted)
	U32 currentRecordReceiveLength;
	TLSRecordHeader currentReceiveHeader;

	static constexpr U32 sendQueueCap = 1 << 6;
	static constexpr U32 sendQueueMask = sendQueueCap - 1;
	TLSSendEntry sendQueue[sendQueueCap];
	U32 sendQueueWritePos;
	U32 sendQueueReadPos;

	X509Certificate* certificate;

	U32 tcpConnection;
	Byte diffieHellmanPrivateKey[SECP256R1_KEY_SIZE];
	Byte diffieHellmanPublicKey[SECP256R1_KEY_SIZE * 2];
	X509SignatureAlgorithm certSignatureAlgorithm;
	BigInteger certRSAModulusOrECDSAPointX;
	BigInteger certRSAPublicKeyOrECDSAPointY;
	Byte masterSecret[SHA256_HASH_SIZE];
	Byte serverTrafficSecret[SHA256_HASH_SIZE];
	Byte serverKey[AES_KEY_SIZE_BYTES];
	Byte serverIV[AES_GCM_IV_SIZE];
	Byte clientTrafficSecret[SHA256_HASH_SIZE];
	Byte clientKey[AES_KEY_SIZE_BYTES];
	Byte clientIV[AES_GCM_IV_SIZE];
	U64 encryptedMessageCounter;
	U64 decryptedMessageCounter;
	B32 shouldKeyUpdate;
	B32 shouldSendKeyUpdate;
	SHA256 transcriptHash;

	TLSConnectionState state;
	B32 isServer;
	TLSAlertDescription closePendingAlert;

	void send_to_tcp() {
		if (sendRecordPos == sendRecordDataEnd) {
			sendRecordPos = sendRecordDataEnd = 0;
			return;
		}
		SyscallTCPSendArgs sendArgs;
		sendArgs.bufferAddress = U64(sendRecord + sendRecordPos);
		sendArgs.bufferSize = sendRecordDataEnd - sendRecordPos;
		sendArgs.blockIndex = tcpConnection;
		I64 dataAccepted = I64(g_syscallProc(SYSCALL_TCP_SEND, U64(&sendArgs)));
		if (dataAccepted >= 0) {
			sendRecordPos += dataAccepted;
		} else {
			close_no_alert();
		}
	}

	void close_no_alert() {
		if (state == TLS_CONNECTION_STATE_CLOSED) {
			return;
		}
		TLS_DEBUG_LOG("CLOSING TCP CONNECTION\n", 2);
		g_syscallProc(SYSCALL_TCP_CLOSE, tcpConnection);
		tcpConnection = TCP_CONNECTION_BLOCK_INVALID_IDX;
		state = TLS_CONNECTION_STATE_CLOSED;
	}

	void send_alert_record(TLSAlertDescription alert) {
		if (sizeof(sendRecord) - sendRecordDataEnd < 32) {
			// No space
			return;
		}
		if (sendRecordPos == sendRecordDataEnd) {
			sendRecordPos = sendRecordDataEnd = 0;
		}
		U32 recordStart = sendRecordDataEnd;
		BigEndianByteBuf record;
		record.wrap(sendRecord + sendRecordDataEnd, sizeof(sendRecord) - sendRecordDataEnd);
		if (state > TLS_CLIENT_STATE_WAIT_SERVER_HELLO) {
			// Send alert encrypted
			// Record header
			record.write_u8(TLS_CONTENT_TYPE_APPLICATION_DATA); // type
			record.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
			record.write_u16(3); // length
			//struct {
			//	opaque content[TLSPlaintext.length];
			//	ContentType type;
			//	uint8 zeros[length_of_padding];
			//} TLSInnerPlaintext;
			//struct {
			//	AlertLevel level;
			//	AlertDescription description;
			//} Alert;
			record.write_u8(alert == TLS_ALERT_CLOSE_NOTIFY ? TLS_ALERT_LEVEL_WARNING : TLS_ALERT_LEVEL_FATAL);
			record.write_u8(alert);
			record.write_u8(TLS_CONTENT_TYPE_ALERT);
			sendRecordDataEnd += record.offset;
			// No zero padding
			encrypt_record(recordStart);
		} else {
			// Send alert unencrypted
			// Record header
			record.write_u8(TLS_CONTENT_TYPE_ALERT); // type
			record.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
			record.write_u16(2); // length
			//struct {
			//	AlertLevel level;
			//	AlertDescription description;
			//} Alert;
			record.write_u8(TLS_ALERT_LEVEL_FATAL);
			record.write_u8(alert);
		}
		send_to_tcp();
		if (state == TLS_CONNECTION_STATE_CLOSED) {
			// TCP error
			return;
		}
	}

	// Called from errors processing received messages or from a user close
	void error_alert(TLSAlertDescription alert) {
		if (alert != TLS_ALERT_CLOSE_NOTIFY) {
			TLS_DEBUG_LOG("ALERT TRIGGERED: ", 1);
			TLS_DEBUG_LOG_NUM(alert, 1);
		}
		if (state == TLS_CONNECTION_STATE_CLOSED || state == TLS_CONNECTION_STATE_ALERT_PENDING || state == TLS_CONNECTION_STATE_CLOSE_PENDING) {
			return;
		}
		closePendingAlert = alert;
		if (state != TLS_CONNECTION_STATE_CONNECTED) {
			if (alert == TLS_ALERT_CLOSE_NOTIFY) {
				// This alert is more appropriate for cancellation during a handshake
				alert = TLS_ALERT_USER_CANCELED;
			}
			// Clear data pending send, since we never completed a handshake
			sendQueueReadPos = sendQueueWritePos = 0;
			shouldKeyUpdate = shouldSendKeyUpdate = false;
			// Send immediately if in handshake
			send_alert_record(alert);
			if (state == TLS_CONNECTION_STATE_CLOSED) {
				// TCP error
				return;
			}
			if (sendRecordPos == sendRecordDataEnd) {
				close_no_alert();
				return;
			}
			state = TLS_CONNECTION_STATE_CLOSE_PENDING;
		} else {
			state = TLS_CONNECTION_STATE_ALERT_PENDING;
			send_data();
		}
	}

	void decrypt_record(U32 offset, U32 length) {
		if (length < 16) {
			error_alert(TLS_ALERT_DECRYPT_ERROR);
			return;
		}
		U8 iv[AES_GCM_IV_SIZE];
		memcpy(iv, isServer ? clientIV : serverIV, AES_GCM_IV_SIZE);
		for (U32 i = 0; i < 8; i++) {
			iv[i + 4] ^= (decryptedMessageCounter >> ((7 - i) * 8)) & 0xFF;
		}
		decryptedMessageCounter++;
		AESGCM aes;
		aes.init(isServer ? clientKey : serverKey, iv);
		B32 successfulDecrypt = aes.decrypt(receiveBuffer + offset, receiveBuffer + offset + length - 16, receiveBuffer + offset, length - 16, &currentReceiveHeader, 5);
		if (!successfulDecrypt) {
			error_alert(TLS_ALERT_DECRYPT_ERROR);
		}
	}

	void encrypt_record(U32 offset) {
		U32 fullLength = sendRecordDataEnd - offset;
		if (sendRecordDataEnd + 16 >= sizeof(sendRecord)) {
			// Should never happen
			error_alert(TLS_ALERT_INTERNAL_ERROR);
			return;
		}
		Byte* iv = isServer ? serverIV : clientIV;
		U8 modifiedIV[AES_GCM_IV_SIZE];
		memcpy(modifiedIV, iv, 4);
		for (U32 i = 4; i < AES_GCM_IV_SIZE; i++) {
			modifiedIV[i] = iv[i] ^ ((encryptedMessageCounter >> ((AES_GCM_IV_SIZE - 1 - i) * 8)) & 0xFF);
		}
		encryptedMessageCounter++;

		// Minus 5 for record header, plus 16 for auth tag
		U16 recordLength = fullLength - 5 + 16;
		// Patch record header length
		sendRecord[offset + 3] = recordLength >> 8;
		sendRecord[offset + 4] = recordLength & 0xFF;

		AESGCM aes;
		aes.init(isServer ? serverKey : clientKey, modifiedIV);
		aes.encrypt(sendRecord + offset + fullLength, sendRecord + offset + 5, sendRecord + offset + 5, fullLength - 5, sendRecord + offset, 5);
		sendRecordDataEnd += 16;
	}

	enum ServerResponseType {
		SERVER_RESPONSE_NORMAL,
		SERVER_RESPONSE_RETRY_REQUEST
	};

	// Handles the RECVD_CH and NEGOTIATED states
	// If retry request, sends only the hello part with the hello retry request random magic
	// Otherwise, sends hello, encrypted extensions, cert/cert verify, finished, and transitions to application keys
	void send_server_response_to_hello(ServerResponseType helloType, Byte* clientLegacySessionId, U32 clientLegacySessionIdLength, TLSCipherSuite chosenCipherSuite, Byte* clientPublicKey) {
		TLS_DEBUG_LOG("SEND SERVER_HELLO\n", 3);

		if (helloType == SERVER_RESPONSE_RETRY_REQUEST) {
			// Update hash according to RFC 8446 section 4.4.1
			U8 clientHello1HashData[4 + SHA256_HASH_SIZE];
			clientHello1HashData[0] = TLS_HANDSHAKE_TYPE_MESSAGE_HASH;
			clientHello1HashData[1] = 0;
			clientHello1HashData[2] = 0;
			clientHello1HashData[3] = SHA256_HASH_SIZE;
			transcriptHash.digest(clientHello1HashData + 4);
			transcriptHash.init();
			transcriptHash.update(clientHello1HashData, sizeof(clientHello1HashData));
		}

		BigEndianByteBuf hello;
		hello.wrap(sendRecord + sendRecordDataEnd, sizeof(sendRecord) - sendRecordDataEnd);

		// Record header
		hello.write_u8(TLS_CONTENT_TYPE_HANDSHAKE); // type
		hello.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
		U32 recordLengthPatchPos = hello.write_u16(0); // length

		// Write a handshake server hello message
		hello.write_u8(TLS_HANDSHAKE_TYPE_SERVER_HELLO); // msg_type
		U32 handshakeMessageLengthPatchPos = hello.write_u24(0); // length

		//struct {
		//	ProtocolVersion legacy_version = 0x0303;    /* TLS v1.2 */
		//	Random random;
		//	opaque legacy_session_id_echo<0..32>;
		//	CipherSuite cipher_suite;
		//	uint8 legacy_compression_method = 0;
		//	Extension extensions<6..2^16-1>;
		//} ServerHello;
		hello.write_u16(TLS_PROTOCOL_VERSION_1_2); // legacy_version
		Keccak random;
		random.make_secure_random();
		if (helloType == SERVER_RESPONSE_RETRY_REQUEST) {
			hello.write_bytes(tlsHelloRetryRequestMagic, sizeof(tlsHelloRetryRequestMagic));
		} else {
			random.squeeze(hello.bytes + hello.offset, 32); // random
			hello.offset += 32;
		}
		hello.write_u8(clientLegacySessionIdLength);
		hello.write_bytes(clientLegacySessionId, clientLegacySessionIdLength); // legacy_session_id_echo
		hello.write_u16(chosenCipherSuite); // cipher_suite
		hello.write_u8(0); // legacy_compression_method

		// Write extensions
		U32 extensionsPatchPos = hello.write_u16(0);

		//struct {
		//	ExtensionType extension_type;
		//	opaque extension_data<0..2^16-1>;
		//} Extension;

		{
			// This extension tells the server that we're using TLS 1.3, not 1.2
			hello.write_u16(TLS_EXTENSION_TYPE_SUPPORTED_VERSIONS);
			hello.write_u16(2);
			//struct {
			//	select (Handshake.msg_type) {
			//		case client_hello:
			//			 ProtocolVersion versions<2..254>;
			//
			//		case server_hello: /* and HelloRetryRequest */
			//			 ProtocolVersion selected_version;
			//	};
			//} SupportedVersions;
			hello.write_u16(TLS_PROTOCOL_VERSION_1_3);
		}

		{
			hello.write_u16(TLS_EXTENSION_TYPE_KEY_SHARE);
			U32 extensionPatchPos = hello.write_u16(0);
			if (helloType == SERVER_RESPONSE_RETRY_REQUEST) {
				//struct {
				//	NamedGroup selected_group;
				//} KeyShareHelloRetryRequest;
				hello.write_u16(TLS_NAMED_GROUP_SECP256R1);
			} else {
				//struct {
				//	NamedGroup group;
				//	opaque key_exchange<1..2^16-1>;
				//} KeyShareEntry;
				//struct {
				//	KeyShareEntry server_share;
				//} KeyShareServerHello;
				hello.write_u16(TLS_NAMED_GROUP_SECP256R1);
				U32 keyExchangePatchPos = hello.write_u16(0);
				// Write public key
				// 4 means the key is not sent compressed
				// I'm not figuring out how to write code to compress a key that is only 64 bytes long
				hello.write_u8(4);
				hello.write_bytes(diffieHellmanPublicKey, 32 + 32);
				hello.patch_u16(keyExchangePatchPos, hello.offset - keyExchangePatchPos - 2);
			}
			hello.patch_u16(extensionPatchPos, hello.offset - extensionPatchPos - 2);
		}

		hello.patch_u16(extensionsPatchPos, hello.offset - extensionsPatchPos - 2);
		hello.patch_u24(handshakeMessageLengthPatchPos, hello.offset - handshakeMessageLengthPatchPos - 3);
		hello.patch_u16(recordLengthPatchPos, hello.offset - recordLengthPatchPos - 2);

		transcriptHash.update(hello.bytes + 5, hello.offset - 5);

		sendRecordDataEnd += hello.offset;

		if (helloType == SERVER_RESPONSE_RETRY_REQUEST) {
			send_data();
			state = TLS_CONNECTION_STATE_START;
			if (tcpConnection == TCP_CONNECTION_BLOCK_INVALID_IDX) {
				// Send error occured
				state = TLS_CONNECTION_STATE_CLOSED;
			}
			return;
		}

		// Handshake key calc
		const U32 hashLength = SHA256_HASH_SIZE;
		const U32 encryptionKeySize = AES_KEY_SIZE_BYTES;
		U8 clientToServerHelloHash[hashLength];
		SHA256 tmpHash;
		tmpHash.copy_from(transcriptHash);
		tmpHash.digest(clientToServerHelloHash);

		U8 earlySecret[hashLength];
		U8 zeros[hashLength]{};
		U8 emptyHash[hashLength];
		sha256(emptyHash, "", 0);
		// early secret
		sha256_hkdf_extract(earlySecret, "", 0, zeros, hashLength);
		// handshake secret
		U8 derivedSecret[hashLength];
		sha256_derive_secret(derivedSecret, hashLength, earlySecret, hashLength, "derived", 7, emptyHash, hashLength);

		U8 sharedSecret[hashLength];
		B32 keyCalcSuccess = secp256r1_ecdhe(sharedSecret, clientPublicKey, clientPublicKey + 32, diffieHellmanPrivateKey);
		TLS_VERIFY_ALERT_RETURN(keyCalcSuccess, HANDSHAKE_FAILURE);

		U8 handshakeSecret[hashLength];
		sha256_hkdf_extract(handshakeSecret, derivedSecret, hashLength, sharedSecret, 32);
		// Calc client and server handshake keys
		sha256_derive_secret(clientTrafficSecret, hashLength, handshakeSecret, hashLength, "c hs traffic", 12, clientToServerHelloHash, hashLength);
		sha256_derive_secret(serverTrafficSecret, hashLength, handshakeSecret, hashLength, "s hs traffic", 12, clientToServerHelloHash, hashLength);
		sha256_hkdf_expand_label(clientKey, encryptionKeySize, clientTrafficSecret, hashLength, "key", 3, "", 0);
		sha256_hkdf_expand_label(serverKey, encryptionKeySize, serverTrafficSecret, hashLength, "key", 3, "", 0);
		sha256_hkdf_expand_label(clientIV, 12, clientTrafficSecret, hashLength, "iv", 2, "", 0);
		sha256_hkdf_expand_label(serverIV, 12, serverTrafficSecret, hashLength, "iv", 2, "", 0);

		// master secret
		sha256_derive_secret(derivedSecret, hashLength, handshakeSecret, hashLength, "derived", 7, emptyHash, hashLength);
		sha256_hkdf_extract(masterSecret, derivedSecret, hashLength, zeros, hashLength);

		encryptedMessageCounter = 0;
		decryptedMessageCounter = 0;

		// Change cipher spec isn't really necessary, but we'll send it for middlebox compatibility
		// Should be sent sometime between ClientHello and Finished
		BigEndianByteBuf& changeCipherSpec = hello;
		changeCipherSpec.wrap(sendRecord + sendRecordDataEnd, sizeof(sendRecord) - sendRecordDataEnd);
		// Record header
		changeCipherSpec.write_u8(TLS_CONTENT_TYPE_CHANGE_CIPHER_SPEC); // type
		changeCipherSpec.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
		changeCipherSpec.write_u16(1); // length
		changeCipherSpec.write_u8(1); // Change cipher spec is defined to contain exactly one byte, 0x01
		sendRecordDataEnd += changeCipherSpec.offset;


		BigEndianByteBuf& handshakeResponse = hello;
		handshakeResponse.wrap(sendRecord + sendRecordDataEnd, sizeof(sendRecord) - sendRecordDataEnd);
		U32 handshakeResponseOffset = sendRecordDataEnd;
		// Record header
		handshakeResponse.write_u8(TLS_CONTENT_TYPE_APPLICATION_DATA); // type
		handshakeResponse.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
		recordLengthPatchPos = handshakeResponse.write_u16(0); // length

		U32 transcriptHashPos = handshakeResponse.offset;

		// EncryptedExtensions
		handshakeResponse.write_u8(TLS_HANDSHAKE_TYPE_ENCRYPTED_EXTENSIONS); // msg_type
		U32 encryptedExtensionsPatchPos = handshakeResponse.write_u24(0); // length
		//struct {
		//	Extension extensions<0..2^16-1>;
		//} EncryptedExtensions;
		handshakeResponse.write_u16(0); // no encrypted extensions
		handshakeResponse.patch_u24(encryptedExtensionsPatchPos, handshakeResponse.offset - encryptedExtensionsPatchPos - 3);

		if (certificate) {
			// Certificate
			handshakeResponse.write_u8(TLS_HANDSHAKE_TYPE_CERTIFICATE);
			U32 certificatePatchPos = handshakeResponse.write_u24(0);
			//struct {
			//	opaque certificate_request_context<0..2^8-1>;
			//	CertificateEntry certificate_list<0..2^24-1>;
			//} Certificate;
			handshakeResponse.write_u8(0); // no request context
			U32 certificateListPatchPos = handshakeResponse.write_u24(0);
			//struct {
			//	select (certificate_type) {
			//    case RawPublicKey:
			//      /* From RFC 7250 ASN.1_subjectPublicKeyInfo */
			//      opaque ASN1_subjectPublicKeyInfo<1..2^24-1>;
			//    case X509:
			//      opaque cert_data<1..2^24-1>;
			//	};
			//	Extension extensions<0..2^16-1>;
			//} CertificateEntry;
			// We're always X509
			handshakeResponse.write_u24(certificate->certLength);
			handshakeResponse.write_bytes(certificate->cert, certificate->certLength);
			handshakeResponse.write_u16(0); // no extensions
			handshakeResponse.patch_u24(certificateListPatchPos, handshakeResponse.offset - certificateListPatchPos - 3);
			handshakeResponse.patch_u24(certificatePatchPos, handshakeResponse.offset - certificatePatchPos - 3);
			transcriptHash.update(handshakeResponse.bytes + transcriptHashPos, handshakeResponse.offset - transcriptHashPos);
			transcriptHashPos = handshakeResponse.offset;

			// CertificateVerify
			handshakeResponse.write_u8(TLS_HANDSHAKE_TYPE_CERTIFICATE_VERIFY);
			U32 certificateVerifyPatchPos = handshakeResponse.write_u24(0);
			//struct {
			//	SignatureScheme algorithm;
			//	opaque signature<0..2^16-1>;
			//} CertificateVerify;
			handshakeResponse.write_u16(TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256);
			U32 signatureLengthPatchPos = handshakeResponse.write_u16(0);
			// The signature is over 64 0x20s followed by "TLS 1.3, server CertificateVerify" followed by a null separator followed by the transcript hash
			Byte toSign[64 + 33 + 1 + SHA256_HASH_SIZE];
			memcpy(toSign, "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20TLS 1.3, server CertificateVerify", 64 + 33 + 1);
			tmpHash.copy_from(transcriptHash);
			tmpHash.digest(toSign + 64 + 33 + 1);
			const U32 rsaBits = 2048;
			const U32 signatureLength = rsaBits / 8;
			rsassa_pss_sign_sha256(handshakeResponse.bytes + handshakeResponse.offset, toSign, sizeof(toSign), random, certificate->rsaPrivateKey, certificate->rsaModulus, rsaBits);
			//rsassa_pkcs1_sign_sha256(handshakeResponse.bytes + handshakeResponse.offset, toSign, sizeof(toSign), certificate->rsaPrivateKey, certificate->rsaModulus, rsaBits);
			handshakeResponse.offset += signatureLength;
			handshakeResponse.patch_u16(signatureLengthPatchPos, handshakeResponse.offset - signatureLengthPatchPos - 2);
			handshakeResponse.patch_u24(certificateVerifyPatchPos, handshakeResponse.offset - certificateVerifyPatchPos - 3);
		}
		transcriptHash.update(handshakeResponse.bytes + transcriptHashPos, handshakeResponse.offset - transcriptHashPos);
		transcriptHashPos = handshakeResponse.offset;

		handshakeResponse.write_u8(TLS_HANDSHAKE_TYPE_FINISHED);
		U32 finishedPatchPos = handshakeResponse.write_u24(0);
		// finished_key = HKDF-Expand-Label(BaseKey, "finished", "", Hash.length)
		// verify_data = HMAC(finished_key, Transcript-Hash(Handshake Context, Certificate*, CertificateVerify*))
		//struct {
		//	opaque verify_data[Hash.length];
		//} Finished;
		Byte finishedKey[SHA256_HASH_SIZE];
		sha256_hkdf_expand_label(finishedKey, sizeof(finishedKey), serverTrafficSecret, SHA256_HASH_SIZE, "finished", 8, "", 0);
		Byte helloToNowHash[SHA256_HASH_SIZE];
		tmpHash.copy_from(transcriptHash);
		tmpHash.digest(helloToNowHash);
		Byte verifyData[SHA256_HASH_SIZE];
		sha256_hmac(verifyData, finishedKey, sizeof(finishedKey), helloToNowHash, sizeof(helloToNowHash));
		handshakeResponse.write_bytes(verifyData, sizeof(verifyData));
		handshakeResponse.patch_u24(finishedPatchPos, handshakeResponse.offset - finishedPatchPos - 3);
		transcriptHash.update(handshakeResponse.bytes + transcriptHashPos, handshakeResponse.offset - transcriptHashPos);

		handshakeResponse.write_u8(TLS_CONTENT_TYPE_HANDSHAKE); // real content type
		handshakeResponse.patch_u16(recordLengthPatchPos, handshakeResponse.offset - recordLengthPatchPos - 2);
		sendRecordDataEnd += handshakeResponse.offset;
		encrypt_record(handshakeResponseOffset);


		send_data();
		if (tcpConnection == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			// Send error occured
			state = TLS_CONNECTION_STATE_CLOSED;
			return;
		}
		state = TLS_SERVER_STATE_WAIT_FLIGHT2;
		// No client auth
		state = TLS_CONNECTION_STATE_WAIT_FINISHED;
	}

	void send_client_hello(StrA serverName, Keccak& random) {
		TLS_DEBUG_LOG("SEND CLIENT_HELLO\n", 3);

		BigEndianByteBuf hello;
		hello.wrap(sendRecord + sendRecordDataEnd, sizeof(sendRecord) - sendRecordDataEnd);

		// Record header
		hello.write_u8(TLS_CONTENT_TYPE_HANDSHAKE); // type
		hello.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
		U32 recordLengthPatchPos = hello.write_u16(0); // length

		// Write a handshake client hello message
		hello.write_u8(TLS_HANDSHAKE_TYPE_CLIENT_HELLO); // msg_type
		U32 handshakeMessageLengthPatchPos = hello.write_u24(0); // length

		//struct {
		//	  ProtocolVersion legacy_version = 0x0303;    /* TLS v1.2 */
		//	  Random random;
		//	  opaque legacy_session_id<0..32>;
		//	  CipherSuite cipher_suites<2..2^16-2>;
		//	  opaque legacy_compression_methods<1..2^8-1>;
		//    Extension extensions<8..2^16-1>;
		//} ClientHello;
		hello.write_u16(TLS_PROTOCOL_VERSION_1_2); // legacy_version
		random.squeeze(hello.bytes + hello.offset, 32); // random
		hello.offset += 32;
		// Write a random 32 byte legacy_session_id
		hello.write_u8(32);
		random.squeeze(hello.bytes + hello.offset, 32);
		hello.offset += 32;
		// cipher_suites
		U32 cipherSuitePatchPos = hello.write_u16(0);
		hello.write_u16(TLS_AES_128_GCM_SHA256);
		hello.patch_u16(cipherSuitePatchPos, hello.offset - cipherSuitePatchPos - 2);
		// Write null for legacy_compression_methods
		hello.write_u8(1);
		hello.write_u8(0);

		// Write extensions
		U32 extensionsPatchPos = hello.write_u16(0);

		//struct {
		//	ExtensionType extension_type;
		//	opaque extension_data<0..2^16-1>;
		//} Extension;

		if (serverName.str) {
			hello.write_u16(TLS_EXTENSION_TYPE_SERVER_NAME);

			U32 extensionPatchPos = hello.write_u16(0);
			// RFC 6066 Section 3
			//struct {
			//  ServerName server_name_list<1..2^16-1>
			//} ServerNameList;
			U32 serverNameListPatchPos = hello.write_u16(0);
			//struct {
			//	NameType name_type;
			//	select (name_type) {
			//		case host_name: HostName;
			//	} name;
			//} ServerName;
			//opaque HostName<1..2^16-1>;
			hello.write_u8(TLS_NAME_TYPE_HOST_NAME); // name_type
			hello.write_u16(serverName.length);
			hello.write_bytes(serverName.str, serverName.length); // host_name

			hello.patch_u16(serverNameListPatchPos, hello.offset - serverNameListPatchPos - 2);
			hello.patch_u16(extensionPatchPos, hello.offset - extensionPatchPos - 2);
		}

		{
			// This extension tells the server that we're using TLS 1.3, not 1.2
			hello.write_u16(TLS_EXTENSION_TYPE_SUPPORTED_VERSIONS);
			U32 extensionPatchPos = hello.write_u16(0);
			//struct {
			//	select (Handshake.msg_type) {
			//		case client_hello:
			//			 ProtocolVersion versions<2..254>;
			//
			//		case server_hello: /* and HelloRetryRequest */
			//			 ProtocolVersion selected_version;
			//	};
			//} SupportedVersions;
			U32 versionsPatchPos = hello.write_u8(0);
			hello.write_u16(TLS_PROTOCOL_VERSION_1_3);

			hello.patch_u8(versionsPatchPos, hello.offset - versionsPatchPos - 1);
			hello.patch_u16(extensionPatchPos, hello.offset - extensionPatchPos - 2);
		}

		{
			hello.write_u16(TLS_EXTENSION_TYPE_SUPPORTED_GROUPS);
			U32 extensionPatchPos = hello.write_u16(0);
			//struct {
			//  NamedGroup named_group_list<2..2^16-1>;
			//} NamedGroupList;
			U32 namedGroupPatchPos = hello.write_u16(0);
			hello.write_u16(TLS_NAMED_GROUP_SECP256R1);

			hello.patch_u16(namedGroupPatchPos, hello.offset - namedGroupPatchPos - 2);
			hello.patch_u16(extensionPatchPos, hello.offset - extensionPatchPos - 2);
		}

		{
			hello.write_u16(TLS_EXTENSION_TYPE_SIGNATURE_ALGORITHMS);
			U32 extensionPatchPos = hello.write_u16(0);
			//struct {
			//	SignatureScheme supported_signature_algorithms<2..2^16-2>;
			//} SignatureSchemeList;
			U32 signatureAndHashAlgorithmsPatchPos = hello.write_u16(0);
			hello.write_u16(TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA256);
			hello.write_u16(TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256);
			hello.write_u16(TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA256);
			hello.write_u16(TLS_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256);

			hello.patch_u16(signatureAndHashAlgorithmsPatchPos, hello.offset - signatureAndHashAlgorithmsPatchPos - 2);
			hello.patch_u16(extensionPatchPos, hello.offset - extensionPatchPos - 2);
		}

		{
			hello.write_u16(TLS_EXTENSION_TYPE_KEY_SHARE);
			U32 extensionPatchPos = hello.write_u16(0);
			//struct {
			//	NamedGroup group;
			//	opaque key_exchange<1..2^16-1>;
			//} KeyShareEntry;
			//struct {
			//	KeyShareEntry client_shares<0..2^16-1>;
			//} KeyShareClientHello;
			U32 clientSharesPatchPos = hello.write_u16(0);
			hello.write_u16(TLS_NAMED_GROUP_SECP256R1);
			U32 keyExchangePatchPos = hello.write_u16(0);
			// Write public key
			// 4 means the key is not sent compressed
			// I'm not figuring out how to write code to compress a key that is only 64 bytes long
			hello.write_u8(4);
			hello.write_bytes(diffieHellmanPublicKey, 32 + 32);
			hello.patch_u16(keyExchangePatchPos, hello.offset - keyExchangePatchPos - 2);

			hello.patch_u16(clientSharesPatchPos, hello.offset - clientSharesPatchPos - 2);
			hello.patch_u16(extensionPatchPos, hello.offset - extensionPatchPos - 2);
		}

		hello.patch_u16(extensionsPatchPos, hello.offset - extensionsPatchPos - 2);
		hello.patch_u24(handshakeMessageLengthPatchPos, hello.offset - handshakeMessageLengthPatchPos - 3);
		hello.patch_u16(recordLengthPatchPos, hello.offset - recordLengthPatchPos - 2);

		transcriptHash.update(hello.bytes + 5, hello.offset - 5);

		sendRecordDataEnd += hello.offset;

		send_data();
		if (tcpConnection == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			// Send error occured
			state = TLS_CONNECTION_STATE_CLOSED;
			return;
		}
		state = TLS_CLIENT_STATE_WAIT_SERVER_HELLO;
	}

	void send_client_change_cipher_spec_and_finished() {
		TLS_DEBUG_LOG("SEND CHANGE_CIPHER_SPEC AND FINISHED\n", 3);
		BigEndianByteBuf sendBuf;
		sendBuf.wrap(sendRecord + sendRecordDataEnd, sizeof(sendRecord) - sendRecordDataEnd);

		// Send change cipher spec (dummy byte of 0x01)
		// Record header
		sendBuf.write_u8(TLS_CONTENT_TYPE_CHANGE_CIPHER_SPEC); // type
		sendBuf.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
		sendBuf.write_u16(1); // length
		sendBuf.write_u8(1);

		// Send Finished
		U32 finishedOffset = sendRecordDataEnd + sendBuf.offset;
		// Record header
		sendBuf.write_u8(TLS_CONTENT_TYPE_APPLICATION_DATA); // type
		sendBuf.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
		U32 recordLengthPatchPos = sendBuf.write_u16(0); // length
		//struct {
		//	opaque content[TLSPlaintext.length];
		//	ContentType type;
		//	uint8 zeros[length_of_padding];
		//} TLSInnerPlaintext;
		sendBuf.write_u8(TLS_HANDSHAKE_TYPE_FINISHED);
		sendBuf.write_u24(SHA256_HASH_SIZE);
		//struct {
		//	opaque verify_data[Hash.length];
		//} Finished;

		// Calculate verify data
		Byte helloToServerFinishedHash[SHA256_HASH_SIZE];
		SHA256 tmpHash;
		tmpHash.copy_from(transcriptHash);
		tmpHash.digest(helloToServerFinishedHash);
		Byte finishedKey[SHA256_HASH_SIZE];
		sha256_hkdf_expand_label(finishedKey, sizeof(finishedKey), clientTrafficSecret, SHA256_HASH_SIZE, "finished", 8, "", 0);
		Byte verifyData[SHA256_HASH_SIZE];
		sha256_hmac(verifyData, finishedKey, sizeof(finishedKey), helloToServerFinishedHash, SHA256_HASH_SIZE);

		sendBuf.write_bytes(verifyData, sizeof(verifyData));

		sendBuf.write_u8(TLS_CONTENT_TYPE_HANDSHAKE);
		// No zero padding
		sendBuf.patch_u16(recordLengthPatchPos, sendBuf.offset - recordLengthPatchPos - 2);

		sendRecordDataEnd += sendBuf.offset;
		encrypt_record(finishedOffset);

		send_data();
	}

	void init(X509Certificate* cert) {
		state = TLS_CONNECTION_STATE_CLOSED;
		sendBufferPos = 0;
		sendBufferDataEnd = 0;
		receiveBufferUserReadPos = 0;
		receiveBufferUserDataEnd = 0;
		receiveBufferDataEnd = 0;
		sendRecordPos = 0;
		sendRecordDataEnd = 0;
		currentRecordReceiveLength = 0;
		certificate = cert;
		tcpConnection = TCP_CONNECTION_BLOCK_INVALID_IDX;
		certRSAModulusOrECDSAPointX.init(64);
		certRSAPublicKeyOrECDSAPointY.init(64);
		BigInteger::zero(certRSAModulusOrECDSAPointX);
		BigInteger::zero(certRSAPublicKeyOrECDSAPointY);
		encryptedMessageCounter = 0;
		decryptedMessageCounter = 0;
		shouldKeyUpdate = false;
		shouldSendKeyUpdate = false;
		sendQueueWritePos = 0;
		sendQueueReadPos = 0;
		transcriptHash.init();
	}

	B32 accept_connection_from_client(U32 client, X509Certificate* cert) {
		TLS_DEBUG_LOG("ACCEPTING CLIENT\n", 3);
		init(cert);
		isServer = true;
		tcpConnection = client;
		Keccak random;
		random.make_secure_random();
		secp256r1_generate_keypair(random, diffieHellmanPublicKey, diffieHellmanPrivateKey);
		state = TLS_CONNECTION_STATE_START;
		receive_data();
		return true;
	}

	B32 connect_to_server(StrA remoteServerName, U32 remoteIP, U16 remotePort, U16 localPort) {
		init(nullptr);
		isServer = false;

		tcpConnection = g_syscallProc(SYSCALL_TCP_OPEN, U64(remoteIP) << TCP_OPEN_SYSCALL_ARG_REMOTE_IP_SHIFT | U64(remotePort) << TCP_OPEN_SYSCALL_ARG_REMOTE_PORT_SHIFT | U64(localPort) << TCP_OPEN_SYSCALL_ARG_LOCAL_PORT_SHIFT);
		if (tcpConnection == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			print("TCP open failed\n");
			return false;
		}

		Keccak random;
		random.make_secure_random();
		secp256r1_generate_keypair(random, diffieHellmanPublicKey, diffieHellmanPrivateKey);
		send_client_hello(remoteServerName, random);

		state = TLS_CLIENT_STATE_WAIT_SERVER_HELLO;
		return true;
	}

	B32 connect_to_server(StrA remoteServerName, U16 remotePort, U16 localPort) {
		SyscallDNSLookupArgs dnsLookupArgs;
		if (remoteServerName.length > ARRAY_COUNT(dnsLookupArgs.name)) {
			return false;
		}
		dnsLookupArgs.alwaysZero = 0;
		memcpy(dnsLookupArgs.name, remoteServerName.str, remoteServerName.length);
		if (remoteServerName.length < ARRAY_COUNT(dnsLookupArgs.name)) {
			dnsLookupArgs.name[remoteServerName.length] = 0;
		}
		TLS_DEBUG_LOG("ATTEMPTING CONNECTION TO: ", 2);
		TLS_DEBUG_LOG(dnsLookupArgs.name, 2);
		TLS_DEBUG_LOG("\n", 2);
		U64 lookupResult = g_syscallProc(SYSCALL_DNS_LOOKUP_BLOCKING, U64(&dnsLookupArgs));
		U32 ip = lookupResult & DNS_LOOKUP_SYSCALL_RESULT_IP_MASK;
		U32 dnsLookupResultCode = lookupResult >> DNS_LOOKUP_SYSCALL_RESULT_RESULT_CODE_SHIFT & DNS_LOOKUP_SYSCALL_RESULT_RESULT_CODE_MASK;
		U32 ttlMillis = lookupResult >> DNS_LOOKUP_SYSCALL_RESULT_TTL_SHIFT;
		if (dnsLookupResultCode != DNS_LOOKUP_RESULT_SUCCESS) {
			print("DNS lookup failed\n");
			print_num(lookupResult);
			return false;
		}
		return connect_to_server(remoteServerName, ip, remotePort, localPort);
	}

	// RFC 8446 Appendix A.1 (client state machine)
	//                            START <----+
	//            Send ClientHello |        | Recv HelloRetryRequest
	//       [K_send = early data] |        |
	//                             v        |
	//        /                 WAIT_SH ----+
	//        |                    | Recv ServerHello
	//        |                    | K_recv = handshake
	//    Can |                    V
	//   send |                 WAIT_EE
	//  early |                    | Recv EncryptedExtensions
	//   data |           +--------+--------+
	//        |     Using |                 | Using certificate
	//        |       PSK |                 v
	//        |           |            WAIT_CERT_CR
	//        |           |        Recv |       | Recv CertificateRequest
	//        |           | Certificate |       v
	//        |           |             |    WAIT_CERT
	//        |           |             |       | Recv Certificate
	//        |           |             v       v
	//        |           |              WAIT_CV
	//        |           |                 | Recv CertificateVerify
	//        |           +> WAIT_FINISHED <+
	//        |                  | Recv Finished
	//        \                  | [Send EndOfEarlyData]
	//                           | K_send = handshake
	//                           | [Send Certificate [+ CertificateVerify]]
	// Can send                  | Send Finished
	// app data   -->            | K_send = K_recv = application
	// after here                v
	//                       CONNECTED
	//
	// 
	// 
	// RFC 8446 Appendix A.2 (server state machine)
	//                              START <-----+
	//	             Recv ClientHello |         | Send HelloRetryRequest
	//								  v         |
	//							   RECVD_CH ----+
	//								  | Select parameters
	//								  v
	//							   NEGOTIATED
	//								  | Send ServerHello
	//								  | K_send = handshake
	//								  | Send EncryptedExtensions
	//								  | [Send CertificateRequest]
	// Can send                       | [Send Certificate + CertificateVerify]
	// app data                       | Send Finished
	// after   -->                    | K_send = application
	// here                  +--------+--------+
	//			    No 0-RTT |                 | 0-RTT
	//					     |                 |
	//   K_recv = handshake  |                 | K_recv = early data
	// [Skip decrypt errors] |    +------> WAIT_EOED -+
	//					     |    |       Recv |      | Recv EndOfEarlyData
	//					     |    | early data |      | K_recv = handshake
	//					     |    +------------+      |
	//					     |                        |
	//					     +> WAIT_FLIGHT2 <--------+
	//								|
	//					   +--------+--------+
	//			   No auth |                 | Client auth
	//					   |                 |
	//					   |                 v
	//					   |             WAIT_CERT
	//					   |        Recv |       | Recv Certificate
	//					   |       empty |       v
	//					   | Certificate |    WAIT_CV
	//					   |             |       | Recv
	//					   |             v       | CertificateVerify
	//					   +-> WAIT_FINISHED <---+
	//								| Recv Finished
	//								| K_recv = application
	//								v
	//							CONNECTED
	void process_handshake_messages(BigEndianByteBuf& handshakeMessages) {
		while (handshakeMessages.offset != handshakeMessages.capacity) {
			TLSHandshakeType handshakeType = static_cast<TLSHandshakeType>(handshakeMessages.read_u8());
			U32 handshakeMessageDataLength = handshakeMessages.read_u24();
			TLS_VERIFY_ALERT_RETURN(handshakeMessages.has_data_left(handshakeMessageDataLength), DECODE_ERROR);
			U32 handshakeMessagesCapacity = handshakeMessages.capacity;
			handshakeMessages.capacity = handshakeMessages.offset + handshakeMessageDataLength;
			switch (state) {
			case TLS_CLIENT_STATE_WAIT_ENCRYPTED_EXTENSIONS: {
				TLS_VERIFY_ALERT_RETURN(handshakeType == TLS_HANDSHAKE_TYPE_ENCRYPTED_EXTENSIONS, DECODE_ERROR); 
				TLS_DEBUG_LOG("  RECEIVE ENCRYPTED_EXTENSIONS\n", 3);
				//struct {
				//	Extension extensions<0..2^16-1>;
				//} EncryptedExtensions;
				U16 encryptedExtensionsEnd = handshakeMessages.read_u16();
				encryptedExtensionsEnd += handshakeMessages.offset;
				while (!handshakeMessages.failed && handshakeMessages.offset < encryptedExtensionsEnd) {
					//struct {
					//	ExtensionType extension_type;
					//	opaque extension_data<0..2^16-1>;
					//} Extension;
					TLSExtensionType extensionType = static_cast<TLSExtensionType>(handshakeMessages.read_u16());
					U16 extensionLength = handshakeMessages.read_u16();
					handshakeMessages.skip_bytes(extensionLength);
				}

				state = TLS_CLIENT_STATE_WAIT_CERTIFICATE_CERTIFICATE_REQUEST;
			} break;
			case TLS_CLIENT_STATE_WAIT_CERTIFICATE_CERTIFICATE_REQUEST: {
				if (handshakeType == TLS_HANDSHAKE_TYPE_CERTIFICATE_REQUEST) {
					TLS_DEBUG_LOG("  RECEIVE CERTIFICATE_REQUEST\n", 3);
					// We don't have a certificate, so ignore this message
					handshakeMessages.skip_bytes(handshakeMessageDataLength);
					state = TLS_CONNECTION_STATE_WAIT_CERTIFICATE;
					break;
				}
				TLS_VERIFY_ALERT_RETURN(handshakeType == TLS_HANDSHAKE_TYPE_CERTIFICATE, UNEXPECTED_MESSAGE);
				state = TLS_CONNECTION_STATE_WAIT_CERTIFICATE;
			} [[fallthrough]]
			case TLS_CONNECTION_STATE_WAIT_CERTIFICATE: {
				TLS_VERIFY_ALERT_RETURN(handshakeType == TLS_HANDSHAKE_TYPE_CERTIFICATE, UNEXPECTED_MESSAGE);
				TLS_DEBUG_LOG("  RECEIVE CERTIFICATE\n", 3);
				//struct {
				//	opaque certificate_request_context<0..2^8-1>;
				//	CertificateEntry certificate_list<0..2^24-1>;
				//} Certificate;
				U8 contextLength = handshakeMessages.read_u8();
				// This is a server certificate, so the length must be 0
				TLS_VERIFY_ALERT_RETURN(contextLength == 0, ILLEGAL_PARAMETER);
				handshakeMessages.skip_bytes(contextLength);

				U32 certificatesEnd = handshakeMessages.read_u24();
				certificatesEnd += handshakeMessages.offset;
				TLS_VERIFY_ALERT_RETURN(certificatesEnd <= handshakeMessages.capacity, DECODE_ERROR);
				while (!handshakeMessages.failed && handshakeMessages.offset < certificatesEnd) {
					//struct {
					//	select (certificate_type) {
					//		case RawPublicKey:
					//			/* From RFC 7250 ASN.1_subjectPublicKeyInfo */
					//			opaque ASN1_subjectPublicKeyInfo<1..2^24-1>;
					//
					//		case X509:
					//			opaque cert_data<1..2^24-1>;
					//	};
					//	Extension extensions<0..2^16-1>;
					//} CertificateEntry;
					// We should always be X509, since we didn't negotiate RawPublicKey
					U32 certificateLength = handshakeMessages.read_u24();
					TLS_VERIFY_ALERT_RETURN(handshakeMessages.offset + certificateLength <= certificatesEnd, DECODE_ERROR);
					void* cert = handshakeMessages.bytes + handshakeMessages.offset;
					X509SignatureAlgorithm certSignatureAlgorithm;
					void* publicKey = nullptr;
					U32 publicKeyLength = 0;
					X509Error certParseError = parse_asn1_der_cert2(&certSignatureAlgorithm, &publicKey, &publicKeyLength, cert, certificateLength);
					TLS_VERIFY_ALERT_RETURN(certParseError != X509_ERROR_PARSE_FAILED && certParseError != X509_ERROR_WRONG_VERSION, BAD_CERTIFICATE);
					// Don't actually do anything with the extracted key, since we don't support all algorithms and we're not verifying against a CA anyway
					handshakeMessages.skip_bytes(certificateLength);

					// Skip extensions
					U16 extensionsLength = handshakeMessages.read_u16();
					handshakeMessages.skip_bytes(extensionsLength);
				}
				TLS_VERIFY_ALERT_RETURN(handshakeMessages.offset == certificatesEnd, DECODE_ERROR);
				state = TLS_CONNECTION_STATE_WAIT_CERTIFICATE_VERIFY;
			} break;
			case TLS_CONNECTION_STATE_WAIT_CERTIFICATE_VERIFY: {
				TLS_VERIFY_ALERT_RETURN(handshakeType == TLS_HANDSHAKE_TYPE_CERTIFICATE_VERIFY, UNEXPECTED_MESSAGE);
				TLS_DEBUG_LOG("  RECEIVE CERTIFICATE_VERIFY\n", 3);
				//struct {
				//	SignatureScheme algorithm;
				//	opaque signature<0..2^16-1>;
				//} CertificateVerify;

				// We're not actually verifying against a CA, so we can just skip this
				TLSSignatureScheme signatureScheme = static_cast<TLSSignatureScheme>(handshakeMessages.read_u16());
				U16 signatureDataLength = handshakeMessages.read_u16();
				handshakeMessages.skip_bytes(signatureDataLength);

				state = TLS_CONNECTION_STATE_WAIT_FINISHED;
			} break;
			case TLS_CONNECTION_STATE_WAIT_FINISHED: {
				TLS_VERIFY_ALERT_RETURN(handshakeType == TLS_HANDSHAKE_TYPE_FINISHED, UNEXPECTED_MESSAGE);
				TLS_DEBUG_LOG("  RECEIVE FINISHED\n", 3);
				SHA256 tmpHash;
				// finished_key = HKDF-Expand-Label(BaseKey, "finished", "", Hash.length)
				// verify_data = HMAC(finished_key, Transcript-Hash(Handshake Context, Certificate*, CertificateVerify*))
				// * Only included if present.
				// BaseKey is server_handshake_traffic_secret if server, client_handshake_traffic_secret if client,
				// and client_application_traffic_secret_n if post handshake
				// 
				//struct {
				//	opaque verify_data[Hash.length];
				//} Finished;
				Byte remoteHandshakeVerify[SHA256_HASH_SIZE];
				handshakeMessages.read_bytes(remoteHandshakeVerify, sizeof(remoteHandshakeVerify));

				Byte transcriptHashForFinishedVerify[SHA256_HASH_SIZE];
				tmpHash.copy_from(transcriptHash);
				tmpHash.digest(transcriptHashForFinishedVerify);

				Byte finishedKey[SHA256_HASH_SIZE];
				sha256_hkdf_expand_label(finishedKey, sizeof(finishedKey), isServer ? clientTrafficSecret : serverTrafficSecret, SHA256_HASH_SIZE, "finished", 8, "", 0);
				Byte localHandshakeVerify[SHA256_HASH_SIZE];
				sha256_hmac(localHandshakeVerify, finishedKey, sizeof(finishedKey), transcriptHashForFinishedVerify, sizeof(transcriptHashForFinishedVerify));

				TLS_VERIFY_ALERT_RETURN(memcmp(remoteHandshakeVerify, localHandshakeVerify, SHA256_HASH_SIZE) == 0, DECRYPT_ERROR);

				TLS_VERIFY_ALERT_RETURN(!handshakeMessages.failed && handshakeMessages.offset == handshakeMessages.capacity, DECODE_ERROR);

				if (!isServer) {
					// If we're the client, add the server finished to the hash and send our finished
					// If we're the server, we already sent a finished and added it to the hash, and we don't need to add the client finished to the hash
					U32 dataToBeHashedLength = handshakeMessageDataLength + 4; // hash includes the handshake message header
					transcriptHash.update(handshakeMessages.bytes + handshakeMessages.offset - dataToBeHashedLength, dataToBeHashedLength);
					send_client_change_cipher_spec_and_finished();
				}

				// Application key calc
				const U32 hashLength = SHA256_HASH_SIZE;
				const U32 keyLength = AES_KEY_SIZE_BYTES;
				const U32 ivLength = AES_GCM_IV_SIZE;

				Byte helloToServerFinishedHash[hashLength];
				if (isServer) {
					memcpy(helloToServerFinishedHash, transcriptHashForFinishedVerify, SHA256_HASH_SIZE);
				} else {
					tmpHash.copy_from(transcriptHash);
					tmpHash.digest(helloToServerFinishedHash);
				}

				sha256_hkdf_expand_label(serverTrafficSecret, hashLength, masterSecret, hashLength, "s ap traffic", 12, helloToServerFinishedHash, hashLength);
				sha256_hkdf_expand_label(serverKey, keyLength, serverTrafficSecret, hashLength, "key", 3, "", 0);
				sha256_hkdf_expand_label(serverIV, ivLength, serverTrafficSecret, hashLength, "iv", 2, "", 0);
				sha256_hkdf_expand_label(clientTrafficSecret, hashLength, masterSecret, hashLength, "c ap traffic", 12, helloToServerFinishedHash, hashLength);
				sha256_hkdf_expand_label(clientKey, keyLength, clientTrafficSecret, hashLength, "key", 3, "", 0);
				sha256_hkdf_expand_label(clientIV, ivLength, clientTrafficSecret, hashLength, "iv", 2, "", 0);

				encryptedMessageCounter = 0;
				decryptedMessageCounter = 0;

				state = TLS_CONNECTION_STATE_CONNECTED;
				return;
			} break;
			default: {
				// Should never get here
				error_alert(TLS_ALERT_INTERNAL_ERROR);
				return;
			}
			}
			TLS_VERIFY_ALERT_RETURN(handshakeMessages.offset == handshakeMessages.capacity, DECODE_ERROR);
			handshakeMessages.capacity = handshakeMessagesCapacity;
			TLS_VERIFY_ALERT_RETURN(!handshakeMessages.failed, DECODE_ERROR);
			U32 dataToBeHashedLength = handshakeMessageDataLength + 4; // hash includes the handshake message header
			transcriptHash.update(handshakeMessages.bytes + handshakeMessages.offset - dataToBeHashedLength, dataToBeHashedLength);
		}
	}
	void process_received_record() {
		TLS_DEBUG_LOG("RECORD RECEIVED\n", 3);
		currentRecordReceiveLength = 0;
		if (state != TLS_CLIENT_STATE_WAIT_SERVER_HELLO && state != TLS_CONNECTION_STATE_START && currentReceiveHeader.contentType != TLS_CONTENT_TYPE_ALERT) {
			// Make sure to ignore the legacy_record_version field in TLSPlaintext, only validate in TLSCiphertext
			TLS_VERIFY_ALERT_RETURN(bswap16(currentReceiveHeader.protocolVersion) == TLS_PROTOCOL_VERSION_1_2, UNEXPECTED_MESSAGE);
		}

		U32 recordLength = bswap16(currentReceiveHeader.length);

		BigEndianByteBuf record;
		record.wrap(receiveBuffer + receiveBufferDataEnd - recordLength, recordLength);

		if (currentReceiveHeader.contentType == TLS_CONTENT_TYPE_ALERT) {
			TLS_DEBUG_LOG("  RECEIVED ALERT\n", 1);
			close_no_alert();
			TLSAlertLevel level = static_cast<TLSAlertLevel>(record.read_u8());
			TLSAlertDescription description = static_cast<TLSAlertDescription>(record.read_u8());
			if (!record.failed) {
				TLS_DEBUG_LOG_NUM(U32(level), 1);
				TLS_DEBUG_LOG(", Description ", 1);
				TLS_DEBUG_LOG_NUM(U32(description), 1);
				TLS_DEBUG_LOG("\n", 1);
			} else {
				TLS_DEBUG_LOG("Alert malformed\n", 1);
			}
			return;
		}

		if (currentReceiveHeader.contentType == TLS_CONTENT_TYPE_CHANGE_CIPHER_SPEC) {
			// Change cipher spec can't be sent after the handshake is finished
			TLS_VERIFY_ALERT_RETURN(state != TLS_CONNECTION_STATE_CONNECTED, UNEXPECTED_MESSAGE);
			// Change cipher spec should contain exactly one byte: 0x01
			TLS_VERIFY_ALERT_RETURN(recordLength == 1, DECODE_ERROR);
			U8 changeCipherSpecDummyByte = record.read_u8();
			TLS_VERIFY_ALERT_RETURN(changeCipherSpecDummyByte == 1, ILLEGAL_PARAMETER);
			receiveBufferDataEnd -= record.capacity;
			if (receiveBufferUserReadPos == receiveBufferDataEnd) {
				receiveBufferUserReadPos = receiveBufferUserDataEnd = receiveBufferDataEnd = 0;
			}
			return;
		}

		switch (state) {
		case TLS_CONNECTION_STATE_CLOSED: {
			// Should never be here
			error_alert(TLS_ALERT_UNEXPECTED_MESSAGE);
			return;
		} break;
		case TLS_CONNECTION_STATE_START: {
			// Receiving a message as a client wouldn't make sense here, since the client always sends the first message.
			TLS_VERIFY_ALERT_RETURN(isServer, UNEXPECTED_MESSAGE);
			TLS_VERIFY_ALERT_RETURN(currentReceiveHeader.contentType == TLS_CONTENT_TYPE_HANDSHAKE, UNEXPECTED_MESSAGE);
			TLSHandshakeType handshakeType = static_cast<TLSHandshakeType>(record.read_u8());
			TLS_VERIFY_ALERT_RETURN(handshakeType == TLS_HANDSHAKE_TYPE_CLIENT_HELLO, UNEXPECTED_MESSAGE);
			TLS_DEBUG_LOG("  RECEIVED CLIENT_HELLO\n", 3);
			U32 length = record.read_u24();
			// Handling hello messages with more than 16kb would be annoying, and I don't feel like doing it
			TLS_VERIFY_ALERT_RETURN(length <= record.capacity - record.offset, RECORD_OVERFLOW);

			//uint16 ProtocolVersion;
			//opaque Random[32];
			//
			//uint8 CipherSuite[2];    /* Cryptographic suite selector */
			//
			//struct {
			//	ProtocolVersion legacy_version = 0x0303;    /* TLS v1.2 */
			//	Random random;
			//	opaque legacy_session_id<0..32>;
			//	CipherSuite cipher_suites<2..2^16-2>;
			//	opaque legacy_compression_methods<1..2^8-1>;
			//	Extension extensions<8..2^16-1>;
			//} ClientHello;
			U16 legacyVersion = record.read_u16();
			TLS_VERIFY_ALERT_RETURN(legacyVersion == TLS_PROTOCOL_VERSION_1_2, ILLEGAL_PARAMETER);
			record.skip_bytes(32); // random
			U32 legacySessionIdLength = record.read_u8();
			TLS_VERIFY_ALERT_RETURN(legacySessionIdLength <= 32u, ILLEGAL_PARAMETER);
			Byte* legacySessionId = record.bytes + record.offset;
			record.skip_bytes(legacySessionIdLength);
			U32 cipherSuitesLength = record.read_u16();
			TLS_VERIFY_ALERT_RETURN(cipherSuitesLength >= 2, ILLEGAL_PARAMETER);
			TLS_VERIFY_ALERT_RETURN(record.has_data_left(cipherSuitesLength), DECODE_ERROR);
			U32 cipherSuitesEnd = cipherSuitesLength + record.offset;
			TLSCipherSuite chosenCipherSuite;
			while (record.offset < cipherSuitesEnd) {
				TLSCipherSuite cipherSuite = static_cast<TLSCipherSuite>(record.read_u16());
				if (cipherSuite == TLS_AES_128_GCM_SHA256) {
					chosenCipherSuite = cipherSuite;
					goto foundCipherSuite;
				}
			}
			error_alert(TLS_ALERT_HANDSHAKE_FAILURE);
			return;
		foundCipherSuite:;
			record.offset = cipherSuitesEnd;
			U8 legacyCompressionMethodsLength = record.read_u8();
			TLS_VERIFY_ALERT_RETURN(legacyCompressionMethodsLength == 1, ILLEGAL_PARAMETER);
			U8 legacyCompressionFormat = record.read_u8();
			TLS_VERIFY_ALERT_RETURN(legacyCompressionFormat == 0, ILLEGAL_PARAMETER);
			U32 extensionsLength = record.read_u16();
			TLS_VERIFY_ALERT_RETURN(extensionsLength >= 8u, ILLEGAL_PARAMETER);
			TLS_VERIFY_ALERT_RETURN(record.has_data_left(extensionsLength), DECODE_ERROR);
			U32 extensionsEnd = record.offset + extensionsLength;
			U32 extensionsOldCapacity = record.capacity;
			record.capacity = extensionsEnd;

			// The x and y curve position numbers for SECP256R1
			Byte clientPublicKey[32 + 32];
			B32 hasTLS13 = false;
			B32 hasCorrectKeyShare = false;
			B32 hasASupportedGroup = false;
			B32 hasASupportedSignatureAlgorithm = false;
			B32 hasSignatureAlgorithmsExtension = false;
			B32 hasSupportedGroupsExtension = false;
			B32 hasKeyShareExtension = false;
			while (!record.failed && record.offset < extensionsEnd) {
				//struct {
				//	ExtensionType extension_type;
				//	opaque extension_data<0..2^16-1>;
				//} Extension;
				TLSExtensionType extensionType = static_cast<TLSExtensionType>(record.read_u16());
				U16 extensionDataLength = record.read_u16();
				TLS_VERIFY_ALERT_RETURN(record.has_data_left(extensionDataLength), DECODE_ERROR);
				record.capacity = record.offset + extensionDataLength;
				switch (extensionType) {
				case TLS_EXTENSION_TYPE_SUPPORTED_VERSIONS: {
					TLS_DEBUG_LOG("    EXTENSION: SUPPORTED_VERSIONS\n", 3);
					//struct {
					//  select (Handshake.msg_type) {
					//	  case client_hello:
					//		   ProtocolVersion versions<2..254>;
					//
					//	  case server_hello: /* and HelloRetryRequest */
					//		   ProtocolVersion selected_version;
					//  };
					//} SupportedVersions;
					U32 protocolVersionsLength = record.read_u8();
					TLS_VERIFY_ALERT_RETURN(record.has_data_left(protocolVersionsLength), DECODE_ERROR);
					TLS_VERIFY_ALERT_RETURN(protocolVersionsLength >= 2u && (protocolVersionsLength & 1) == 0, ILLEGAL_PARAMETER);
					while (!record.failed && protocolVersionsLength) {
						U16 protocolVersion = record.read_u16();
						hasTLS13 |= protocolVersion == TLS_PROTOCOL_VERSION_1_3;
						protocolVersionsLength -= 2;
					}
				} break;
				case TLS_EXTENSION_TYPE_KEY_SHARE: {
					TLS_DEBUG_LOG("    EXTENSION: KEY_SHARE\n", 3);
					hasKeyShareExtension = true;
					//struct {
					// NamedGroup group;
					// opaque key_exchange<1..2^16-1>;
					//} KeyShareEntry;
					//struct {
					//	KeyShareEntry client_shares<0..2^16-1>;
					//} KeyShareClientHello;
					U32 keySharesLength = record.read_u16();
					TLS_VERIFY_ALERT_RETURN(record.offset + keySharesLength == record.capacity, DECODE_ERROR);
					while (!record.failed && record.offset < record.capacity) {
						TLSNamedGroup group = static_cast<TLSNamedGroup>(record.read_u16());
						U32 keyLength = record.read_u16();
						if (group == TLS_NAMED_GROUP_SECP256R1) {
							// 1 byte for compression, and two 32 byte integers for the key
							TLS_VERIFY_ALERT_RETURN(keyLength == 1 + 32 + 32, ILLEGAL_PARAMETER);
							U8 compression = record.read_u8();
							// Don't accept compressed keys
							TLS_VERIFY_ALERT_RETURN(compression == 4, ILLEGAL_PARAMETER);
							record.read_bytes(clientPublicKey, keyLength - 1);
							hasCorrectKeyShare = true;
							hasASupportedGroup = true;
						} else {
							record.skip_bytes(keyLength);
						}
					}
				} break;
				case TLS_EXTENSION_TYPE_SUPPORTED_GROUPS: {
					TLS_DEBUG_LOG("    EXTENSION: SUPPORTED_GROUPS\n", 3);
					hasSupportedGroupsExtension = true;
					//struct {
					//	NamedGroup named_group_list<2..2^16-1>;
					//} NamedGroupList;
					U32 namedGroupsLength = record.read_u16();
					TLS_VERIFY_ALERT_RETURN(record.offset + namedGroupsLength == record.capacity, DECODE_ERROR);
					while (!record.failed && record.offset < record.capacity) {
						TLSNamedGroup group = static_cast<TLSNamedGroup>(record.read_u16());
						hasASupportedGroup |= group == TLS_NAMED_GROUP_SECP256R1;
					}
				} break;
				case TLS_EXTENSION_TYPE_SIGNATURE_ALGORITHMS: {
					TLS_DEBUG_LOG("    EXTENSION: SIGNATURE_ALGORITHMS\n", 3);
					hasSignatureAlgorithmsExtension = true;
					//struct {
					//	SignatureScheme supported_signature_algorithms<2..2^16-2>;
					//} SignatureSchemeList;
					U32 supportedSignatureAlgorithmsLength = record.read_u16();
					TLS_VERIFY_ALERT_RETURN(supportedSignatureAlgorithmsLength >= 2u, ILLEGAL_PARAMETER);
					TLS_VERIFY_ALERT_RETURN(record.offset + supportedSignatureAlgorithmsLength == record.capacity, DECODE_ERROR);
					while (!record.failed && record.offset < record.capacity) {
						TLSSignatureScheme signatureScheme = static_cast<TLSSignatureScheme>(record.read_u16());
						hasASupportedSignatureAlgorithm |= signatureScheme == TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA256;
					}
				} break;
				case TLS_EXTENSION_TYPE_SIGNATURE_ALGORITHMS_CERT: {
					TLS_DEBUG_LOG("    EXTENSION: SIGNATURE_ALGORITHMS_CERT\n", 3);
					// I'm just going to ignore this one.
					// I only have one certificate, so it's not like anything meaningful would change from parsing this
					record.skip_bytes(extensionDataLength);
				} break;
				default: {
					record.skip_bytes(extensionDataLength);
				} break;
				}
				TLS_VERIFY_ALERT_RETURN(!record.failed && record.offset == record.capacity, DECODE_ERROR);
				record.capacity = extensionsEnd;
			}
			TLS_VERIFY_ALERT_RETURN(!record.failed, DECODE_ERROR);
			TLS_VERIFY_ALERT_RETURN(record.offset == record.capacity, DECODE_ERROR);
			record.capacity = extensionsOldCapacity;
			TLS_VERIFY_ALERT_RETURN(hasTLS13, PROTOCOL_VERSION);
			TLS_VERIFY_ALERT_RETURN(hasASupportedGroup && hasASupportedSignatureAlgorithm, HANDSHAKE_FAILURE);
			TLS_VERIFY_ALERT_RETURN(hasSignatureAlgorithmsExtension && hasKeyShareExtension && hasSupportedGroupsExtension, MISSING_EXTENSION);
			transcriptHash.update(receiveBuffer + receiveBufferDataEnd - recordLength, recordLength);
			if (hasCorrectKeyShare) {
				send_server_response_to_hello(SERVER_RESPONSE_NORMAL, legacySessionId, legacySessionIdLength, chosenCipherSuite, clientPublicKey);
			} else {
				send_server_response_to_hello(SERVER_RESPONSE_RETRY_REQUEST, legacySessionId, legacySessionIdLength, chosenCipherSuite, nullptr);
			}
		} break;
		case TLS_CLIENT_STATE_WAIT_SERVER_HELLO: {
			// While technically a handshake message, this isn't processed with the rest of them
			// since server hello must always be on a record boundary due to the key change immediately after
			TLS_VERIFY_ALERT_RETURN(currentReceiveHeader.contentType == TLS_CONTENT_TYPE_HANDSHAKE, UNEXPECTED_MESSAGE);
			TLSHandshakeType handshakeType = static_cast<TLSHandshakeType>(record.read_u8());
			TLS_VERIFY_ALERT_RETURN(handshakeType == TLS_HANDSHAKE_TYPE_SERVER_HELLO, UNEXPECTED_MESSAGE);
			TLS_DEBUG_LOG("  RECEIVED SERVER_HELLO\n", 3);
			U32 length = record.read_u24();
			// Handling hello messages with more than 16kb would be annoying, and I don't feel like doing it
			TLS_VERIFY_ALERT_RETURN(length <= record.capacity - record.offset, RECORD_OVERFLOW);

			//struct {
			//	ProtocolVersion legacy_version = 0x0303;    /* TLS v1.2 */
			//	Random random;
			//	opaque legacy_session_id_echo<0..32>;
			//	CipherSuite cipher_suite;
			//	uint8 legacy_compression_method = 0;
			//	Extension extensions<6..2^16-1>;
			//} ServerHello;

			// Check enough data left for necessary ServerHello data
			TLS_VERIFY_ALERT_RETURN(record.has_data_left(2 + 32 + 1 + 32 + 2 + 1 + 2), DECODE_ERROR);

			U16 legacyVersion = record.read_u16();
			TLS_VERIFY_ALERT_RETURN(legacyVersion == TLS_PROTOCOL_VERSION_1_2, ILLEGAL_PARAMETER);
			// random field
			if (memcmp(record.bytes + record.offset, tlsHelloRetryRequestMagic, sizeof(tlsHelloRetryRequestMagic)) == 0) {
				TLS_DEBUG_LOG("SERVER HELLO WAS RETRY_REQUEST\n", 3);
				U8 clientHello1HashData[4 + SHA256_HASH_SIZE];
				clientHello1HashData[0] = TLS_HANDSHAKE_TYPE_MESSAGE_HASH;
				clientHello1HashData[1] = 0;
				clientHello1HashData[2] = 0;
				clientHello1HashData[3] = SHA256_HASH_SIZE;
				transcriptHash.digest(clientHello1HashData + 4);
				transcriptHash.init();
				transcriptHash.update(clientHello1HashData, sizeof(clientHello1HashData));
				// This is a HelloRetryRequest, nothing would change since we only support one key type
				error_alert(TLS_ALERT_ILLEGAL_PARAMETER);
				return;
			}
			if (memcmp(record.bytes + record.offset + 24, tlsTLS12NegotiationMagic, sizeof(tlsTLS12NegotiationMagic)) == 0 ||
				memcmp(record.bytes + record.offset + 24, tlsTLS11NegotiationMagic, sizeof(tlsTLS11NegotiationMagic)) == 0) {
				TLS_DEBUG_LOG("SERVER HELLO WAS TLS 1.2 OR TLS 1.1\n", 3);
				// Don't support TLS 1.1 or 1.2
				error_alert(TLS_ALERT_ILLEGAL_PARAMETER);
				return;
			}
			record.skip_bytes(32); // skip over random
			U8 legacySessionIdEchoLength = record.read_u8();
			TLS_VERIFY_ALERT_RETURN(legacySessionIdEchoLength == 32, ILLEGAL_PARAMETER);
			record.offset += legacySessionIdEchoLength;
			TLSCipherSuite cipherSuite = static_cast<TLSCipherSuite>(record.read_u16());
			TLS_VERIFY_ALERT_RETURN(cipherSuite == TLS_AES_128_GCM_SHA256, HANDSHAKE_FAILURE);
			U8 legacyCompressionMethod = record.read_u8();
			TLS_VERIFY_ALERT_RETURN(legacyCompressionMethod == 0, ILLEGAL_PARAMETER);

			//struct {
			//	ExtensionType extension_type;
			//	opaque extension_data<0..2^16-1>;
			//} Extension;
			U16 extensionsLength = record.read_u16();
			TLS_VERIFY_ALERT_RETURN(record.has_data_left(extensionsLength), DECODE_ERROR);

			// The x and y curve position numbers for SECP256R1
			Byte serverPublicKey[32 + 32];
			B32 hasTLS13 = false;
			B32 hasKeyShare = false;

			BigEndianByteBuf extensions;
			extensions.wrap(record.bytes + record.offset, extensionsLength);
			while (extensions.has_data_left(sizeof(TLSExtensionType) + sizeof(U16))) {
				TLSExtensionType extensionType = static_cast<TLSExtensionType>(extensions.read_u16());
				U16 extensionDataLength = extensions.read_u16();
				TLS_VERIFY_ALERT_RETURN(extensions.has_data_left(extensionDataLength), DECODE_ERROR);
				U32 extensionsCapacity = extensions.capacity;
				extensions.capacity = extensions.offset + extensionDataLength;

				switch (extensionType) {
				case TLS_EXTENSION_TYPE_SUPPORTED_VERSIONS: {
					TLS_DEBUG_LOG("    EXTENSION: SUPPORTED_VERSIONS\n", 3);
					//struct {
					//  select (Handshake.msg_type) {
					//	  case client_hello:
					//		   ProtocolVersion versions<2..254>;
					//
					//	  case server_hello: /* and HelloRetryRequest */
					//		   ProtocolVersion selected_version;
					//  };
					//} SupportedVersions;
					TLS_VERIFY_ALERT_RETURN(extensionDataLength == 2, DECODE_ERROR);
					U16 protocolVersion = extensions.read_u16();
					if (protocolVersion == TLS_PROTOCOL_VERSION_1_3) {
						TLS_DEBUG_LOG("    SUPPORTS TLS 1.3\n", 3);
						hasTLS13 = true;
					}
				} break;
				case TLS_EXTENSION_TYPE_KEY_SHARE: {
					TLS_DEBUG_LOG("    EXTENSION: KEY_SHARE\n", 3);
					//struct {
					// NamedGroup group;
					// opaque key_exchange<1..2^16-1>;
					//} KeyShareEntry;
					//struct {
					//  KeyShareEntry server_share;
					//} KeyShareServerHello;
					TLSNamedGroup group = static_cast<TLSNamedGroup>(extensions.read_u16());
					TLS_VERIFY_ALERT_RETURN(group == TLS_NAMED_GROUP_SECP256R1, HANDSHAKE_FAILURE);
					U16 keyExchangeLength = extensions.read_u16();
					// 1 byte for compression, and two 32 byte integers for the key
					TLS_VERIFY_ALERT_RETURN(keyExchangeLength == 1 + 32 + 32, ILLEGAL_PARAMETER);
					U8 compression = extensions.read_u8();
					// Don't accept compressed keys
					TLS_VERIFY_ALERT_RETURN(compression == 4, ILLEGAL_PARAMETER);
					extensions.read_bytes(serverPublicKey, keyExchangeLength - 1);
					hasKeyShare = true;
				} break;
				default: {
					extensions.offset += extensionDataLength;
				} break;
				}

				TLS_VERIFY_ALERT_RETURN(extensions.offset == extensions.capacity, DECODE_ERROR);
				extensions.capacity = extensionsCapacity;
			}
			TLS_VERIFY_ALERT_RETURN(!extensions.failed, DECODE_ERROR);
			TLS_VERIFY_ALERT_RETURN(extensions.offset == extensions.capacity, DECODE_ERROR);
			TLS_VERIFY_ALERT_RETURN(hasKeyShare && hasTLS13, HANDSHAKE_FAILURE);
			record.skip_bytes(extensions.offset);

			TLS_DEBUG_LOG("HANDSHAKE KEY CALC\n", 3);

			// Server hello done, key calc
			transcriptHash.update(receiveBuffer + receiveBufferDataEnd - recordLength, recordLength);

			const U32 hashLength = SHA256_HASH_SIZE;
			const U32 encryptionKeySize = AES_KEY_SIZE_BYTES;
			U8 clientToServerHelloHash[hashLength];
			SHA256 tmpHash;
			tmpHash.copy_from(transcriptHash);
			tmpHash.digest(clientToServerHelloHash);

			U8 earlySecret[hashLength];
			U8 zeros[hashLength]{};
			U8 emptyHash[hashLength];
			sha256(emptyHash, "", 0);
			// early secret
			sha256_hkdf_extract(earlySecret, "", 0, zeros, hashLength);
			// handshake secret
			U8 derivedSecret[hashLength];
			sha256_derive_secret(derivedSecret, hashLength, earlySecret, hashLength, "derived", 7, emptyHash, hashLength);

			U8 sharedSecret[hashLength];
			B32 keyCalcSuccess = secp256r1_ecdhe(sharedSecret, serverPublicKey, serverPublicKey + 32, diffieHellmanPrivateKey);
			TLS_VERIFY_ALERT_RETURN(keyCalcSuccess, HANDSHAKE_FAILURE);

			U8 handshakeSecret[hashLength];
			sha256_hkdf_extract(handshakeSecret, derivedSecret, hashLength, sharedSecret, 32);
			// Calc client and server handshake keys
			sha256_derive_secret(clientTrafficSecret, hashLength, handshakeSecret, hashLength, "c hs traffic", 12, clientToServerHelloHash, hashLength);
			sha256_derive_secret(serverTrafficSecret, hashLength, handshakeSecret, hashLength, "s hs traffic", 12, clientToServerHelloHash, hashLength);
			sha256_hkdf_expand_label(clientKey, encryptionKeySize, clientTrafficSecret, hashLength, "key", 3, "", 0);
			sha256_hkdf_expand_label(serverKey, encryptionKeySize, serverTrafficSecret, hashLength, "key", 3, "", 0);
			sha256_hkdf_expand_label(clientIV, 12, clientTrafficSecret, hashLength, "iv", 2, "", 0);
			sha256_hkdf_expand_label(serverIV, 12, serverTrafficSecret, hashLength, "iv", 2, "", 0);

			// master secret
			sha256_derive_secret(derivedSecret, hashLength, handshakeSecret, hashLength, "derived", 7, emptyHash, hashLength);
			sha256_hkdf_extract(masterSecret, derivedSecret, hashLength, zeros, hashLength);

			encryptedMessageCounter = 0;
			decryptedMessageCounter = 0;
			state = TLS_CLIENT_STATE_WAIT_ENCRYPTED_EXTENSIONS;
		} break;
		case TLS_CLIENT_STATE_WAIT_ENCRYPTED_EXTENSIONS:
		case TLS_CLIENT_STATE_WAIT_CERTIFICATE_CERTIFICATE_REQUEST:
		case TLS_CONNECTION_STATE_WAIT_CERTIFICATE:
		case TLS_CONNECTION_STATE_WAIT_CERTIFICATE_VERIFY:
		case TLS_CONNECTION_STATE_WAIT_FINISHED: {
			TLS_VERIFY_ALERT_RETURN(currentReceiveHeader.contentType == TLS_CONTENT_TYPE_APPLICATION_DATA, UNEXPECTED_MESSAGE);
			decrypt_record(receiveBufferDataEnd - recordLength, recordLength);
			if (state == TLS_CONNECTION_STATE_CLOSED) {
				// Decrypt error must have occured, just exit
				return;
			}
			TLS_VERIFY_ALERT_RETURN(recordLength >= 17, DECODE_ERROR);
			recordLength -= 17; // Skip auth tag and point at the first byte of the decrypted message
			// Skip zero padding
			U32 zeroPadding = 0;
			while (recordLength > 0 && record.bytes[recordLength] == 0) {
				recordLength--, zeroPadding++;
			}
			TLSContentType realContentType = static_cast<TLSContentType>(record.bytes[recordLength]);
			if (realContentType == TLS_CONTENT_TYPE_HANDSHAKE) {
				record.capacity = recordLength;
				process_handshake_messages(record);
				TLS_VERIFY_ALERT_RETURN(record.offset == record.capacity, DECODE_ERROR);
			} else if (realContentType == TLS_CONTENT_TYPE_ALERT) {
				TLSAlertLevel level = static_cast<TLSAlertLevel>(record.read_u8());
				TLSAlertDescription description = static_cast<TLSAlertDescription>(record.read_u8());
				if (description == TLS_ALERT_CLOSE_NOTIFY) {
					error_alert(TLS_ALERT_CLOSE_NOTIFY);
				} else {
					close_no_alert();
					TLS_DEBUG_LOG("RECEIVED ALERT\n", 1);
					if (!record.failed) {
						TLS_DEBUG_LOG_NUM(U32(level), 1);
						TLS_DEBUG_LOG(", Description ", 1);
						TLS_DEBUG_LOG_NUM(U32(description), 1);
						TLS_DEBUG_LOG("\n", 1);
					} else {
						TLS_DEBUG_LOG("Alert malformed\n", 1);
					}
				}
				// Not a data message, discard its contents
				receiveBufferDataEnd -= record.capacity;
				return;
			} else {
				TLS_VERIFY_ALERT_RETURN(false, UNEXPECTED_MESSAGE);
			}
			recordLength += 17 + zeroPadding; // Add back in the auth tag and zero padding so we properly discard them
			record.capacity += 17 + zeroPadding;
			record.skip_bytes(17 + zeroPadding);
		} break;
		case TLS_CONNECTION_STATE_CONNECTED: {
			TLS_VERIFY_ALERT_RETURN(currentReceiveHeader.contentType == TLS_CONTENT_TYPE_APPLICATION_DATA, UNEXPECTED_MESSAGE);
			//struct {
			//	opaque content[TLSPlaintext.length];
			//	ContentType type;
			//	uint8 zeros[length_of_padding];
			//} TLSInnerPlaintext;
			decrypt_record(receiveBufferDataEnd - recordLength, recordLength);
			if (state == TLS_CONNECTION_STATE_CLOSED) {
				// Decrypt error must have occured, just exit
				return;
			}
			TLS_VERIFY_ALERT_RETURN(recordLength >= 17, DECODE_ERROR);
			recordLength -= 17; // Skip auth tag and point at the first byte of the decrypted message
			// Skip zero padding
			U32 zeroPadding = 0;
			while (recordLength > 0 && record.bytes[recordLength] == 0) {
				recordLength--, zeroPadding++;
			}
			TLSContentType realContentType = static_cast<TLSContentType>(record.bytes[recordLength]);
			if (realContentType == TLS_CONTENT_TYPE_APPLICATION_DATA) {
				TLS_DEBUG_LOG("RECEIVED APPLICATION DATA\n", 3);
				receiveBufferDataEnd -= 17 - zeroPadding; // discard auth tag, zero padding, and content type
				receiveBufferUserDataEnd = receiveBufferDataEnd;
				return;
			} else if (realContentType == TLS_CONTENT_TYPE_HANDSHAKE) {
				TLS_DEBUG_LOG("RECEIVED HANDSHAKE MESSAGE\n", 3);
				TLSHandshakeType handshakeType = static_cast<TLSHandshakeType>(record.read_u8());
				U32 handshakeLength = record.read_u24();
				TLS_VERIFY_ALERT_RETURN(record.offset + handshakeLength <= record.capacity, DECODE_ERROR);
				if (handshakeType == TLS_HANDSHAKE_TYPE_KEY_UPDATE) {
					TLS_DEBUG_LOG("  KEY_UPDATE\n", 3);
					//struct {
					//	KeyUpdateRequest request_update;
					//} KeyUpdate;
					TLSKeyUpdateRequest updateRequest = static_cast<TLSKeyUpdateRequest>(record.read_u8());
					TLS_VERIFY_ALERT_RETURN(!record.failed, DECODE_ERROR);

					const U32 hashLength = SHA256_HASH_SIZE;
					const U32 keyLength = AES_KEY_SIZE_BYTES;
					const U32 ivLength = AES_GCM_IV_SIZE;

					Byte* updateTrafficSecret = isServer ? clientTrafficSecret : serverTrafficSecret;
					Byte* updateKey = isServer ? clientKey : serverKey;
					Byte* updateIV = isServer ? clientIV : serverIV;
					Byte oldTrafficSecret[hashLength];
					memcpy(oldTrafficSecret, updateTrafficSecret, hashLength);
					sha256_hkdf_expand_label(updateTrafficSecret, hashLength, oldTrafficSecret, hashLength, "traffic upd", 11, "", 0);
					memset(oldTrafficSecret, 0, sizeof(oldTrafficSecret));
					sha256_hkdf_expand_label(updateKey, keyLength, updateTrafficSecret, hashLength, "key", 3, "", 0);
					sha256_hkdf_expand_label(updateIV, ivLength, updateTrafficSecret, hashLength, "iv", 2, "", 0);
					decryptedMessageCounter = 0;

					shouldKeyUpdate |= updateRequest == TLS_KEY_UPDATE_REQUESTED;
				} else if (handshakeType == TLS_HANDSHAKE_TYPE_NEW_SESSION_TICKET) {
					// Ignore, we're not handling PSKs
					record.skip_bytes(handshakeLength);
				} else {
					TLS_VERIFY_ALERT_RETURN(false, UNEXPECTED_MESSAGE);
				}
			} else if (realContentType == TLS_CONTENT_TYPE_ALERT) {
				TLSAlertLevel level = static_cast<TLSAlertLevel>(record.read_u8());
				TLSAlertDescription description = static_cast<TLSAlertDescription>(record.read_u8());
				if (description == TLS_ALERT_CLOSE_NOTIFY) {
					error_alert(TLS_ALERT_CLOSE_NOTIFY);
				} else {
					close_no_alert();
					TLS_DEBUG_LOG("RECEIVED ALERT\n", 1);
					if (!record.failed) {
						TLS_DEBUG_LOG_NUM(U32(level), 1);
						TLS_DEBUG_LOG(", Description ", 1);
						TLS_DEBUG_LOG_NUM(U32(description), 1);
						TLS_DEBUG_LOG("\n", 1);
					} else {
						TLS_DEBUG_LOG("Alert malformed\n", 1);
					}
				}
				// Not a data message, discard its contents
				receiveBufferDataEnd -= record.capacity;
				return;
			} else {
				TLS_VERIFY_ALERT_RETURN(false, UNEXPECTED_MESSAGE);
			}
			record.skip_bytes(17 + zeroPadding); // Skip auth tag, zero padding, and content type
		} break;
		}

		TLS_VERIFY_ALERT_RETURN(!record.failed && record.offset == record.capacity, DECODE_ERROR);

		// Record was completely consumed unless it was an application data
		receiveBufferDataEnd -= record.offset;
		if (receiveBufferUserReadPos == receiveBufferDataEnd) {
			receiveBufferUserReadPos = receiveBufferUserDataEnd = receiveBufferDataEnd = 0;
		}
	}

	// Return true if could receive more data
	B32 receive_data() {
		if (state == TLS_CONNECTION_STATE_ALERT_PENDING || state == TLS_CONNECTION_STATE_CLOSE_PENDING || state == TLS_CONNECTION_STATE_CLOSED) {
			return false;
		}
		B32 couldReceiveMoreData = false;
		while (true) {
			Byte* buffer;
			U32 bufferSize;
			if (currentRecordReceiveLength < sizeof(TLSRecordHeader)) {
				buffer = reinterpret_cast<Byte*>(&currentReceiveHeader) + currentRecordReceiveLength;
				bufferSize = 5 - currentRecordReceiveLength;
			} else {
				if (receiveBufferDataEnd == sizeof(receiveBuffer) && receiveBufferUserReadPos > 0) {
					move_receive_data_to_beginning_of_buffer();
				}
				U32 recordLength = bswap16(currentReceiveHeader.length);
				buffer = receiveBuffer + receiveBufferDataEnd;
				bufferSize = min<U32>(sizeof(receiveBuffer) - receiveBufferDataEnd, recordLength - (currentRecordReceiveLength - 5));
			}
			if (bufferSize == 0) {
				couldReceiveMoreData = true;
				break;
			}
			SyscallTCPReceiveArgs recvArgs;
			recvArgs.bufferAddress = U64(buffer);
			recvArgs.bufferSize = bufferSize;
			recvArgs.blockIndex = tcpConnection;
			I64 receiveResult = I64(g_syscallProc(SYSCALL_TCP_RECEIVE, U64(&recvArgs)));
			if (receiveResult != -1) {
				U32 dataReceived = U32(receiveResult);
				U32 dataLeftInTCPBuffer = U32(U64(receiveResult) >> 32);
				if (dataReceived != 0) {
					currentRecordReceiveLength += dataReceived;
					if (currentRecordReceiveLength > sizeof(TLSRecordHeader)) {
						receiveBufferDataEnd += dataReceived;
					}
				} else {
					break;
				}
			} else {
				TLS_DEBUG_LOG("Connection closed\n", 2);
				state = TLS_CONNECTION_STATE_CLOSED;
				break;
			}
			if (currentRecordReceiveLength > sizeof(TLSRecordHeader)) {
				U32 recordLength = bswap16(currentReceiveHeader.length);
				if (recordLength > TLS_MAX_RECORD_LENGTH) {
					error_alert(TLS_ALERT_RECORD_OVERFLOW);
					break;
				}
				if (currentRecordReceiveLength - sizeof(TLSRecordHeader) >= recordLength) {
					process_received_record();
					if (state == TLS_CONNECTION_STATE_ALERT_PENDING || state == TLS_CONNECTION_STATE_CLOSE_PENDING || state == TLS_CONNECTION_STATE_CLOSED) {
						break;
					}
				} else if (receiveBufferDataEnd == sizeof(receiveBuffer)) {
					if (receiveBufferUserReadPos > 0) {
						move_receive_data_to_beginning_of_buffer();
					} else {
						if (state != TLS_CONNECTION_STATE_CONNECTED) {
							// Too much data received during handshake
							// Since the user will never process it and free up space, this is an error
							// Should never happen in practice
							error_alert(TLS_ALERT_RECORD_OVERFLOW);
						} else {
							couldReceiveMoreData = true;
						}
						break;
					}
				}
			}
		}
		return couldReceiveMoreData;
	}

	void force_key_update() {
		if (state != TLS_CONNECTION_STATE_CONNECTED) {
			return;
		}
		shouldSendKeyUpdate = true;
		shouldKeyUpdate = true;
	}

	void send_data() {
		if (state == TLS_CONNECTION_STATE_CLOSED) {
			return;
		}
		if (sendQueueWritePos == sendQueueReadPos && sendRecordPos == sendRecordDataEnd && !shouldKeyUpdate && state != TLS_CONNECTION_STATE_ALERT_PENDING) {
			// Nothing to send
			if (state == TLS_CONNECTION_STATE_CLOSE_PENDING) {
				close_no_alert();
			}
			return;
		}
		if (sendRecordPos != sendRecordDataEnd) {
			send_to_tcp();
			if (state == TLS_CONNECTION_STATE_CLOSED) {
				// TCP send error
				return;
			}
		}
		if (sendRecordPos != sendRecordDataEnd) {
			// Try to send more later
			return;
		}
		if (state != TLS_CONNECTION_STATE_CONNECTED && state != TLS_CONNECTION_STATE_ALERT_PENDING) {
			return;
		}
		sendRecordPos = sendRecordDataEnd = 0;
		if (shouldKeyUpdate) {
			BigEndianByteBuf record;
			record.wrap(sendRecord, sizeof(sendRecord));
			// Record Header
			record.write_u8(TLS_CONTENT_TYPE_APPLICATION_DATA); // type
			record.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
			U32 recordLengthPatchPos = record.write_u16(0); // length
			//struct {
			//	opaque content[TLSPlaintext.length];
			//	ContentType type;
			//	uint8 zeros[length_of_padding];
			//} TLSInnerPlaintext;
			//struct {
			//  HandshakeType msg_type;
			//  uint24 length;
			//	KeyUpdate keyUpdate;
			//} Handshake;
			//struct {
			//	KeyUpdateRequest request_update;
			//} KeyUpdate;
			record.write_u8(TLS_HANDSHAKE_TYPE_KEY_UPDATE);
			record.write_u24(1); // One byte for KeyUpdate
			record.write_u8(shouldSendKeyUpdate ? TLS_KEY_UPDATE_REQUESTED : TLS_KEY_UPDATE_NOT_REQUESTED);
			record.write_u8(TLS_CONTENT_TYPE_HANDSHAKE);
			sendRecordDataEnd = record.offset;
			// No zero padding
			encrypt_record(0);
			send_to_tcp();
			shouldKeyUpdate = false;
			shouldSendKeyUpdate = false;
			if (state == TLS_CONNECTION_STATE_CLOSED) {
				// TCP error
				return;
			}
			const U32 hashLength = SHA256_HASH_SIZE;
			const U32 keyLength = AES_KEY_SIZE_BYTES;
			const U32 ivLength = AES_GCM_IV_SIZE;

			Byte* updateTrafficSecret = isServer ? serverTrafficSecret : clientTrafficSecret;
			Byte* updateKey = isServer ? serverKey : clientKey;
			Byte* updateIV = isServer ? serverIV : clientIV;
			Byte oldTrafficSecret[hashLength];
			memcpy(oldTrafficSecret, updateTrafficSecret, hashLength);
			sha256_hkdf_expand_label(updateTrafficSecret, hashLength, oldTrafficSecret, hashLength, "traffic upd", 11, "", 0);
			sha256_hkdf_expand_label(updateKey, keyLength, updateTrafficSecret, hashLength, "key", 3, "", 0);
			sha256_hkdf_expand_label(updateIV, ivLength, updateTrafficSecret, hashLength, "iv", 2, "", 0);
			encryptedMessageCounter = 0;
		}

		while (sendQueueReadPos != sendQueueWritePos) {
			if (sendRecordPos != sendRecordDataEnd) {
				// Try to send the rest later
				return;
			}
			BigEndianByteBuf record;
			record.wrap(sendRecord, sizeof(sendRecord));
			// Record Header
			record.write_u8(TLS_CONTENT_TYPE_APPLICATION_DATA); // type
			record.write_u16(TLS_PROTOCOL_VERSION_1_2); // version
			record.write_u16(0); // length

			U32 encodeSize = 0;
			while (sendQueueReadPos != sendQueueWritePos && encodeSize < TLS_MAX_RECORD_DATA_LENGTH) {
				TLSSendEntry& entry = sendQueue[sendQueueReadPos & sendQueueMask];
				U32 amountToWrite;
				if (entry.resource) {
					amountToWrite = min(TLS_MAX_RECORD_DATA_LENGTH - encodeSize, entry.size);
					record.write_bytes(entry.resource, amountToWrite);
					entry.resource += amountToWrite;
				} else {
					// Sanity clamp to make sure we're not trying to send more data than we have
					entry.size = min<U32>(entry.size, sendBufferDataEnd - sendBufferPos);
					amountToWrite = min(TLS_MAX_RECORD_DATA_LENGTH - encodeSize, entry.size);
					record.write_bytes(sendBuffer + sendBufferPos, amountToWrite);
					entry.resource += amountToWrite;
					sendBufferPos += amountToWrite;
					if (sendBufferPos == sendBufferDataEnd) {
						sendBufferPos = sendBufferDataEnd = 0;
					}
				}
				encodeSize += amountToWrite;
				entry.size -= amountToWrite;
				if (entry.size == 0) {
					sendQueueReadPos++;
				}
			}
			record.write_u8(TLS_CONTENT_TYPE_APPLICATION_DATA);
			sendRecordDataEnd += record.offset;
			// No zero padding
			encrypt_record(0);
			TLS_DEBUG_LOG("SEND APPLICATION DATA\n", 3);
			send_to_tcp();
			if (state == TLS_CONNECTION_STATE_CLOSED) {
				// TCP failure
				return;
			}
		}

		if (state == TLS_CONNECTION_STATE_ALERT_PENDING) {
			send_alert_record(closePendingAlert);
			if (sendRecordPos == sendRecordDataEnd) {
				close_no_alert();
			} else {
				state = TLS_CONNECTION_STATE_CLOSE_PENDING;
			}
		}
	}

	U32 received_user_data_size() {
		return receiveBufferUserDataEnd - receiveBufferUserReadPos;
	}

	U32 skip_received_bytes(U32 amount) {
		amount = min(amount, receiveBufferUserDataEnd - receiveBufferUserReadPos);
		receiveBufferUserReadPos += amount;
		if (receiveBufferUserReadPos == receiveBufferDataEnd) {
			receiveBufferUserReadPos = receiveBufferUserDataEnd = receiveBufferDataEnd = 0;
		}
		return amount;
	}

	void discard_received_user_data() {
		receiveBufferUserReadPos = receiveBufferUserDataEnd;
		if (receiveBufferUserReadPos == receiveBufferDataEnd) {
			receiveBufferUserReadPos = receiveBufferUserDataEnd = receiveBufferDataEnd = 0;
		}
	}

	void move_send_data_to_beginning_of_buffer() {
		if (sendBufferPos != 0) {
			memmove(sendBuffer, sendBuffer + sendBufferPos, sendBufferDataEnd - sendBufferPos);
			sendBufferDataEnd -= sendBufferPos;
			sendBufferPos = 0;
		}
	}

	void move_receive_data_to_beginning_of_buffer() {
		if (receiveBufferUserReadPos != 0) {
			memmove(receiveBuffer, receiveBuffer + receiveBufferUserReadPos, receiveBufferDataEnd - receiveBufferUserReadPos);
			receiveBufferDataEnd -= receiveBufferUserReadPos;
			receiveBufferUserDataEnd -= receiveBufferUserReadPos;
			receiveBufferUserReadPos = 0;
		}
	}

	B32 queue_buffer_to_send(const void* buffer, U32 size) {
		if (state == TLS_CONNECTION_STATE_CLOSED || state == TLS_CONNECTION_STATE_ALERT_PENDING || state == TLS_CONNECTION_STATE_CLOSE_PENDING) {
			return false;
		}
		if (size == 0) {
			return true;
		}
		if (buffer == nullptr && sendQueueWritePos != sendQueueReadPos && sendQueue[(sendQueueWritePos - 1) & sendQueueMask].resource == nullptr) {
			sendQueue[(sendQueueWritePos - 1) & sendQueueMask].size += size;
			return true;
		}
		if (sendQueueWritePos - sendQueueReadPos < sendQueueCap) {
			sendQueue[(sendQueueWritePos++) & sendQueueMask] = TLSSendEntry{ reinterpret_cast<const Byte*>(buffer), size };
			return true;
		}
		return false;
	}

	TLSConnection& write_bytes(const void* bytes, U32 length) {
		while (length) {
			if (state == TLS_CONNECTION_STATE_CLOSED || state == TLS_CONNECTION_STATE_ALERT_PENDING || state == TLS_CONNECTION_STATE_CLOSE_PENDING) {
				return *this;
			}
			U32 queuedDataForSend = min<U32>(length, sizeof(sendBuffer) - sendBufferDataEnd);
			if (queuedDataForSend < length && sendBufferPos > 0) {
				move_send_data_to_beginning_of_buffer();
				queuedDataForSend = min<U32>(length, sizeof(sendBuffer) - sendBufferDataEnd);
			}
			memcpy(sendBuffer + sendBufferDataEnd, bytes, queuedDataForSend);
			if (queue_buffer_to_send(nullptr, queuedDataForSend)) {
				bytes = reinterpret_cast<const Byte*>(bytes) + queuedDataForSend;
				length -= queuedDataForSend;
				sendBufferDataEnd += queuedDataForSend;
			}
			send_data();
		}
	}

	TLSConnection& write_str(const char* str) {
		while (*str) {
			if (state == TLS_CONNECTION_STATE_CLOSED || state == TLS_CONNECTION_STATE_ALERT_PENDING || state == TLS_CONNECTION_STATE_CLOSE_PENDING) {
				return *this;
			}
			U32 queuedDataForSend = 0;
			while (*str && sendBufferDataEnd < sizeof(sendBuffer)) {
				sendBuffer[sendBufferDataEnd++] = *str;
				str++, queuedDataForSend++;
			}
			if (!queue_buffer_to_send(nullptr, queuedDataForSend)) {
				str -= queuedDataForSend;
				sendBufferDataEnd -= queuedDataForSend;
			}
			if (str[0] != '\0') {
				if (sendBufferPos != 0) {
					move_send_data_to_beginning_of_buffer();
				} else {
					// Try to send data until we have enough space to fit the string (blocking, should never happen in practice)
					send_data();
				}
			}
		}
		return *this;
	}

	// HTTP convenience methods
	TLSConnection& head(const char* directory) {
		return write_str("HEAD ").write_str(directory).write_str(" HTTP/1.1\r\n");
	}
	TLSConnection& post(const char* directory) {
		return write_str("POST ").write_str(directory).write_str(" HTTP/1.1\r\n");
	}
	TLSConnection& get(const char* directory) {
		return write_str("GET ").write_str(directory).write_str(" HTTP/1.1\r\n");
	}
	TLSConnection& field(const char* fieldName, const char* fieldValue) {
		return write_str(fieldName).write_str(": ").write_str(fieldValue).write_str("\r\n");
	}
	TLSConnection& content_length(U32* patchPos) {
		// This method should not be called if more data will be encoded than space left in the send buffer
		// Otherwise the empty content length may be flushed and the patch position will be bogus
		// Use transfer encoding if there needs to be more space
		write_str("Content-Length:          \r\n");
		// patchPos gets a sendBufferPos relative pointer to the end of the patch field
		*patchPos = sendBufferDataEnd - 3 - sendBufferPos;
		return *this;
	}
	TLSConnection& patch_content_length(U32 sendBufferPosRelativePatchPos, U32 length) {
		U32 patchPos = sendBufferPos + sendBufferPosRelativePatchPos;
		sendBuffer[patchPos] = '0';
		while (length != 0) {
			sendBuffer[patchPos--] = length % 10 + '0';
			length /= 10;
		}
		return *this;
	}
	TLSConnection& end_http_header() {
		return write_str("\r\n");
	}
};

#undef TLS_VERIFY_ALERT_RETURN