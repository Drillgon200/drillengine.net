#pragma once
#include "CryptoHash.h"
#include "CryptoSignature.h"
#include "AES.h"
#include "RSA.h"
#include "ECC.h"
#include "ASN1.h"
#include "PondOS.h"

enum TLSRecordType {
	TLS_RECORD_TYPE_INVALID = 0,
	TLS_RECORD_TYPE_CHANGE_CIPHER_SPEC = 20,
	TLS_RECORD_TYPE_ALERT = 21,
	TLS_RECORD_TYPE_HANDSHAKE = 22,
	TLS_RECORD_TYPE_APPLICATION_DATA = 23
};

enum TLSHandshakeType {
	TLS_HANDSHAKE_CLIENT_HELLO = 1,
	TLS_HANDSHAKE_SERVER_HELLO = 2,
	TLS_HANDSHAKE_NEW_SESSION_TICKET = 4,
	TLS_HANDSHAKE_END_OF_EARLY_DATA = 5,
	TLS_HANDSHAKE_ENCRYPTED_EXTENSIONS = 8,
	TLS_HANDSHAKE_CERTIFICATE = 11,
	TLS_HANDSHAKE_CERTIFICATE_REQUEST = 13,
	TLS_HANDSHAKE_CERTIFICATE_VERIFY = 15,
	TLS_HANDSHAKE_FINISHED = 20,
	TLS_HANDSHAKE_KEY_UPDATE = 24,
	TLS_HANDSHAKE_MESSAGE_HASH = 254
};

enum TLSCipherSuite {
	TLS_AES_128_GCM_SHA256 = 0x1301,
	TLS_AES_256_GCM_SHA384 = 0x1302,
	TLS_CHACHA20_POLY1305_SHA256 = 0x1303,
	TLS_AES_128_CCM_SHA256 = 0x1304,
	TLS_AES_128_CCM_8_SHA256 = 0x1305
};

enum TLSSignatureScheme {
	TLS_RSA_PKCS1_SHA256 = 0x0401,
	TLS_RSA_PKCS1_SHA384 = 0x0501,
	TLS_RSA_PKCS1_SHA512 = 0x0601,

	TLS_ECDSA_SECP256R1_SHA256 = 0x0403,
	TLS_ECDSA_SECP384R1_SHA384 = 0x0503,
	TLS_ECDSA_SECP521R1_SHA512 = 0x0603,

	TLS_RSS_PSS_RSAE_SHA256 = 0x0804,
	TLS_RSS_PSS_RSAE_SHA384 = 0x0805,
	TLS_RSS_PSS_RSAE_SHA512 = 0x0806,

	TLS_ED25519 = 0x0807,
	TLS_ED448 = 0x0808,

	TLS_RSS_PSS_PSS_SHA256 = 0x0809,
	TLS_RSS_PSS_PSS_SHA384 = 0x080A,
	TLS_RSS_PSS_PSS_SHA512 = 0x080B,

	TLS_RSA_PKCS1_SHA1 = 0x0201,
	TLS_ECDSA_SHA1 = 0x0203
};

enum TLSExtension {
	TLS_EXTENSION_SERVER_NAME = 0,
	TLS_EXTENSION_MAX_FRAGMENT_LENGTH = 1,
	TLS_EXTENSION_STATUS_REQUEST = 5,
	TLS_EXTENSION_SUPPORTED_GROUPS = 10,
	TLS_EXTENSION_SIGNATURE_ALGORITHMS = 13,
	TLS_EXTENSION_USE_SRTP = 14,
	TLS_EXTENSION_HEARTBEAT = 15,
	TLS_EXTENSION_APPLICATION_LAYER_PROTOCOL_NEGOTIATION = 16,
	TLS_EXTENSION_SIGNED_CERTIFICATE_TIMESTAMP = 18,
	TLS_EXTENSION_CLIENT_CERTIFICATE_TYPE = 19,
	TLS_EXTENSION_SERVER_CERTIFICATE_TYPE = 20,
	TLS_EXTENSION_PADDING = 21,
	TLS_EXTENSION_PRE_SHARED_KEY = 41,
	TLS_EXTENSION_EARLY_DATA = 42,
	TLS_EXTENSION_SUPPORTED_VERSIONS = 43,
	TLS_EXTENSION_COOKIE = 44,
	TLS_EXTENSION_PSK_KEY_EXCHANGE_MODES = 45,
	TLS_EXTENSION_CERTIFICATE_AUTHORITIES = 47,
	TLS_EXTENSION_OID_FILTERS = 48,
	TLS_EXTENSION_POST_HANDSHAKE_AUTH = 49,
	TLS_EXTENSION_SIGNATURE_ALGORITHMS_CERT = 50,
	TLS_EXTENSION_KEY_SHARE = 51
};

enum TLSNamedGroup {
	TLS_GROUP_SECP256R1 = 0x0017,
	TLS_GROUP_SECP384R1 = 0x0018,
	TLS_GROUP_SECP521R1 = 0x0019,

	TLS_GROUP_X25519 = 0x001D,
	TLS_GROUP_X448 = 0x001E,

	TLS_GROUP_FFDHE2048 = 0x0100,
	TLS_GROUP_FFDHE3072 = 0x0101,
	TLS_GROUP_FFDHE4096 = 0x0102,
	TLS_GROUP_FFDHE6144 = 0x0103,
	TLS_GROUP_FFDHE8192 = 0x0104
};

enum TLSServerNameType {
	TLS_SERVER_NAME_HOST_NAME = 0
};

enum TLSAlertLevel {
	TLS_ALERT_LEVEL_WARNING = 1,
	TLS_ALERT_LEVEL_FATAL = 2
};

enum TLSAlertDescription {
	TLS_ALERT_CLOSE_NOTIFY = 0,
	TLS_ALERT_UNEXPECTED_MESSAGE = 10,
	TLS_ALERT_BAD_RECORD_MAC = 20,
	TLS_ALERT_RECORD_OVERFLOW = 22,
	TLS_ALERT_HANDSHAKE_FAILURE = 40,
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
	TLS_ALERT_PROTOCOL_VERSION = 70,
	TLS_ALERT_INSUFFICIENT_SECURITY = 71,
	TLS_ALERT_INTERNAL_ERROR = 80,
	TLS_ALERT_INAPPROPRIATE_FALLBACK = 86,
	TLS_ALERT_USER_CANCELED = 90,
	TLS_ALERT_MISSING_EXTENSION = 109,
	TLS_ALERT_UNSUPPORTED_EXTENSION = 110,
	TLS_ALERT_UNRECOGNIZED_NAME = 112,
	TLS_ALERT_BAD_CERTIFICATE_STATUS_RESPONSE = 113,
	TLS_ALERT_UNKNOWN_PSK_IDENTITY = 115,
	TLS_ALERT_CERTIFICATE_REQUIRED = 116,
	TLS_ALERT_NO_APPLICATION_PROTOCOL = 120
};

enum TLSKeyUpdateRequest {
	TLS_KEY_UPDATE_NOT_REQUESTED = 0,
	TLS_KEY_UPDATE_REQUESTED = 1
};

enum ConnectionError {
	CONNECTION_ERROR_SUCCESS,
	CONNECTION_ERROR_WSA_INIT_FAILED,
	CONNECTION_ERROR_GET_ADDR_INFO_FAIL,
	CONNECTION_ERROR_SOCKET_CREATION_ERROR,
	CONNECTION_ERROR_BIND_FAILED,
	CONNECTION_ERROR_LISTEN_FAILED,
	CONNECTION_ERROR_CONNECT_FAILED,
	CONNECTION_ERROR_SEND_FAILED
};

enum NetworkTransferResult {
	NET_TRANSFER_RESULT_COMPLETE,
	NET_TRANSFER_RESULT_BAD_LENGTH,
	NET_TRANSFER_RESULT_CONNECTION_CLOSED,
	NET_TRANSFER_RESULT_INCOMPLETE,
	NET_TRANSFER_RESULT_CONNECTION_FAILURE

};

struct TLSRecord {
	// Records contain less than 2^14 bytes max
	static constexpr u32 MAX_RECORD_SIZE = 0x4000;
	// Subtract the bytes for the real record type and the encryption signature
	static constexpr u32 MAX_APPLICATION_DATA_SIZE = MAX_RECORD_SIZE - 1 - 16;
	// Add 5 for the header
	static constexpr u32 DATA_BUFFER_SIZE = MAX_RECORD_SIZE + 5;
	u8 dataBuffer[DATA_BUFFER_SIZE];
	u32 dataOffset;
	u32 numBytesSentOrReceived;

	void write_bytes(const void* bytes, u32 length) {
		for (u32 i = 0; i < length; i++) {
			dataBuffer[dataOffset + i] = reinterpret_cast<const u8*>(bytes)[i];
		}
		dataOffset += length;
	}

	void read_bytes(void* bytes, u32 length) {
		for (u32 i = 0; i < length; i++) {
			reinterpret_cast<u8*>(bytes)[i] = dataBuffer[dataOffset + i];
		}
		dataOffset += length;
	}

	void write_byte(u8 data) {
		dataBuffer[dataOffset++] = data;
	}

	u8 read_byte() {
		return dataBuffer[dataOffset++];
	}

	void write_int16(u16 data) {
		dataBuffer[dataOffset++] = data >> 8;
		dataBuffer[dataOffset++] = data & 0xFF;
	}

	void patch_int16(u16 data, u32 pos) {
		dataBuffer[pos] = data >> 8;
		dataBuffer[pos + 1] = data & 0xFF;
	}

	u16 read_int16() {
		u16 result = (dataBuffer[dataOffset] << 8) | dataBuffer[dataOffset + 1];
		dataOffset += 2;
		return result;
	}

	u16 read_int16(u32 pos) {
		return (dataBuffer[pos] << 8) | dataBuffer[pos + 1];
	}

	void write_int24(u32 data) {
		dataBuffer[dataOffset++] = (data >> 16) & 0xFF;
		dataBuffer[dataOffset++] = (data >> 8) & 0xFF;
		dataBuffer[dataOffset++] = data & 0xFF;
	}

	void write_int24(u32 data, u32 pos) {
		dataBuffer[pos + 0] = (data >> 16) & 0xFF;
		dataBuffer[pos + 1] = (data >> 8) & 0xFF;
		dataBuffer[pos + 2] = data & 0xFF;
	}

	u32 read_int24() {
		u32 result =
			(dataBuffer[dataOffset + 0] << 16) |
			(dataBuffer[dataOffset + 1] << 8) |
			dataBuffer[dataOffset + 2];
		dataOffset += 3;
		return result;
	}

	void write_int32(u32 data) {
		dataBuffer[dataOffset++] = data >> 24;
		dataBuffer[dataOffset++] = (data >> 16) & 0xFF;
		dataBuffer[dataOffset++] = (data >> 8) & 0xFF;
		dataBuffer[dataOffset++] = data & 0xFF;
	}

	u32 read_int32() {
		u32 result =
			(dataBuffer[dataOffset + 0] << 24) |
			(dataBuffer[dataOffset + 1] << 16) |
			(dataBuffer[dataOffset + 2] << 8) |
			dataBuffer[dataOffset + 3];
		dataOffset += 4;
		return result;
	}

	void write_int64(u64 data) {
		dataBuffer[dataOffset++] = data >> 56;
		dataBuffer[dataOffset++] = (data >> 48) & 0xFF;
		dataBuffer[dataOffset++] = (data >> 40) & 0xFF;
		dataBuffer[dataOffset++] = (data >> 32) & 0xFF;
		dataBuffer[dataOffset++] = (data >> 24) & 0xFF;
		dataBuffer[dataOffset++] = (data >> 16) & 0xFF;
		dataBuffer[dataOffset++] = (data >> 8) & 0xFF;
		dataBuffer[dataOffset++] = data & 0xFF;
	}

	u64 read_int64() {
		u64 result =
			(static_cast<u64>(dataBuffer[dataOffset + 0]) << 56) |
			(static_cast<u64>(dataBuffer[dataOffset + 1]) << 48) |
			(static_cast<u64>(dataBuffer[dataOffset + 2]) << 40) |
			(static_cast<u64>(dataBuffer[dataOffset + 3]) << 32) |
			(static_cast<u64>(dataBuffer[dataOffset + 4]) << 24) |
			(static_cast<u64>(dataBuffer[dataOffset + 5]) << 16) |
			(static_cast<u64>(dataBuffer[dataOffset + 6]) << 8) |
			static_cast<u64>(dataBuffer[dataOffset + 7]);
		dataOffset += 8;
		return result;
	}

	u32 write_header(TLSRecordType type) {
		u32 start = dataOffset;
		write_byte(type);
		// 0x0303 means TLS 1.2 (must be sent to this for compatibility with middleboxes that only accept TLS 1.2)
		write_int16(0x0303);
		// Length, will be updated when the message is complete
		write_int16(0);
		return start;
	}

	void patch_header_length(u32 recordBegin) {
		u16 dataLength = dataOffset - 5 - recordBegin;
		dataBuffer[recordBegin + 3] = dataLength >> 8;
		dataBuffer[recordBegin + 4] = dataLength & 0xFF;
	}

	void reset() {
		dataOffset = 0;
		numBytesSentOrReceived = 0;
	}

	void encrypt(u8 key[AES_KEY_SIZE_BYTES], u8 iv[AES_GCM_IV_SIZE], u64& numMessagesEncrypted, u32 recordStart) {
		u8 modifiedIV[AES_GCM_IV_SIZE];
		memcpy(modifiedIV, iv, 4);
		for (u32 i = 4; i < AES_GCM_IV_SIZE; i++) {
			modifiedIV[i] = iv[i] ^ ((numMessagesEncrypted >> ((AES_GCM_IV_SIZE - 1 - i) * 8)) & 0xFF);
		}
		numMessagesEncrypted++;

		patch_int16(dataOffset - recordStart - 5 + 16, recordStart + 3);

		AESGCM aes;
		aes.init(key, modifiedIV);
		aes.encrypt(dataBuffer + dataOffset, dataBuffer + recordStart + 5, dataBuffer + recordStart + 5, dataOffset - recordStart - 5, dataBuffer + recordStart, 5);
		dataOffset += 16;
	}

