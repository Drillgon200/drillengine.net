// ACME implemention for getting a certificate from Let's Encrypt
// Implemented from RFC 8555

#include "JSON.h"
#include "TLS.h"

const char* DRILL_ACME_USER_AGENT = "DrillACME/1.0";

B32 encode_certificate_signing_request(void* csr, U32* csrLength, BigInteger& rsaModulo, BigInteger& rsaPublicExponent, BigInteger& rsaPrivateExponent, const char* domainName) {
	U32 csrMaxLength = *csrLength;
	ASN1Writer asn1{ csr, csrMaxLength };
	U32 certificationRequest = asn1.begin_sequence();
		U32 toBeSignedStart = asn1.pos;
		U32 certificationRequestInfo = asn1.begin_sequence();
			// version
			asn1.write_integer(0);

			constexpr U32 relativeDistinguishedNameCount = 3;
			const void* identifiers[relativeDistinguishedNameCount]{ ASN1_RAW_OID_COUNTRY_NAME, ASN1_RAW_OID_STATE_OR_PROVINCE_NAME, ASN1_RAW_OID_COMMON_NAME };
			const U32 identifierLengths[relativeDistinguishedNameCount]{ sizeof(ASN1_RAW_OID_COUNTRY_NAME), sizeof(ASN1_RAW_OID_STATE_OR_PROVINCE_NAME), sizeof(ASN1_RAW_OID_COMMON_NAME) };
			const char* names[relativeDistinguishedNameCount]{ "US", "California", domainName };
			asn1.write_x501_name(names, identifiers, identifierLengths, relativeDistinguishedNameCount);

			U32 subjectPKInfo = asn1.begin_sequence();
				U32 algorithm = asn1.begin_sequence();
					asn1.write_oid(ASN1_OID_RSA_ENCRYPTION, sizeof(ASN1_OID_RSA_ENCRYPTION) / sizeof(U32));
					asn1.write_null();
				asn1.end_sequence(algorithm);

				U32 subjectPublicKey = asn1.write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_BIT_STRING, false);
					// No unused bits
					asn1.write_raw_byte(0);
					// RFC 3279, 2.3.1  RSA Keys
					U32 rsaPublicKey = asn1.begin_sequence();
						asn1.write_big_integer(rsaModulo);
						asn1.write_big_integer(rsaPublicExponent);
					asn1.end_sequence(rsaPublicKey);
				asn1.patch_length(subjectPublicKey);
			asn1.end_sequence(subjectPKInfo);

			U32 attributes = asn1.write_identifier_length(ASN1_CLASS_CONTEXT_SPECIFIC, 0, true);
				// RFC 2986 4
				U32 attributeSequence = asn1.begin_sequence();
					asn1.write_oid(ASN1_OID_EXTENSION_REQUEST, sizeof(ASN1_OID_EXTENSION_REQUEST) / sizeof(U32));
					U32 extensionSet = asn1.begin_set();
						// RFC 2985 5.4.2
						// Should have subjectAltName, technically using the name directly is deprecated
						U32 generalNamesSequence = asn1.begin_sequence();
							U32 subjectAltNameSequence = asn1.begin_sequence();
								asn1.write_raw_oid(ASN1_RAW_OID_SUBJECT_ALT_NAME, sizeof(ASN1_RAW_OID_SUBJECT_ALT_NAME));
								U32 octetString = asn1.write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_OCTET_STRING, false);
									U32 nameSequence = asn1.begin_sequence();
										U32 dnsName = asn1.write_identifier_length(ASN1_CLASS_CONTEXT_SPECIFIC, 2, false);
											asn1.write_raw_bytes(domainName, strlen(domainName));
										asn1.patch_length(dnsName);
									asn1.end_sequence(nameSequence);
								asn1.patch_length(octetString);
							asn1.end_sequence(subjectAltNameSequence);
						asn1.end_sequence(generalNamesSequence);
					asn1.end_set(extensionSet);
				asn1.end_sequence(attributeSequence);
			asn1.patch_length(attributes);
		asn1.end_sequence(certificationRequestInfo);
		U32 toBeSignedEnd = asn1.pos;

		U32 algorithmIdentifier = asn1.begin_sequence();
			asn1.write_oid(ASN1_OID_RSA_PKCS1_SHA256, sizeof(ASN1_OID_RSA_PKCS1_SHA256) / sizeof(U32));
			asn1.write_null();
		asn1.end_sequence(algorithmIdentifier);

		U32 signature = asn1.write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_BIT_STRING, false);
			// No unused bits
			asn1.write_raw_byte(0);
			// sign it here
			U32 rsaBitLength = BigInteger::bitlength(rsaModulo);
			VERIFY_RETURN(asn1.pos + (rsaBitLength / 8) <= asn1.cap, false);
			rsassa_pkcs1_sign_sha256(asn1.data + asn1.pos, asn1.data + toBeSignedStart, toBeSignedEnd - toBeSignedStart, rsaPrivateExponent, rsaModulo, rsaBitLength);
			asn1.pos += rsaBitLength / 8;
		asn1.patch_length(signature);
	asn1.end_sequence(certificationRequest);
	*csrLength = asn1.pos;
	return !asn1.errored;
}

struct ACMEAccount {
	SECP256R1Key signingKey;
	static constexpr U32 accountURLMaxLength = 256;
	char accountURL[accountURLMaxLength];
	U32 accountURLLength;

	B32 is_created() {
		return accountURLLength != 0;
	}
};

