#pragma once

#include "PondOS.h"
#include "TLS.h"
#include "JSON.h"
#include "ASN1.h"
#include "CryptoSignature.h"
#include "HTTP.h"

// ACME implemention for getting a certificate from Let's Encrypt
// Implemented from RFC 8555

struct ACMEAccount {
	SECP256R1Key signingKey;
	char* accountURL;

	bool is_created() {
		return accountURL != nullptr;
	}
};

struct ACMEState {
	HTTPServer server;
	ACMEAccount account;
	TLSClient outgoingClient;
	u16 port;
	bool active;
};

const char* DRILL_ACME_USER_AGENT = "User-Agent: DrillACME/1.0";

inline bool acme_query_directory(char* acmeURLBuffer, const u32 acmeURLBufferSize, i32* newAccountOffset, i32* newNonceOffset, i32* newOrderOffset, TLSClient& client, const char* host) {
	client.write_str("GET /directory HTTP/1.1\r\n");
	client.write_str("Host: ").write_str(host).write_str("\r\n");
	client.write_str("Content-Type: application/jose+json\r\n");
	client.write_str(DRILL_ACME_USER_AGENT).write_str("\r\n");
	client.write_str("\r\n");
	client.send_data();

	char* responseData = nullptr;
	i32 length = http_read_full_content(client, &responseData);
	VERIFY_RETURN(length != HTTP_READ_FAILED, false);

	u32 currentAcmeURLOffset = 0;
	JSONReader json;
	json.init(responseData, length);
	json.begin_object();
	Range jsonFieldName;
	Range jsonFieldValue;
	while (json.read_field_name(&jsonFieldName)) {
		DataRange nameDataRange = jsonFieldName.data_range(json.data);
		if (nameDataRange == "newAccount" ||
			nameDataRange == "newNonce" ||
			nameDataRange == "newOrder") {
			bool isStringField = json.read_string_field(&jsonFieldValue);
			VERIFY_RETURN(isStringField, false);
			VERIFY_RETURN(currentAcmeURLOffset + jsonFieldValue.length() + 1 <= acmeURLBufferSize, false);

			if (nameDataRange == "newAccount") { VERIFY_RETURN(*newAccountOffset == -1, false); *newAccountOffset = currentAcmeURLOffset; } else if (nameDataRange == "newNonce") { VERIFY_RETURN(*newNonceOffset == -1, false);   *newNonceOffset = currentAcmeURLOffset; } else if (nameDataRange == "newOrder") { VERIFY_RETURN(*newOrderOffset == -1, false);   *newOrderOffset = currentAcmeURLOffset; }
			memcpy(acmeURLBuffer + currentAcmeURLOffset, json.data + jsonFieldValue.start, jsonFieldValue.length());
			currentAcmeURLOffset += jsonFieldValue.length();
			acmeURLBuffer[currentAcmeURLOffset++] = '\0';
		} else {
			json.skip_value();
		}
	}
	json.end_object();
	client.clear_receive_buffer();

	return !json.errored;
}

inline bool acme_request_nonce(char* nonce, u32* nonceMaxSizeAndOutLength, TLSClient& client, const char* host, const char* newNonceDirectory) {
	/* New syntax?
	client.head(newNonceDirectory)
		  .field("Host", host)
		  .field("User-Agent", DRILL_ACME_USER_AGENT)
		  .end_http_and_send_synchronous();
	*/
	client.write_str("HEAD ").write_str(newNonceDirectory).write_str(" HTTP/1.1\r\n");
	client.write_str("Host: ").write_str(host).write_str("\r\n");
	client.write_str(DRILL_ACME_USER_AGENT).write_str("\r\n");
	client.write_str("\r\n");
	client.send_data();

	HTTPField replayNonce{ "Replay-Nonce" };
	char* responseData = nullptr;
	i32 contentLength = http_read_full_content(client, &responseData, &replayNonce, 1);
	VERIFY_RETURN(contentLength != HTTP_READ_FAILED, false);
	VERIFY_RETURN(replayNonce.has_value() && replayNonce.value.length() <= *nonceMaxSizeAndOutLength, false);
	*nonceMaxSizeAndOutLength = replayNonce.value.length();
	memcpy(nonce, client.receiveBuffer + replayNonce.value.start, *nonceMaxSizeAndOutLength);
	client.clear_receive_buffer();
	return true;
}

inline void acme_query_account_status(ACMEAccount& account, TLSClient& client, char* nonce, u32* nonceLength, u32 nonceMaxLength) {
	if (!account.is_created()) {
		return;
	}
	//TODO

}

