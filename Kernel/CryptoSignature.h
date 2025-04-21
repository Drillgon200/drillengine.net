#pragma once

#include "BigInteger.h"
#include "ECC.h"
#include "CryptoHash.h"
#include "ASN1.h"

// RSA stuff below is from RFC 8017

// ASN.1 DER encodings of this struct for each hash algorithm. It's set up so that the hash can be appended directly after this string of bytes
// DigestInfo ::= SEQUENCE {
//	digestAlgorithm AlgorithmIdentifier,
//	digest OCTET STRING
// }
const u8 PKCS_HASH_MD2_DER[]{ 0x30, 0x20, 0x30, 0x0c, 0x06, 0x08, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x02, 0x05, 0x00, 0x04, 0x10 };
const u8 PKCS_HASH_MD5_DER[]{ 0x30, 0x20, 0x30, 0x0c, 0x06, 0x08, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x05, 0x05, 0x00, 0x04, 0x10 };
const u8 PKCS_HASH_SHA1_DER[]{ 0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14 };
const u8 PKCS_HASH_SHA224_DER[]{ 0x30, 0x2d, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x04, 0x05, 0x00, 0x04, 0x1c };
const u8 PKCS_HASH_SHA256_DER[]{ 0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20 };
const u8 PKCS_HASH_SHA384_DER[]{ 0x30, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02, 0x05, 0x00, 0x04, 0x30 };
const u8 PKCS_HASH_SHA512_DER[]{ 0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03, 0x05, 0x00, 0x04, 0x40 };
const u8 PKCS_HASH_SHA512_224_DER[]{ 0x30, 0x2d, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x05, 0x05, 0x00, 0x04, 0x1c };
const u8 PKCS_HASH_SHA512_256_DER[]{ 0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x06, 0x05, 0x00, 0x04, 0x20 };

// RSA signature primative (exponent key form)
inline bool rsasp1(BigInteger& messageRepresentative, BigInteger& privateKeyExponent, BigInteger& privateKeyModulo) {
	if (BigInteger::cmp(messageRepresentative, privateKeyModulo) >= 0) {
		return false;
	}
	BigInteger::pow_mod_montgomery(messageRepresentative, privateKeyExponent, privateKeyModulo);
	return true;
}

// RSA signature primative (chinese remainder key form)
inline bool rsasp1(BigInteger& messageRepresentative, BigInteger& p, BigInteger& q, BigInteger& dP, BigInteger& dQ, BigInteger& qInv, BigInteger& privateKeyModulo) {
	if (BigInteger::cmp(messageRepresentative, privateKeyModulo) >= 0) {
		return false;
	}
	BigInteger s1; s1.init(p.cap);
	BigInteger s2; s2.init(p.cap);
	BigInteger h; h.init(p.cap);

	BigInteger::assign(s1, messageRepresentative);
	BigInteger::pow_mod_montgomery(s1, dP, p);
	BigInteger::assign(s2, messageRepresentative);
	BigInteger::pow_mod_montgomery(s2, dQ, q);

	BigInteger::sub(s1, s1, s2);
	BigInteger::mul(h, s1, qInv);
	BigInteger::div(h, p, nullptr);

	BigInteger::mul(messageRepresentative, q, h);
	BigInteger::add(messageRepresentative, messageRepresentative, s2);
	BigInteger::div(messageRepresentative, privateKeyModulo, nullptr);

	return true;
}

// RSA verification primative
inline bool rsavp1(BigInteger& signatureRepresentative, BigInteger& publicKeyExponent, BigInteger& publicKeyModulo) {
	if (BigInteger::cmp(signatureRepresentative, publicKeyModulo) >= 0) {
		return false;
	}
	BigInteger::pow_mod_montgomery(signatureRepresentative, publicKeyExponent, publicKeyModulo);
	return true;
}