enum ACMEState {
	ACME_STATE_IDLE,
	ACME_STATE_FAILED,
	ACME_STATE_WAIT_DIRECTORY_QUERY,
	ACME_STATE_WAIT_NONCE,
	ACME_STATE_WAIT_ACCOUNT_QUERY,
	ACME_STATE_WAIT_NEW_ACCOUNT,
	ACME_STATE_WAIT_CERT_ISSUANCE,
	ACME_STATE_WAIT_GET_AUTHORIZATION,
	ACME_STATE_WAIT_NEXT_AUTHORIZATION_QUERY_TIME,
	ACME_STATE_WAIT_LOCAL_SERVER_READY_NOTIFY,
	ACME_STATE_WAIT_AUTHORIZATION_QUERY,
	ACME_STATE_WAIT_FINALIZE,
	ACME_STATE_WAIT_CERT_ISSANCE_NEXT_QUERY_TIME,
	ACME_STATE_WAIT_CERT_ISSUANCE_QUERY,
	ACME_STATE_WAIT_CERT_DOWNLOAD,
	ACME_STATE_CERT_READY
};

struct ACMEAgent {
	TLSConnection client;
	HTTPReader httpReader;
	static constexpr U32 maxReadableHTTPFields = 4;
	HTTPField httpFields[maxReadableHTTPFields];
	U32 numHTTPFieldsToRead;
	HTTPServer httpServer;
	ACMEAccount currentAccount;
	X509Certificate* certificate;

	// This buffer is a bump allocator for URLs that have to be saved
	static constexpr U32 urlBufferSize = 2048;
	char urlBuffer[urlBufferSize];
	U32 urlBufferPos;
	I32 urlBufferNewAccountOffset;
	I32 urlBufferNewNonceOffset;
	I32 urlBufferNewOrderOffset;
	I32 urlBufferCurrentOrderOffset;
	I32 urlBufferFinalizeOffset;
	I32 urlBufferAuthorizationOffset;
	U32 numAuthorizations;
	I32 urlBufferChallengeURLOffset;
	U32 urlBufferChallengeTokenServableURLOffset;
	I32 urlBufferChallengeTokenOffset;
	U32 urlBufferChallengeTokenLength;
	I32 urlBufferCertDownloadOffset;
	B32 currentAuthorizationComplete;
	U32 authorizationQueryAttemptsRemaining;
	U64 authorizationQueryTimeoutTimeMilliseconds;
	U64 authorizationQueryNextAttemptTimeMilliseconds;
	U64 certificateProcessingNextQueryTimeMilliseconds;
	U64 certificateProcessingTimeoutTimeMilliseconds;
	B32 certIssued;
	static constexpr U32 nonceBufferCap = 512;
	char nonceBuffer[nonceBufferCap];
	U32 nonceLength;

	const char* host;
	const char* domainNameToBeCertified;
	const char* directoryPath;

	ACMEState state;

	void init(const char* hostIn, const char* domainNameToBeCertifiedIn) {
		client.state = TLS_CONNECTION_STATE_CLOSED;
		httpServer.open = false;
		certificate = nullptr;
		host = hostIn;
		domainNameToBeCertified = domainNameToBeCertifiedIn;
		state = ACME_STATE_IDLE;
	}

	B32 post_jws_json(const char* url, U32 urlLen, const char* data, U32 dataLength) {
		HTTPURL urlReader;
		B32 urlParseSuccess = urlReader.parse(url, urlLen);
		VERIFY_RETURN(urlParseSuccess, false);

		U32 contentLengthPatchPos;
		client.post(urlReader.src + urlReader.path.start)
			.field("Host", host)
			.field("Content-Type", "application/jose+json")
			.field("User-Agent", DRILL_ACME_USER_AGENT)
			.content_length(&contentLengthPatchPos)
			.end_http_header();
		client.move_send_data_to_beginning_of_buffer();
		U32 encodedLength = 0;
		B32 encodedSuccess = jws_encode_es256(&encodedLength, reinterpret_cast<char*>(client.sendBuffer) + client.sendBufferDataEnd, sizeof(TLSConnection::sendBuffer) - client.sendBufferDataEnd, data, dataLength, nonceBuffer, nonceLength, urlReader.src, currentAccount.accountURLLength > 0 ? currentAccount.accountURL : nullptr, currentAccount.signingKey);
		VERIFY_RETURN(encodedSuccess, false);
		client.sendBufferDataEnd += encodedLength;
		VERIFY_RETURN(client.queue_buffer_to_send(nullptr, encodedLength), false);
		client.patch_content_length(contentLengthPatchPos, encodedLength)
			.send_data();
		httpFields[0].reset("Replay-Nonce");
		numHTTPFieldsToRead = 1;
		return true;
	}

	B32 post_as_get(const char* url, U32 urlLen) {
		return post_jws_json(url, urlLen, "", 0);
	}

	// Step 8, download cert
	B32 download_cert() {
		print("ACME downloading cert\n");

		HTTPURL urlReader;
		B32 urlParseSuccess = urlReader.parse(urlBuffer + urlBufferCertDownloadOffset, strlen(urlBuffer + urlBufferCertDownloadOffset));
		VERIFY_RETURN(urlParseSuccess, false);

		U32 contentLengthPatchPos;
		client.post(urlReader.src + urlReader.path.start)
			.field("Host", host)
			.field("Content-Type", "application/jose+json")
			.field("Accept", "application/pem-certificate-chain") // Let's encrypt won't return application/pkcs7-mime anymore
			.field("User-Agent", DRILL_ACME_USER_AGENT)
			.content_length(&contentLengthPatchPos)
			.end_http_header();
		client.move_send_data_to_beginning_of_buffer();
		U32 encodedLength = 0;
		B32 encodedSuccess = jws_encode_es256(&encodedLength, reinterpret_cast<char*>(client.sendBuffer) + client.sendBufferDataEnd, sizeof(TLSConnection::sendBuffer) - client.sendBufferDataEnd, "", 0, nonceBuffer, nonceLength, urlReader.src, currentAccount.accountURL, currentAccount.signingKey);
		VERIFY_RETURN(encodedSuccess, false);
		client.sendBufferDataEnd += encodedLength;
		VERIFY_RETURN(client.queue_buffer_to_send(nullptr, encodedLength), false);
		client.patch_content_length(contentLengthPatchPos, encodedLength)
			.send_data();
		numHTTPFieldsToRead = 0;
		return true;
	}
	B32 handle_cert_download_response(const void* responseData, U32 responseDataLength) {
		print("ACME handling cert download response\n");
		PEM2DER pemConverter;
		U32 decodedSize = pemConverter.pem_to_der(certificate->cert, X509Certificate::certMaxLength, reinterpret_cast<const char*>(responseData), responseDataLength);
		VERIFY_RETURN(!pemConverter.errored, false);
		certificate->certLength = decodedSize;
		return true;
	}