inline bool acme_request_new_account(ACMEAccount& account, TLSClient& client, char* nonce, u32* nonceLength, u32 nonceMaxLength, const char* newAccountURL, const char* host, const char* directory) {
	Keccak random; random.make_secure_random();
	// I really don't like the &account.signingKey.publicKey.x[0] thing, but I think it's alright since these structs have a pack pragma and I'm pretty sure C++ preserves struct member order
	secp256r1_generate_keypair(random, &account.signingKey.publicKey.x[0], account.signingKey.privateKey.n);

	const char* jsonAccountPayload =
		R"({
	"termsOfServiceAgreed": true,
	"contact": [
		"mailto:drillgon200@gmail.com"
	]			
})";

	/* Alt syntax
	u32 contentLengthPatchPos;
	client.post(directory)
		  .field("Host", host)
		  .field("Content-Type", "application/jose+json")
		  .field("User-Agent", DRILL_ACME_USER_AGENT)
		  .patchable_content_length(7, &contentLengthPatchPos)
		  .end_header();

	...

	client.patch_content_length(contentLengthpatchPos, encodedLength);
	client.end_http_and_send_synchronous();
	*/


	client.write_str("POST ").write_str(directory).write_str(" HTTP/1.1\r\n");
	client.write_str("Host: ").write_str(host).write_str("\r\n");
	client.write_str("Content-Type: application/jose+json\r\n");
	client.write_str(DRILL_ACME_USER_AGENT).write_str("\r\n");
	// Leave 7 spaces for backpatching length (don't really need that many, but just in case)
	u32 maxContentLength = 9999999;
	client.write_str("Content-Length:       \r\n");
	u32 contentLengthBackpatchPos = client.sendBufferPos - 3;
	client.write_str("\r\n");
	u32 encodedLength = 0;
	bool encodeSuccess = jws_encode_es256(&encodedLength, client.sendBuffer + client.sendBufferPos, client.sendBufferCap - client.sendBufferPos, jsonAccountPayload, strlen(jsonAccountPayload), nonce, *nonceLength, newAccountURL, nullptr, account.signingKey);
	VERIFY_RETURN(encodeSuccess && encodedLength <= maxContentLength, false);
	client.sendBufferPos += encodedLength;
	// Backpatch content length
	while (encodedLength != 0) {
		client.sendBuffer[contentLengthBackpatchPos--] = (encodedLength % 10) + '0';
		encodedLength /= 10;
	}
	client.send_data();

	HTTPField fields[2]{ HTTPField{ "Replay-Nonce" }, HTTPField{ "Location" } };
	HTTPField& newNonce = fields[0];
	HTTPField& location = fields[1];
	char* responseData = nullptr;
	i32 contentLength = http_read_full_content(client, &responseData, fields, 2);
	VERIFY_RETURN(contentLength >= 0, false);
	VERIFY_RETURN(newNonce.has_value() && location.has_value(), false);
	VERIFY_RETURN(newNonce.value.length() < nonceMaxLength, false);
	*nonceLength = newNonce.value.length();
	memcpy(nonce, client.receiveBuffer + newNonce.value.start, newNonce.value.length());

	bool accountValid = false;
	JSONReader json; json.init(responseData, contentLength);
	json.begin_object();
	Range jsonFieldName;
	Range jsonFieldValue;
	while (json.read_field_name(&jsonFieldName)) {
		if (jsonFieldName.data_range(json.data) == "status") {
			VERIFY_RETURN(!accountValid, false);
			bool isStringField = json.read_string_field(&jsonFieldValue);
			VERIFY_RETURN(isStringField, false);
			accountValid = jsonFieldValue.data_range(json.data) == "valid";
		} else {
			json.skip_value();
		}
	}
	json.end_object();
	VERIFY_RETURN(!json.errored, false);
	VERIFY_RETURN(accountValid, false);

	// If the account URL is greater than 512 characters, we're probably talking to a malicious server anyway
	constexpr u32 maxAccountURLSize = 512;
	VERIFY_RETURN(location.value.length() <= maxAccountURLSize, false);
	char* accountURL = reinterpret_cast<char*>(malloc(location.value.length() + 1));
	memcpy(accountURL, client.receiveBuffer + location.value.start, location.value.length());
	accountURL[location.value.length()] = '\0';
	client.clear_receive_buffer();
	free(account.accountURL);
	account.accountURL = accountURL;
	return true;
}