inline void mgf1_sha256_add_mask(void* mask, u32 maskLength, const void* mgfSeed, u32 seedLength) {
	u32 hashLength = SHA256_HASH_SIZE;
	u32 loopEnd = (maskLength + hashLength - 1) / hashLength;
	SHA256 mgfHash;
	mgfHash.init();
	mgfHash.update(mgfSeed, seedLength);
	SHA256 sha;
	for (u32 i = 0; i < loopEnd; i++) {
		sha.copy_from(mgfHash);
		u8 counterData[4];
		counterData[0] = i >> 24;
		counterData[1] = (i >> 16) & 0xFF;
		counterData[2] = (i >> 8) & 0xFF;
		counterData[3] = i & 0xFF;
		sha.update(counterData, 4);
		u32 outputLength = min(maskLength, hashLength);
		sha.digest();
		for (u32 i = 0; i < outputLength; i++) {
			reinterpret_cast<u8*>(mask)[i] ^= reinterpret_cast<u8*>(sha.state)[i];
		}
		maskLength -= outputLength;
		mask = reinterpret_cast<u8*>(mask) + outputLength;
	}
}

inline void emsa_pss_encode_sha256(void* encodedMessage, u32 encodedMessageBits, const void* message, u32 messageLength, Keccak& random) {
	const u32 hashLength = SHA256_HASH_SIZE;
	const u32 saltLength = hashLength;
	u32 encodedMessageLength = (encodedMessageBits + 7) / 8;

	u8 messageHash[hashLength];
	sha256(messageHash, message, messageLength);

	u8 salt[saltLength];
	random.squeeze(salt, saltLength);

	u8 messagePrime[8 + hashLength + saltLength];
	memset(messagePrime, 0, 8);
	memcpy(messagePrime + 8, messageHash, hashLength);
	memcpy(messagePrime + 8 + hashLength, salt, saltLength);

	sha256(messageHash, messagePrime, 8 + hashLength + saltLength);

	u32 emZeroPadLength = encodedMessageLength - saltLength - hashLength - 2;
	memset(encodedMessage, 0, emZeroPadLength);
	reinterpret_cast<u8*>(encodedMessage)[emZeroPadLength] = 0x01;
	memcpy(reinterpret_cast<u8*>(encodedMessage) + emZeroPadLength + 1, salt, saltLength);
	mgf1_sha256_add_mask(encodedMessage, encodedMessageLength - hashLength - 1, messageHash, hashLength);
	reinterpret_cast<u8*>(encodedMessage)[0] &= 0xFF >> (encodedMessageLength * 8 - encodedMessageBits);
	memcpy(reinterpret_cast<u8*>(encodedMessage) + (encodedMessageLength - hashLength - 1), messageHash, hashLength);
	reinterpret_cast<u8*>(encodedMessage)[encodedMessageLength - 1] = 0xBC;
}

inline bool emsa_pss_verify_sha256(void* encodedMessage, u32 encodedMessageBits, const void* message, u32 messageLength) {
	const u32 hashLength = SHA256_HASH_SIZE;
	const u32 saltLength = hashLength;
	u32 encodedMessageLength = (encodedMessageBits + 7) / 8;

	u8 messageHash[hashLength];
	sha256(messageHash, message, messageLength);

	if (encodedMessageLength < hashLength + saltLength + 2) {
		return false;
	}

	if (reinterpret_cast<u8*>(encodedMessage)[encodedMessageLength - 1] != 0xBC) {
		return false;
	}

	if ((reinterpret_cast<u8*>(encodedMessage)[0] & ~(0xFF >> (encodedMessageLength * 8 - encodedMessageBits))) != 0) {
		return false;
	}

	u32 emZeroPadLength = encodedMessageLength - saltLength - hashLength - 2;
	u32 hashOffset = encodedMessageLength - hashLength - 1;
	u32 saltOffset = encodedMessageLength - saltLength - hashLength - 1;

	mgf1_sha256_add_mask(encodedMessage, hashOffset, reinterpret_cast<u8*>(encodedMessage) + hashOffset, hashLength);
	reinterpret_cast<u8*>(encodedMessage)[0] &= 0xFF >> (encodedMessageLength * 8 - encodedMessageBits);

	for (u32 i = 0; i < emZeroPadLength; i++) {
		if (reinterpret_cast<u8*>(encodedMessage)[i] != 0) {
			return false;
		}
	}

	if (reinterpret_cast<u8*>(encodedMessage)[emZeroPadLength] != 0x01) {
		return false;
	}

	u8 messagePrime[8 + hashLength + saltLength];
	memset(messagePrime, 0, 8);
	memcpy(messagePrime + 8, messageHash, hashLength);
	memcpy(messagePrime + 8 + hashLength, reinterpret_cast<u8*>(encodedMessage) + saltOffset, saltLength);
	sha256(messageHash, messagePrime, 8 + hashLength + saltLength);

	return memcmp(messageHash, reinterpret_cast<u8*>(encodedMessage) + hashOffset, hashLength) == 0;
}

