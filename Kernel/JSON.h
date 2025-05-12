#pragma once

#include "Util.h"
#include "CryptoSignature.h"

// I don't like json much, but that's what the ACME protocol uses, so I guess I'll cop
// Implemented from RFC 8259

struct JSONReader {
	const char* data;
	U32 cap;
	U32 pos;
	bool errored;

	U32 skipRecurseDepth;

	void init(const void* src, U32 length) {
		data = (const char*)src;
		cap = length;
		pos = 0;
		errored = false;
		skipRecurseDepth = 0;
	}

	void eat_whitespace() {
		VERIFY_RETURN(!errored,);
		while (pos < cap && is_whitespace(data[pos])) {
			pos++;
		}
	}

	bool read_token(char token) {
		VERIFY_RETURN(!errored, false);
		eat_whitespace();
		if (pos < cap && data[pos] == token) {
			pos++;
			return true;
		}
		return false;
	}

	void expect_token(char token) {
		VERIFY_RETURN(!errored,);
		if (!read_token(token)) {
			errored = true;
		}
	}

	bool peek_token(char token) {
		VERIFY_RETURN(!errored, false);
		eat_whitespace();
		return pos < cap && data[pos] == token;
	}

	void begin_object() {
		expect_token('{');
	}

	void end_object() {
		expect_token('}');
		read_token(',');
	}

	void begin_array() {
		expect_token('[');
	}

	void end_array() {
		expect_token(']');
		read_token(',');
	}

	bool read_string(StrA* str) {
		VERIFY_RETURN(!errored, false);
		U32 prevPos = pos;
		if (!read_token('"')) {
			return false;
		}
		if (str) {
			str->str = data + pos;
		}
		while (pos < cap && data[pos] != '"') {
			if ((pos + 1) < cap && data[pos] == '\\' && data[pos + 1] == '"') {
				pos++;
			}
			pos++;
		}
		if (pos >= cap || data[pos] != '"') {
			pos = prevPos;
			return false;
		}
		if (str) {
			str->length = data + pos - str->str;
		}
		pos++;
		return true;
	}

	bool read_field_name(StrA* fieldName) {
		VERIFY_RETURN(!errored, false);
		U32 prevPos = pos;
		if (!read_string(fieldName)) {
			return false;
		}
		if (!read_token(':')) {
			pos = prevPos;
			return false;
		}
		return true;
	}

	bool read_string_field(StrA* fieldValue) {
		VERIFY_RETURN(!errored, false);
		if (!read_string(fieldValue)) {
			return false;
		}
		read_token(',');
		return true;
	}
	StrA consume_string_field() {
		StrA s{};
		return read_string_field(&s) ? s : (errored = true, StrA{});
	}

	bool match_string(const char* str) {
		U32 prevPos = pos;
		while (pos < cap && str[0] != '\0' && str[0] == data[pos]) {
			str++, pos++;
		}
		if (str[0] != '\0') {
			pos = prevPos;
			return false;
		}
		return true;
	}

	bool skip_number() {
		VERIFY_RETURN(!errored, false);
		U32 oldPos = pos;
		// [ minus ] int [ frac ] [ exp ]

		eat_whitespace();

		// [ minus ]
		if (peek_token('-')) {
			pos++;
		}

		// int
		if (pos >= cap || !is_digit(data[pos])) {
			// Requires at least one digit
			pos = oldPos;
			return false;
		}
		if (data[pos] == '0') {
			// No leading zeros, except for one if the number is zero
			pos++;
		} else {
			pos++;
			while (pos < cap && is_digit(data[pos])) {
				pos++;
			}
		}

		// [ frac ]
		if (peek_token('.')) {
			pos++;
			if (pos >= cap || !is_digit(data[pos])) {
				// Requires at least one digit
				pos = oldPos;
				return false;
			}
			pos++;
			while (pos < cap && is_digit(data[pos])) {
				pos++;
			}
		}

		// [ exp ]
		if (peek_token('e') || peek_token('E')) {
			pos++;
			if (peek_token('+') || peek_token('-')) {
				pos++;
			}
			if (pos >= cap || !is_digit(data[pos])) {
				// Requires at least one digit
				pos = oldPos;
				return false;
			}
			pos++;
			while (pos < cap && is_digit(data[pos])) {
				pos++;
			}
		}
		return true;
	}