inline bool acme_apply_for_cert(char* authorizationBuf, u32* authorizationMaxSizeAndCount, char* finalizeURL, u32* finalizeURLMaxAndLength, char* nonce, u32* nonceLength, u32 nonceMaxLength, const char* newOrderURL, const char* host, const char* newOrderDirectory, TLSClient& client, ACMEAccount& account) {
	constexpr u32 jsonBufferSize = 1024;
	char jsonBuffer[jsonBufferSize];
	JSONWriter payload; payload.init(jsonBuffer, jsonBufferSize);
	payload.begin_object();
	payload.write_str("\"identifiers\":").begin_array();
	payload.begin_object().write_str("\"type\":\"dns\",\"value\":\"webtest.drillengine.net\"").end_object();
	payload.end_array();
	payload.end_object();
	VERIFY_RETURN(!payload.errored, false);

	client.write_str("POST ").write_str(newOrderDirectory).write_str(" HTTP/1.1\r\n");
	client.write_str("Host: ").write_str(host).write_str("\r\n");
	client.write_str("Content-Type: application/jose+json\r\n");
	client.write_str(DRILL_ACME_USER_AGENT).write_str("\r\n");
	// Leave 7 spaces for backpatching length (don't really need that many, but just in case)
	u32 maxContentLength = 9999999;
	client.write_str("Content-Length:       \r\n");
	u32 contentLengthBackpatchPos = client.sendBufferPos - 3;
	client.write_str("\r\n");
	u32 encodedLength = 0;
	bool encodeSuccess = jws_encode_es256(&encodedLength, client.sendBuffer + client.sendBufferPos, client.sendBufferCap - client.sendBufferPos, payload.data, payload.pos, nonce, *nonceLength, newOrderURL, account.accountURL, account.signingKey);
	VERIFY_RETURN(encodeSuccess && encodedLength <= maxContentLength, false);
	client.sendBufferPos += encodedLength;
	// Backpatch content length
	while (encodedLength != 0) {
		client.sendBuffer[contentLengthBackpatchPos--] = (encodedLength % 10) + '0';
		encodedLength /= 10;
	}
	client.send_data();

	HTTPField fields[2]{ HTTPField{ "Replay-Nonce" }, HTTPField{ "Location" } };
	HTTPField& newNonce = fields[0];
	HTTPField& location = fields[1];
	char* responseData = nullptr;
	i32 contentLength = http_read_full_content(client, &responseData, fields, 2);
	VERIFY_RETURN(contentLength >= 0, false);
	VERIFY_RETURN(newNonce.has_value() && location.has_value(), false);
	VERIFY_RETURN(newNonce.value.length() < nonceMaxLength, false);
	*nonceLength = newNonce.value.length();
	memcpy(nonce, client.receiveBuffer + newNonce.value.start, newNonce.value.length());

	u32 authorizationMaxSize = *authorizationMaxSizeAndCount;
	u32 authorizationBufferOffset = 0;
	u32 numAuthorizations = 0;
	JSONReader json; json.init(responseData, contentLength);
	bool hasAuthorizations = false;
	bool hasFinalize = false;
	json.begin_object();
	Range jsonFieldName;
	Range jsonFieldValue;
	while (json.read_field_name(&jsonFieldName)) {
		DataRange name = jsonFieldName.data_range(json.data);
		if (name == "status") {
			bool isStringField = json.read_string_field(&jsonFieldValue);
			VERIFY_RETURN(isStringField, false);
			VERIFY_RETURN(jsonFieldValue.data_range(json.data) == "pending" || jsonFieldValue.data_range(json.data) == "ready", false);
		} else if (name == "authorizations") {
			hasAuthorizations = true;
			json.begin_array();
			while (json.read_string_field(&jsonFieldValue)) {
				VERIFY_RETURN(authorizationBufferOffset + jsonFieldValue.length() + 1 <= authorizationMaxSize, false);
				memcpy(authorizationBuf + authorizationBufferOffset, json.data + jsonFieldValue.start, jsonFieldValue.length());
				authorizationBufferOffset += jsonFieldValue.length();
				authorizationBuf[authorizationBufferOffset++] = '\0';
				numAuthorizations++;
			}
			json.end_array();
		} else if (name == "finalize") {
			hasFinalize = true;
			bool isStringField = json.read_string_field(&jsonFieldValue);
			VERIFY_RETURN(isStringField, false);
			VERIFY_RETURN(jsonFieldValue.length() + 1 < *finalizeURLMaxAndLength, false);
			memcpy(finalizeURL, json.data + jsonFieldValue.start, jsonFieldValue.length());
			finalizeURL[jsonFieldValue.length()] = '\0';
			*finalizeURLMaxAndLength = jsonFieldValue.length();
		} else {
			json.skip_value();
		}
	}
	json.end_object();
	VERIFY_RETURN(!json.errored, false);
	VERIFY_RETURN(hasFinalize && hasAuthorizations, false);

	client.clear_receive_buffer();

	*authorizationMaxSizeAndCount = numAuthorizations;
	return true;
}

i64 current_time_millis() {
	return i64(g_syscallProc(SYSCALL_TIME, 0));
}