inline void rsassa_pss_sign_sha256(void* signature, const void* message, u32 messageLength, Keccak& random, BigInteger& rsaPrivateKey, BigInteger& rsaPrime, u32 rsaBits) {
	// assert(rsaBits < 4096);
	u8 emsaEncodedMessage[512];
	emsa_pss_encode_sha256(emsaEncodedMessage, rsaBits - 1, message, messageLength, random);

	BigInteger rsaMessage; rsaMessage.init(64);
	BigInteger::assign_bytes_big_endian(rsaMessage, emsaEncodedMessage, rsaBits / 8);
	rsasp1(rsaMessage, rsaPrivateKey, rsaPrime);

	BigInteger::write_bytes_big_endian(signature, rsaBits / 8, rsaMessage);
}

inline bool rsassa_pss_verify_sha256(void* signature, const void* message, u32 messageLength, BigInteger& rsaPublicKey, BigInteger& rsaPrime, u32 rsaBits) {
	// assert(rsaBits < 4096);
	BigInteger rsaMessage; rsaMessage.init(64);
	BigInteger::assign_bytes_big_endian(rsaMessage, signature, rsaBits / 8);

	if (rsavp1(rsaMessage, rsaPublicKey, rsaPrime) == false) {
		return false;
	}
	u8 emsaEncodedMessage[512];
	BigInteger::write_bytes_big_endian(emsaEncodedMessage, rsaBits / 8, rsaMessage);

	return emsa_pss_verify_sha256(emsaEncodedMessage, rsaBits - 1, message, messageLength);
}

inline void emsa_pkcs1_v1_5_encode_sha256(void* encodedMessage, u32 encodedMessageLength, const void* message, u32 messageLength) {
	const u32 hashLength = SHA256_HASH_SIZE;
	const u32 saltLength = hashLength;

	u8 messageHash[hashLength];
	sha256(messageHash, message, messageLength);

	u32 digestInfoLength = sizeof(PKCS_HASH_SHA256_DER) + hashLength;
	if (encodedMessageLength < digestInfoLength + 11) {
		return;
	}

	u32 paddingLength = encodedMessageLength - digestInfoLength - 3;
	u8* encodedMessageBytes = reinterpret_cast<u8*>(encodedMessage);
	encodedMessageBytes[0] = 0x00;
	encodedMessageBytes[1] = 0x01;
	memset(encodedMessageBytes + 2, 0xFF, paddingLength);
	encodedMessageBytes[2 + paddingLength] = 0x00;
	memcpy(encodedMessageBytes + 2 + paddingLength + 1, PKCS_HASH_SHA256_DER, sizeof(PKCS_HASH_SHA256_DER));
	memcpy(encodedMessageBytes + 2 + paddingLength + 1 + sizeof(PKCS_HASH_SHA256_DER), messageHash, hashLength);
}

