#include "Util.h"
#include "BigInteger.h"
#include "ASN1.h"

enum X509Algorithm {
	X509_ALGORITHM_RSA_PKCS1_SHA256,
	X509_ALGORITHM_RSS_PSS_PSS_SHA256,
	X509_ALGORITHM_ECDSA_SECP256R1_SHA256
};

struct X509Certificate {
	static constexpr U32 certMaxLength = 10 * 1024;
	U8 cert[certMaxLength];
	U32 certLength;
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

enum X509Error {
	X509_ERROR_SUCCESS,
	X509_ERROR_PARSE_FAILED,
	X509_ERROR_WRONG_VERSION,
	X509_ERROR_UNSUPPORTED_CA_SIGNATURE_ALGORITHM,
	X509_ERROR_UNSUPPORTED_SIGNATURE_ALGORITHM
};

inline X509Error parse_asn1_der_cert2(X509SignatureAlgorithm* algorithm, void** publicKey, U32* publicKeyLength, void* vcert, U32 certLength) {
	X509Error errorCode = X509_ERROR_SUCCESS;
	ASN1Reader cert{ vcert, certLength };
	U32 tagId;
	ASN1ClassOfTag tagClass;
	// Cert sequence
	U32 certSequenceEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);

	{ // TBSCertificate sequence (TBS means To Be Signed)
		enum TBSCertContextTags {
			TBS_ID_VERSION = 0,
			TBS_ID_ISSUER_UNIQUE_ID = 1,
			TBS_ID_SUBJECT_UNIQUE_ID = 2,
			TBS_ID_EXTENSIONS = 3
		};
		U32 tbsCertSequenceEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);

		// version (MUST be present in TLS 1.3, since the default is v1 and TLS 1.3 requires v3)
		U32 versionEnd = cert.read_expected_identifier_end(ASN1_CLASS_CONTEXT_SPECIFIC, TBS_ID_VERSION);
		I64 version = cert.read_integer_tag();
		// verify version 3
		if (version != 2) {
			errorCode = X509_ERROR_WRONG_VERSION;
			goto parseError;
		}
		cert.verify_section_complete(versionEnd);

		// skip serial number
		cert.skip_field();

		{ // AlgorithmIdentifier sequence, signature field
			U32 signatureEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);

			X509Algorithm certificateSignatureScheme;
			U32 algorithmIdentifierLength = cert.read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_OBJECT_IDENTIFIER);
			if (cert.oid_match(ASN1_OID_RSA_PKCS1_SHA256, sizeof(ASN1_OID_RSA_PKCS1_SHA256) / sizeof(U32), algorithmIdentifierLength)) {
				certificateSignatureScheme = X509_ALGORITHM_RSA_PKCS1_SHA256;
				cert.read_null_tag();
			} else if (cert.oid_match(ASN1_OID_RSASSA_PSS, sizeof(ASN1_OID_RSASSA_PSS) / sizeof(U32), algorithmIdentifierLength)) {
				// In this case, the hash algorithm is found in the parameters
				certificateSignatureScheme = X509_ALGORITHM_RSS_PSS_PSS_SHA256;
				//TODO
			} else if (cert.oid_match(ASN1_OID_ECDSA_SHA256, sizeof(ASN1_OID_ECDSA_SHA256) / sizeof(U32), algorithmIdentifierLength)) {
				certificateSignatureScheme = X509_ALGORITHM_ECDSA_SECP256R1_SHA256;
			} else {
				errorCode = X509_ERROR_UNSUPPORTED_CA_SIGNATURE_ALGORITHM;
				goto parseError;
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
			U32 subjectPublicKeyInfoEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);
			{ // AlgorithmIdentifier sequence, algorithm field
				U32 algorithmEnd = cert.read_expected_identifier_end(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF);

				U32 algorithmIdentifierLength = cert.read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_OBJECT_IDENTIFIER);
				if (cert.oid_match(ASN1_OID_RSA_ENCRYPTION, sizeof(ASN1_OID_RSA_ENCRYPTION) / sizeof(U32), algorithmIdentifierLength)) {
					cert.read_null_tag();
					*algorithm = X509_SIGNATURE_RSA_GENERIC;
				} else if (cert.oid_match(ASN1_OID_RSASSA_PSS, sizeof(ASN1_OID_RSASSA_PSS) / sizeof(U32), algorithmIdentifierLength)) {
					cert.read_null_tag();
					*algorithm = X509_SIGNATURE_RSA_PSS;
				} else if (cert.oid_match(ASN1_OID_EC_PUBLIC_KEY, sizeof(ASN1_OID_EC_PUBLIC_KEY) / sizeof(U32), algorithmIdentifierLength)) {
					// This is actually a choice between a generic ECParameters, a named curve identifier, and implicitly CA
					// I'm only supporting named curves
					U32 curveIdentifierLength = cert.read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_OBJECT_IDENTIFIER);
					if (cert.oid_match(ASN1_OID_CURVE_SECP256R1, sizeof(ASN1_OID_CURVE_SECP256R1) / sizeof(U32), curveIdentifierLength)) {
						*algorithm = X509_SIGNATURE_EC_SECP256R1;
					} else {
						errorCode = X509_ERROR_UNSUPPORTED_SIGNATURE_ALGORITHM;
						goto parseError;
					}
				} else {
					errorCode = X509_ERROR_UNSUPPORTED_SIGNATURE_ALGORITHM;
					goto parseError;
				}

				cert.verify_section_complete(algorithmEnd);
			}

			U32 subjectPublicKeyLength = cert.read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_BIT_STRING);
			U32 subjectPublicKeyBitLength;
			U8* subjectPublicKey = cert.read_bitstring(subjectPublicKeyLength, &subjectPublicKeyBitLength);
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
	if (cert.errored) {
		errorCode = X509_ERROR_PARSE_FAILED;
	}