	// Expects the record header to already be read
	b32 decrypt(u8 key[AES_KEY_SIZE_BYTES], u8 iv[AES_GCM_IV_SIZE], u64& numMessagesDecrypted, u32 recordEnd) {
		u8 modifiedIV[AES_GCM_IV_SIZE];
		memcpy(modifiedIV, iv, 4);
		for (u32 i = 4; i < AES_GCM_IV_SIZE; i++) {
			modifiedIV[i] = iv[i] ^ ((numMessagesDecrypted >> ((AES_GCM_IV_SIZE - 1 - i) * 8)) & 0xFF);
		}
		numMessagesDecrypted++;

		AESGCM aes;
		aes.init(key, modifiedIV);
		return aes.decrypt(dataBuffer + dataOffset, dataBuffer + (recordEnd - 16), dataBuffer + dataOffset, recordEnd - dataOffset - 16, dataBuffer + dataOffset - 5, 5);
	}

	void init(TLSRecordType type) {
		dataOffset = 0;
		numBytesSentOrReceived = 0;
		write_byte(type);
		// 0x0303 means TLS 1.2 (must be sent to this for compatibility with middleboxes that only accept TLS 1.2)
		write_int16(0x0303);
		// Length, will be updated when the message is complete
		write_int16(0);
	}

	b32 send_completed() {
		return numBytesSentOrReceived == dataOffset;
	}

	b32 receive_completed() {
		if (numBytesSentOrReceived < 5) {
			return false;
		}
		u32 length = read_int16(3);
		return numBytesSentOrReceived >= length + 5;
	}

	ConnectionError _send_data(u32 tcpConnectionIdx) {
		SyscallTCPSendArgs sendArgs;
		sendArgs.bufferAddress = reinterpret_cast<u64>(dataBuffer + numBytesSentOrReceived);
		sendArgs.bufferSize = dataOffset - numBytesSentOrReceived;
		sendArgs.blockIndex = tcpConnectionIdx;
		i64 dataAccepted = i64(g_syscallProc(SYSCALL_TCP_SEND, reinterpret_cast<u64>(&sendArgs)));
		if (dataAccepted == -1) {
			return CONNECTION_ERROR_SEND_FAILED;
		}
		numBytesSentOrReceived += dataAccepted;
		return CONNECTION_ERROR_SUCCESS;
	}

	NetworkTransferResult _receive_data(u32 tcpConnectionIdx) {
		SyscallTCPReceiveArgs receiveArgs;
		receiveArgs.blockIndex = tcpConnectionIdx;
		receiveArgs.bufferAddress = reinterpret_cast<u64>(dataBuffer + numBytesSentOrReceived);
		// Make sure to get a header before reading rest of record data so we know how much to read
		if (numBytesSentOrReceived < 5) {
			receiveArgs.bufferSize = 5 - numBytesSentOrReceived;
			i64 amountReceived = i64(g_syscallProc(SYSCALL_TCP_RECEIVE, reinterpret_cast<u64>(&receiveArgs)));
			if (amountReceived == -1) {
				return NET_TRANSFER_RESULT_CONNECTION_FAILURE;
			}
			numBytesSentOrReceived += amountReceived;
		}
		if (numBytesSentOrReceived < 5) {
			return NET_TRANSFER_RESULT_INCOMPLETE;
		}

		u32 length = read_int16(3);
		if (length > MAX_RECORD_SIZE) {
			return NET_TRANSFER_RESULT_BAD_LENGTH;
		}
		if (numBytesSentOrReceived >= length + 5) {
			return NET_TRANSFER_RESULT_COMPLETE;
		}

		receiveArgs.bufferAddress = reinterpret_cast<u64>(dataBuffer + numBytesSentOrReceived);
		receiveArgs.bufferSize = length + 5 - numBytesSentOrReceived;
		i64 amountReceived = i64(g_syscallProc(SYSCALL_TCP_RECEIVE, reinterpret_cast<u64>(&receiveArgs)));
		if (amountReceived == -1) {
			return NET_TRANSFER_RESULT_CONNECTION_FAILURE;
		}
		numBytesSentOrReceived += amountReceived;
		return numBytesSentOrReceived >= length + 5 ? NET_TRANSFER_RESULT_COMPLETE : NET_TRANSFER_RESULT_INCOMPLETE;
	}

	NetworkTransferResult _receive_all_data(u32 tcpConnectionIdx) {
		SyscallTCPReceiveArgs receiveArgs;
		receiveArgs.blockIndex = tcpConnectionIdx;
		receiveArgs.bufferAddress = reinterpret_cast<u64>(dataBuffer + numBytesSentOrReceived);
		receiveArgs.bufferSize = DATA_BUFFER_SIZE - numBytesSentOrReceived;
		i64 amountReceived = i64(g_syscallProc(SYSCALL_TCP_RECEIVE, reinterpret_cast<u64>(&receiveArgs)));
		if (amountReceived == -1) {
			return NET_TRANSFER_RESULT_CONNECTION_FAILURE;
		}
		numBytesSentOrReceived += amountReceived;
		if (numBytesSentOrReceived < 5) {
			return NET_TRANSFER_RESULT_INCOMPLETE;
		}
		u32 length = read_int16(3);
		if (length > MAX_RECORD_SIZE) {
			return NET_TRANSFER_RESULT_BAD_LENGTH;
		}
		if (numBytesSentOrReceived >= length + 5) {
			return NET_TRANSFER_RESULT_COMPLETE;
		}
		return NET_TRANSFER_RESULT_INCOMPLETE;
	}
};

struct X509Certificate {
	static constexpr u32 certMaxLength = 10 * 1024;
	u8 cert[certMaxLength];
	u32 certLength;
	BigInteger rsaPrivateKey;
	BigInteger rsaPublicKey;
	BigInteger rsaModulus;

	void init() {
		rsaPrivateKey.init(64);
		rsaPublicKey.init(64);
		rsaModulus.init(64);
	}
};

enum X509SignatureAlgorithm {
	X509_SIGNATURE_RSA_GENERIC,
	X509_SIGNATURE_RSA_PSS,
	X509_SIGNATURE_EC_GENERIC,
	X509_SIGNATURE_EC_IMPLICITLY_CA,
	X509_SIGNATURE_EC_SECP256R1
};

b32 parse_asn1_der_cert(X509SignatureAlgorithm* algorithm, void** publicKey, u32* publicKeyLength, void* vcert, u32 certLength) {
#define CERT_CHECK_ERROR(cond) if(cond){ __debugbreak(); return false; }
	ASN1Reader cert{ vcert, certLength };
	u32 tagId;
	ASN1ClassOfTag tagClass;
	// Cert sequence
	u32 certSequenceEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);

	{ // TBSCertificate sequence (TBS means To Be Signed)
		enum TBSCertContextTags {
			TBS_ID_VERSION = 0,
			TBS_ID_ISSUER_UNIQUE_ID = 1,
			TBS_ID_SUBJECT_UNIQUE_ID = 2,
			TBS_ID_EXTENSIONS = 3
		};
		u32 tbsCertSequenceEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);

		// version (MUST be present in TLS 1.3, since the default is v1 and TLS 1.3 requires v3)
		u32 versionEnd = cert.read_expected_identifier_end(ASN1_CLASS_CONTEXT_SPECIFIC, TBS_ID_VERSION);
		i64 version = cert.read_integer_tag();
		// verify version 3
		CERT_CHECK_ERROR(version != 2);
		cert.verify_section_complete(versionEnd);

		// skip serial number
		cert.skip_field();

		{ // AlgorithmIdentifier sequence, signature field
			u32 signatureEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);

			TLSSignatureScheme certificateSignatureScheme;
			u32 algorithmIdentifierLength = cert.read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_OBJECT_IDENTIFIER);
			if (cert.oid_match(ASN1_OID_RSA_PKCS1_SHA256, sizeof(ASN1_OID_RSA_PKCS1_SHA256) / sizeof(u32), algorithmIdentifierLength)) {
				certificateSignatureScheme = TLS_RSA_PKCS1_SHA256;
				cert.read_null_tag();
			} else if (cert.oid_match(ASN1_OID_RSASSA_PSS, sizeof(ASN1_OID_RSASSA_PSS) / sizeof(u32), algorithmIdentifierLength)) {
				// In this case, the hash algorithm is found in the parameters
				certificateSignatureScheme = TLS_RSS_PSS_PSS_SHA256;
				//TODO
			} else if (cert.oid_match(ASN1_OID_ECDSA_SHA256, sizeof(ASN1_OID_ECDSA_SHA256) / sizeof(u32), algorithmIdentifierLength)) {
				certificateSignatureScheme = TLS_ECDSA_SECP256R1_SHA256;
			} else {
				CERT_CHECK_ERROR(true);
			}

			cert.verify_section_complete(signatureEnd);
		}

		// Skip issuer field
		cert.skip_field();
		// Skip validity field
		cert.skip_field();
		// Skip subject field
		cert.skip_field();

		{ // SubjectPublicKeyInfo structure, subjectPublicKeyInfo field
			u32 subjectPublicKeyInfoEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);
			{ // AlgorithmIdentifier sequence, algorithm field
				u32 algorithmEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);

				u32 algorithmIdentifierLength = cert.read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_OBJECT_IDENTIFIER);
				if (cert.oid_match(ASN1_OID_RSA_ENCRYPTION, sizeof(ASN1_OID_RSA_ENCRYPTION) / sizeof(u32), algorithmIdentifierLength)) {
					cert.read_null_tag();
					*algorithm = X509_SIGNATURE_RSA_GENERIC;
				} else if (cert.oid_match(ASN1_OID_RSASSA_PSS, sizeof(ASN1_OID_RSASSA_PSS) / sizeof(u32), algorithmIdentifierLength)) {
					cert.read_null_tag();
					*algorithm = X509_SIGNATURE_RSA_PSS;
				} else if (cert.oid_match(ASN1_OID_EC_PUBLIC_KEY, sizeof(ASN1_OID_EC_PUBLIC_KEY) / sizeof(u32), algorithmIdentifierLength)) {
					// This is actually a choice between a generic ECParameters, a named curve identifier, and implicitly CA
					// I'm only supporting named curves
					u32 curveIdentifierLength = cert.read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_OBJECT_IDENTIFIER);
					if (cert.oid_match(ASN1_OID_CURVE_SECP256R1, sizeof(ASN1_OID_CURVE_SECP256R1) / sizeof(u32), curveIdentifierLength)) {
						*algorithm = X509_SIGNATURE_EC_SECP256R1;
					} else {
						CERT_CHECK_ERROR(true);
					}
				} else {
					CERT_CHECK_ERROR(true);
				}

				cert.verify_section_complete(algorithmEnd);
			}

			u32 subjectPublicKeyLength = cert.read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_BIT_STRING);
			u32 subjectPublicKeyBitLength;
			u8* subjectPublicKey = cert.read_bitstring(subjectPublicKeyLength, &subjectPublicKeyBitLength);
			*publicKey = subjectPublicKey;
			*publicKeyLength = (subjectPublicKeyBitLength + 7) / 8;

			cert.verify_section_complete(subjectPublicKeyInfoEnd);
		}

		// Skip optional issuerUniqueID
		tagClass = cert.read_identifier(&tagId);
		if (tagClass == ASN1_CLASS_CONTEXT_SPECIFIC && tagId == 1) {
			// Skip issuerUniqueID
			cert.pos += cert.read_length();
			tagClass = cert.read_identifier(&tagId);
		}
		if (tagClass == ASN1_CLASS_CONTEXT_SPECIFIC && tagId == 2) {
			// Skip subjectUniqueID
			cert.pos += cert.read_length();
			tagClass = cert.read_identifier(&tagId);
		}
		if (tagClass == ASN1_CLASS_CONTEXT_SPECIFIC && tagId == 3) {
			// Skip extensions
			cert.pos += cert.read_length();
		}

		cert.verify_section_complete(tbsCertSequenceEnd);
	}

	// Skip signatureAlgorithm
	cert.skip_field();
	// Skip signatureValue
	cert.skip_field();

	cert.verify_section_complete(certSequenceEnd);
	CERT_CHECK_ERROR(cert.errored);
#undef CERT_CHECK_ERROR
	return true;
}

enum TLSServerState {
	TLS_SERVER_STATE_HELLO,
	TLS_SERVER_STATE_HANDSHAKE,
	TLS_SERVER_STATE_APPLICATION,
	TLS_SERVER_STATE_CLOSE_PENDING
};

struct TLSSendEntry {
	// Null if should send from the internal send buffer
	u8* resource;
	u32 size;
};

struct TLSServerClientConnection {
	// Keeping two here to send a message while another is being received. Wastes 16 kb, whatever.
	TLSRecord sendRecord;
	TLSRecord receiveRecord;
	u32 dataLeftInReceiveRecord;
	SHA256 runningMessageHash;
	u32 clientTCPConnection;
	TLSServerState serverState;
	u64 connectionId;
	u8 sessionId[32];
	u8 serverPrivateKey[32];
	u8 clientPublicKey[32 + 32];
	u8 masterSecret[SHA256_HASH_SIZE];
	X509Certificate* certificate;

	// Encryption keys
	u8 clientTrafficSecret[SHA256_HASH_SIZE];
	u8 clientKey[AES_KEY_SIZE_BYTES];
	u8 clientIV[AES_GCM_IV_SIZE];
	u64 clientMessageDecryptCount;
	u8 serverTrafficSecret[SHA256_HASH_SIZE];
	u8 serverKey[AES_KEY_SIZE_BYTES];
	u8 serverIV[AES_GCM_IV_SIZE];
	u64 serverMessageEncryptCount;
	b32 shouldKeyUpdate;
	b32 shouldSendKeyUpdate;

	TLSCipherSuite cipherSuite;

	u32 sendBufferPos;
	u32 sendBufferDataEnd;
	u32 receiveBufferPos;
	u32 receiveBufferDataEnd;
	u32 sendQueueWritePos;
	u32 sendQueueReadPos;
	static constexpr u32 sendBufferCap = 8 * 1024;
	u8 sendBuffer[sendBufferCap];
	static constexpr u32 receiveBufferCap = 8 * 1024;
	u8 receiveBuffer[receiveBufferCap];
	static constexpr u32 sendQueueCap = 1 << 6;
	static constexpr u32 sendQueueMask = sendQueueCap - 1;
	TLSSendEntry sendQueue[sendQueueCap];