inline bool acme_complete_http_authoriazations(TLSClient& client, char* authorizations, u32 authorizationCount, char* nonce, u32* nonceLength, u32 nonceMaxLength, const char* host, ACMEAccount& account) {
	HTTPServer http; http.init();
	while (authorizationCount > 0) {
		HTTPURL url;
		u32 authorizationLength = strlen(authorizations);
		bool urlParseSuccess = url.parse(authorizations, authorizationLength);
		VERIFY_RETURN(urlParseSuccess, false);

		// Request authorization, get HTTP-01 challenge
		constexpr u32 challengeBufferSize = 1024;
		u32 challengeURLLength = 0;
		u32 challengeTokenLength = 0;
		char challengeURL[challengeBufferSize];
		char challengeToken[challengeBufferSize];

		client.write_str("POST ").write_str(authorizations + url.path.start).write_str(" HTTP/1.1\r\n");
		client.write_str("Host: ").write_str(host).write_str("\r\n");
		client.write_str("Content-Type: application/jose+json\r\n");
		client.write_str(DRILL_ACME_USER_AGENT).write_str("\r\n");
		// Leave 7 spaces for backpatching
		u32 maxContentLength = 9999999;
		client.write_str("Content-Length:       \r\n");
		u32 contentLengthBackpatchPos = client.sendBufferPos - 3;
		client.write_str("\r\n");
		u32 encodedLength = 0;
		// Post as get, encode nothing
		bool encodeSuccess = jws_encode_es256(&encodedLength, client.sendBuffer + client.sendBufferPos, client.sendBufferCap - client.sendBufferPos, "", 0, nonce, *nonceLength, authorizations, account.accountURL, account.signingKey);
		VERIFY_RETURN(encodeSuccess && encodedLength <= maxContentLength, false);
		client.sendBufferPos += encodedLength;
		// Backpatch content length
		while (encodedLength != 0) {
			client.sendBuffer[contentLengthBackpatchPos--] = (encodedLength % 10) + '0';
			encodedLength /= 10;
		}
		client.send_data();

		HTTPField fields[1]{ HTTPField{ "Replay-Nonce" } };
		HTTPField& newNonce = fields[0];
		char* responseData = nullptr;
		i32 contentLength = http_read_full_content(client, &responseData, fields, 1);
		VERIFY_RETURN(contentLength >= 0, false);
		VERIFY_RETURN(newNonce.has_value(), false);
		VERIFY_RETURN(newNonce.value.length() < nonceMaxLength, false);
		*nonceLength = newNonce.value.length();
		memcpy(nonce, client.receiveBuffer + newNonce.value.start, newNonce.value.length());
		JSONReader json; json.init(responseData, contentLength);
		json.begin_object();
		Range jsonFieldName;
		Range jsonFieldValue;
		while (json.read_field_name(&jsonFieldName)) {
			DataRange fieldName = jsonFieldName.data_range(json.data);
			if (fieldName == "challenges") {
				json.begin_array();
				bool hasHTTP01Challenge = false;
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
						VERIFY_RETURN(jsonURL.length() + 1 < challengeBufferSize && jsonToken.length() < challengeBufferSize, false)
							hasHTTP01Challenge = true;
						memcpy(challengeURL, json.data + jsonURL.start, jsonURL.length());
						challengeURL[jsonURL.length()] = '\0';
						challengeURLLength = jsonURL.length();
						memcpy(challengeToken, json.data + jsonToken.start, jsonToken.length());
						challengeToken[jsonToken.length()] = '\0';
						challengeTokenLength = jsonToken.length();
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

		urlParseSuccess = url.parse(challengeURL, challengeURLLength);
		VERIFY_RETURN(urlParseSuccess, false);
		constexpr u32 resourceBufferSize = 1024;
		// Resource URL takes the form /.well-known/acme-challenge/<token>
		char resourceURL[resourceBufferSize];
		const char* resourcePrefix = "/.well-known/acme-challenge/";
		const u32 resourcePrefixLength = strlen(resourcePrefix);
		VERIFY_RETURN(resourcePrefixLength + challengeTokenLength + 1 <= resourceBufferSize, false);
		memcpy(resourceURL, resourcePrefix, resourcePrefixLength);
		memcpy(resourceURL + resourcePrefixLength, challengeToken, challengeTokenLength);
		resourceURL[resourcePrefixLength + challengeTokenLength] = '\0';
		// Resource takes the form <token>.<base64 of jwk thumbprint of account public key>
		char resource[resourceBufferSize];
		memcpy(resource, challengeToken, challengeTokenLength);
		resource[challengeTokenLength] = '.';
		u32 thumbprintSize = resourceBufferSize - challengeTokenLength - 2;
		VERIFY_RETURN(compute_jwk_thumbprint_base64(resource + challengeTokenLength + 1, &thumbprintSize, account.signingKey.publicKey), false);
		resource[challengeTokenLength + 1 + thumbprintSize] = '\0';
		http.serve_resource(HTTPResource{ resourceURL, resource, challengeTokenLength + 1 + thumbprintSize });

		client.clear_receive_buffer();

		// Post an empty json to challengeURL to notify it that we're ready to complete the challenge
		client.write_str("POST ").write_str(challengeURL + url.path.start).write_str(" HTTP/1.1\r\n");
		client.write_str("Host: ").write_str(host).write_str("\r\n");
		client.write_str("Content-Type: application/jose+json\r\n");
		client.write_str(DRILL_ACME_USER_AGENT).write_str("\r\n");
		// Leave 7 spaces for backpatching
		client.write_str("Content-Length:       \r\n");
		contentLengthBackpatchPos = client.sendBufferPos - 3;
		client.write_str("\r\n");
		encodedLength = 0;
		encodeSuccess = jws_encode_es256(&encodedLength, client.sendBuffer + client.sendBufferPos, client.sendBufferCap - client.sendBufferPos, "{}", 2, nonce, *nonceLength, challengeURL, account.accountURL, account.signingKey);
		VERIFY_RETURN(encodeSuccess && encodedLength <= maxContentLength, false);
		client.sendBufferPos += encodedLength;
		// Backpatch content length
		while (encodedLength != 0) {
			client.sendBuffer[contentLengthBackpatchPos--] = (encodedLength % 10) + '0';
			encodedLength /= 10;
		}
		client.send_data();

		newNonce.reset();
		contentLength = http_read_full_content(client, &responseData, fields, 1);
		VERIFY_RETURN(contentLength >= 0, false);
		VERIFY_RETURN(newNonce.has_value(), false);
		VERIFY_RETURN(newNonce.value.length() < nonceMaxLength, false);
		*nonceLength = newNonce.value.length();
		memcpy(nonce, client.receiveBuffer + newNonce.value.start, newNonce.value.length());
		client.clear_receive_buffer();

		u32 maxQueryAttempts = 50;
		u32 queriesExecuted = 0;
		constexpr u32 timeBetweenAttemptsMilliseconds = 200;
		constexpr u32 serverTimeoutMilliseconds = 10000;
		i64 queryStartTime = current_time_millis();
		i64 lastCheckedTime = 0;
		bool authorized = false;
		while (http.open && queriesExecuted < maxQueryAttempts) {
			http.accept_new_connections();
			http.read_connection_data();
			http.take_requests();

			if ((current_time_millis() - lastCheckedTime) >= timeBetweenAttemptsMilliseconds) {
				lastCheckedTime = current_time_millis();
				queriesExecuted++;

				// Request authorization again, see if validated
				client.write_str("POST ").write_str(authorizations + url.path.start).write_str(" HTTP/1.1\r\n");
				client.write_str("Host: ").write_str(host).write_str("\r\n");
				client.write_str("Content-Type: application/jose+json\r\n");
				client.write_str(DRILL_ACME_USER_AGENT).write_str("\r\n");
				// Leave 7 spaces for backpatching
				u32 maxContentLength = 9999999;
				client.write_str("Content-Length:       \r\n");
				u32 contentLengthBackpatchPos = client.sendBufferPos - 3;
				client.write_str("\r\n");
				encodedLength = 0;
				// Post as get, encode nothing
				encodeSuccess = jws_encode_es256(&encodedLength, client.sendBuffer + client.sendBufferPos, client.sendBufferCap - client.sendBufferPos, "", 0, nonce, *nonceLength, authorizations, account.accountURL, account.signingKey);
				VERIFY_RETURN(encodeSuccess && encodedLength <= maxContentLength, false);
				client.sendBufferPos += encodedLength;
				// Backpatch content length
				while (encodedLength != 0) {
					client.sendBuffer[contentLengthBackpatchPos--] = (encodedLength % 10) + '0';
					encodedLength /= 10;
				}
				client.send_data();

				newNonce.reset();
				responseData = nullptr;
				contentLength = http_read_full_content(client, &responseData, fields, 1);
				VERIFY_RETURN(contentLength >= 0, false);
				VERIFY_RETURN(newNonce.has_value(), false);
				VERIFY_RETURN(newNonce.value.length() < nonceMaxLength, false);
				*nonceLength = newNonce.value.length();
				memcpy(nonce, client.receiveBuffer + newNonce.value.start, newNonce.value.length());
				JSONReader json; json.init(responseData, contentLength);

				json.begin_object();
				while (json.read_field_name(&jsonFieldName)) {
					DataRange fieldName = jsonFieldName.data_range(json.data);
					if (fieldName == "status") {
						VERIFY_RETURN(json.read_string_field(&jsonFieldValue), false);
						DataRange status = jsonFieldValue.data_range(json.data);
						if (status == "valid") {
							authorized = true;
						} else if (status == "invalid") {
							// Something went wrong
							return false;
						} else {
							// Not authorized yet, keep waiting
						}
					} else {
						json.skip_value();
					}
				}
				json.end_object();

				if (authorized) {
					break;
				}
			}
			client.clear_receive_buffer();
			if ((current_time_millis() - queryStartTime) >= serverTimeoutMilliseconds) {
				break;
			}
		}
		VERIFY_RETURN(authorized, false);
		http.clear_resources();
		http.close_connections();
		authorizations += authorizationLength + 1;
		authorizationCount--;
	}
	http.close();
	return true;
}

// Implemented from RFC 2986
inline bool acme_encode_certificate_signing_request(void* csr, u32* csrLength, BigInteger& rsaModulo, BigInteger& rsaPublicExponent, BigInteger& rsaPrivateExponent) {
	u32 csrMaxLength = *csrLength;
	ASN1Writer asn1{ csr, csrMaxLength };
	u32 certificationRequest = asn1.begin_sequence();
	u32 toBeSignedStart = asn1.pos;
	u32 certificationRequestInfo = asn1.begin_sequence();
	// version
	asn1.write_integer(0);

	constexpr u32 relativeDistinguishedNameCount = 3;
	const void* identifiers[relativeDistinguishedNameCount]{ ASN1_RAW_OID_COUNTRY_NAME, ASN1_RAW_OID_STATE_OR_PROVINCE_NAME, ASN1_RAW_OID_COMMON_NAME };
	const u32 identifierLengths[relativeDistinguishedNameCount]{ sizeof(ASN1_RAW_OID_COUNTRY_NAME), sizeof(ASN1_RAW_OID_STATE_OR_PROVINCE_NAME), sizeof(ASN1_RAW_OID_COMMON_NAME) };
	const char* names[relativeDistinguishedNameCount]{ "US", "California", "webtest.drillengine.net" };
	asn1.write_x501_name(names, identifiers, identifierLengths, relativeDistinguishedNameCount);

	u32 subjectPKInfo = asn1.begin_sequence();
	u32 algorithm = asn1.begin_sequence();
	asn1.write_oid(ASN1_OID_RSA_ENCRYPTION, sizeof(ASN1_OID_RSA_ENCRYPTION) / sizeof(u32));
	asn1.write_null();
	asn1.end_sequence(algorithm);

	u32 subjectPublicKey = asn1.write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_BIT_STRING, false);
	// No unused bits
	asn1.write_raw_byte(0);
	// RFC 3279, 2.3.1  RSA Keys
	u32 rsaPublicKey = asn1.begin_sequence();
	asn1.write_big_integer(rsaModulo);
	asn1.write_big_integer(rsaPublicExponent);
	asn1.end_sequence(rsaPublicKey);
	asn1.patch_length(subjectPublicKey);
	asn1.end_sequence(subjectPKInfo);

	u32 attributes = asn1.write_identifier_length(ASN1_CLASS_CONTEXT_SPECIFIC, 0, true);
	// No attributes
	asn1.patch_length(attributes);
	asn1.end_sequence(certificationRequestInfo);
	u32 toBeSignedEnd = asn1.pos;

	u32 algorithmIdentifier = asn1.begin_sequence();
	asn1.write_oid(ASN1_OID_RSA_PKCS1_SHA256, sizeof(ASN1_OID_RSA_PKCS1_SHA256) / sizeof(u32));
	asn1.write_null();
	asn1.end_sequence(algorithmIdentifier);

	u32 signature = asn1.write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_BIT_STRING, false);
	// No unused bits
	asn1.write_raw_byte(0);
	// sign it here
	u32 rsaBitLength = BigInteger::bitlength(rsaModulo);
	VERIFY_RETURN(asn1.pos + (rsaBitLength / 8) <= asn1.cap, false);
	rsassa_pkcs1_sign_sha256(asn1.data + asn1.pos, asn1.data + toBeSignedStart, toBeSignedEnd - toBeSignedStart, rsaPrivateExponent, rsaModulo, rsaBitLength);
	asn1.pos += rsaBitLength / 8;
	asn1.patch_length(signature);
	asn1.end_sequence(certificationRequest);
	*csrLength = asn1.pos;
	return !asn1.errored;
}