inline void rsassa_pkcs1_sign_sha256(void* signature, const void* message, u32 messageLength, BigInteger& rsaPrivateKey, BigInteger& rsaPrime, u32 rsaBits) {
	// assert(rsaBits < 4096);
	u8 emsaEncodedMessage[512];
	emsa_pkcs1_v1_5_encode_sha256(emsaEncodedMessage, rsaBits / 8, message, messageLength);

	BigInteger rsaMessage; rsaMessage.init(64);
	BigInteger::assign_bytes_big_endian(rsaMessage, emsaEncodedMessage, rsaBits / 8);
	rsasp1(rsaMessage, rsaPrivateKey, rsaPrime);

	BigInteger::write_bytes_big_endian(signature, rsaBits / 8, rsaMessage);
}

inline bool rsassa_pkcs1_verify_sha256(void* signature, const void* message, u32 messageLength, BigInteger& rsaPublicKey, BigInteger& rsaPrime, u32 rsaBits) {
	// assert(rsaBits < 4096);
	BigInteger rsaMessage; rsaMessage.init(64);
	BigInteger::assign_bytes_big_endian(rsaMessage, signature, rsaBits / 8);

	if (rsavp1(rsaMessage, rsaPublicKey, rsaPrime) == false) {
		return false;
	}
	u8 emsaEncodedMessage[512];
	u8 emsaEncodedMessagePrime[512];
	BigInteger::write_bytes_big_endian(emsaEncodedMessage, rsaBits / 8, rsaMessage);
	emsa_pkcs1_v1_5_encode_sha256(emsaEncodedMessagePrime, rsaBits / 8, message, messageLength);

	return memcmp(emsaEncodedMessage, emsaEncodedMessagePrime, rsaBits / 8) == 0;
}

// EC stuff from "The Elliptic Curve Digital Signature Algorithm"
// https://www.cs.miami.edu/home/burt/learning/Csc609.142/ecdsa-cert.pdf

#define ECDSA_SECP256R1_SIGNATURE_LENGTH 64

inline void ecdsa_sign_secpr1_sha256(void* signature, const u8 messageHash[32], Keccak& random, void* privateKeyBytes) {
	BigInteger privateKey; privateKey.init(64);
	BigInteger::assign_bytes_big_endian(privateKey, privateKeyBytes, 32);

	u8 kBytes[32];
	u8 xBytes[32 + 32];
	secp256r1_generate_keypair(random, xBytes, kBytes);

	BigInteger k; k.init(64);
	BigInteger r; r.init(64);
	BigInteger::assign_bytes_big_endian(k, kBytes, 32);
	BigInteger::assign_bytes_big_endian(r, xBytes, 32);
	if (BigInteger::cmp(r, EC_SECP256r1_PARAMS.order) >= 0) {
		BigInteger::sub(r, r, EC_SECP256r1_PARAMS.order);
	}
	BigInteger::write_bytes_big_endian(signature, 32, r);

	BigInteger e; e.init(64);
	BigInteger::assign_bytes_big_endian(e, messageHash, 32);

	BigInteger s; s.init(64);
	BigInteger::mul(s, privateKey, r);
	BigInteger::div(s, EC_SECP256r1_PARAMS.order, nullptr);
	BigInteger::add(r, s, e);
	BigInteger::modular_inverse(k, EC_SECP256r1_PARAMS.order);
	BigInteger::mul(s, r, k);
	BigInteger::div(s, EC_SECP256r1_PARAMS.order, nullptr);
	BigInteger::write_bytes_big_endian(reinterpret_cast<u8*>(signature) + 32, 32, s);
}

inline void ecdsa_sign_secpr1_sha256(void* signature, const void* message, u32 messageLength, Keccak& random, void* privateKeyBytes) {
	u8 messageHash[32];
	sha256(messageHash, message, messageLength);
	ecdsa_sign_secpr1_sha256(signature, messageHash, random, privateKeyBytes);
}