	b32 init(u32 tcpConnectionIdx) {
		clientTCPConnection = tcpConnectionIdx;
		serverState = TLS_SERVER_STATE_HELLO;
		sendRecord.reset();
		receiveRecord.reset();
		dataLeftInReceiveRecord = 0;
		runningMessageHash.init();
		shouldKeyUpdate = false;
		shouldSendKeyUpdate = false;
		sendBufferPos = 0;
		sendBufferDataEnd = 0;
		receiveBufferPos = 0;
		receiveBufferDataEnd = 0;
		sendQueueWritePos = 0;
		sendQueueReadPos = 0;
		return true;
	}

#ifdef _DEBUG
#define CONNECTION_ERROR serverState = TLS_SERVER_STATE_CLOSE_PENDING; return
#else
#define CONNECTION_ERROR serverState = TLS_SERVER_STATE_CLOSE_PENDING; return
#endif
#define CHECK_CONNECTION_PROBLEM(cond) if(cond) { CONNECTION_ERROR; }

	void calc_handshake_keys() {
		SHA256 tmpHash;
		tmpHash.copy_from(runningMessageHash);
		u8 clientToServerHelloHash[SECP256R1_KEY_SIZE * 2];
		tmpHash.digest(clientToServerHelloHash);

		const u32 hashLength = SHA256_HASH_SIZE;
		const u32 encryptionKeySize = AES_KEY_SIZE_BYTES;
		const u32 encryptionIVSize = AES_GCM_IV_SIZE;

		u8 earlySecret[hashLength];
		u8 zeros[hashLength]{};
		u8 hashedZeros[hashLength]{};
		u8 emptyHash[hashLength];
		sha256(hashedZeros, zeros, hashLength);
		sha256(emptyHash, "", 0);

		// early secret
		sha256_hkdf_extract(earlySecret, "", 0, zeros, hashLength);

		// handshake secret
		u8 derivedSecret[hashLength];
		sha256_derive_secret(derivedSecret, hashLength, earlySecret, hashLength, "derived", 7, emptyHash, hashLength);
		u8 sharedSecret[hashLength];

		b32 keyCalcSuccess = secp256r1_ecdhe(sharedSecret, clientPublicKey, clientPublicKey + 32, serverPrivateKey);
		CHECK_CONNECTION_PROBLEM(!keyCalcSuccess);

		u8 handshakeSecret[hashLength];
		sha256_hkdf_extract(handshakeSecret, derivedSecret, hashLength, sharedSecret, hashLength);
		

		// handshake keys
		sha256_derive_secret(clientTrafficSecret, hashLength, handshakeSecret, hashLength, "c hs traffic", 12, clientToServerHelloHash, hashLength);
		sha256_hkdf_expand_label(clientKey, encryptionKeySize, clientTrafficSecret, hashLength, "key", 3, "", 0);
		sha256_hkdf_expand_label(clientIV, encryptionIVSize, clientTrafficSecret, hashLength, "iv", 2, "", 0);
		clientMessageDecryptCount = 0;

		sha256_derive_secret(serverTrafficSecret, hashLength, handshakeSecret, hashLength, "s hs traffic", 12, clientToServerHelloHash, hashLength);
		sha256_hkdf_expand_label(serverKey, encryptionKeySize, serverTrafficSecret, hashLength, "key", 3, "", 0);
		sha256_hkdf_expand_label(serverIV, encryptionIVSize, serverTrafficSecret, hashLength, "iv", 2, "", 0);
		serverMessageEncryptCount = 0;


		// master secret
		sha256_derive_secret(derivedSecret, hashLength, handshakeSecret, hashLength, "derived", 7, emptyHash, hashLength);
		sha256_hkdf_extract(masterSecret, derivedSecret, hashLength, zeros, hashLength);
	}

	void calc_application_keys() {
		const u32 hashLength = SHA256_HASH_SIZE;
		const u32 encryptionKeySize = AES_KEY_SIZE_BYTES;
		const u32 encryptionIVSize = AES_GCM_IV_SIZE;

		u8 helloToServerFinishedHash[hashLength];
		SHA256 tmpHash;
		tmpHash.copy_from(runningMessageHash);
		tmpHash.digest(helloToServerFinishedHash);

		sha256_hkdf_expand_label(clientTrafficSecret, hashLength, masterSecret, hashLength, "c ap traffic", 12, helloToServerFinishedHash, hashLength);
		sha256_hkdf_expand_label(clientKey, encryptionKeySize, clientTrafficSecret, hashLength, "key", 3, "", 0);
		sha256_hkdf_expand_label(clientIV, encryptionIVSize, clientTrafficSecret, hashLength, "iv", 2, "", 0);
		clientMessageDecryptCount = 0;

		sha256_hkdf_expand_label(serverTrafficSecret, hashLength, masterSecret, hashLength, "s ap traffic", 12, helloToServerFinishedHash, hashLength);
		sha256_hkdf_expand_label(serverKey, encryptionKeySize, serverTrafficSecret, hashLength, "key", 3, "", 0);
		sha256_hkdf_expand_label(serverIV, encryptionIVSize, serverTrafficSecret, hashLength, "iv", 2, "", 0);
		serverMessageEncryptCount = 0;
	}

	void force_key_update() {
		if (serverState != TLS_SERVER_STATE_APPLICATION) {
			return;
		}

		if (TLSRecord::DATA_BUFFER_SIZE - sendRecord.dataOffset > 32) {
			const u32 hashLength = SHA256_HASH_SIZE;
			const u32 encryptionKeySize = AES_KEY_SIZE_BYTES;
			const u32 encryptionIVSize = AES_GCM_IV_SIZE;

			u32 sendRecordBegin = sendRecord.write_header(TLS_RECORD_TYPE_APPLICATION_DATA);
			sendRecord.write_byte(TLS_HANDSHAKE_KEY_UPDATE);
			sendRecord.write_int24(1);
			sendRecord.write_byte(TLS_KEY_UPDATE_REQUESTED);
			sendRecord.write_byte(TLS_RECORD_TYPE_HANDSHAKE);
			sendRecord.patch_int16(sendRecord.dataOffset - sendRecordBegin - 5 + 16, sendRecordBegin + 3);
			sendRecord.encrypt(serverKey, serverIV, serverMessageEncryptCount, sendRecordBegin);
			ConnectionError sendError = sendRecord._send_data(clientTCPConnection);
			CHECK_CONNECTION_PROBLEM(sendError != CONNECTION_ERROR_SUCCESS);

			u8 oldTrafficSecret[hashLength];
			memcpy(oldTrafficSecret, serverTrafficSecret, hashLength);
			sha256_hkdf_expand_label(serverTrafficSecret, hashLength, oldTrafficSecret, hashLength, "traffic upd", 11, "", 0);
			sha256_hkdf_expand_label(serverKey, encryptionKeySize, serverTrafficSecret, hashLength, "key", 3, "", 0);
			sha256_hkdf_expand_label(serverIV, encryptionIVSize, serverTrafficSecret, hashLength, "iv", 2, "", 0);
			serverMessageEncryptCount = 0;

			shouldSendKeyUpdate = false;
		} else {
			shouldSendKeyUpdate = true;
		}
	}

	void do_server_hello() {
		Keccak random;
		random.make_secure_random();

		u8 serverPublicKey[32 + 32];
		secp256r1_generate_keypair(random, serverPublicKey, serverPrivateKey);
		
		u32 recordStart = sendRecord.write_header(TLS_RECORD_TYPE_HANDSHAKE);
		sendRecord.write_byte(TLS_HANDSHAKE_SERVER_HELLO);
		u32 helloLengthPos = sendRecord.dataOffset;
		sendRecord.write_int24(0);

		// Server version
		sendRecord.write_int16(0x0303);
		// Server random
		random.squeeze(sendRecord.dataBuffer + sendRecord.dataOffset, 32);
		sendRecord.dataOffset += 32;
		// Echo client session id
		sendRecord.write_bytes(sessionId, 32);
		sendRecord.write_int16(cipherSuite);
		// Compression method
		sendRecord.write_byte(0);


		u32 extensionsLengthPos = sendRecord.dataOffset;
		sendRecord.write_int16(0);

		sendRecord.write_int16(TLS_EXTENSION_SUPPORTED_VERSIONS);
		sendRecord.write_int16(2);
		sendRecord.write_int16(0x0304);

		sendRecord.write_int16(TLS_EXTENSION_KEY_SHARE);
		//TODO I think this is wrong, should be 4 + 1 + 32 + 32?
		sendRecord.write_int16(2 + 1 + 32 + 32);
		sendRecord.write_int16(TLS_GROUP_SECP256R1);
		sendRecord.write_int16(1 + 32 + 32);
		sendRecord.write_byte(4);
		sendRecord.write_bytes(serverPublicKey, 32 + 32);

		sendRecord.patch_int16(sendRecord.dataOffset - extensionsLengthPos - 2, extensionsLengthPos);


		sendRecord.write_int24(sendRecord.dataOffset - helloLengthPos - 3, helloLengthPos);
		sendRecord.patch_header_length(recordStart);

		runningMessageHash.update(sendRecord.dataBuffer + 5, sendRecord.dataOffset - 5);
	}

	void do_server_handshake() {
		const u32 hashLength = SHA256_HASH_SIZE;
		SHA256 tmpHash;

		u32 recordStart = sendRecord.write_header(TLS_RECORD_TYPE_CHANGE_CIPHER_SPEC);
		sendRecord.patch_int16(1, recordStart + 3);
		sendRecord.write_byte(1);

		recordStart = sendRecord.write_header(TLS_RECORD_TYPE_APPLICATION_DATA);
		sendRecord.write_byte(TLS_HANDSHAKE_ENCRYPTED_EXTENSIONS);
		sendRecord.write_int24(2);
		sendRecord.write_int16(0);
		runningMessageHash.update(sendRecord.dataBuffer + recordStart + 5, sendRecord.dataOffset - recordStart - 5);
		sendRecord.write_byte(TLS_RECORD_TYPE_HANDSHAKE);
		sendRecord.encrypt(serverKey, serverIV, serverMessageEncryptCount, recordStart);

		recordStart = sendRecord.write_header(TLS_RECORD_TYPE_APPLICATION_DATA);
		sendRecord.write_byte(TLS_HANDSHAKE_CERTIFICATE);
		sendRecord.write_int24(1 + 3 + 3 + certificate->certLength + 2);
		// certificate request context length
		sendRecord.write_byte(0);
		// certificate list length
		sendRecord.write_int24(3 + certificate->certLength + 2);
		// cert_data length
		sendRecord.write_int24(certificate->certLength);
		sendRecord.write_bytes(certificate->cert, certificate->certLength);
		// extensions length
		sendRecord.write_int16(0);
		runningMessageHash.update(sendRecord.dataBuffer + recordStart + 5, sendRecord.dataOffset - recordStart - 5);
		sendRecord.write_byte(TLS_RECORD_TYPE_HANDSHAKE);
		sendRecord.encrypt(serverKey, serverIV, serverMessageEncryptCount, recordStart);

		recordStart = sendRecord.write_header(TLS_RECORD_TYPE_APPLICATION_DATA);
		sendRecord.write_byte(TLS_HANDSHAKE_CERTIFICATE_VERIFY);
		constexpr u32 rsaBits = 2048;
		constexpr u32 signatureSize = rsaBits / 8;
		sendRecord.write_int24(2 + 2 + signatureSize);
		sendRecord.write_int16(TLS_RSA_PKCS1_SHA256);
		sendRecord.write_int16(signatureSize);
		const char* contextString = "TLS 1.3, server CertificateVerify";
		constexpr u32 contextStringLength = 33;
		constexpr u32 signatureContextLength = 64 + contextStringLength + 1 + hashLength;
		u8 signatureContext[signatureContextLength];
		memset(signatureContext, 0x20, 64);
		memcpy(signatureContext + 64, contextString, contextStringLength);
		signatureContext[64 + contextStringLength] = 0x00;
		tmpHash.copy_from(runningMessageHash);
		tmpHash.digest(signatureContext + 64 + contextStringLength + 1);
		u8 signature[signatureSize];
		rsassa_pkcs1_sign_sha256(signature, signatureContext, signatureContextLength, certificate->rsaPrivateKey, certificate->rsaModulus, rsaBits);
		sendRecord.write_bytes(signature, signatureSize);
		runningMessageHash.update(sendRecord.dataBuffer + recordStart + 5, sendRecord.dataOffset - recordStart - 5);
		sendRecord.write_byte(TLS_RECORD_TYPE_HANDSHAKE);
		sendRecord.encrypt(serverKey, serverIV, serverMessageEncryptCount, recordStart);

		recordStart = sendRecord.write_header(TLS_RECORD_TYPE_APPLICATION_DATA);
		sendRecord.write_byte(TLS_HANDSHAKE_FINISHED);
		sendRecord.write_int24(hashLength);
		u8 finishedKey[hashLength];
		sha256_hkdf_expand_label(finishedKey, hashLength, serverTrafficSecret, hashLength, "finished", 8, "", 0);
		u8 helloToNowHash[hashLength];
		tmpHash.copy_from(runningMessageHash);
		tmpHash.digest(helloToNowHash);
		u8 verifyData[hashLength];
		sha256_hmac(verifyData, finishedKey, hashLength, helloToNowHash, hashLength);
		sendRecord.write_bytes(verifyData, hashLength);
		runningMessageHash.update(sendRecord.dataBuffer + recordStart + 5, sendRecord.dataOffset - recordStart - 5);
		sendRecord.write_byte(TLS_RECORD_TYPE_HANDSHAKE);
		sendRecord.encrypt(serverKey, serverIV, serverMessageEncryptCount, recordStart);
	}