inline bool acme_finalize_order(X509Certificate& cert, char* certificateDownloadURL, u32* certificateDownloadURLSizeAndMaxSize, TLSClient& client, ACMEAccount& account, char* nonce, u32* nonceLength, u32 nonceMaxLength, const char* finalizeURL, const char* host, const char* finalizeDirectory) {
	constexpr u32 bufferSize = 1024;
	u32 csrLength = bufferSize;
	byte csrBuffer[bufferSize];
	rsa_keygen_2048(&cert.rsaPublicKey, &cert.rsaPrivateKey, &cert.rsaModulus);
	VERIFY_RETURN(acme_encode_certificate_signing_request(csrBuffer, &csrLength, cert.rsaModulus, cert.rsaPublicKey, cert.rsaPrivateKey), false);
	//print_bytes(csrBuffer, csrLength, "");
	char jsonBuffer[bufferSize];
	JSONWriter csrEncoder; csrEncoder.init(jsonBuffer, bufferSize);
	csrEncoder.begin_object();
	csrEncoder.write_str("\"csr\":\"").write_base64(csrBuffer, csrLength).write('"');
	csrEncoder.end_object();
	VERIFY_RETURN(!csrEncoder.errored, false);

	client.write_str("POST ").write_str(finalizeDirectory).write_str(" HTTP/1.1\r\n");
	client.write_str("Host: ").write_str(host).write_str("\r\n");
	client.write_str("Content-Type: application/jose+json\r\n");
	client.write_str(DRILL_ACME_USER_AGENT).write_str("\r\n");
	// Leave 7 spaces for backpatching
	u32 maxContentLength = 9999999;
	client.write_str("Content-Length:       \r\n");
	u32 contentLengthBackpatchPos = client.sendBufferPos - 3;
	client.write_str("\r\n");
	u32 encodedLength;
	bool encodeSuccess = encodeSuccess = jws_encode_es256(&encodedLength, client.sendBuffer + client.sendBufferPos, client.sendBufferCap - client.sendBufferPos, csrEncoder.data, csrEncoder.pos, nonce, *nonceLength, finalizeURL, account.accountURL, account.signingKey);
	VERIFY_RETURN(encodeSuccess && encodedLength <= maxContentLength, false);
	client.sendBufferPos += encodedLength;
	// Backpatch content length
	while (encodedLength != 0) {
		client.sendBuffer[contentLengthBackpatchPos--] = (encodedLength % 10) + '0';
		encodedLength /= 10;
	}
	client.send_data();

	HTTPField fields[1]{ HTTPField{ "Replay-Nonce" } };
	HTTPField& newNonce = fields[0];
	char* responseData = nullptr;
	i32 contentLength = http_read_full_content(client, &responseData, fields, 1);
	VERIFY_RETURN(contentLength >= 0, false);
	VERIFY_RETURN(newNonce.has_value(), false);
	VERIFY_RETURN(newNonce.value.length() < nonceMaxLength, false);
	*nonceLength = newNonce.value.length();
	memcpy(nonce, client.receiveBuffer + newNonce.value.start, newNonce.value.length());
	JSONReader json; json.init(responseData, contentLength);
	bool hasCertificateURL = false;
	json.begin_object();
	Range jsonFieldName;
	Range jsonFieldValue;
	while (json.read_field_name(&jsonFieldName)) {
		if (jsonFieldName.data_range(json.data) == "status") {
			VERIFY_RETURN(json.read_string_field(&jsonFieldValue), false);
			VERIFY_RETURN(jsonFieldValue.data_range(json.data) == "valid", false);
		} else if (jsonFieldName.data_range(json.data) == "certificate") {
			VERIFY_RETURN(json.read_string_field(&jsonFieldValue), false);
			VERIFY_RETURN(jsonFieldValue.length() + 1 < *certificateDownloadURLSizeAndMaxSize, false);
			memcpy(certificateDownloadURL, json.data + jsonFieldValue.start, jsonFieldValue.length());
			certificateDownloadURL[jsonFieldValue.length()] = '\0';
			*certificateDownloadURLSizeAndMaxSize = jsonFieldValue.length();
			hasCertificateURL = true;
		} else {
			json.skip_value();
		}
	}
	json.end_object();
	client.clear_receive_buffer();
	client.receive_data();
	VERIFY_RETURN(hasCertificateURL && !json.errored, false);
	return true;
}