parseError:;
	return errorCode;
}

// I really should generalize these parser classes, lots of duplicate code between them

// Parses first certificate in a PEM certificate chain into DER
// Certificate chain is of this format
// ALPHA = 'a'-'z' / 'A'-'Z'
// DIGIT = '0'-'9'
// eol = CRLF / CR / LF
// preeb = "-----BEGIN CERTIFICATE-----"
// posteb = "-----END CERTIFICATE-----"
// base64char = ALPHA / DIGIT / "+" / "/"
// base64pad = "="
// base64fullline = 64base64char eol
// strictbase64finl = *15(4base64char) (4base64char / 3base64char base64pad / 2base64char 2base64pad) eol
// strictbase64text = *base64fullline strictbase64finl
// stricttextualmsg = preeb eol strictbase64text posteb eol
// certchain = stricttextualmsg *(eol stricttextualmsg)
struct PEM2DER {
	const char* src;
	U32 srcLength;
	U32 pos;
	B32 errored;

	B32 match_string(const char* str) {
		// match char until null character
		U32 prevPos = pos;
		while (pos < srcLength && str[0] != '\0' && str[0] == src[pos]) {
			pos++, str++;
		}
		if (str[0] != '\0') {
			pos = prevPos;
			return false;
		}
		return true;
	}

	U8 base64_decode_6bits(const char charIn) {
		if (charIn == '/') {
			return 63;
		} else if (charIn == '+') {
			return 62;
		} else if (charIn >= '0' && charIn <= '9') {
			return (charIn - '0') + 52;
		} else if (charIn >= 'A' && charIn <= 'Z') {
			return charIn - 'A';
		} else if (charIn >= 'a' && charIn <= 'z') {
			return (charIn - 'a') + 26;
		} else {
			return 0xFF;
		}
	}

	void consume_eol() {
		U32 prevPos = pos;
		if (pos < srcLength && src[pos] == '\r') {
			pos++;
		}
		if (pos < srcLength && src[pos] == '\n') {
			pos++;
		}
		if (prevPos == pos) {
			errored = true;
		}
	}

	void consume_preeb() {
		if (!match_string("-----BEGIN CERTIFICATE-----")) {
			errored = true;
		}
	}

	void consume_posteb() {
		if (!match_string("-----END CERTIFICATE-----")) {
			errored = true;
		}
	}

	U32 pem_to_der(Byte* dst, U32 dstCapacity, const char* srcIn, U32 srcLengthIn) {
		src = srcIn;
		srcLength = srcLengthIn;
		pos = 0;
		errored = false;
		U32 outSize = 0;
		consume_preeb();
		consume_eol();
		while (!errored) {
			if (pos < srcLength && src[pos] == '-') {
				// All lines were the full 64 characters, previous pass must have consumed the strictbase64finl
				break;
			}
			for (U32 i = 0; i < 64 && pos + 4 <= srcLength; i += 4) {
				if (pos < srcLength && (src[pos] == '\r' || src[pos] == '\n')) {
					consume_eol();
					goto foundStrictbase64finl;
				}
				U8 bits0 = base64_decode_6bits(src[pos + 0]);
				U8 bits1 = base64_decode_6bits(src[pos + 1]);
				U8 bits2 = base64_decode_6bits(src[pos + 2]);
				U8 bits3 = base64_decode_6bits(src[pos + 3]);
				if (bits0 == 0xFF || bits1 == 0xFF) {
					errored = true;
					return -1;
				}
				if (bits2 != 0xFF && bits3 != 0xFF) {
					if (dstCapacity < 3) {
						errored = true;
						return -1;
					}
					dst[0] = (bits0 << 2) | (bits1 >> 4);
					dst[1] = (bits1 << 4) | (bits2 >> 2);
					dst[2] = (bits2 << 6) | bits3;
					pos += 4, dst += 3, outSize += 3, dstCapacity -= 3;
				} else if (bits2 != 0xFF && src[pos + 3] == '=') {
					if (dstCapacity < 2) {
						errored = true;
						return -1;
					}
					dst[0] = (bits0 << 2) | (bits1 >> 4);
					dst[1] = (bits1 << 4) | (bits2 >> 2);
					pos += 4, dst += 2, outSize += 2, dstCapacity -= 2;
					consume_eol();
					goto foundStrictbase64finl;
				} else if (src[pos + 2] == '=' && src[pos + 3] == '=') {
					if (dstCapacity < 1) {
						errored = true;
						return -1;
					}
					dst[0] = (bits0 << 2) | (bits1 >> 4);
					pos += 4, dst += 1, outSize += 1, dstCapacity -= 1;
					consume_eol();
					goto foundStrictbase64finl;
				} else {
					errored = true;
					return -1;
				}
			}
			consume_eol();
		}
	foundStrictbase64finl:;
		consume_posteb();
		consume_eol();
		// Ignore the rest, we only want to write out the first certificate
		return errored ? -1 : outSize;
	}
};