	void handle_client_hello(u32 dataBegin, u16 messageEnd) {
		CHECK_CONNECTION_PROBLEM((receiveRecord.dataOffset + 2 + 32 + 1 + 32 + 2) > messageEnd);
		// Version should be 0x0303
		u16 version = receiveRecord.read_int16();
		// Client random
		receiveRecord.dataOffset += 32;
		u8 sessionIdLength = receiveRecord.read_byte();
		CHECK_CONNECTION_PROBLEM(sessionIdLength != 32);
		receiveRecord.read_bytes(sessionId, sessionIdLength);
		
		u16 cipherSuitesEnd = receiveRecord.read_int16();
		cipherSuitesEnd += receiveRecord.dataOffset;
		CHECK_CONNECTION_PROBLEM(cipherSuitesEnd > messageEnd);
		b32 hasServerCipher = false;
		while (receiveRecord.dataOffset + 1 < cipherSuitesEnd) {
			TLSCipherSuite clientCipherSuite = static_cast<TLSCipherSuite>(receiveRecord.read_int16());
			if (clientCipherSuite == TLS_AES_128_GCM_SHA256) {
				hasServerCipher = true;
			}
		}
		cipherSuite = TLS_AES_128_GCM_SHA256;

		CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 2 > messageEnd);
		u8 legacyCompressionMethodsLength = receiveRecord.read_byte();
		CHECK_CONNECTION_PROBLEM(legacyCompressionMethodsLength != 1);
		u8 legacyCompressionMethod = receiveRecord.read_byte();
		CHECK_CONNECTION_PROBLEM(legacyCompressionMethod != 0);

		CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 2 > messageEnd);
		u32 extensionsEnd = receiveRecord.read_int16();
		extensionsEnd += receiveRecord.dataOffset;
		CHECK_CONNECTION_PROBLEM(extensionsEnd > messageEnd);
		while (receiveRecord.dataOffset < extensionsEnd) {
			CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 4 < extensionsEnd);
			TLSExtension extensionType = static_cast<TLSExtension>(receiveRecord.read_int16());
			u32 extensionEnd = receiveRecord.read_int16();
			extensionEnd += receiveRecord.dataOffset;
			CHECK_CONNECTION_PROBLEM(extensionEnd > extensionsEnd);

			if (extensionType == TLS_EXTENSION_SUPPORTED_VERSIONS) {
				CHECK_CONNECTION_PROBLEM(extensionEnd + 3 > extensionEnd);
				u8 tlsVersionEnd = receiveRecord.read_byte();
				CHECK_CONNECTION_PROBLEM(tlsVersionEnd < 2);
				tlsVersionEnd += receiveRecord.dataOffset;
				b32 hasTls13 = false;
				while (receiveRecord.dataOffset + 1 < tlsVersionEnd) {
					u16 version = receiveRecord.read_int16();
					if (version == 0x0304) {
						hasTls13 = true;
					}
				}
				CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset != tlsVersionEnd);
				CHECK_CONNECTION_PROBLEM(!hasTls13);
			} else if (extensionType == TLS_EXTENSION_SERVER_NAME) {
				u32 nameListEnd = receiveRecord.read_int16();
				CHECK_CONNECTION_PROBLEM(nameListEnd < 3);
				nameListEnd += receiveRecord.dataOffset;
				CHECK_CONNECTION_PROBLEM(nameListEnd > extensionEnd);

				TLSServerNameType nameType = static_cast<TLSServerNameType>(receiveRecord.read_byte());
				CHECK_CONNECTION_PROBLEM(nameType != TLS_SERVER_NAME_HOST_NAME);
				u16 hostNameLength = receiveRecord.read_int16();
				CHECK_CONNECTION_PROBLEM(hostNameLength + receiveRecord.dataOffset > extensionEnd);
			} else if (extensionType == TLS_EXTENSION_SUPPORTED_GROUPS) {
				u32 supportedGroupEnd = receiveRecord.read_int16();
				supportedGroupEnd += receiveRecord.dataOffset;
				CHECK_CONNECTION_PROBLEM(supportedGroupEnd != extensionEnd);

				b32 hasSecp256r1 = false;
				while (receiveRecord.dataOffset + 1 < supportedGroupEnd) {
					TLSNamedGroup group = static_cast<TLSNamedGroup>(receiveRecord.read_int16());
					if (group == TLS_GROUP_SECP256R1) {
						hasSecp256r1 = true;
					}
				}
				CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset != supportedGroupEnd);
				CHECK_CONNECTION_PROBLEM(!hasSecp256r1);
			} else if (extensionType == TLS_EXTENSION_SIGNATURE_ALGORITHMS) {
				u32 signatureSchemeEnd = receiveRecord.read_int16();
				signatureSchemeEnd += receiveRecord.dataOffset;
				CHECK_CONNECTION_PROBLEM(signatureSchemeEnd != extensionEnd);

				b32 hasSecpr1ECDSA = false;
				while (receiveRecord.dataOffset + 1 < signatureSchemeEnd) {
					TLSSignatureScheme signatureScheme = static_cast<TLSSignatureScheme>(receiveRecord.read_int16());
					if (signatureScheme == TLS_ECDSA_SECP256R1_SHA256) {
						hasSecpr1ECDSA = true;
					}
				}
				CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset != signatureSchemeEnd);
				CHECK_CONNECTION_PROBLEM(!hasSecpr1ECDSA);
			} else if (extensionType == TLS_EXTENSION_KEY_SHARE) {
				u32 keyShareEnd = receiveRecord.read_int16();
				keyShareEnd += receiveRecord.dataOffset;
				CHECK_CONNECTION_PROBLEM(keyShareEnd > extensionEnd);
				b32 hasSecp256r1 = false;
				while (receiveRecord.dataOffset + 3 < keyShareEnd) {
					TLSNamedGroup group = static_cast<TLSNamedGroup>(receiveRecord.read_int16());
					u32 publicKeyEnd = receiveRecord.read_int16();
					publicKeyEnd += receiveRecord.dataOffset;
					if (group == TLS_GROUP_SECP256R1) {
						u8 legacyCompression = receiveRecord.read_byte();
						CHECK_CONNECTION_PROBLEM(legacyCompression != 4);
						CHECK_CONNECTION_PROBLEM(publicKeyEnd - receiveRecord.dataOffset != 32 + 32);
						receiveRecord.read_bytes(clientPublicKey, 32 + 32);
						hasSecp256r1 = true;
					} else {
						receiveRecord.dataOffset = publicKeyEnd;
					}
				}
				CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset != keyShareEnd);
				CHECK_CONNECTION_PROBLEM(!hasSecp256r1);
			} else {
				receiveRecord.dataOffset = extensionEnd;
			}
			CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset != extensionEnd);
		}
		CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset != extensionsEnd);

		runningMessageHash.update(receiveRecord.dataBuffer + dataBegin, messageEnd - dataBegin);

		calc_handshake_keys();
	}

	void handle_client_handshake(TLSHandshakeType messageType, u32 messageEnd) {
		if (messageType == TLS_HANDSHAKE_FINISHED) {
			const u32 hashLength = SHA256_HASH_SIZE;
			CHECK_CONNECTION_PROBLEM(messageEnd - receiveRecord.dataOffset != hashLength);

			u8 helloToNowHash[hashLength];
			SHA256 tmpHash;
			tmpHash.copy_from(runningMessageHash);
			tmpHash.digest(helloToNowHash);

			u8 finishedKey[hashLength];
			sha256_hkdf_expand_label(finishedKey, hashLength, clientTrafficSecret, hashLength, "finished", 8, "", 0);
			u8 verifyData[hashLength];
			sha256_hmac(verifyData, finishedKey, hashLength, helloToNowHash, hashLength);

			b32 handshakeVerified = memcmp(verifyData, receiveRecord.dataBuffer + receiveRecord.dataOffset, hashLength) == 0;
			CHECK_CONNECTION_PROBLEM(!handshakeVerified);
			receiveRecord.dataOffset += hashLength;

			calc_application_keys();

			serverState = TLS_SERVER_STATE_APPLICATION;
		} else {
			CONNECTION_ERROR;
		}
	}

	void process_record(TLSRecordType recordType, u32 dataBegin, u32 recordEnd) {
		if (serverState == TLS_SERVER_STATE_HELLO) {
			if (recordType == TLS_RECORD_TYPE_HANDSHAKE) {
				TLSHandshakeType messageType = static_cast<TLSHandshakeType>(receiveRecord.read_byte());
				CHECK_CONNECTION_PROBLEM(messageType != TLS_HANDSHAKE_CLIENT_HELLO);
				u32 messageLength = receiveRecord.read_int24();
				u32 messageEnd = receiveRecord.dataOffset + messageLength;
				CHECK_CONNECTION_PROBLEM(messageEnd > recordEnd);
				handle_client_hello(dataBegin, messageEnd);
				dataBegin = messageEnd;
			} else {
				CONNECTION_ERROR;
			}
			sendRecord.reset();
			do_server_hello();
			serverState = TLS_SERVER_STATE_HANDSHAKE;
			do_server_handshake();
			sendRecord._send_data(clientTCPConnection);
		} else if (serverState == TLS_SERVER_STATE_HANDSHAKE) {
			if (recordType == TLS_RECORD_TYPE_APPLICATION_DATA) {
				b32 decryptSuccess = receiveRecord.decrypt(clientKey, clientIV, clientMessageDecryptCount, recordEnd);
				CHECK_CONNECTION_PROBLEM(!decryptSuccess);

				u32 messagesEnd = recordEnd - 17;
				u32 zeroPadding = 0;
				for (; messagesEnd > 0 && receiveRecord.dataBuffer[messagesEnd] == 0; messagesEnd--, zeroPadding++);
				TLSRecordType realRecordType = static_cast<TLSRecordType>(receiveRecord.dataBuffer[messagesEnd]);
				if (realRecordType == TLS_RECORD_TYPE_HANDSHAKE) {
					while (receiveRecord.dataOffset < messagesEnd) {
						u32 messageStart = receiveRecord.dataOffset;
						TLSHandshakeType messageType = static_cast<TLSHandshakeType>(receiveRecord.read_byte());
						u32 messageLength = receiveRecord.read_int24();
						u32 messageEnd = receiveRecord.dataOffset + messageLength;
						CHECK_CONNECTION_PROBLEM(messageEnd > messagesEnd);
						handle_client_handshake(messageType, messageEnd);
						CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset != messageEnd);
						runningMessageHash.update(receiveRecord.dataBuffer + messageStart, messageEnd - messageStart);
					}
				} else if (realRecordType == TLS_RECORD_TYPE_ALERT) {
					CHECK_CONNECTION_PROBLEM(messagesEnd - receiveRecord.dataOffset != 2);
					TLSAlertLevel alertLevel = static_cast<TLSAlertLevel>(receiveRecord.read_byte());
					TLSAlertDescription alert = static_cast<TLSAlertDescription>(receiveRecord.read_byte());
					CONNECTION_ERROR;
				} else {
					CONNECTION_ERROR;
				}

				receiveRecord.dataOffset += zeroPadding;
				receiveRecord.dataOffset += 17;
			} else if (recordType == TLS_RECORD_TYPE_CHANGE_CIPHER_SPEC) {
				CHECK_CONNECTION_PROBLEM(recordEnd != receiveRecord.dataOffset + 1);
				u8 data = receiveRecord.read_byte();
				CHECK_CONNECTION_PROBLEM(data != 0x01);
			} else {
				CONNECTION_ERROR;
			}
		} else if (serverState == TLS_SERVER_STATE_APPLICATION) {
			if (recordType == TLS_RECORD_TYPE_APPLICATION_DATA) {
				b32 decryptSuccess = receiveRecord.decrypt(clientKey, clientIV, clientMessageDecryptCount, recordEnd);
				CHECK_CONNECTION_PROBLEM(!decryptSuccess);

				u32 messagesEnd = recordEnd - 16 - 1;
				u32 zeroPadding = 0;
				for (; messagesEnd > 0 && receiveRecord.dataBuffer[messagesEnd] == 0; messagesEnd--, zeroPadding++);
				TLSRecordType realRecordType = static_cast<TLSRecordType>(receiveRecord.dataBuffer[messagesEnd]);
				if (realRecordType == TLS_RECORD_TYPE_APPLICATION_DATA) {

				} else if (realRecordType == TLS_RECORD_TYPE_HANDSHAKE) {
					CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 4 >= messagesEnd);
					TLSHandshakeType handshakeType = static_cast<TLSHandshakeType>(receiveRecord.read_byte());
					u32 handshakeLength = receiveRecord.read_int24();
					u32 handshakeEnd = receiveRecord.dataOffset + handshakeLength;
					CHECK_CONNECTION_PROBLEM(handshakeEnd > messagesEnd);

					if (handshakeType == TLS_HANDSHAKE_KEY_UPDATE) {
						CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 1 > handshakeEnd);
						TLSKeyUpdateRequest updateRequest = static_cast<TLSKeyUpdateRequest>(receiveRecord.read_byte());

						const u32 hashLength = SHA256_HASH_SIZE;
						const u32 encryptionKeySize = AES_KEY_SIZE_BYTES;
						const u32 encryptionIVSize = AES_GCM_IV_SIZE;

						u8 oldTrafficSecret[hashLength];
						memcpy(oldTrafficSecret, clientTrafficSecret, hashLength);
						sha256_hkdf_expand_label(clientTrafficSecret, hashLength, oldTrafficSecret, hashLength, "traffic upd", 11, "", 0);
						sha256_hkdf_expand_label(clientKey, encryptionKeySize, clientTrafficSecret, hashLength, "key", 3, "", 0);
						sha256_hkdf_expand_label(clientIV, encryptionIVSize, clientTrafficSecret, hashLength, "iv", 2, "", 0);
						clientMessageDecryptCount = 0;

						shouldKeyUpdate |= updateRequest == TLS_KEY_UPDATE_REQUESTED;
					} else {
						CONNECTION_ERROR;	
					}
				} else if (realRecordType == TLS_RECORD_TYPE_ALERT) {
					CHECK_CONNECTION_PROBLEM((messagesEnd - receiveRecord.dataOffset) != 2);
					TLSAlertLevel alertLevel = static_cast<TLSAlertLevel>(receiveRecord.read_byte());
					TLSAlertDescription alert = static_cast<TLSAlertDescription>(receiveRecord.read_byte());
					CONNECTION_ERROR;
				} else {
					CONNECTION_ERROR;
				}
				// Do something with the application data here
				u32 dataLength = messagesEnd - receiveRecord.dataOffset;
				u32 receivedLength = min(dataLength, u32(sizeof(receiveBuffer)) - receiveBufferDataEnd);
				u32 receivedLengthIfBufferWasContiguous = min(dataLength, receiveBufferCap - receiveBufferDataEnd + receiveBufferPos);
				if (receivedLengthIfBufferWasContiguous > receivedLength) {
					receivedLength = receivedLengthIfBufferWasContiguous;
					memmove(receiveBuffer, receiveBuffer + receiveBufferPos, receiveBufferDataEnd - receiveBufferPos);
					receiveBufferDataEnd -= receiveBufferPos;
					receiveBufferPos = 0;
				}
				memcpy(receiveBuffer + receiveBufferDataEnd, receiveRecord.dataBuffer + receiveRecord.dataOffset, receivedLength);
				
				receiveRecord.dataOffset += receivedLength;
				dataLeftInReceiveRecord = dataLength - receivedLength;
				if (receivedLength == dataLength) {
					receiveRecord.dataOffset += zeroPadding;
					receiveRecord.dataOffset += 1 + 16;
				}
			} else if (recordType == TLS_RECORD_TYPE_ALERT) {
				CHECK_CONNECTION_PROBLEM((recordEnd - receiveRecord.dataOffset) != 2);
				TLSAlertLevel alertLevel = static_cast<TLSAlertLevel>(receiveRecord.read_byte());
				TLSAlertDescription alert = static_cast<TLSAlertDescription>(receiveRecord.read_byte());
				CONNECTION_ERROR;
			} else {
				CONNECTION_ERROR;
			}
		}
		CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset != recordEnd);
	}

	void process_pending_messages() {
		while (receiveRecord.numBytesSentOrReceived - receiveRecord.dataOffset >= 5) {
			TLSRecordType recordType = static_cast<TLSRecordType>(receiveRecord.read_byte());
			u16 version = receiveRecord.read_int16();
			u16 size = receiveRecord.read_int16();
			CHECK_CONNECTION_PROBLEM(size > TLSRecord::MAX_RECORD_SIZE);
			if (receiveRecord.dataOffset + size > receiveRecord.numBytesSentOrReceived) {
				break;
			}
			process_record(recordType, receiveRecord.dataOffset, receiveRecord.dataOffset + size);
		}
		if (receiveRecord.dataOffset < receiveRecord.numBytesSentOrReceived) {
			memmove(receiveRecord.dataBuffer, receiveRecord.dataBuffer + receiveRecord.dataOffset, receiveRecord.numBytesSentOrReceived - receiveRecord.dataOffset);
			receiveRecord.numBytesSentOrReceived -= receiveRecord.dataOffset;
			receiveRecord.dataOffset = 0;
		}
	}

	// Will only process one record's worth of data at a time.
	// If it returns true, call it again to possibly process more
	b32 receive_data() {
		if (dataLeftInReceiveRecord > 0) {
			u32 amountToTransfer = min(dataLeftInReceiveRecord, receiveBufferCap - receiveBufferDataEnd);
			u32 amountToTransferIfBufferWasContiguous = min(dataLeftInReceiveRecord, receiveBufferCap - receiveBufferDataEnd + receiveBufferPos);
			if (amountToTransferIfBufferWasContiguous > amountToTransfer) {
				amountToTransfer = amountToTransferIfBufferWasContiguous;
				memmove(receiveBuffer, receiveBuffer + receiveBufferPos, receiveBufferDataEnd - receiveBufferPos);
				receiveBufferDataEnd -= receiveBufferPos;
				receiveBufferPos = 0;
			}
			memcpy(receiveBuffer + receiveBufferDataEnd, receiveRecord.dataBuffer + receiveRecord.dataOffset, amountToTransfer);
			receiveBufferDataEnd += amountToTransfer;
			receiveRecord.dataOffset += amountToTransfer;
			dataLeftInReceiveRecord -= amountToTransfer;
			// Check to make sure we read the whole record
			if (dataLeftInReceiveRecord == 0) {
				u32 recordLength = receiveRecord.read_int16(3);
				u32 recordEnd = 5 + recordLength;
				while (receiveRecord.dataOffset != recordEnd && receiveRecord.dataBuffer[receiveRecord.dataOffset] == 0) {
					receiveRecord.dataOffset++;
				}
				receiveRecord.dataOffset++;
				receiveRecord.dataOffset += 16;
				if (receiveRecord.dataOffset != recordEnd) {
					serverState = TLS_SERVER_STATE_CLOSE_PENDING;
					return false;
				}
			} else {
				return true;
			}
		}
		u32 prevBytesReceived = receiveRecord.numBytesSentOrReceived;
		NetworkTransferResult receiveResult = receiveRecord._receive_all_data(clientTCPConnection);
		if (receiveResult != NET_TRANSFER_RESULT_COMPLETE) {
			if (receiveResult != NET_TRANSFER_RESULT_INCOMPLETE) {
				serverState = TLS_SERVER_STATE_CLOSE_PENDING; 
			}
			return false;
		}
		b32 possiblyMoreDataToReceive = prevBytesReceived != receiveRecord.numBytesSentOrReceived || prevBytesReceived == TLSRecord::DATA_BUFFER_SIZE;
		process_pending_messages();
		return possiblyMoreDataToReceive;
	}

	void send_data() {
		if (serverState != TLS_SERVER_STATE_APPLICATION || (sendBufferPos == 0 && sendRecord.send_completed())) {
			return;
		}
		if (!sendRecord.send_completed()) {
			ConnectionError sendError = sendRecord._send_data(clientTCPConnection);
			if (sendError != CONNECTION_ERROR_SUCCESS) {
				CONNECTION_ERROR;
			}
		}

		if (!sendRecord.send_completed()) {
			return;
		}

		if (shouldSendKeyUpdate) {
			force_key_update();
			if (!shouldSendKeyUpdate) {
				shouldKeyUpdate = false;
			}
		}

		if (shouldKeyUpdate) {
			sendRecord.init(TLS_RECORD_TYPE_APPLICATION_DATA);
			sendRecord.write_byte(TLS_HANDSHAKE_KEY_UPDATE);
			sendRecord.write_int24(1);
			sendRecord.write_byte(shouldSendKeyUpdate ? TLS_KEY_UPDATE_REQUESTED : TLS_KEY_UPDATE_NOT_REQUESTED);
			sendRecord.write_byte(TLS_RECORD_TYPE_HANDSHAKE);
			sendRecord.encrypt(serverKey, serverIV, serverMessageEncryptCount, 0);
			ConnectionError sendError = sendRecord._send_data(clientTCPConnection);
			if (sendError != CONNECTION_ERROR_SUCCESS) {
				serverState = TLS_SERVER_STATE_CLOSE_PENDING;
				return;
			}

			u8 oldTrafficSecret[SHA256_HASH_SIZE];
			memcpy(oldTrafficSecret, serverTrafficSecret, SHA256_HASH_SIZE);
			sha256_hkdf_expand_label(serverTrafficSecret, SHA256_HASH_SIZE, oldTrafficSecret, SHA256_HASH_SIZE, "traffic upd", 11, "", 0);
			sha256_hkdf_expand_label(serverKey, AES_KEY_SIZE_BYTES, serverTrafficSecret, SHA256_HASH_SIZE, "key", 3, "", 0);
			sha256_hkdf_expand_label(serverIV, AES_GCM_IV_SIZE, serverTrafficSecret, SHA256_HASH_SIZE, "iv", 2, "", 0);
			serverMessageEncryptCount = 0;
			shouldKeyUpdate = false;
			shouldSendKeyUpdate = false;
		}

		while (sendQueueReadPos != sendQueueWritePos) {
			if (!sendRecord.send_completed()) {
				return;
			}
			sendRecord.init(TLS_RECORD_TYPE_APPLICATION_DATA);
			u32 encodeSize = 0;
			while (sendQueueReadPos != sendQueueWritePos && encodeSize < TLSRecord::MAX_APPLICATION_DATA_SIZE) {
				TLSSendEntry& entry = sendQueue[sendQueueReadPos & sendQueueMask];
				u32 amountToWrite;
				if (entry.resource) {
					amountToWrite = min(TLSRecord::MAX_APPLICATION_DATA_SIZE - encodeSize, entry.size);
					sendRecord.write_bytes(entry.resource, amountToWrite);
					entry.resource += amountToWrite;
				} else {
					entry.size = min(entry.size, sendBufferDataEnd - sendBufferPos);
					amountToWrite = min(TLSRecord::MAX_APPLICATION_DATA_SIZE - encodeSize, entry.size);
					sendRecord.write_bytes(sendBuffer + sendBufferPos, amountToWrite);
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
			sendRecord.write_byte(TLS_RECORD_TYPE_APPLICATION_DATA);
			sendRecord.encrypt(serverKey, serverIV, serverMessageEncryptCount, 0);
			ConnectionError sendError = sendRecord._send_data(clientTCPConnection);
			if (sendError != CONNECTION_ERROR_SUCCESS) {
				serverState = TLS_SERVER_STATE_CLOSE_PENDING;
				return;
			}
		}
	}

	b32 queue_buffer_to_send(void* buffer, u32 length) {
		if (buffer == nullptr && sendQueueWritePos != sendQueueReadPos && sendQueue[(sendQueueWritePos - 1) & sendQueueMask].resource == nullptr) {
			sendQueue[(sendQueueWritePos - 1) & sendQueueMask].size += length;
			return true;
		}
		if (sendQueueWritePos - sendQueueReadPos < sendQueueCap) {
			sendQueue[(sendQueueWritePos++) & sendQueueMask] = TLSSendEntry{ reinterpret_cast<u8*>(buffer), length };
			return true;
		}
		return false;
	}

	TLSServerClientConnection& write_str(const char* str) {
		if (serverState == TLS_SERVER_STATE_CLOSE_PENDING) {
			return *this;
		}
		while (str[0] != '\0') {
			u32 dataQueuedForSend = 0;
			while (str[0] != '\0' && sendBufferDataEnd < sizeof(sendBuffer)) {
				sendBuffer[sendBufferDataEnd++] = str[0];
				str++, dataQueuedForSend++;
			}
			if (str[0] != '\0') {
				if (sendBufferPos != 0) {
					memmove(sendBuffer, sendBuffer + sendBufferPos, sendBufferDataEnd - sendBufferPos);
					sendBufferDataEnd -= sendBufferPos;
					sendBufferPos = 0;
				} else {
					queue_buffer_to_send(nullptr, dataQueuedForSend);
					send_data();
				}
			}
		}
		return *this;
	}

	TLSServerClientConnection& patch_content_length(u32 pos, u32 contentStart) {
		u32 length = sendBufferPos - contentStart;
		while (length != 0 && sendBuffer[pos] == ' ') {
			sendBuffer[pos--] = (length % 10) + '0';
			length /= 10;
		}
		return *this;
	}

	void clear_receive_buffer() {
		receiveBufferPos = 0;
		receiveBufferDataEnd = 0;
		dataLeftInReceiveRecord = 0;
	}

#undef CHECK_CONNECTION_PROBLEM
#undef CONNECTION_ERROR
};

// Immediately abort the connection if we find anything that doesn't match
#ifdef _DEBUG
#define CONNECTION_ERROR state = TLS_CLIENT_STATE_CLOSED; g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection); return
#else
#define CONNECTION_ERROR state = TLS_CLIENT_STATE_CLOSED; g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection); return
#endif
#define CHECK_CONNECTION_PROBLEM(cond) if(cond) { CONNECTION_ERROR; }

enum TLSClientState : u32 {
	TLS_CLIENT_STATE_CLOSED,
	TLS_CLIENT_STATE_HELLO_SENT,
	TLS_CLIENT_STATE_HELLO_RECEIVED,
	TLS_CLIENT_STATE_OPEN
};

struct TLSClient {
	char sendBuffer[16 * 1024];
	char receiveBuffer[16 * 1024];
	u32 sendBufferPos;
	u32 receiveBufferPos;
	u32 sendBufferCap;
	u32 receiveBufferCap;
	TLSClientState state;

	u32 serverTCPConnection;
	u8 clientPrivateKey[32];
	u8 clientPublicKey[32 + 32];
	X509SignatureAlgorithm certSignatureAlgorithm;
	BigInteger certRSAModulusOrECDSAPointX;
	BigInteger certRSAPublicKeyOrECDSAPointY;
	u8 masterSecret[SHA256_HASH_SIZE];
	u8 serverApplicationTrafficSecret[SHA256_HASH_SIZE];
	u8 serverApplicationKey[AES_KEY_SIZE_BYTES];
	u8 serverApplicationIV[AES_GCM_IV_SIZE];
	u8 clientApplicationTrafficSecret[SHA256_HASH_SIZE];
	u8 clientApplicationKey[AES_KEY_SIZE_BYTES];
	u8 clientApplicationIV[AES_GCM_IV_SIZE];
	u64 encryptedMessageCounter;
	u64 decryptedMessageCounter;
	b32 shouldKeyUpdate;
	SHA256 recordsHash;

	TLSRecord sendRecord;
	TLSRecord receiveRecord;
	u32 dataLeftInReceiveRecord;

	void send_data() {
		if (state != TLS_CLIENT_STATE_OPEN || (sendBufferPos == 0 && sendRecord.send_completed())) {
			return;
		}
		if (!sendRecord.send_completed()) {
			ConnectionError sendError = sendRecord._send_data(serverTCPConnection);
			if (sendError != CONNECTION_ERROR_SUCCESS) {
				g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection);
				state = TLS_CLIENT_STATE_CLOSED;
				return;
			}
		}
		if (!sendRecord.send_completed()) {
			return;
		}
		if (shouldKeyUpdate) {
			sendRecord.init(TLS_RECORD_TYPE_APPLICATION_DATA);
			sendRecord.write_byte(TLS_HANDSHAKE_KEY_UPDATE);
			sendRecord.write_int24(1);
			sendRecord.write_byte(TLS_KEY_UPDATE_NOT_REQUESTED);
			sendRecord.write_byte(TLS_RECORD_TYPE_HANDSHAKE);
			sendRecord.encrypt(clientApplicationKey, clientApplicationIV, encryptedMessageCounter, 0);
			ConnectionError sendError = sendRecord._send_data(serverTCPConnection);
			if (sendError != CONNECTION_ERROR_SUCCESS) {
				g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection);
				state = TLS_CLIENT_STATE_CLOSED;
				return;
			}

			u8 oldTrafficSecret[SHA256_HASH_SIZE];
			memcpy(oldTrafficSecret, clientApplicationTrafficSecret, SHA256_HASH_SIZE);
			sha256_hkdf_expand_label(clientApplicationTrafficSecret, SHA256_HASH_SIZE, oldTrafficSecret, SHA256_HASH_SIZE, "traffic upd", 11, "", 0);
			sha256_hkdf_expand_label(clientApplicationKey, AES_KEY_SIZE_BYTES, clientApplicationTrafficSecret, SHA256_HASH_SIZE, "key", 3, "", 0);
			sha256_hkdf_expand_label(clientApplicationIV, AES_GCM_IV_SIZE, clientApplicationTrafficSecret, SHA256_HASH_SIZE, "iv", 2, "", 0);
			encryptedMessageCounter = 0;
			shouldKeyUpdate = false;
		}

		u32 encodeOffset = 0;
		while (encodeOffset != sendBufferPos) {
			if (!sendRecord.send_completed()) {
				sendBufferPos -= encodeOffset;
				memmove(sendBuffer, sendBuffer + encodeOffset, sendBufferPos);
				return;
			}
			u32 encodeSize = min(TLSRecord::MAX_APPLICATION_DATA_SIZE, sendBufferPos - encodeOffset);
			sendRecord.init(TLS_RECORD_TYPE_APPLICATION_DATA);
			sendRecord.write_bytes(sendBuffer + encodeOffset, encodeSize);
			sendRecord.write_byte(TLS_RECORD_TYPE_APPLICATION_DATA);
			sendRecord.encrypt(clientApplicationKey, clientApplicationIV, encryptedMessageCounter, 0);
			ConnectionError sendError = sendRecord._send_data(serverTCPConnection);
			if (sendError != CONNECTION_ERROR_SUCCESS) {
				g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection);
				state = TLS_CLIENT_STATE_CLOSED;
				return;
			}
			encodeOffset += encodeSize;
		}
		sendBufferPos = 0;
	}

	void process_received_record() {
		if (state == TLS_CLIENT_STATE_OPEN) {
			TLSRecordType recordType = static_cast<TLSRecordType>(receiveRecord.read_byte());
			u16 version = receiveRecord.read_int16();
			u16 recordLength = receiveRecord.read_int16();
			u32 recordEnd = receiveRecord.dataOffset + recordLength;
			CHECK_CONNECTION_PROBLEM(recordEnd > (TLSRecord::MAX_RECORD_SIZE + 5));

			if (recordType == TLS_RECORD_TYPE_APPLICATION_DATA) {
				CHECK_CONNECTION_PROBLEM(recordLength < 17);
				b32 decryptSuccess = receiveRecord.decrypt(serverApplicationKey, serverApplicationIV, decryptedMessageCounter, recordEnd);
				CHECK_CONNECTION_PROBLEM(!decryptSuccess);

				u32 messagesEnd = recordEnd - 17;
				u32 zeroPadding = 0;
				for (; messagesEnd > 0 && receiveRecord.dataBuffer[messagesEnd] == 0; messagesEnd--, zeroPadding++);
				TLSRecordType realRecordType = static_cast<TLSRecordType>(receiveRecord.dataBuffer[messagesEnd]);

				if (realRecordType == TLS_RECORD_TYPE_ALERT) {
					TLSAlertLevel alertLevel = static_cast<TLSAlertLevel>(receiveRecord.read_byte());
					TLSAlertDescription alert = static_cast<TLSAlertDescription>(receiveRecord.read_byte());
					if (alert == TLS_ALERT_CLOSE_NOTIFY) {
						// Technically wrong, should send a second close notify I think?
						state = TLS_CLIENT_STATE_CLOSED;
						g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection);
					} else {
						CONNECTION_ERROR;
					}
				} else if (realRecordType == TLS_RECORD_TYPE_HANDSHAKE) {
					CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 4 >= messagesEnd);
					TLSHandshakeType handshakeType = static_cast<TLSHandshakeType>(receiveRecord.read_byte());
					u32 handshakeLength = receiveRecord.read_int24();
					u32 handshakeEnd = receiveRecord.dataOffset + handshakeLength;
					CHECK_CONNECTION_PROBLEM(handshakeEnd > messagesEnd);

					if (handshakeType == TLS_HANDSHAKE_KEY_UPDATE) {
						CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 1 > handshakeEnd);
						TLSKeyUpdateRequest updateRequest = static_cast<TLSKeyUpdateRequest>(receiveRecord.read_byte());

						u8 oldTrafficSecret[SHA256_HASH_SIZE];
						memcpy(oldTrafficSecret, serverApplicationTrafficSecret, SHA256_HASH_SIZE);
						sha256_hkdf_expand_label(serverApplicationTrafficSecret, SHA256_HASH_SIZE, oldTrafficSecret, SHA256_HASH_SIZE, "traffic upd", 11, "", 0);
						sha256_hkdf_expand_label(serverApplicationKey, AES_BLOCK_SIZE_BYTES, serverApplicationTrafficSecret, SHA256_HASH_SIZE, "key", 3, "", 0);
						sha256_hkdf_expand_label(serverApplicationIV, AES_GCM_IV_SIZE, serverApplicationTrafficSecret, SHA256_HASH_SIZE, "iv", 2, "", 0);
						decryptedMessageCounter = 0;

						shouldKeyUpdate |= updateRequest == TLS_KEY_UPDATE_REQUESTED;
					} else if (handshakeType == TLS_HANDSHAKE_NEW_SESSION_TICKET) {
						CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 10 > handshakeEnd);
						u32 ticketLifetime = receiveRecord.read_int32();
						u32 ticketAgeAdd = receiveRecord.read_int32();

						u8 nonceDataLength = receiveRecord.read_byte();
						CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + nonceDataLength > handshakeEnd);
						receiveRecord.dataOffset += nonceDataLength;

						CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 2 > handshakeEnd);
						u16 ticketDataLength = receiveRecord.read_int16();
						CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + ticketDataLength > handshakeEnd);
						receiveRecord.dataOffset += ticketDataLength;

						CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 2 > handshakeEnd);
						u16 ticketExtensionDataLength = receiveRecord.read_int16();
						CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + ticketExtensionDataLength > handshakeEnd);
						u32 ticketExtensionEnd = receiveRecord.dataOffset + ticketExtensionDataLength;
						while (receiveRecord.dataOffset < ticketExtensionEnd) {
							CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + 4 > ticketExtensionEnd);
							u16 ticketRandomExtensionType = receiveRecord.read_int16();
							u16 ticketExtensionLength = receiveRecord.read_int16();
							CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset + ticketExtensionLength > ticketExtensionEnd);
							receiveRecord.dataOffset += ticketExtensionLength;
						}
					} else {
						CONNECTION_ERROR;
					}
				} else if (realRecordType == TLS_RECORD_TYPE_APPLICATION_DATA) {
					u32 dataLength = recordLength - 16 - 1;
					u32 receivedLength = min(dataLength, receiveBufferCap - receiveBufferPos);
					memcpy(receiveBuffer + receiveBufferPos, receiveRecord.dataBuffer + receiveRecord.dataOffset, receivedLength);
					receiveBufferPos += receivedLength;
					receiveRecord.dataOffset += receivedLength;
					dataLeftInReceiveRecord = dataLength - receivedLength;
				} else {
					CONNECTION_ERROR;
				}

				if (dataLeftInReceiveRecord == 0) {
					receiveRecord.dataOffset += zeroPadding;
					receiveRecord.dataOffset++;
					receiveRecord.dataOffset += 16;
				}

			} else if (recordType == TLS_RECORD_TYPE_ALERT) {
				CHECK_CONNECTION_PROBLEM(recordLength != 2);
				TLSAlertLevel alertLevel = static_cast<TLSAlertLevel>(receiveRecord.read_byte());
				TLSAlertDescription alert = static_cast<TLSAlertDescription>(receiveRecord.read_byte());
				CONNECTION_ERROR;
			} else {
				CONNECTION_ERROR;
			}

			CHECK_CONNECTION_PROBLEM(dataLeftInReceiveRecord == 0 && receiveRecord.dataOffset != recordEnd);
			if (dataLeftInReceiveRecord == 0) {
				receiveRecord.reset();
			}
		} else if (state == TLS_CLIENT_STATE_HELLO_SENT) {
			TLSRecord& hello = receiveRecord;
			
			TLSRecordType recordType = static_cast<TLSRecordType>(hello.read_byte());

			u16 version = hello.read_int16();
			u16 handshakeMessageLength = hello.read_int16();
			if (recordType == TLS_RECORD_TYPE_ALERT) {
				CHECK_CONNECTION_PROBLEM(handshakeMessageLength != 2);
				TLSAlertLevel alertLevel = static_cast<TLSAlertLevel>(hello.read_byte());
				TLSAlertDescription alert = static_cast<TLSAlertDescription>(hello.read_byte());
				CONNECTION_ERROR;
			}
			CHECK_CONNECTION_PROBLEM(recordType != TLS_RECORD_TYPE_HANDSHAKE);
			CHECK_CONNECTION_PROBLEM(handshakeMessageLength < 4);

			TLSHandshakeType handshakeType = static_cast<TLSHandshakeType>(hello.read_byte());
			CHECK_CONNECTION_PROBLEM(handshakeType != TLS_HANDSHAKE_SERVER_HELLO);

			u32 dataLength = hello.read_int24();
			CHECK_CONNECTION_PROBLEM(dataLength < (2 + 32 + 1) || dataLength != (handshakeMessageLength - 4));
			u32 serverHelloEnd = hello.dataOffset + dataLength;

			u16 serverVersion = hello.read_int16();

			u8 serverRandom[32];
			hello.read_bytes(serverRandom, 32);

			u8 sessionIdLength = hello.read_byte();
			CHECK_CONNECTION_PROBLEM(serverHelloEnd < (hello.dataOffset + sessionIdLength + 2 + 1 + 2));

			// Ignore session id
			hello.dataOffset += sessionIdLength;

			TLSCipherSuite cipherSuite = static_cast<TLSCipherSuite>(hello.read_int16());
			CHECK_CONNECTION_PROBLEM(cipherSuite != TLS_AES_128_GCM_SHA256);
			u8 compressionMethod = hello.read_byte();
			CHECK_CONNECTION_PROBLEM(compressionMethod != 0);
			u16 extensionsLength = hello.read_int16();
			CHECK_CONNECTION_PROBLEM(serverHelloEnd < (hello.dataOffset + extensionsLength));

			// Two numbers, xy coord on curve
			u8 serverPublicKey[32 + 32];

			b32 hasSupportedVersionTLS13Ext = false;
			b32 hasKeyShareExt = false;
			u32 extensionsEnd = hello.dataOffset + extensionsLength;
			while (hello.dataOffset != extensionsEnd) {
				CHECK_CONNECTION_PROBLEM(extensionsEnd < (hello.dataOffset + 4));

				TLSExtension extensionType = static_cast<TLSExtension>(hello.read_int16());
				u16 extensionLength = hello.read_int16();

				u32 extensionEnd = hello.dataOffset + extensionLength;
				CHECK_CONNECTION_PROBLEM(extensionsEnd < extensionEnd);

				if (extensionType == TLS_EXTENSION_SUPPORTED_VERSIONS) {
					for (u32 i = 0; i < extensionLength; i += 2) {
						u16 tlsVersion = hello.read_int16();
						if (tlsVersion == 0x0304) {
							hasSupportedVersionTLS13Ext = true;
						}
					}
				} else if (extensionType == TLS_EXTENSION_KEY_SHARE) {
					CHECK_CONNECTION_PROBLEM(extensionLength < 3);

					TLSNamedGroup keyExchangeGroup = static_cast<TLSNamedGroup>(hello.read_int16());
					CHECK_CONNECTION_PROBLEM(keyExchangeGroup != TLS_GROUP_SECP256R1);

					u16 keyLength = hello.read_int16();
					CHECK_CONNECTION_PROBLEM(keyLength != (32 + 32 + 1) || (hello.dataOffset + keyLength) != extensionEnd);
					u8 legacyCompression = hello.read_byte();
					CHECK_CONNECTION_PROBLEM(legacyCompression != 4);

					hello.read_bytes(serverPublicKey, keyLength - 1);
					hasKeyShareExt = true;
				} else {
					hello.dataOffset += extensionLength;
				}
				CHECK_CONNECTION_PROBLEM(hello.dataOffset != extensionEnd)
			}
			CHECK_CONNECTION_PROBLEM(!hasSupportedVersionTLS13Ext || !hasKeyShareExt);
			CHECK_CONNECTION_PROBLEM(hello.dataOffset != serverHelloEnd);


			// Server hello done, key calc
			recordsHash.update(hello.dataBuffer + 5, serverHelloEnd - 5);
			SHA256 tmpHash;
			tmpHash.copy_from(recordsHash);
			u8 clientToServerHelloHash[32];
			tmpHash.digest(clientToServerHelloHash);

			const u32 hashLength = SHA256_HASH_SIZE;
			const u32 encryptionKeySize = AES_KEY_SIZE_BYTES;
			u8 earlySecret[hashLength];
			u8 zeros[hashLength]{};
			u8 hashedZeros[hashLength]{};
			u8 emptyHash[hashLength];
			sha256(hashedZeros, zeros, hashLength);
			sha256(emptyHash, "", 0);

			// early secret
			sha256_hkdf_extract(earlySecret, "", 0, zeros, hashLength);
			// handshake secret
			u8 derivedSecret[hashLength];
			sha256_derive_secret(derivedSecret, hashLength, earlySecret, hashLength, "derived", 7, emptyHash, hashLength);
			u8 sharedSecret[hashLength];

			b32 keyCalcSuccess = secp256r1_ecdhe(sharedSecret, serverPublicKey, serverPublicKey + 32, clientPrivateKey);
			CHECK_CONNECTION_PROBLEM(!keyCalcSuccess);

			// Aliasing these so we don't waste memory for no reason
			u8* clientHandshakeKey = clientApplicationKey;
			u8* serverHandshakeKey = serverApplicationKey;
			u8* clientHandshakeIV = clientApplicationIV;
			u8* serverHandshakeIV = serverApplicationIV;
			u8* clientHandshakeTrafficSecret = clientApplicationTrafficSecret;
			u8* serverHandshakeTrafficSecret = serverApplicationTrafficSecret;

			u8 handshakeSecret[hashLength];
			sha256_hkdf_extract(handshakeSecret, derivedSecret, hashLength, sharedSecret, 32);
			sha256_derive_secret(clientHandshakeTrafficSecret, hashLength, handshakeSecret, hashLength, "c hs traffic", 12, clientToServerHelloHash, hashLength);
			sha256_derive_secret(serverHandshakeTrafficSecret, hashLength, handshakeSecret, hashLength, "s hs traffic", 12, clientToServerHelloHash, hashLength);

			
			sha256_hkdf_expand_label(clientHandshakeKey, encryptionKeySize, clientHandshakeTrafficSecret, hashLength, "key", 3, "", 0);
			sha256_hkdf_expand_label(serverHandshakeKey, encryptionKeySize, serverHandshakeTrafficSecret, hashLength, "key", 3, "", 0);
			sha256_hkdf_expand_label(clientHandshakeIV, 12, clientHandshakeTrafficSecret, hashLength, "iv", 2, "", 0);
			sha256_hkdf_expand_label(serverHandshakeIV, 12, serverHandshakeTrafficSecret, hashLength, "iv", 2, "", 0);

			// master secret
			sha256_derive_secret(derivedSecret, hashLength, handshakeSecret, hashLength, "derived", 7, emptyHash, hashLength);
			sha256_hkdf_extract(masterSecret, derivedSecret, hashLength, zeros, hashLength);

			decryptedMessageCounter = 0;
			receiveRecord.reset();
			state = TLS_CLIENT_STATE_HELLO_RECEIVED;
		} else if (state == TLS_CLIENT_STATE_HELLO_RECEIVED) {
			TLSRecord& nextRecord = receiveRecord;

			const u32 hashLength = SHA256_HASH_SIZE;
			const u32 encryptionKeySize = AES_KEY_SIZE_BYTES;

			// Aliasing these so we don't waste memory for no reason
			u8* clientHandshakeKey = clientApplicationKey;
			u8* serverHandshakeKey = serverApplicationKey;
			u8* clientHandshakeIV = clientApplicationIV;
			u8* serverHandshakeIV = serverApplicationIV;
			u8* clientHandshakeTrafficSecret = clientApplicationTrafficSecret;
			u8* serverHandshakeTrafficSecret = serverApplicationTrafficSecret;

			SHA256 tmpHash;

			TLSRecordType recordType = static_cast<TLSRecordType>(nextRecord.read_byte());
			u16 version = nextRecord.read_int16();
			u16 recordLength = nextRecord.read_int16();
			u32 recordEnd = nextRecord.dataOffset + recordLength;
			CHECK_CONNECTION_PROBLEM(recordEnd >= (TLSRecord::MAX_RECORD_SIZE + 5));

			if (recordType == TLS_RECORD_TYPE_CHANGE_CIPHER_SPEC) {
				CHECK_CONNECTION_PROBLEM(recordLength != 1);
				CHECK_CONNECTION_PROBLEM(nextRecord.read_byte() != 1);
			} else if (recordType == TLS_RECORD_TYPE_APPLICATION_DATA) {
				CHECK_CONNECTION_PROBLEM(recordLength < 17);

				b32 serverHandshakeFinished = false;

				u8 iv[AES_GCM_IV_SIZE];
				memcpy(iv, serverHandshakeIV, AES_GCM_IV_SIZE);
				for (u32 i = 0; i < 8; i++) {
					iv[i + 4] ^= (decryptedMessageCounter >> ((7 - i) * 8)) & 0xFF;
				}
				decryptedMessageCounter++;
				AESGCM aes;
				aes.init(serverHandshakeKey, iv);
				b32 successfulDecrypt = aes.decrypt(nextRecord.dataBuffer + 5, nextRecord.dataBuffer + (recordEnd - 16), nextRecord.dataBuffer + 5, recordLength - 16, nextRecord.dataBuffer, 5);
				CHECK_CONNECTION_PROBLEM(!successfulDecrypt);

				u32 messagesEnd = recordEnd - 17;
				u32 zeroPadding = 0;
				for (; messagesEnd > 0 && nextRecord.dataBuffer[messagesEnd] == 0; messagesEnd--, zeroPadding++);
				TLSRecordType realRecordType = static_cast<TLSRecordType>(nextRecord.dataBuffer[messagesEnd]);
				CHECK_CONNECTION_PROBLEM(realRecordType != TLS_RECORD_TYPE_HANDSHAKE);

				while (nextRecord.dataOffset < messagesEnd) {
					TLSHandshakeType hanshakeMessageType = static_cast<TLSHandshakeType>(nextRecord.read_byte());
					u32 dataLength = nextRecord.read_int24();
					u32 dataEnd = nextRecord.dataOffset + dataLength;
					CHECK_CONNECTION_PROBLEM((recordEnd - 16) < dataEnd);
					if (hanshakeMessageType == TLS_HANDSHAKE_ENCRYPTED_EXTENSIONS) {
						CHECK_CONNECTION_PROBLEM(dataLength < 2);
						u16 encryptedExtensionEnd = nextRecord.read_int16();
						encryptedExtensionEnd += nextRecord.dataOffset;
						CHECK_CONNECTION_PROBLEM(dataEnd < encryptedExtensionEnd);

						while (nextRecord.dataOffset != encryptedExtensionEnd) {
							CHECK_CONNECTION_PROBLEM(encryptedExtensionEnd < (nextRecord.dataOffset + 4));

							TLSExtension extensionType = static_cast<TLSExtension>(nextRecord.read_int16());
							u16 extensionLength = nextRecord.read_int16();

							u32 extensionEnd = nextRecord.dataOffset + extensionLength;
							CHECK_CONNECTION_PROBLEM(encryptedExtensionEnd < extensionEnd);

							nextRecord.dataOffset += extensionLength;
						}
					} else if (hanshakeMessageType == TLS_HANDSHAKE_CERTIFICATE) {
						CHECK_CONNECTION_PROBLEM(dataLength < 1);
						u8 contextLength = nextRecord.read_byte();
						// contextLength must be 0 unless used for post handshake authentication
						CHECK_CONNECTION_PROBLEM(contextLength != 0);
						//CHECK_CONNECTION_PROBLEM(dataEnd < (nextRecord.dataOffset + contextLength));
						//nextRecord.dataOffset += contextLength;

						CHECK_CONNECTION_PROBLEM(dataEnd < (nextRecord.dataOffset + 3));
						u32 certificatesLength = nextRecord.read_int24();
						u32 certificatesEnd = nextRecord.dataOffset + certificatesLength;
						CHECK_CONNECTION_PROBLEM(dataEnd < (nextRecord.dataOffset + certificatesLength));

						u32 certLength = nextRecord.read_int24();
						CHECK_CONNECTION_PROBLEM(certificatesEnd < (nextRecord.dataOffset + certLength));
						void* cert = nextRecord.dataBuffer + nextRecord.dataOffset;
						void* publicKey = nullptr;
						u32 publicKeyLength = 0;
						b32 certParseSuccess = parse_asn1_der_cert(&certSignatureAlgorithm, &publicKey, &publicKeyLength, cert, certLength);
						CHECK_CONNECTION_PROBLEM(!certParseSuccess);

						if (certSignatureAlgorithm == X509_SIGNATURE_RSA_GENERIC || certSignatureAlgorithm == X509_SIGNATURE_RSA_PSS) {
							ASN1Reader keyExtractor{ publicKey, publicKeyLength };
							u32 keyStructEnd = keyExtractor.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);
							keyExtractor.read_big_integer_tag(certRSAModulusOrECDSAPointX, keyStructEnd);
							keyExtractor.read_big_integer_tag(certRSAPublicKeyOrECDSAPointY, keyStructEnd);
							keyExtractor.verify_section_complete(keyStructEnd);
							CHECK_CONNECTION_PROBLEM(keyExtractor.errored);
						} else if (certSignatureAlgorithm == X509_SIGNATURE_EC_SECP256R1) {
							b32 loadSuccess = secp256r1_load_uncompressed(certRSAModulusOrECDSAPointX, certRSAPublicKeyOrECDSAPointY, publicKey, publicKeyLength);
							CHECK_CONNECTION_PROBLEM(!loadSuccess);
						} else {
							CHECK_CONNECTION_PROBLEM(true);
						}

						nextRecord.dataOffset = certificatesEnd;
					} else if (hanshakeMessageType == TLS_HANDSHAKE_CERTIFICATE_VERIFY) {
						CHECK_CONNECTION_PROBLEM(dataLength < 4);
						TLSSignatureScheme signatureScheme = static_cast<TLSSignatureScheme>(nextRecord.read_int16());
						u16 signatureDataLength = nextRecord.read_int16();
						void* signature = nextRecord.dataBuffer + nextRecord.dataOffset;
						CHECK_CONNECTION_PROBLEM(dataEnd < (nextRecord.dataOffset + signatureDataLength));

						const char* contextString = "TLS 1.3, server CertificateVerify";
						constexpr u32 contextStringLength = 33;
						constexpr u32 signatureContextLength = 64 + contextStringLength + 1 + hashLength;
						u8 signatureContext[signatureContextLength];
						memset(signatureContext, 0x20, 64);
						memcpy(signatureContext + 64, contextString, contextStringLength);
						signatureContext[64 + contextStringLength] = 0x00;
						tmpHash.copy_from(recordsHash);
						tmpHash.digest(signatureContext + 64 + contextStringLength + 1);

						b32 certificateVerified = false;
						if (certSignatureAlgorithm == X509_SIGNATURE_RSA_GENERIC || certSignatureAlgorithm == X509_SIGNATURE_RSA_PSS) {
							if (certSignatureAlgorithm == X509_SIGNATURE_RSA_GENERIC) {
								// rsaEncryption is generic, any RSA can use its key
								CHECK_CONNECTION_PROBLEM(signatureScheme != TLS_RSA_PKCS1_SHA256 && signatureScheme != TLS_RSS_PSS_RSAE_SHA256 && signatureScheme != TLS_RSS_PSS_PSS_SHA256);
							} else {
								// rsassa-pss can only be used for the PSS algorithm
								CHECK_CONNECTION_PROBLEM(signatureScheme != TLS_RSS_PSS_RSAE_SHA256 && signatureScheme != TLS_RSS_PSS_PSS_SHA256);
							}
							u32 rsaBits = signatureDataLength * 8;
							CHECK_CONNECTION_PROBLEM(BigInteger::bitlength(certRSAModulusOrECDSAPointX) != rsaBits);
							// TLS 1.3 requires RSA >= 2048
							CHECK_CONNECTION_PROBLEM(rsaBits < 2048);
							// I only support up to RSA 4096 for now
							CHECK_CONNECTION_PROBLEM(rsaBits > 4096);
							if (signatureScheme == TLS_RSA_PKCS1_SHA256) {
								certificateVerified = rsassa_pkcs1_verify_sha256(signature, signatureContext, signatureContextLength, certRSAPublicKeyOrECDSAPointY, certRSAModulusOrECDSAPointX, rsaBits);
							} else if (signatureScheme == TLS_RSS_PSS_RSAE_SHA256 || signatureScheme == TLS_RSS_PSS_PSS_SHA256) {
								// These two are the same signature/verification algorithm, the only difference is whether the key is stored under the rsaEncryption or rsassa-pss oid in the cert
								certificateVerified = rsassa_pss_verify_sha256(signature, signatureContext, signatureContextLength, certRSAPublicKeyOrECDSAPointY, certRSAModulusOrECDSAPointX, rsaBits);
							}
						} else if (certSignatureAlgorithm == X509_SIGNATURE_EC_SECP256R1) {
							CHECK_CONNECTION_PROBLEM(signatureScheme != TLS_ECDSA_SECP256R1_SHA256);
							BigInteger signatureR; signatureR.init(64);
							BigInteger signatureS; signatureS.init(64);
							if (ecdsa_load_asn1(signatureR, signatureS, signature, signatureDataLength)) {
								certificateVerified = ecdsa_verify_secpr1_sha256(signatureR, signatureS, signatureContext, signatureContextLength, certRSAModulusOrECDSAPointX, certRSAPublicKeyOrECDSAPointY);
							}
						} else {
							CHECK_CONNECTION_PROBLEM(true);
						}

						CHECK_CONNECTION_PROBLEM(!certificateVerified);
						// Great! We've proved that the messages really have come from the same place as the certificate!
						// This means nothing since we haven't verified the certificate itself, but whatever.
						// I really don't feel like finding out how to deal with certificate authorities.

						nextRecord.dataOffset += signatureDataLength;
					} else if (hanshakeMessageType == TLS_HANDSHAKE_FINISHED) {
						CHECK_CONNECTION_PROBLEM(dataLength != hashLength);

						u8 serverHandshakeVerify[hashLength];
						nextRecord.read_bytes(serverHandshakeVerify, hashLength);

						u8 helloToNowHash[hashLength];
						tmpHash.copy_from(recordsHash);
						tmpHash.digest(helloToNowHash);

						u8 finishedKey[hashLength];
						sha256_hkdf_expand_label(finishedKey, hashLength, serverHandshakeTrafficSecret, hashLength, "finished", 8, "", 0);

						u8 clientHandshakeVerify[hashLength];
						sha256_hmac(clientHandshakeVerify, finishedKey, hashLength, helloToNowHash, hashLength);

						CHECK_CONNECTION_PROBLEM(memcmp(serverHandshakeVerify, clientHandshakeVerify, hashLength) != 0);
						serverHandshakeFinished = true;
					} else {
						CONNECTION_ERROR;
					}
					CHECK_CONNECTION_PROBLEM(nextRecord.dataOffset != dataEnd);

					recordsHash.update(nextRecord.dataBuffer + (nextRecord.dataOffset - dataLength - 4), dataLength + 4);
				}

				CHECK_CONNECTION_PROBLEM(nextRecord.dataOffset != messagesEnd);
				nextRecord.dataOffset += zeroPadding;

				// Skip record type and auth tag
				nextRecord.dataOffset++;
				nextRecord.dataOffset += 16;

				if (serverHandshakeFinished) {
					// Application data key derivation
					u8 helloToServerFinishedHash[hashLength];
					tmpHash.copy_from(recordsHash);
					tmpHash.digest(helloToServerFinishedHash);

					sha256_hkdf_expand_label(serverApplicationTrafficSecret, hashLength, masterSecret, hashLength, "s ap traffic", 12, helloToServerFinishedHash, hashLength);
					sha256_hkdf_expand_label(serverApplicationKey, encryptionKeySize, serverApplicationTrafficSecret, hashLength, "key", 3, "", 0);
					sha256_hkdf_expand_label(serverApplicationIV, AES_GCM_IV_SIZE, serverApplicationTrafficSecret, hashLength, "iv", 2, "", 0);

					u8 oldClientTrafficSecret[SHA256_HASH_SIZE];
					u8 oldClientIV[AES_GCM_IV_SIZE];
					u8 oldClientKey[AES_KEY_SIZE_BYTES];
					memcpy(oldClientTrafficSecret, clientHandshakeTrafficSecret, SHA256_HASH_SIZE);
					memcpy(oldClientIV, clientHandshakeIV, AES_GCM_IV_SIZE);
					memcpy(oldClientKey, clientHandshakeKey, AES_KEY_SIZE_BYTES);
					sha256_hkdf_expand_label(clientApplicationTrafficSecret, hashLength, masterSecret, hashLength, "c ap traffic", 12, helloToServerFinishedHash, hashLength);
					sha256_hkdf_expand_label(clientApplicationKey, encryptionKeySize, clientApplicationTrafficSecret, hashLength, "key", 3, "", 0);
					sha256_hkdf_expand_label(clientApplicationIV, AES_GCM_IV_SIZE, clientApplicationTrafficSecret, hashLength, "iv", 2, "", 0);

					// Send client data back
					sendRecord.init(TLS_RECORD_TYPE_CHANGE_CIPHER_SPEC);
					sendRecord.patch_int16(1, 3);
					sendRecord.write_byte(1);
					ConnectionError sendError = sendRecord._send_data(serverTCPConnection);
					CHECK_CONNECTION_PROBLEM(sendError != CONNECTION_ERROR_SUCCESS);
					CHECK_CONNECTION_PROBLEM(sendRecord.numBytesSentOrReceived != sendRecord.dataOffset);

					sendRecord.reset();
					sendRecord.init(TLS_RECORD_TYPE_APPLICATION_DATA);
					sendRecord.write_byte(TLS_HANDSHAKE_FINISHED);
					sendRecord.write_int24(hashLength);
					u8 finishedKey[hashLength];
					sha256_hkdf_expand_label(finishedKey, hashLength, oldClientTrafficSecret, hashLength, "finished", 8, "", 0);
					u8 verifyData[hashLength];
					sha256_hmac(verifyData, finishedKey, hashLength, helloToServerFinishedHash, hashLength);
					sendRecord.write_bytes(verifyData, hashLength);
					sendRecord.write_byte(TLS_RECORD_TYPE_HANDSHAKE);
					sendRecord.patch_int16(sendRecord.dataOffset - 5 + 16, 3);

					aes.init(oldClientKey, oldClientIV);
					aes.encrypt(sendRecord.dataBuffer + sendRecord.dataOffset, sendRecord.dataBuffer + 5, sendRecord.dataBuffer + 5, sendRecord.dataOffset - 5, sendRecord.dataBuffer, 5);
					sendRecord.dataOffset += 16;
					
					sendError = sendRecord._send_data(serverTCPConnection);
					CHECK_CONNECTION_PROBLEM(sendError != CONNECTION_ERROR_SUCCESS);
					CHECK_CONNECTION_PROBLEM(sendRecord.numBytesSentOrReceived != sendRecord.dataOffset);

					sendRecord.reset();
					encryptedMessageCounter = 0;
					decryptedMessageCounter = 0;
					state = TLS_CLIENT_STATE_OPEN;
				}
			}
		}
	}

	// Will receive at most one record
	// If it returns true, call it again to possibly process more data
	b32 receive_data() {
		if (state == TLS_CLIENT_STATE_CLOSED) {
			return false;
		}
		if (dataLeftInReceiveRecord > 0) {
			u32 amountToTransfer = min(dataLeftInReceiveRecord, receiveBufferCap - receiveBufferPos);
			memcpy(receiveBuffer + receiveBufferPos, receiveRecord.dataBuffer + receiveRecord.dataOffset, amountToTransfer);
			receiveBufferPos += amountToTransfer;
			receiveRecord.dataOffset += amountToTransfer;
			dataLeftInReceiveRecord -= amountToTransfer;
			// Check to make sure we read the whole record
			if (dataLeftInReceiveRecord == 0) {
				u32 recordLength = receiveRecord.read_int16(3);
				u32 recordEnd = 5 + recordLength;
				while (receiveRecord.dataOffset != recordEnd && receiveRecord.dataBuffer[receiveRecord.dataOffset] == 0) {
					receiveRecord.dataOffset++;
				}
				receiveRecord.dataOffset++;
				receiveRecord.dataOffset += 16;
				CHECK_CONNECTION_PROBLEM(receiveRecord.dataOffset != recordEnd);
				if (receiveRecord.dataOffset != recordEnd) {
					state = TLS_CLIENT_STATE_CLOSED;
					g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection);
					return false;
				}
			} else {
				return true;
			}
		}

		u32 prevBytesReceived = receiveRecord.numBytesSentOrReceived;
		NetworkTransferResult receiveResult = receiveRecord._receive_data(serverTCPConnection);
		if (receiveResult != NET_TRANSFER_RESULT_COMPLETE) {
			if (receiveResult != NET_TRANSFER_RESULT_INCOMPLETE) {
				state = TLS_CLIENT_STATE_CLOSED;
				g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection);
			}
			return false;
		}
		b32 possiblyMoreDataToReceive = prevBytesReceived != receiveRecord.numBytesSentOrReceived || prevBytesReceived == TLSRecord::DATA_BUFFER_SIZE;
		process_received_record();
		return possiblyMoreDataToReceive;
	}

	TLSClient& write_str(const char* str) {
		if (state == TLS_CLIENT_STATE_CLOSED) {
			return *this;
		}
		while (str[0] != '\0') {
			while (str[0] != '\0' && sendBufferPos < sizeof(sendBuffer)) {
				sendBuffer[sendBufferPos++] = str[0];
				str++;
			}
			if (str[0] != '\0') {
				send_data();
			}
		}
		return *this;
	}

	TLSClient& patch_content_length(u32 pos, u32 contentStart) {
		u32 length = sendBufferPos - contentStart;
		while (length != 0 && sendBuffer[pos] == ' ') {
			sendBuffer[pos--] = (length % 10) + '0';
			length /= 10;
		}
		return *this;
	}

	void clear_receive_buffer() {
		receiveBufferPos = 0;
		dataLeftInReceiveRecord = 0;
	}

	void close() {
		if (state == TLS_CLIENT_STATE_CLOSED) {
			return;
		}
		sendRecord.init(TLS_RECORD_TYPE_APPLICATION_DATA);
		sendRecord.write_byte(TLS_ALERT_LEVEL_WARNING);
		sendRecord.write_byte(TLS_ALERT_CLOSE_NOTIFY);
		sendRecord.write_byte(TLS_RECORD_TYPE_ALERT);
		sendRecord.encrypt(clientApplicationIV, clientApplicationIV, encryptedMessageCounter, 0);
		sendRecord._send_data(serverTCPConnection);

		g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection);
		state = TLS_CLIENT_STATE_CLOSED;
	}

	b32 connect(const char* remoteServerName, u32 remoteIP, u16 localPort) {
		sendBufferPos = 0;
		receiveBufferPos = 0;
		sendBufferCap = sizeof(sendBuffer);
		receiveBufferCap = sizeof(receiveBuffer);
		dataLeftInReceiveRecord = 0;
		state = TLS_CLIENT_STATE_CLOSED;
		shouldKeyUpdate = false;
		sendRecord.reset();
		receiveRecord.reset();
		certSignatureAlgorithm = X509_SIGNATURE_EC_IMPLICITLY_CA;
		certRSAModulusOrECDSAPointX.init(64);
		certRSAPublicKeyOrECDSAPointY.init(64);

		serverTCPConnection = g_syscallProc(SYSCALL_TCP_OPEN, remoteIP | (u64(443) << 32) | (u64(localPort) << 48));

		if (serverTCPConnection == TCP_CONNECTION_BLOCK_INVALID_IDX) {
			return false;
		}

		Keccak random;
		random.make_secure_random();

		secp256r1_generate_keypair(random, clientPublicKey, clientPrivateKey);

		SHA256 tmpHash;
		recordsHash.init();

		TLSRecord& hello = sendRecord;
		hello.init(TLS_RECORD_TYPE_HANDSHAKE);
		hello.write_byte(TLS_HANDSHAKE_CLIENT_HELLO);
		u32 lengthPos = hello.dataOffset;
		hello.write_int24(0);
		// Write TLS 1.2 for compatibility
		hello.write_int16(0x0303);
		// Random field
		random.squeeze(hello.dataBuffer + hello.dataOffset, 32);
		hello.dataOffset += 32;
		// More compatibility, write a random 32 byte legacy session id
		hello.write_byte(32);
		random.squeeze(hello.dataBuffer + hello.dataOffset, 32);
		hello.dataOffset += 32;
		// Write list of cipher suites
		hello.write_int16(2);
		hello.write_int16(TLS_AES_128_GCM_SHA256);
		// Write legacy compression method of null
		hello.write_byte(1);
		hello.write_byte(0);

		// Write extensions
		u32 extensionLengthPos = hello.dataOffset;
		hello.write_int16(0);

		if (remoteServerName) {
			hello.write_int16(TLS_EXTENSION_SERVER_NAME);
			u32 serverNameLengthPos = hello.dataOffset;
			hello.write_int16(0);
			hello.write_int16(0);
			hello.write_byte(TLS_SERVER_NAME_HOST_NAME);
			u16 hostLength = strlen(remoteServerName);
			hello.write_int16(hostLength);
			hello.write_bytes(remoteServerName, hostLength);
			// Backpatch length fields
			u32 len = hello.dataOffset - serverNameLengthPos - 2;
			hello.patch_int16(hello.dataOffset - serverNameLengthPos - 2, serverNameLengthPos);
			hello.patch_int16(hello.dataOffset - serverNameLengthPos - 4, serverNameLengthPos + 2);
		}

		// This is the one that tells the other party we're actually a TLS 1.3 connection
		hello.write_int16(TLS_EXTENSION_SUPPORTED_VERSIONS);
		// 3 bytes of extension data
		hello.write_int16(3);
		// 2 bytes of versions
		hello.write_byte(2);
		// TLS 1.3 version number
		hello.write_int16(0x0304);

		hello.write_int16(TLS_EXTENSION_SUPPORTED_GROUPS);
		u32 supportedGroupsLengthPos = hello.dataOffset;
		hello.write_int16(0);
		hello.write_int16(0);
		// Only support secp256r1 group
		hello.write_int16(TLS_GROUP_SECP256R1);
		// Backpatch length fields
		hello.patch_int16(hello.dataOffset - supportedGroupsLengthPos - 2, supportedGroupsLengthPos);
		hello.patch_int16(hello.dataOffset - supportedGroupsLengthPos - 4, supportedGroupsLengthPos + 2);

		hello.write_int16(TLS_EXTENSION_SIGNATURE_ALGORITHMS);
		u32 signatureAlgorithmLengthPos = hello.dataOffset;
		hello.write_int16(0);
		hello.write_int16(0);
		// Write supported signature algorithms
		hello.write_int16(TLS_RSA_PKCS1_SHA256);
		hello.write_int16(TLS_RSS_PSS_RSAE_SHA256);
		hello.write_int16(TLS_RSS_PSS_PSS_SHA256);
		hello.write_int16(TLS_ECDSA_SECP256R1_SHA256);
		// Backpatch length fields
		hello.patch_int16(hello.dataOffset - signatureAlgorithmLengthPos - 2, signatureAlgorithmLengthPos);
		hello.patch_int16(hello.dataOffset - signatureAlgorithmLengthPos - 4, signatureAlgorithmLengthPos + 2);

		hello.write_int16(TLS_EXTENSION_KEY_SHARE);
		u32 keyShareLengthPos = hello.dataOffset;
		hello.write_int16(0);
		hello.write_int16(0);
		// Write key share
		hello.write_int16(TLS_GROUP_SECP256R1);
		// 32 bytes per curve coordinates, x and y
		hello.write_int16(1 + 32 + 32);
		// Write public key
		hello.write_byte(4);
		hello.write_bytes(clientPublicKey, 32 + 32);
		// Backpatch length fields
		hello.patch_int16(hello.dataOffset - keyShareLengthPos - 2, keyShareLengthPos);
		hello.patch_int16(hello.dataOffset - keyShareLengthPos - 4, keyShareLengthPos + 2);

		// Backpatch length fields
		hello.patch_int16(hello.dataOffset - extensionLengthPos - 2, extensionLengthPos);
		hello.write_int24(hello.dataOffset - lengthPos - 3, lengthPos);
		hello.patch_int16(hello.dataOffset - 5, 3);

		// Client hello done
		recordsHash.update(hello.dataBuffer + 5, hello.dataOffset - 5);
		tmpHash.copy_from(recordsHash);
		u8 clientHelloHash[32];
		tmpHash.digest(clientHelloHash);

		ConnectionError helloSendErr = hello._send_data(serverTCPConnection);
		if (helloSendErr != CONNECTION_ERROR_SUCCESS) {
			g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection);
			return false;
		}
		if (hello.numBytesSentOrReceived != hello.dataOffset) {
			g_syscallProc(SYSCALL_TCP_CLOSE, serverTCPConnection);
			return false;
		}

		sendRecord.reset();
		state = TLS_CLIENT_STATE_HELLO_SENT;
		return true;
	}

	b32 connect(const char* serverName, u16 port) {
		SyscallDNSLookupArgs dnsLookupArgs{};
		strcpy(dnsLookupArgs.name, serverName);
		u64 dnsResult = g_syscallProc(SYSCALL_DNS_LOOKUP_BLOCKING, reinterpret_cast<u64>(&dnsLookupArgs));
		if (((dnsResult >> 32) &0b11) != DNS_LOOKUP_RESULT_SUCCESS){
			return false;
		}
		u32 dnsResultIP = dnsResult & 0xFFFFFFFF;

		connect(serverName, dnsResultIP, port);
	}
};
#undef CHECK_CONNECTION_PROBLEM
#undef CONNECTION_ERROR