inline bool acme_download_cert(X509Certificate& cert, TLSClient& client, ACMEAccount& account, char* nonce, u32* nonceLength, const char* certDownloadURL, const char* host, const char* directory) {
	client.write_str("POST ").write_str(directory).write_str(" HTTP/1.1\r\n");
	client.write_str("Host: ").write_str(host).write_str("\r\n");
	client.write_str("Content-Type: application/jose+json\r\n");
	client.write_str("Accept: application/pkcs7-mime\r\n");
	client.write_str(DRILL_ACME_USER_AGENT).write_str("\r\n");
	// Leave 7 spaces for backpatching
	u32 maxContentLength = 9999999;
	client.write_str("Content-Length:       \r\n");
	u32 contentLengthBackpatchPos = client.sendBufferPos - 3;
	client.write_str("\r\n");
	u32 encodedLength;
	bool encodeSuccess = encodeSuccess = jws_encode_es256(&encodedLength, client.sendBuffer + client.sendBufferPos, client.sendBufferCap - client.sendBufferPos, "", 0, nonce, *nonceLength, certDownloadURL, account.accountURL, account.signingKey);
	VERIFY_RETURN(encodeSuccess && encodedLength <= maxContentLength, false);
	client.sendBufferPos += encodedLength;
	// Backpatch content length
	while (encodedLength != 0) {
		client.sendBuffer[contentLengthBackpatchPos--] = (encodedLength % 10) + '0';
		encodedLength /= 10;
	}
	client.send_data();

	char* responseData = nullptr;
	i32 contentLength = http_read_full_content(client, &responseData);
	VERIFY_RETURN(contentLength >= 0 && contentLength <= X509Certificate::certMaxLength, false);
	memcpy(cert.cert, responseData, contentLength);
	cert.certLength = contentLength;
	client.clear_receive_buffer();
	return true;
}