inline bool ecdsa_verify_secpr1_sha256(BigInteger& r, BigInteger& s, const void* message, u32 messageLength, BigInteger& publicKeyXIn, BigInteger& publicKeyYIn) {
	BigInteger publicKeyX; publicKeyX.init(64);
	BigInteger publicKeyY; publicKeyY.init(64);
	BigInteger::assign(publicKeyX, publicKeyXIn);
	BigInteger::assign(publicKeyY, publicKeyYIn);

	if (BigInteger::cmp(r, 1) < 0 || BigInteger::cmp(r, EC_SECP256r1_PARAMS.order) >= 0) {
		return false;
	}
	if (BigInteger::cmp(s, 1) < 0 || BigInteger::cmp(s, EC_SECP256r1_PARAMS.order) >= 0) {
		return false;
	}

	BigInteger e; e.init(64);
	u8 messageHash[32];
	sha256(messageHash, message, messageLength);
	BigInteger::assign_bytes_big_endian(e, messageHash, 32);

	BigInteger::modular_inverse(s, EC_SECP256r1_PARAMS.order);

	BigInteger u1; u1.init(64);
	BigInteger u2; u2.init(64);
	BigInteger::mul(u1, e, s);
	BigInteger::div(u1, EC_SECP256r1_PARAMS.order, nullptr);
	BigInteger::mul(u2, r, s);
	BigInteger::div(u2, EC_SECP256r1_PARAMS.order, nullptr);

	bool isPointAtInfinity = !ec_two_power_mul(publicKeyX, publicKeyY, EC_SECP256r1_PARAMS.generatorX, EC_SECP256r1_PARAMS.generatorY, EC_SECP256r1_PARAMS.curveA, u2, u1, EC_SECP256r1_PARAMS.prime);
	if (isPointAtInfinity) {
		return false;
	}
	if (BigInteger::cmp(publicKeyX, EC_SECP256r1_PARAMS.order) >= 0) {
		BigInteger::sub(publicKeyX, publicKeyX, EC_SECP256r1_PARAMS.order);
	}
	return BigInteger::cmp(publicKeyX, r) == 0;
}

inline bool ecdsa_verify_secpr1_sha256(void* signature, u32 signatureLength, const void* message, u32 messageLength, BigInteger& publicKeyXIn, BigInteger& publicKeyYIn) {
	if (signatureLength != 32 + 32) {
		return false;
	}
	BigInteger r; r.init(64);
	BigInteger s; s.init(64);
	BigInteger::assign_bytes_big_endian(r, signature, 32);
	BigInteger::assign_bytes_big_endian(s, reinterpret_cast<u8*>(signature) + 32, 32);
	return ecdsa_verify_secpr1_sha256(r, s, message, messageLength, publicKeyXIn, publicKeyYIn);
}

inline bool ecdsa_verify_secpr1_sha256(void* signature, u32 signatureLength, const void* message, u32 messageLength, void* publicKeyBytes) {
	BigInteger publicKeyX; publicKeyX.init(64);
	BigInteger publicKeyY; publicKeyY.init(64);
	BigInteger::assign_bytes_big_endian(publicKeyX, publicKeyBytes, 32);
	BigInteger::assign_bytes_big_endian(publicKeyY, reinterpret_cast<u8*>(publicKeyBytes) + 32, 32);
	return ecdsa_verify_secpr1_sha256(signature, signatureLength, message, messageLength, publicKeyX, publicKeyY);
}

inline bool ecdsa_load_asn1(BigInteger& r, BigInteger& s, void* data, u32 dataLength) {
	ASN1Reader ecSignatureReader{ data, dataLength };
	u32 signatureSectionEnd = ecSignatureReader.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);
	ecSignatureReader.read_big_integer_tag(r, signatureSectionEnd);
	ecSignatureReader.read_big_integer_tag(s, signatureSectionEnd);
	ecSignatureReader.verify_section_complete(signatureSectionEnd);
	return !ecSignatureReader.errored;
}