	void skip_object() {
		VERIFY_RETURN(!errored,);
		begin_object();
		while (read_field_name(nullptr)) {
			skip_value();
			VERIFY_RETURN(!errored,);
		}
		end_object();
	}

	void skip_array() {
		VERIFY_RETURN(!errored,);
		begin_array();
		while (true) {
			eat_whitespace();
			if (peek_token(']')) {
				break;
			}
			skip_value();
			VERIFY_RETURN(!errored,);
		}
		end_array();
	}

	void skip_value() {
		// Since this is a recursive parser (the recursive version looks nice), make some attempt at preventing stack overflow.
		constexpr U32 jsonRecurseMax = 128;
		if (skipRecurseDepth > jsonRecurseMax) {
			errored = true;
		}
		VERIFY_RETURN(!errored,);
		skipRecurseDepth++;
		eat_whitespace();
		// value = false / null / true / object / array / number / string
		if (match_string("true") || match_string("false") || match_string("null")) {
			// literal values
		} else if (peek_token('"') && read_string(nullptr)) {
			// string value
		} else if (peek_token('{')) {
			skip_object();
		} else if (peek_token('[')) {
			skip_array();
		} else if (skip_number()) {
			// number value
		} else {
			errored = true;
		}
		read_token(',');
		skipRecurseDepth--;
	}
};

struct JSONWriter {
	char* data;
	U32 cap;
	U32 pos;
	bool errored;

	void init(char* out, U32 length) {
		data = out;
		cap = length;
		pos = 0;
		errored = false;
	}

	JSONWriter& write(char c) {
		VERIFY_RETURN(!errored, *this);
		if (pos < cap) {
			data[pos++] = c;
		} else {
			errored = true;
		}
		return *this;
	}

	JSONWriter& write_str(const char* str) {
		VERIFY_RETURN(!errored, *this);
		while (pos < cap && str[0] != '\0') {
			data[pos++] = str[0];
			str++;
		}
		if (str[0] != '\0') {
			errored = true;
		}
		return *this;
	}

	JSONWriter& write_data(const void* src, U32 length) {
		VERIFY_RETURN(!errored, *this);
		if (pos + length <= cap) {
			memcpy(data + pos, src, length);
			pos += length;
		} else {
			errored = true;
		}
		return *this;
	}

	JSONWriter& write_base64(const void* src, U32 length) {
		VERIFY_RETURN(!errored, *this);
		U32 base64Length = base64url_encoded_size(length);
		if (pos + base64Length <= cap) {
			pos += base64url_encode(data + pos, cap - pos, src, length);
		} else {
			errored = true;
		}
		return *this;
	}

	JSONWriter& begin_object() {
		return write('{');
	}

	JSONWriter& end_object() {
		return write('}');
	}

	JSONWriter& begin_array() {
		return write('[');
	}

	JSONWriter& end_array() {
		return write(']');
	}
};

inline bool copy_string_to_buffer(char* out, I32 outCap, const char* str) {
	U32 length = strlen(str);
	if (length > outCap) {
		return false;
	}
	memcpy(out, str, length);
	return true;
}

inline void write_jwk_es256_object(JSONWriter& json, SECP256R1PublicKey& ecPublicKey) {
	json.begin_object();
	json.write_str("\"kty\":\"EC\",");
	json.write_str("\"crv\":\"P-256\",");
	json.write_str("\"x\":\"").write_base64(ecPublicKey.x, SECP256R1_KEY_SIZE).write_str("\",");
	json.write_str("\"y\":\"").write_base64(ecPublicKey.y, SECP256R1_KEY_SIZE).write_str("\",");
	json.write_str("\"use\":\"sig\"");
	json.end_object();
}