inline bool acme_get_new_cert(X509Certificate& cert, const char* host, u16 localPort, ACMEAccount& account) {
	TLSClient acmeClient;
	acmeClient.connect(host, localPort);

	// Step 1: find all the urls for the requests we'll have to make
	constexpr u32 acmeURLBufferSize = 1024;
	char acmeURLBuffer[acmeURLBufferSize];
	i32 newAccountOffset = -1;
	i32 newNonceOffset = -1;
	i32 newOrderOffset = -1;
	HTTPURL urlParser;
	if (!acme_query_directory(acmeURLBuffer, acmeURLBufferSize, &newAccountOffset, &newNonceOffset, &newOrderOffset, acmeClient, host)) {
		return false;
	}

	// Step 2, request a nonce
	bool urlParseSuccess = urlParser.parse(acmeURLBuffer + newNonceOffset, strlen(acmeURLBuffer + newNonceOffset));
	VERIFY_RETURN(urlParseSuccess, false);
	VERIFY_RETURN(urlParser.hostName.data_range(acmeURLBuffer + newNonceOffset) == host, false);
	constexpr u32 nonceBufferCap = 512;
	u32 nonceLength = nonceBufferCap;
	char nonceBuffer[nonceBufferCap];
	if (!acme_request_nonce(nonceBuffer, &nonceLength, acmeClient, host, acmeURLBuffer + newNonceOffset + urlParser.path.start)) {
		return false;
	}

	// Step 3, create or reuse an account
	acme_query_account_status(account, acmeClient, nonceBuffer, &nonceLength, nonceBufferCap);
	if (!account.is_created()) {
		urlParseSuccess = urlParser.parse(acmeURLBuffer + newAccountOffset, strlen(acmeURLBuffer + newAccountOffset));
		VERIFY_RETURN(urlParseSuccess, false);
		if (!acme_request_new_account(account, acmeClient, nonceBuffer, &nonceLength, nonceBufferCap, acmeURLBuffer + newAccountOffset, host, acmeURLBuffer + newAccountOffset + urlParser.path.start)) {
			return false;
		}
	}

	/*std::cout << "Account URL: " << account.accountURL << '\n';
	std::cout << "Account signing private key: \n";
	BigInteger num{ 64 };
	BigInteger::assign_bytes_big_endian(num, account.signingKey.privateKey.n, 32);
	BigInteger::print_hex(num);
	std::cout << "Account signing public key: \n";
	BigInteger::assign_bytes_big_endian(num, account.signingKey.publicKey.x, 32);
	BigInteger::print_hex(num);
	BigInteger::assign_bytes_big_endian(num, account.signingKey.publicKey.y, 32);
	BigInteger::print_hex(num);*/

	// Step 4, apply for certificate issuance
	constexpr u32 maxAuthorizationData = 2048;
	u32 authorizationCount = maxAuthorizationData;
	char authorizations[maxAuthorizationData];
	constexpr u32 maxFinalizeURLSize = 512;
	u32 finalizeURLSize = maxFinalizeURLSize;
	char finalizeURL[maxFinalizeURLSize];
	VERIFY_RETURN(urlParser.parse(acmeURLBuffer + newOrderOffset, strlen(acmeURLBuffer + newOrderOffset)), false);
	if (!acme_apply_for_cert(authorizations, &authorizationCount, finalizeURL, &finalizeURLSize, nonceBuffer, &nonceLength, nonceBufferCap, acmeURLBuffer + newOrderOffset, host, acmeURLBuffer + newOrderOffset + urlParser.path.start, acmeClient, account)) {
		return false;
	}

	// Step 5, set up HTTP server and respond to requests
	if (!acme_complete_http_authoriazations(acmeClient, authorizations, authorizationCount, nonceBuffer, &nonceLength, nonceBufferCap, host, account)) {
		return false;
	}

	// Step 6, finalize order
	constexpr u32 certDownloadURLBufSize = 512;
	u32 certDownloadURLSize = certDownloadURLBufSize;
	char certDownloadURL[certDownloadURLBufSize];
	VERIFY_RETURN(urlParser.parse(finalizeURL, finalizeURLSize), false);
	if (!acme_finalize_order(cert, certDownloadURL, &certDownloadURLSize, acmeClient, account, nonceBuffer, &nonceLength, nonceBufferCap, finalizeURL, host, finalizeURL + urlParser.path.start)) {
		return false;
	}

	// Step 7, download cert
	VERIFY_RETURN(urlParser.parse(certDownloadURL, certDownloadURLSize), false);
	if (!acme_download_cert(cert, acmeClient, account, nonceBuffer, &nonceLength, certDownloadURL, host, certDownloadURL + urlParser.path.start)) {
		return false;
	}

	acmeClient.close();
	return true;
}