	// Step 7 (optional), wait for cert to get issued
	B32 query_cert_issued_status() {
		print("ACME querying cert issued status\n");
		return post_as_get(urlBuffer + urlBufferCurrentOrderOffset, strlen(urlBuffer + urlBufferCurrentOrderOffset));
	}
	B32 handle_cert_issued_query_response(const void* responseData, U32 responseDataLength) {
		print("ACME handling cert issued query response\n");
		HTTPField& replayNonce = httpFields[0];
		VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() < nonceBufferCap, false);
		memcpy(nonceBuffer, httpReader.src + replayNonce.value.start, nonceLength = nonceLength);

		// This is close to a duplicate of the code in handle_order_finalize_response,
		// but I don't see a way to unduplicate it that I like
		JSONReader json; json.init(responseData, responseDataLength);
		json.begin_object();
		Range jsonFieldName;
		Range jsonFieldValue;
		while (json.read_field_name(&jsonFieldName)) {
			if (jsonFieldName.data_range(json.data) == "status") {
				VERIFY_RETURN(json.read_string_field(&jsonFieldValue), false);
				if (jsonFieldValue.data_range(json.data) == "valid") {
					certIssued = true;
				} else if (jsonFieldValue.data_range(json.data) == "processing") {
					// Check back in a few seconds.
					certificateProcessingNextQueryTimeMilliseconds = current_time_millis() + 1000;
				} else {
					VERIFY_RETURN(false, false);
				}
				VERIFY_RETURN(jsonFieldValue.data_range(json.data) == "valid", false);
			} else if (jsonFieldName.data_range(json.data) == "certificate") {
				VERIFY_RETURN(json.read_string_field(&jsonFieldValue), false);
				VERIFY_RETURN(urlBufferPos + jsonFieldValue.length() + 1 <= urlBufferSize, false);
				urlBufferCertDownloadOffset = urlBufferPos;
				memcpy(urlBuffer + urlBufferPos, json.data + jsonFieldValue.start, jsonFieldValue.length());
				urlBufferPos += jsonFieldValue.length();
				urlBuffer[urlBufferPos++] = '\0';
			} else {
				json.skip_value();
			}
		}
		json.end_object();
		if (certIssued) {
			VERIFY_RETURN(urlBufferCertDownloadOffset != -1, false);
		}
		VERIFY_RETURN(!json.errored, false);
		return true;
	}

	// Step 6, finalize order
	B32 finalize_order() {
		print("ACME sending order finalize\n");
		constexpr U32 bufferSize = 1024;
		U32 csrLength = bufferSize;
		Byte csrBuffer[bufferSize];
		rsa_keygen_2048(&certificate->rsaPublicKey, &certificate->rsaPrivateKey, &certificate->rsaModulus);
		VERIFY_RETURN(encode_certificate_signing_request(csrBuffer, &csrLength, certificate->rsaModulus, certificate->rsaPublicKey, certificate->rsaPrivateKey, domainNameToBeCertified), false);
		char jsonBuffer[bufferSize];
		JSONWriter csrEncoder; csrEncoder.init(jsonBuffer, bufferSize);
		csrEncoder.begin_object();
		csrEncoder.write_str("\"csr\":\"").write_base64(csrBuffer, csrLength).write('"');
		csrEncoder.end_object();
		VERIFY_RETURN(!csrEncoder.errored, false);
		return post_jws_json(urlBuffer + urlBufferFinalizeOffset, strlen(urlBuffer + urlBufferFinalizeOffset), csrEncoder.data, csrEncoder.pos);
	}
	B32 handle_order_finalize_response(const void* responseData, U32 responseDataLength) {
		print("ACME handling order finalize response\n");
		HTTPField& replayNonce = httpFields[0];
		VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() < nonceBufferCap, false);
		memcpy(nonceBuffer, httpReader.src + replayNonce.value.start, nonceLength = nonceLength);

		JSONReader json; json.init(responseData, responseDataLength);
		json.begin_object();
		Range jsonFieldName;
		Range jsonFieldValue;
		while (json.read_field_name(&jsonFieldName)) {
			if (jsonFieldName.data_range(json.data) == "status") {
				VERIFY_RETURN(json.read_string_field(&jsonFieldValue), false);
				if (jsonFieldValue.data_range(json.data) == "valid") {
					certIssued = true;
				} else if (jsonFieldValue.data_range(json.data) == "processing") {
					// Haven't failed yet, but need to check back later
					// I'm not sure why certificate processing even takes significant time
					// Maybe the cert has to propagate to a bunch of servers or something?
					U64 currentTime = current_time_millis();
					certificateProcessingNextQueryTimeMilliseconds = currentTime + 1000;
					certificateProcessingTimeoutTimeMilliseconds = currentTime + 20000;
				} else {
					VERIFY_RETURN(false, false);
				}
			} else if (jsonFieldName.data_range(json.data) == "certificate") {
				VERIFY_RETURN(json.read_string_field(&jsonFieldValue), false);
				VERIFY_RETURN(urlBufferPos + jsonFieldValue.length() + 1 <= urlBufferSize, false);
				urlBufferCertDownloadOffset = urlBufferPos;
				memcpy(urlBuffer + urlBufferPos, json.data + jsonFieldValue.start, jsonFieldValue.length());
				urlBufferPos += jsonFieldValue.length();
				urlBuffer[urlBufferPos++] = '\0';
			} else {
				json.skip_value();
			}
		}
		json.end_object();
		if (certIssued) {
			VERIFY_RETURN(urlBufferCertDownloadOffset != -1, false);
		}
		VERIFY_RETURN(!json.errored, false);
		return true;
	}

	// Step 5, set up HTTP server and respond to requests
	B32 get_http_authorization() {
		print("ACME Completing authorization ");
		print_num(numAuthorizations - 1);
		print("\n");
		currentAuthorizationComplete = false;
		return post_as_get(urlBuffer + urlBufferAuthorizationOffset, strlen(urlBuffer + urlBufferAuthorizationOffset));
	}
	B32 handle_get_http_authorization_response(const void* responseData, U32 responseDataLength) {
		print("ACME handling get http authorization response\n");
		HTTPField& replayNonce = httpFields[0];
		VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() < nonceBufferCap, false);
		memcpy(nonceBuffer, httpReader.src + replayNonce.value.start, nonceLength = replayNonce.value.length());

		JSONReader json; json.init(responseData, responseDataLength);
		json.begin_object();
		Range jsonFieldName;
		Range jsonFieldValue;
		while (json.read_field_name(&jsonFieldName)) {
			DataRange fieldName = jsonFieldName.data_range(json.data);
			if (fieldName == "challenges") {
				json.begin_array();
				B32 hasHTTP01Challenge = false;
				while (json.read_token('{')) {
					Range jsonType{};
					Range jsonURL{};
					Range jsonToken{};
					while (json.read_field_name(&jsonFieldName)) {
						fieldName = jsonFieldName.data_range(json.data);
						if (fieldName == "type") {
							VERIFY_RETURN(json.read_string_field(&jsonType), false);
						} else if (fieldName == "url") {
							VERIFY_RETURN(json.read_string_field(&jsonURL), false);
						} else if (fieldName == "token") {
							VERIFY_RETURN(json.read_string_field(&jsonToken), false);
						} else {
							json.skip_value();
						}
					}
					json.end_object();
					if (jsonType.data_range(json.data) == "http-01") {
						VERIFY_RETURN(jsonURL.length() != 0 && jsonToken.length() != 0, false);
						// The servable url is in the format /.well-known/acme-challenge/<token>.<base64url(Thumbprint(accountKey))>
						const char* directoryBase = "/.well-known/acme-challenge/";
						U32 directoryBaseLength = strlen(directoryBase);
						U32 servableTokenURLSize = directoryBaseLength + jsonToken.length() + 1 + base64url_encoded_size(SHA256_HASH_SIZE);
						VERIFY_RETURN(jsonURL.length() + 1 + servableTokenURLSize + 1 < urlBufferSize, false)
							hasHTTP01Challenge = true;
						if (urlBufferChallengeURLOffset != -1) {
							// Free previous challenge url just in case the json is malformed and there's more than one http-01
							urlBufferPos = urlBufferChallengeURLOffset;
						}
						urlBufferChallengeURLOffset = urlBufferPos;
						memcpy(urlBuffer + urlBufferChallengeURLOffset, json.data + jsonURL.start, jsonURL.length());
						urlBufferPos += jsonURL.length();
						urlBuffer[urlBufferPos++] = '\0';
						urlBufferChallengeTokenServableURLOffset = urlBufferPos;
						memcpy(urlBuffer + urlBufferChallengeTokenServableURLOffset, directoryBase, directoryBaseLength);
						urlBufferPos += directoryBaseLength;
						urlBufferChallengeTokenOffset = urlBufferPos;
						urlBufferChallengeTokenLength = jsonToken.length();
						memcpy(urlBuffer + urlBufferChallengeTokenOffset, json.data + jsonToken.start, jsonToken.length());
						urlBufferPos += jsonToken.length();
						urlBuffer[urlBufferPos++] = '.';
						U32 thumbprintSize = base64url_encoded_size(SHA256_HASH_SIZE);
						B32 thumbprintComputationSuccess = compute_jwk_thumbprint_base64(urlBuffer + urlBufferPos, &thumbprintSize, currentAccount.signingKey.publicKey);
						VERIFY_RETURN(thumbprintComputationSuccess, false);
						urlBufferPos += thumbprintSize;
						urlBuffer[urlBufferPos++] = '\0';
						while (json.peek_token('{')) {
							json.skip_value();
						}
					}
				}
				VERIFY_RETURN(hasHTTP01Challenge, false);
				json.end_array();
			} else {
				json.skip_value();
			}
		}
		json.end_object();
		VERIFY_RETURN(!json.errored, false);
		return true;
	}
	B32 notify_acme_server_that_local_http_server_is_ready_for_requests() {
		print("ACME Notifying acme server that local server is up\n");
		return post_jws_json(urlBuffer + urlBufferChallengeURLOffset, strlen(urlBuffer + urlBufferChallengeURLOffset), "{}", 2);
	}
	B32 handle_server_up_notification_response() {
		print("ACME Handling server up notification response\n");
		HTTPField& replayNonce = httpFields[0];
		VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() < nonceBufferCap, false);
		memcpy(nonceBuffer, httpReader.src + replayNonce.value.start, nonceLength = replayNonce.value.length());
		return true;
	}
	void setup_challenge_server() {
		httpServer.init();
		// urlBufferChallengeTokenServableURLOffset is in the format
		// /.well-known/acme-challenge/<token>.<thumbprint>
		// but we need the url to serve only the token part in the name, not the thumbprint part.
		// So, use the first part for the url and the last part in the resource
		U32 thumbprintSize = base64url_encoded_size(SHA256_HASH_SIZE);
		U32 servableURLLength = strlen("/.well-known/acme-challenge/") + urlBufferChallengeTokenLength;
		httpServer.serve_resource(HTTPResource{ urlBuffer + urlBufferChallengeTokenServableURLOffset, urlBuffer + urlBufferChallengeTokenOffset, servableURLLength, urlBufferChallengeTokenLength + 1 + thumbprintSize });
		authorizationQueryAttemptsRemaining = 50;
		// Apparently responding to challenges authorizations can take more than 10 seconds, which seems odd to me
		// Wait for an extremely generous 2 minutes instead
		const U32 serverTimeoutMilliseconds = 2 * 60 * 1000;
		const U32 timeBetweenAttempsMilliseconds = 1000;
		I64 currentTime = current_time_millis();
		authorizationQueryTimeoutTimeMilliseconds = currentTime + serverTimeoutMilliseconds;
		authorizationQueryNextAttemptTimeMilliseconds = currentTime + timeBetweenAttempsMilliseconds;
	}
	B32 query_authorization() {
		print("ACME Querying authorization for completeness\n");
		VERIFY_RETURN(authorizationQueryAttemptsRemaining > 0, false);
		authorizationQueryAttemptsRemaining--;
		return post_as_get(urlBuffer + urlBufferAuthorizationOffset, strlen(urlBuffer + urlBufferAuthorizationOffset));
	}
	B32 handle_query_authorization_response(const void* responseData, U32 responseDataLength) {
		print("ACME Handling authorization completeness query response\n");
		HTTPField& replayNonce = httpFields[0];
		VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() < nonceBufferCap, false);
		memcpy(nonceBuffer, httpReader.src + replayNonce.value.start, nonceLength = replayNonce.value.length());

		JSONReader json; json.init(responseData, responseDataLength);
		Range jsonFieldName;
		Range jsonFieldValue;
		json.begin_object();
		while (json.read_field_name(&jsonFieldName)) {
			DataRange fieldName = jsonFieldName.data_range(json.data);
			if (fieldName == "status") {
				VERIFY_RETURN(json.read_string_field(&jsonFieldValue), false);
				DataRange status = jsonFieldValue.data_range(json.data);
				if (status == "valid") {
					currentAuthorizationComplete = true;
					httpServer.close();
				} else if (status == "invalid") {
					// Something went wrong
					VERIFY_RETURN(false, false);
				} else {
					// Not authorized yet, keep waiting
				}
			} else {
				json.skip_value();
			}
		}
		json.end_object();
		VERIFY_RETURN(!json.errored, false);
		const U32 timeBetweenAttempsMilliseconds = 1000;
		authorizationQueryNextAttemptTimeMilliseconds = current_time_millis() + timeBetweenAttempsMilliseconds;
		return true;
	}

	// Step 4, apply for certificate issuance
	B32 apply_for_cert() {
		print("ACME Applying for cert\n");
		constexpr U32 jsonBufferSize = 1024;
		char jsonBuffer[jsonBufferSize];
		JSONWriter payload; payload.init(jsonBuffer, jsonBufferSize);
		payload.begin_object();
		payload.write_str("\"identifiers\":").begin_array();
		payload.begin_object().write_str("\"type\":\"dns\",\"value\":\"").write_str(domainNameToBeCertified).write_str("\"").end_object();
		payload.end_array();
		payload.end_object();
		VERIFY_RETURN(!payload.errored, false);

		B32 errored = post_jws_json(urlBuffer + urlBufferNewOrderOffset, strlen(urlBuffer + urlBufferNewOrderOffset), payload.data, payload.pos);
		httpFields[0].reset("Replay-Nonce");
		httpFields[1].reset("Location");
		numHTTPFieldsToRead = 2;
		return errored;
	}
	B32 handle_cert_application_response(const void* responseData, U32 responseDataLength) {
		print("ACME Handling cert application response\n");
		HTTPField& replayNonce = httpFields[0];
		VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() < nonceBufferCap, false);
		memcpy(nonceBuffer, httpReader.src + replayNonce.value.start, nonceLength = replayNonce.value.length());

		HTTPField& location = httpFields[1];
		VERIFY_RETURN(location.has_value() && urlBufferPos + location.value.length() + 1 < urlBufferSize, false);
		urlBufferCurrentOrderOffset = urlBufferPos;
		memcpy(urlBuffer + urlBufferCurrentOrderOffset, httpReader.src + location.value.start, location.value.length());
		urlBufferPos += location.value.length();
		urlBuffer[urlBufferPos++] = '\0';

		JSONReader json; json.init(responseData, responseDataLength);
		B32 hasAuthorizations = false;
		B32 hasFinalize = false;
		json.begin_object();
		Range jsonFieldName;
		Range jsonFieldValue;
		while (json.read_field_name(&jsonFieldName)) {
			DataRange name = jsonFieldName.data_range(json.data);
			if (name == "status") {
				B32 isStringField = json.read_string_field(&jsonFieldValue);
				VERIFY_RETURN(isStringField, false);
				VERIFY_RETURN(jsonFieldValue.data_range(json.data) == "pending" || jsonFieldValue.data_range(json.data) == "ready", false);
			} else if (name == "authorizations") {
				hasAuthorizations = true;
				json.begin_array();
				urlBufferAuthorizationOffset = urlBufferPos;
				while (json.read_string_field(&jsonFieldValue)) {
					VERIFY_RETURN(urlBufferPos + jsonFieldValue.length() + 1 <= urlBufferSize, false);
					memcpy(urlBuffer + urlBufferPos, json.data + jsonFieldValue.start, jsonFieldValue.length());
					urlBufferPos += jsonFieldValue.length();
					urlBuffer[urlBufferPos++] = '\0';
					numAuthorizations++;
				}
				json.end_array();
			} else if (name == "finalize") {
				hasFinalize = true;
				B32 isStringField = json.read_string_field(&jsonFieldValue);
				VERIFY_RETURN(isStringField, false);
				VERIFY_RETURN(jsonFieldValue.length() + 1 < urlBufferSize, false);
				urlBufferFinalizeOffset = urlBufferPos;
				memcpy(urlBuffer + urlBufferPos, json.data + jsonFieldValue.start, jsonFieldValue.length());
				urlBufferPos += jsonFieldValue.length();
				urlBuffer[urlBufferPos++] = '\0';
			} else {
				json.skip_value();
			}
		}
		json.end_object();
		VERIFY_RETURN(!json.errored, false);
		VERIFY_RETURN(hasFinalize && hasAuthorizations, false);
		VERIFY_RETURN(numAuthorizations <= 16u, false);
		return true;
	}

	// Step 3 part 2, create an account if we don't have a valid one
	B32 request_new_account() {
		print("ACME Requesting new account\n");
		Keccak random; random.make_secure_random();
		secp256r1_generate_keypair(random, &currentAccount.signingKey.publicKey.x[0], currentAccount.signingKey.privateKey.n);

		const char* jsonAccountPayload = 
			R"({
	"termsOfServiceAgreed": true,
	"contact": [
		"mailto:drillgon200@gmail.com"
	]			
})";
		currentAccount.accountURLLength = 0;
		B32 errored = post_jws_json(urlBuffer + urlBufferNewAccountOffset, strlen(urlBuffer + urlBufferNewAccountOffset), jsonAccountPayload, strlen(jsonAccountPayload));
		httpFields[0].reset("Replay-Nonce");
		httpFields[1].reset("Location");
		numHTTPFieldsToRead = 2;
		return errored;
	}
	B32 handle_new_account_response(const void* responseData, U32 responseDataLength) {
		print("ACME Handling new account response\n");
		HTTPField& replayNonce = httpFields[0];
		VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() < nonceBufferCap, false);
		memcpy(nonceBuffer, httpReader.src + replayNonce.value.start, nonceLength = replayNonce.value.length());

		B32 accountValid = false;
		JSONReader json; json.init(responseData, responseDataLength);
		json.begin_object();
		Range jsonFieldName;
		Range jsonFieldValue;
		while (json.read_field_name(&jsonFieldName)) {
			if (jsonFieldName.data_range(json.data) == "status") {
				VERIFY_RETURN(!accountValid, false);
				B32 isStringField = json.read_string_field(&jsonFieldValue);
				VERIFY_RETURN(isStringField, false);
				accountValid = jsonFieldValue.data_range(json.data) == "valid";
			} else {
				json.skip_value();
			}
		}
		json.end_object();
		VERIFY_RETURN(!json.errored, false);
		VERIFY_RETURN(accountValid, false);

		HTTPField& location = httpFields[1];
		VERIFY_RETURN(location.has_value(), false);
		VERIFY_RETURN(location.value.length() < ACMEAccount::accountURLMaxLength, false);
		memcpy(currentAccount.accountURL, httpReader.src + location.value.start, location.value.length());
		currentAccount.accountURL[location.value.length()] = '\0';
		currentAccount.accountURLLength = location.value.length();
		return true;
	}

	// Step 3 part 1, see if our current account is still ok
	B32 query_account_status() {
		print("ACME Querying account status\n");
		return post_as_get(currentAccount.accountURL, currentAccount.accountURLLength);
	}
	B32 handle_query_account_status_response(const void* responseData, U32 responseDataLength) {
		print("ACME Handling account status response\n");
		HTTPField& replayNonce = httpFields[0];
		VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() < nonceBufferCap, false);
		memcpy(nonceBuffer, httpReader.src + replayNonce.value.start, nonceLength = replayNonce.value.length());

		B32 statusValid = false;
		JSONReader json;
		json.init(responseData, responseDataLength);
		json.begin_object();
		Range jsonFieldName;
		Range jsonFieldValue;
		while (json.read_field_name(&jsonFieldName)) {
			DataRange name = jsonFieldName.data_range(json.data);
			if (name == "status") {
				B32 isStringField = json.read_string_field(&jsonFieldValue);
				if (isStringField) {
					statusValid = jsonFieldValue.data_range(json.data) == "valid";
				} else {
					json.skip_value();
				}
			} else {
				json.skip_value();
			}
		}
		json.end_object();
		if (!statusValid) {
			currentAccount.accountURLLength = 0;
		}
		return !json.errored;
	}

	// Step 2, request a nonce
	void request_nonce() {
		print("ACME Requesting nonce\n");
		client.head(urlBuffer + urlBufferNewNonceOffset)
			.field("Host", host)
			.field("User-Agent", DRILL_ACME_USER_AGENT)
			.end_http_header()
			.send_data();
		httpFields[0].reset("Replay-Nonce");
		numHTTPFieldsToRead = 1;
	}
	B32 handle_nonce_response() {
		print("ACME Handling nonce response\n");
		HTTPField& replayNonce = httpFields[0];
		VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() < nonceBufferCap, false);
		memcpy(nonceBuffer, httpReader.src + replayNonce.value.start, nonceLength = replayNonce.value.length());
		return true;
	}

	// Step 1: find all the urls for the requests we'll have to make
	void query_directory() {
		print("ACME Querying directory\n");
		client.get(directoryPath)
			.field("Host", host)
			.field("Content-Type", "application/jose+json")
			.field("User-Agent", DRILL_ACME_USER_AGENT)
			.end_http_header()
			.send_data();
	}
	B32 handle_query_directory_response(const void* responseData, U32 responseDataLength) {
		print("ACME Handling directory response\n");
		JSONReader json;
		json.init(responseData, responseDataLength);
		json.begin_object();
		Range jsonFieldName;
		Range jsonFieldValue;
		while (json.read_field_name(&jsonFieldName)) {
			DataRange nameDataRange = jsonFieldName.data_range(json.data);
			if (nameDataRange == "newAccount" ||
				nameDataRange == "newNonce" ||
				nameDataRange == "newOrder") {
				B32 isStringField = json.read_string_field(&jsonFieldValue);
				VERIFY_RETURN(isStringField, false);
				VERIFY_RETURN(urlBufferPos + jsonFieldValue.length() + 1 <= urlBufferSize, false);

				if (nameDataRange == "newAccount")      { VERIFY_RETURN(urlBufferNewAccountOffset == -1, false); urlBufferNewAccountOffset = urlBufferPos; } 
				else if (nameDataRange == "newNonce")   { VERIFY_RETURN(urlBufferNewNonceOffset == -1, false);   urlBufferNewNonceOffset   = urlBufferPos; }
				else if (nameDataRange == "newOrder")   { VERIFY_RETURN(urlBufferNewOrderOffset == -1, false);   urlBufferNewOrderOffset   = urlBufferPos; }
				memcpy(urlBuffer + urlBufferPos, json.data + jsonFieldValue.start, jsonFieldValue.length());
				urlBufferPos += jsonFieldValue.length();
				urlBuffer[urlBufferPos++] = '\0';
			} else {
				json.skip_value();
			}
		}
		json.end_object();
		return !json.errored;
	}

	void accept_http_server_connection(U32 client) {
		print("ACME Accepting HTTP server connection\n");
		if (!httpServer.accept_new_connection(client)) {
			g_syscallProc(SYSCALL_TCP_CLOSE, client);
		}
	}

	void do_network_io() {
		if (state == ACME_STATE_IDLE || state == ACME_STATE_FAILED || state == ACME_STATE_CERT_READY) {
			return;
		}
		if (httpServer.open) {
			// This part of the code will be handled by the TCP processing loop
			for (U32 i = 0; i < httpServer.connectionCap; i++) {
				HTTPConnection& client = httpServer.connections[i];
				if (client.tcpConnection == TCP_CONNECTION_BLOCK_INVALID_IDX) {
					continue;
				}
				httpServer.try_send_remaining_data(client);
				httpServer.receive_data(client);
			}
			httpServer.take_requests();
		}
		client.receive_data();
		httpReader.src = reinterpret_cast<char*>(client.receiveBuffer) + client.receiveBufferUserReadPos;
		httpReader.length = client.receiveBufferUserDataEnd - client.receiveBufferUserReadPos;
		httpReader.try_read_more_data(httpFields, numHTTPFieldsToRead);
		if (httpReader.errored) {
			print("ACME HTTP read error\n");
			state = ACME_STATE_FAILED;
			return;
		}
		if (httpReader.readComplete ||
			state == ACME_STATE_WAIT_NEXT_AUTHORIZATION_QUERY_TIME && current_time_millis() >= authorizationQueryNextAttemptTimeMilliseconds ||
			state == ACME_STATE_WAIT_CERT_ISSANCE_NEXT_QUERY_TIME && current_time_millis() >= certificateProcessingNextQueryTimeMilliseconds) {

			switch (state) {
			case ACME_STATE_WAIT_DIRECTORY_QUERY: {
				B32 success = handle_query_directory_response(httpReader.src + httpReader.contentOffset, httpReader.contentLength);
				if (success) {
					request_nonce();
					state = ACME_STATE_WAIT_NONCE;
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_NONCE: {
				B32 success = handle_nonce_response();
				if (success) {
					if (currentAccount.accountURLLength != 0) {
						success = query_account_status();
						state = success ? ACME_STATE_WAIT_ACCOUNT_QUERY : ACME_STATE_FAILED;
					} else {
						success = request_new_account();
						state = success ? ACME_STATE_WAIT_NEW_ACCOUNT : ACME_STATE_FAILED;
					}
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_ACCOUNT_QUERY: {
				B32 success = handle_query_account_status_response(httpReader.src + httpReader.contentOffset, httpReader.contentLength);
				if (success) {
					if (currentAccount.is_created()) {
						success = apply_for_cert();
						state = success ? ACME_STATE_WAIT_CERT_ISSUANCE : ACME_STATE_FAILED;
					} else {
						success = request_new_account();
						state = success ? ACME_STATE_WAIT_NEW_ACCOUNT : ACME_STATE_FAILED;
					}
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_NEW_ACCOUNT: {
				B32 success = handle_new_account_response(httpReader.src + httpReader.contentOffset, httpReader.contentLength);
				if (success) {
					success = apply_for_cert();
					state = success ? ACME_STATE_WAIT_CERT_ISSUANCE : ACME_STATE_FAILED;
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_CERT_ISSUANCE: {
				B32 success = handle_cert_application_response(httpReader.src + httpReader.contentOffset, httpReader.contentLength);
				if (success) {
					if (numAuthorizations == 0) {
						// Should never happen, but just in case...
						success = finalize_order();
						state = success ? ACME_STATE_WAIT_FINALIZE : ACME_STATE_FAILED;
					} else {
						success = get_http_authorization();
						state = success ? ACME_STATE_WAIT_GET_AUTHORIZATION : ACME_STATE_FAILED;
					}
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_GET_AUTHORIZATION: {
				B32 success = handle_get_http_authorization_response(httpReader.src + httpReader.contentOffset, httpReader.contentLength);
				if (success) {
					setup_challenge_server();
					success = notify_acme_server_that_local_http_server_is_ready_for_requests();
					state = success ? ACME_STATE_WAIT_LOCAL_SERVER_READY_NOTIFY : ACME_STATE_FAILED;
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_LOCAL_SERVER_READY_NOTIFY: {
				B32 success = handle_server_up_notification_response();
				if (success) {
					state = ACME_STATE_WAIT_NEXT_AUTHORIZATION_QUERY_TIME;
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_NEXT_AUTHORIZATION_QUERY_TIME: {
				B32 success = query_authorization();
				state = success ? ACME_STATE_WAIT_AUTHORIZATION_QUERY : ACME_STATE_FAILED;
			} break;
			case ACME_STATE_WAIT_AUTHORIZATION_QUERY: {
				B32 success = handle_query_authorization_response(httpReader.src + httpReader.contentOffset, httpReader.contentLength);
				if (success) {
					if (currentAuthorizationComplete) {
						numAuthorizations--;
						urlBufferAuthorizationOffset += strlen(urlBuffer + urlBufferAuthorizationOffset) + 1;
						if (urlBufferChallengeURLOffset >= 0) {
							// Reset bump allocator to before the last authorization
							urlBufferPos = urlBufferChallengeURLOffset;
							urlBufferChallengeURLOffset = -1;
							urlBufferChallengeTokenServableURLOffset = -1;
							urlBufferChallengeTokenOffset = -1;
							urlBufferChallengeTokenLength = 0;
							urlBufferCertDownloadOffset = -1;
							currentAuthorizationComplete = false;
							authorizationQueryAttemptsRemaining = 0;
						}
						if (numAuthorizations > 0) {
							success = get_http_authorization();
							state = success ? ACME_STATE_WAIT_GET_AUTHORIZATION : ACME_STATE_FAILED;
						} else {
							success = finalize_order();
							state = success ? ACME_STATE_WAIT_FINALIZE : ACME_STATE_FAILED;
						}
					} else if (current_time_millis() < authorizationQueryTimeoutTimeMilliseconds) {
						state = ACME_STATE_WAIT_NEXT_AUTHORIZATION_QUERY_TIME;
					} else {
						state = ACME_STATE_FAILED;
					}
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_FINALIZE: {
				B32 success = handle_order_finalize_response(httpReader.src + httpReader.contentOffset, httpReader.contentLength);
				if (success) {
					if (certIssued) {
						success = download_cert();
						state = success ? ACME_STATE_WAIT_CERT_DOWNLOAD : ACME_STATE_FAILED;
					} else {
						state = ACME_STATE_WAIT_CERT_ISSANCE_NEXT_QUERY_TIME;
					}
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_CERT_ISSANCE_NEXT_QUERY_TIME: {
				B32 success = query_cert_issued_status();
				state = success ? ACME_STATE_WAIT_CERT_ISSUANCE_QUERY : ACME_STATE_FAILED;
			} break;
			case ACME_STATE_WAIT_CERT_ISSUANCE_QUERY: {
				B32 success = handle_cert_issued_query_response(httpReader.src + httpReader.contentOffset, httpReader.contentLength);
				if (success) {
					if (certIssued) {
						success = download_cert();
						state = success ? ACME_STATE_WAIT_CERT_DOWNLOAD : ACME_STATE_FAILED;
					} else if (current_time_millis() < certificateProcessingTimeoutTimeMilliseconds) {
						state = ACME_STATE_WAIT_CERT_ISSANCE_NEXT_QUERY_TIME;
					} else {
						state = ACME_STATE_FAILED;
					}
				} else {
					state = ACME_STATE_FAILED;
				}
			} break;
			case ACME_STATE_WAIT_CERT_DOWNLOAD: {
				B32 success = handle_cert_download_response(httpReader.src + httpReader.contentOffset, httpReader.contentLength);
				state = success ? ACME_STATE_CERT_READY : ACME_STATE_FAILED;
			} break;
			}
			if (httpReader.readComplete) {
				// If the read was complete then it was also processed in the above switch
				// We're only doing one request at a time, so it's ok to clear the whole buffer
				client.discard_received_user_data();
				httpReader.init(reinterpret_cast<char*>(client.receiveBuffer) + client.receiveBufferUserReadPos, 0);
			}
		}
		client.send_data();
		if (client.state == TLS_CONNECTION_STATE_CLOSED && state != ACME_STATE_CERT_READY) {
			state = ACME_STATE_FAILED;
		}

		if (state == ACME_STATE_FAILED || state == ACME_STATE_CERT_READY) {
			client.error_alert(TLS_ALERT_CLOSE_NOTIFY);
			client.send_data();
			client.close_no_alert(); // Ideally the alert gets through, but don't wait for TCP if send buffer is full
			httpServer.close();
		}
	}

	B32 request_cert(X509Certificate* certOut, StrA certAuthorityServer, U32 certAuthorityServerIP, const char* directoryPathIn, U16 serverPort, U16 clientPort) {
		print("ACME Requesting cert\n");
		VERIFY_RETURN(certOut != nullptr, false);
		certificate = certOut;
		httpServer.open = false;
		state = ACME_STATE_IDLE;
		numHTTPFieldsToRead = 0;
		urlBufferPos = 0;
		urlBufferNewAccountOffset = -1;
		urlBufferNewNonceOffset = -1;
		urlBufferNewOrderOffset = -1;
		urlBufferCurrentOrderOffset = -1;
		urlBufferFinalizeOffset = -1;
		urlBufferAuthorizationOffset = -1;
		numAuthorizations = 0;
		urlBufferChallengeURLOffset = -1;
		urlBufferChallengeTokenServableURLOffset = -1;
		urlBufferChallengeTokenOffset = -1;
		urlBufferChallengeTokenLength = -1;
		urlBufferCertDownloadOffset = -1;
		certIssued = false;
		nonceLength = 0;
		currentAuthorizationComplete = false;
		directoryPath = directoryPathIn;
		client.close_no_alert();
		B32 connectionSuccess = certAuthorityServerIP != 0 ? client.connect_to_server(certAuthorityServer, certAuthorityServerIP, serverPort, clientPort) : client.connect_to_server(certAuthorityServer, serverPort, clientPort);
		if (!connectionSuccess) {
			state = ACME_STATE_FAILED;
			return false;
		}
		query_directory();
		httpReader.init(reinterpret_cast<char*>(client.receiveBuffer) + client.receiveBufferUserReadPos, 0);
		state = ACME_STATE_WAIT_DIRECTORY_QUERY;
		return true;
	}
};