inline bool jws_encode_es256(U32* bytesWritten, char* output, U32 outputCap, const char* inputData, U32 inputDataLength, const char* nonce, U32 nonceLength, const char* requestURL, const char* accountURL, SECP256R1Key& ecKey) {
	constexpr U32 jsonBufferSize = 1024;
	char jsonBuffer[jsonBufferSize];
	JSONWriter json; json.init(jsonBuffer, jsonBufferSize);
	JSONWriter jws; jws.init(output, outputCap);
	SHA256 sha; sha.init();
	Keccak random; random.make_secure_random();
	// Next time I need to write a json object like this I might try writing a template json as a raw string literal and use something like find/replace to insert fields
	// It would be more readable and probably not noticeably slower in this case
	jws.begin_object();
	jws.write_str("\"protected\":\"");
	json.begin_object();
	json.write_str("\"alg\":\"ES256\",");
	if (accountURL != nullptr) {
		json.write_str("\"kid\":\"").write_str(accountURL).write_str("\",");
	} else {
		json.write_str("\"jwk\":");
		write_jwk_es256_object(json, ecKey.publicKey);
		json.write(',');
	}
	json.write_str("\"nonce\":\"").write_data(nonce, nonceLength).write_str("\",");
	json.write_str("\"url\":\"").write_str(requestURL).write_str("\"");
	json.end_object();
	VERIFY_RETURN(!json.errored, false);
	U32 prev = jws.pos;
	jws.write_base64(json.data, json.pos);
	sha.update(jws.data + prev, jws.pos - prev);
	sha.update(".", 1);

	jws.write_str("\",");

	jws.write_str("\"payload\":\"");
	prev = jws.pos;
	jws.write_base64(inputData, inputDataLength);
	sha.update(jws.data + prev, jws.pos - prev);
	jws.write_str("\",");

	Byte hash[SHA256_HASH_SIZE];
	sha.digest(hash);
	constexpr U32 signatureLength = ECDSA_SECP256R1_SIGNATURE_LENGTH;
	Byte signature[signatureLength];
	ecdsa_sign_secpr1_sha256(signature, hash, random, ecKey.privateKey.n);
	jws.write_str("\"signature\":\"").write_base64(signature, signatureLength).write('"');
	jws.end_object();

	*bytesWritten = jws.pos;
	return !jws.errored;
}

bool compute_jwk_thumbprint_base64(char* out, U32* outMaxAndSize, SECP256R1PublicKey& publicKey) {
	U32 outMax = *outMaxAndSize;
	VERIFY_RETURN(outMax >= base64url_encoded_size(SHA256_HASH_SIZE), false);
	// For SECP256r1, the json only takes a little over a hundred characters
	constexpr U32 intermediateBufferSize = 1024;
	char intermediateJSON[intermediateBufferSize];
	JSONWriter json; json.init(intermediateJSON, intermediateBufferSize);
	json.begin_object();
	// These fields must be in lexicographic order
	json.write_str("\"crv\":\"P-256\",");
	json.write_str("\"kty\":\"EC\",");
	json.write_str("\"x\":\"").write_base64(publicKey.x, SECP256R1_KEY_SIZE).write_str("\",");
	json.write_str("\"y\":\"").write_base64(publicKey.y, SECP256R1_KEY_SIZE).write_str("\"");
	json.end_object();
	VERIFY_RETURN(!json.errored, false);
	Byte hash[SHA256_HASH_SIZE];
	sha256(hash, json.data, json.pos);
	U32 encodedSize = base64url_encode(out, outMax, hash, SHA256_HASH_SIZE);
	VERIFY_RETURN(encodedSize != 0, false);
	*outMaxAndSize = encodedSize;
	return true;
}