#pragma once

#include "BigInteger.h"

// iso, member-body, us, rsadsi, pkcs, pkcs-1, rsaEncryption
constexpr u32 ASN1_OID_RSA_ENCRYPTION[]{ 1, 2, 840, 113549, 1, 1, 1 };
// iso, member-body, us, rsadsi, pkcs, pkcs-1, sha256WithRSAEncryption
constexpr u32 ASN1_OID_RSA_PKCS1_SHA256[]{ 1, 2, 840, 113549, 1, 1, 11 };
// iso, member-body, us, rsadsi, pkcs, pkcs-1, rsassa-pss
constexpr u32 ASN1_OID_RSASSA_PSS[]{ 1, 2, 840, 113549, 1, 1, 10 };
// iso, member-body, us, ansi-x962, signatures, ecdsa-with-SHA2, ecdsa-with-SHA256
constexpr u32 ASN1_OID_ECDSA_SHA256[]{ 1, 2, 840, 10045, 4, 3, 2 };
// iso, member-body, us, ansi-x962, keyType, ecPublicKey
constexpr u32 ASN1_OID_EC_PUBLIC_KEY[]{ 1, 2, 840, 10045, 2, 1 };
// iso, member-body, us, ansi-x962, curves, prime, prime256v1 (secp256r1)
constexpr u32 ASN1_OID_CURVE_SECP256R1[]{ 1, 2, 840, 10045, 3, 1, 7 };

// joint-iso-itu-t, ds, attributeType, countryName
constexpr u8 ASN1_RAW_OID_COUNTRY_NAME[]{ 2 * 40 + 5, 4, 6 };
// joint-iso-itu-t, ds, attributeType, stateOrProvinceName
constexpr u8 ASN1_RAW_OID_STATE_OR_PROVINCE_NAME[]{ 2 * 40 + 5, 4, 8 };
// joint-iso-itu-t, ds, attributeType, commonName
constexpr u8 ASN1_RAW_OID_COMMON_NAME[]{ 2 * 40 + 5, 4, 3 };

enum ASN1UniversalIdentifier {
	ASN1_UNIVERSAL_IDENTIFIER_RESERVED = 0,
	ASN1_BOOLEAN = 1,
	ASN1_INTEGER = 2,
	ASN1_BIT_STRING = 3,
	ASN1_OCTET_STRING = 4,
	ASN1_NULL = 5,
	ASN1_OBJECT_IDENTIFIER = 6,
	ASN1_OBJECT_DESCRIPTOR = 7,
	ASN1_INSTANCE_OF_EXTERNAL = 8,
	ASN1_REAL = 9,
	ASN1_ENUMERATED = 10,
	ASN1_EMBEDDED_PDV = 11,
	ASN1_UTF8_STRING = 12,
	ASN1_RELATIVE_OID = 13,
	ASN1_SEQUENCE_SEQUENCE_OF = 16,
	ASN1_SET_SET_OF = 17,
	ASN1_NUMERIC_STRING = 18,
	ASN1_PRINTABLE_STRING = 19,
	ASN1_TELETEX_STRING = 20,
	ASN1_VIDEOTEX_STRING = 21,
	ASN1_IA5_STRING = 22,
	ASN1_UTC_TIME = 23,
	ASN1_GENERALIZED_TIME = 24,
	ASN1_GRAPHIC_STRING = 25,
	ASN1_VISIBLE_STRING = 26,
	ASN1_GENERAL_STRING = 27,
	ASN1_UNIVERSAL_STRING = 28,
	ASN1_CHARACTER_STRING = 29,
	ASN1_BMP_STRING = 30
};

enum ASN1ClassOfTag {
	ASN1_CLASS_UNIVERSAL = 0,
	ASN1_CLASS_APPLICATION = 1,
	ASN1_CLASS_CONTEXT_SPECIFIC = 2,
	ASN1_CLASS_PRIVATE = 3
};

// Implemented from ASN.1 spec
// https://www.itu.int/ITU-T/studygroups/com17/languages/X.690-0207.pdf

#define ASN1_VERIFY_ERROR_RETURN(cond, returnValue) if(!(cond)) { errored = true; __debugbreak(); return returnValue; }
#define ASN1_CHECK_ERROR_RETURN(returnValue) if(errored) { return returnValue; }

struct ASN1Reader {
	u8* data;
	u32 pos;
	u32 end;
	bool errored;

	ASN1Reader(void* vdata, u32 length) {
		data = reinterpret_cast<u8*>(vdata);
		pos = 0;
		end = length;
		errored = false;
	}

	u64 read_variable_length_integer(u32 maxLength) {
		ASN1_CHECK_ERROR_RETURN(0);
		if (maxLength == 0) {
			return 0;
		}
		ASN1_VERIFY_ERROR_RETURN(pos + maxLength <= end, 0);
		u64 value = 0;
		u8 currentByte;
		do {
			currentByte = data[pos++];
			maxLength--;
			value <<= 7;
			value |= currentByte & 0b01111111;
		} while (maxLength > 0 && currentByte & 0b10000000);
		return value;
	}

	ASN1ClassOfTag read_identifier(u32* identifier) {
		ASN1_CHECK_ERROR_RETURN(static_cast<ASN1ClassOfTag>(~0ui32));
		ASN1_VERIFY_ERROR_RETURN(pos + 1 <= end, static_cast<ASN1ClassOfTag>(~0ui32));

		u8 identifierByte = data[pos++];
		ASN1ClassOfTag classTag = static_cast<ASN1ClassOfTag>(identifierByte >> 6);
		u8 tagNumber = identifierByte & 0b00011111;
		if (tagNumber <= 30) {
			*identifier = tagNumber;
			return classTag;
		}
		bool constructed = (identifierByte >> 5) & 1;
		*identifier = read_variable_length_integer(sizeof(u32));
		return classTag;
	}

	bool is_next_identifier(ASN1ClassOfTag tagClassExpected, u32 identifierExpected) {
		ASN1_CHECK_ERROR_RETURN(0);

		u32 previousPos = pos;
		u32 identifier = 0;
		ASN1ClassOfTag tagClass = read_identifier(&identifier);
		pos = previousPos;
		return tagClassExpected == tagClass && identifierExpected == identifier;
	}

	u32 read_expected_identifier_length(ASN1ClassOfTag tagClassExpected, u32 identifierExpected) {
		ASN1_CHECK_ERROR_RETURN(0);

		u32 identifier = 0;
		ASN1ClassOfTag tagClass = read_identifier(&identifier);
		if (tagClassExpected == tagClass && identifierExpected == identifier) {
			return read_length();
		} else {
			ASN1_VERIFY_ERROR_RETURN(false, 0);
			return 0;
		}
	}

	u32 read_expected_identifier_end(ASN1ClassOfTag tagClassExpected, u32 identifierExpected) {
		u32 length = read_expected_identifier_length(tagClassExpected, identifierExpected);
		return pos + length;
	}

	u32 read_length() {
		ASN1_CHECK_ERROR_RETURN(0);
		ASN1_VERIFY_ERROR_RETURN(pos + 1 <= end, 0);

		u8 firstByte = data[pos++];
		bool longForm = firstByte & 0b10000000;
		if (longForm) {
			u8 numSubsequentOctets = firstByte & 0b01111111;
			ASN1_VERIFY_ERROR_RETURN(numSubsequentOctets < sizeof(u32), 0);
			ASN1_VERIFY_ERROR_RETURN(numSubsequentOctets >= 1, 0);
			ASN1_VERIFY_ERROR_RETURN(pos + numSubsequentOctets <= end, 0);
			u32 length = 0;
			for (u32 i = 0; i < numSubsequentOctets; i++) {
				length <<= 8;
				length |= data[pos++];
			}
			return length;
		} else {
			return firstByte & 0b01111111;
		}
	}

	bool read_bool() {
		ASN1_CHECK_ERROR_RETURN(false);
		ASN1_VERIFY_ERROR_RETURN(pos + 1 < end, 0);

		u8 boolByte = data[pos++];
		// DER encoding rule, booleans must be either all zeros or all ones
		ASN1_VERIFY_ERROR_RETURN(boolByte == 0 || boolByte == 0xFF, false);
		return boolByte;
	}

	i64 read_integer_no_smallest_number_of_bytes_check(u32 length) {
		ASN1_CHECK_ERROR_RETURN(0);
		ASN1_VERIFY_ERROR_RETURN(length >= 1 && length <= 9, 0);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, 0);

		bool negative = data[pos] >> 7;
		u64 result = 0;
		for (u32 i = 0; i < length; i++) {
			result <<= 8;
			result |= data[pos++];
		}
		if (negative && length < 8) {
			// Sign extend
			u64 signMask = (~0ui64) << (length * 8);
			result |= signMask;
		}
		return result;
	}

	i64 read_integer(u32 length) {
		ASN1_CHECK_ERROR_RETURN(0);
		ASN1_VERIFY_ERROR_RETURN(length >= 1 && length <= 9, 0);
		// Guarantee the integer is stored in the smallest number of bytes possible
		ASN1_VERIFY_ERROR_RETURN(data[pos] != 0, 0);
		// Guarantee the first 9 bits of the integer aren't all 1s (invalid form, would mean the first byte is not needed because of twos complement)
		if (length >= 2 && data[pos] == 0xFF) {
			ASN1_VERIFY_ERROR_RETURN((data[pos + 1] & 0b10000000) == 0, 0);
		}
		return read_integer_no_smallest_number_of_bytes_check(length);
	}

	void read_big_integer(BigInteger& out, u32 length) {
		ASN1_CHECK_ERROR_RETURN();
		// The length < out.cap * 8 is slightly wrong because a positive encoding 1 byte larger than the cap can have a zero byte in front to make it positive
		// I don't care enough to make it precise since the out variable should always have at least double the space anyway (for multiplications)
		ASN1_VERIFY_ERROR_RETURN(length >= 1 && length < out.cap * 8, );
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, );

		BigInteger::zero(out);
		BigInteger::try_expand_size(out, (length + 7) / 8);
		bool negative = data[pos] >> 7;
		for (i32 i = length - 1; i >= 0; i--) {
			u32 wordIdx = i >> 3;
			u32 byteIdx = i & 7;
			u64 byteData = negative ? ~data[pos++] : data[pos++];
			out.data[wordIdx] |= byteData << (byteIdx * 8);
		}
		if (negative) {
			BigInteger::add_single(out, out, 1);
		}
		out.negative = negative;
		BigInteger::find_new_size(out, out.size);
	}

	void read_big_integer_tag(BigInteger& out, u32 sectionEnd) {
		ASN1_CHECK_ERROR_RETURN();
		u32 length = read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_INTEGER);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= sectionEnd, );
		read_big_integer(out, length);
	}

	i64 read_integer_tag() {
		ASN1_CHECK_ERROR_RETURN(0);
		u32 length = read_expected_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_INTEGER);
		return read_integer(length);
	}

	u64 read_unsigned(u32 length) {
		ASN1_CHECK_ERROR_RETURN(0);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, 0);
		ASN1_VERIFY_ERROR_RETURN(length <= 8, 0);
		u64 result = 0;
		for (u32 i = 0; i < length; i++) {
			result <<= 8;
			result |= data[pos++];
		}
		return result;
	}

	// NR1-3 are from ISO 6093
	// https://cdn.standards.iteh.ai/samples/12285/039296509e8b40f3b25ba025de60365d/ISO-6093-1985.pdf

	// NR1 is an integer format, looks like 12394 or -0012321, with any number of leading spaces
	float decode_nr1(char* data, u32 length) {
		ASN1_CHECK_ERROR_RETURN(0.0F);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, 0.0F);

		while (length && data[0] == ' ') {
			data++, length--;
		}
		float sign = 1.0F;
		if (length && (data[0] == '-' || data[0] == '+')) {
			sign = data[0] == '-' ? -1.0F : 1.0F;
			data++, length--;
		}
		float val = 0.0F;
		// there must be at least one digit
		while (length && data[0] >= '0' && data[0] <= '9') {
			val = val * 10.0F + static_cast<float>(data[0] - '0');
			data++, length--;
		}
		// -0 isn't allowed, might want to check that
		return sign * val;
	}

	// NR2 is a decimal number format, looks like 123.456 or -,123 (comma also works as a decimal point), any number of leading spaces
	float decode_nr2(char* data, u32 length) {
		ASN1_CHECK_ERROR_RETURN(0.0F);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, 0.0F);

		while (length && data[0] == ' ') {
			data++, length--;
		}
		float sign = 1.0F;
		if (length && (data[0] == '-' || data[0] == '+')) {
			sign = data[0] == '-' ? -1.0F : 1.0F;
			data++, length--;
		}
		bool hasDigits = false;
		float val = 0.0F;
		while (length && data[0] >= '0' && data[0] <= '9') {
			val = val * 10.0F + static_cast<float>(data[0] - '0');
			data++, length--;
			hasDigits = true;
		}
		// verify length && (data[0] == ',' || data[0] == '.')
		data++, length--;
		float decimalPlaceScale = 0.1F;
		while (length && data[0] >= '0' && data[0] <= '9') {
			val += static_cast<float>(data[0] - '0') * decimalPlaceScale;
			decimalPlaceScale *= 0.1F;
			hasDigits = true;
		}
		// verify hasDigits
		return sign * val;
	}

	// NR3 is basically scientific notation, looks like 1.234e56 or -34,222E-123, any number of leading spaces
	float decode_nr3(char* data, u32 length) {
		ASN1_CHECK_ERROR_RETURN(0.0F);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, 0.0F);

		while (length && data[0] == ' ') {
			data++, length--;
		}
		float sign = 1.0F;
		if (length && (data[0] == '-' || data[0] == '+')) {
			sign = data[0] == '-' ? -1.0F : 1.0F;
			data++, length--;
		}
		float significand = decode_nr2(data, length);
		while (length && !(data[0] == 'e' || data[0] == 'E')) {
			data++, length--;
		}
		// verify length > 0
		data++, length--;
		u32 exponent = 0;
		bool hasDigits = false;
		while (length && data[0] >= '0' && data[0] <= '9') {
			exponent = exponent * 10 + static_cast<u32>(data[0] - '0');
			data++, length--;
			hasDigits = true;
		}
		if (exponent == 0) {
			return sign * significand;
		}
		// verify hasDigits
		float exponentMultiplier = 2.0F;
		for (i32 i = _lzcnt_u32(exponent) - 2; i >= 0; i--) {
			exponentMultiplier *= exponentMultiplier;
			u32 bit = (exponent >> i) & 1;
			if (bit) {
				exponentMultiplier *= 2.0F;
			}
		}
		return sign * significand * exponentMultiplier;
	}


	// Man, real numbers suck
	float read_real(u32 length) {
		ASN1_CHECK_ERROR_RETURN(0.0F);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, 0.0F);

		if (length == 0) {
			return 0.0F;
		}
		u32 realEnd = pos + length;
		// guarantee that length > 0
		u8 firstByte = data[pos++];
		float value;
		if (firstByte & 0b10000000) {
			// 0 is positive, 1 is negative
			u32 signBit = (firstByte >> 6) & 1;
			// 00 -> base 2, 01 -> base 8, 10 -> base 16, 11 -> reserved
			u8 base = (firstByte >> 4) & 0b11;
			// guarantee base isn't 11
			u32 scalingFactor = (firstByte >> 2) & 0b11;
			// 00 -> second octet is exponent, in two's complement
			// 01 -> second and third octets are exponent, in two's complement
			// 10 -> second and third and fourth octets are exponent, in two's complement
			// 11 -> second octet encodes number of octets, then the regular integer encoding is used
			u8 exponentFormat = firstByte & 0b11;
			i64 exponent;
			if (exponentFormat == 0b11) {
				ASN1_VERIFY_ERROR_RETURN(pos + 1 <= realEnd, 0.0F);
				u8 exponentLength = data[pos++];
				if (exponentLength + 4 > 9) {
					return signBit ? -F32_INFINITY : F32_INFINITY;
				}
				ASN1_VERIFY_ERROR_RETURN(pos + exponentLength + 4 <= realEnd, 0.0F);
				exponent = read_integer(exponentLength + 4);
			} else {
				ASN1_VERIFY_ERROR_RETURN(pos + exponentFormat + 1 <= realEnd, 0.0F);
				exponent = read_integer_no_smallest_number_of_bytes_check(exponentFormat + 1);
			}
			if (base != 0b00) {
				exponent *= static_cast<i64>(base * 8);
			}
			exponent += scalingFactor;
			ASN1_VERIFY_ERROR_RETURN(realEnd - pos >= 1, 0.0F);
			u64 mantissa = read_unsigned(realEnd - pos);
			// guarantee mantissa LSB is 1
			u32 mantissaLength = 64 - _lzcnt_u64(mantissa);
			// Make sure the bits of the mantissa that we need for an ieee float are in the bottom 23 bits
			// The 24th bit should be 1, it'll be masked out since it's implicit in an ieee float
			mantissa <<= max(0, 23 - static_cast<i32>(mantissaLength - 1));
			mantissa >>= max(0, static_cast<i32>(mantissaLength - 1) - 23);

			exponent += mantissaLength - 1;
			exponent += 127;
			// ieee floats have exponents between 1 and 254, with 0 and 255 being special
			if (exponent <= 0) {
				return 0.0F;
			} else if (exponent >= 255) {
				return signBit ? -F32_INFINITY : F32_INFINITY;
			}
			union {
				u32 floatBits;
				float floatValue;
			} encodedFloat;
			encodedFloat.floatBits = (signBit << 31) | (static_cast<u32>(exponent) << 23) | static_cast<u32>(mantissa & ((1 << 23) - 1));
			value = encodedFloat.floatValue;
		} else {
			if (firstByte & 0b01000000) {
				// 000001 -> ISO 6039 NR1 form
				// 000010 -> ISO 6039 NR2 form
				// 000011 -> ISO 6039 NR3 form
				// others -> reserved
				u8 numberRepresentation = firstByte & 0b00111111;
				if (numberRepresentation == 0b000001) {
					value = decode_nr1(reinterpret_cast<char*>(data) + pos, realEnd - pos);
				} else if (numberRepresentation == 0b000010) {
					value = decode_nr2(reinterpret_cast<char*>(data) + pos, realEnd - pos);
				} else if (numberRepresentation == 0b000011) {
					value = decode_nr3(reinterpret_cast<char*>(data) + pos, realEnd - pos);
				} else {
					value = 0.0F;
				}
				pos = realEnd;
			} else {
				// "special real values"
				ASN1_VERIFY_ERROR_RETURN(pos + 1 <= realEnd, 0.0F);
				u8 specialOctet = data[pos++];
				if (specialOctet == 0b01000000) {
					value = F32_INFINITY;
				} else if (specialOctet == 0b01000001) {
					value = -F32_INFINITY;
				} else {
					// error
					value = 0.0F;
				}
			}
		}
		ASN1_VERIFY_ERROR_RETURN(pos == realEnd, 0.0F);
		return value;
	}

	u8* read_bitstring(u32 length, u32* numBitsOut) {
		ASN1_CHECK_ERROR_RETURN(nullptr);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, nullptr);
		ASN1_VERIFY_ERROR_RETURN(length >= 1, nullptr);

		u8 numUnusedBitsInFinalOctet = data[pos++];
		// verify numUnusedBitsInFinalOctet <= 7
		// if length == 1, verify numUnusedBitsInFinalOctet == 0
		*numBitsOut = (length - 1) * 8 - numUnusedBitsInFinalOctet;
		u8* bitstring = &data[pos];
		pos += length - 1;
		return bitstring;
	}

	u8* read_octetstring(u32 length) {
		ASN1_CHECK_ERROR_RETURN(nullptr);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, nullptr);

		u8* octetstring = &data[pos];
		pos += length;
		return octetstring;
	}

	bool oid_match(const u32* oid, u32 oidLength, u32 length) {
		ASN1_CHECK_ERROR_RETURN(false);
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, false);
		ASN1_VERIFY_ERROR_RETURN(length >= 1, false);
		ASN1_VERIFY_ERROR_RETURN(oidLength >= 2, false);

		u32 oldPos = pos;
		u32 end = oldPos + length;
		u8 firstByte = data[pos++];
		u32 firstSubidentifier = firstByte / 40;
		u32 secondSubidentifier = firstByte % 40;
		if (oid[0] != firstSubidentifier || oid[1] != secondSubidentifier) {
			pos = oldPos;
			return false;
		}
		u32 i = 2;
		for (; i < oidLength && pos < end; i++) {
			u32 subidentifier = read_variable_length_integer(end - pos);
			if (oid[i] != subidentifier) {
				pos = oldPos;
				return false;
			}
		}
		if (i == oidLength && pos == end) {
			return true;
		} else {
			pos = oldPos;
			return false;
		}
	}

	void read_null_tag() {
		ASN1_CHECK_ERROR_RETURN();

		u32 tagId;
		ASN1ClassOfTag tagClass = read_identifier(&tagId);
		ASN1_VERIFY_ERROR_RETURN(tagClass == ASN1_CLASS_UNIVERSAL && tagId == ASN1_NULL, );
		u32 length = read_length();
		ASN1_VERIFY_ERROR_RETURN(length == 0, );
	}

	void skip_field() {
		ASN1_CHECK_ERROR_RETURN();

		u32 tagId;
		ASN1ClassOfTag tagClass = read_identifier(&tagId);
		u32 length = read_length();
		ASN1_VERIFY_ERROR_RETURN(pos + length <= end, );
		pos += length;
	}

	void verify_section_complete(u32 sectionEnd) {
		ASN1_VERIFY_ERROR_RETURN(pos == sectionEnd, );
	}
};


struct ASN1Writer {
	u8* data;
	u32 pos;
	u32 cap;
	bool errored;

	ASN1Writer(void* vdata, u32 length) {
		data = reinterpret_cast<u8*>(vdata);
		pos = 0;
		cap = length;
		errored = false;
	}

	void write_variable_length_integer(u32 number) {
		ASN1_CHECK_ERROR_RETURN();
		u32 bytesUsed = max(1ui32, ((32 - _lzcnt_u32(number)) + 6) / 7);
		ASN1_VERIFY_ERROR_RETURN(pos + bytesUsed <= cap, );
		// 7 bits per byte, the high bit being a flag to see if it's the end of the integer
		for (u32 i = 0; i < bytesUsed - 1; i++, pos++) {
			data[pos] = ((number >> ((bytesUsed - i - 1) * 7)) & 0xFF) | 0x80;
		}
		data[pos++] = number & 0b01111111;
	}

	void write_identifier(ASN1ClassOfTag classOfTag, u32 identifier, bool constructed) {
		ASN1_CHECK_ERROR_RETURN();
		ASN1_VERIFY_ERROR_RETURN(pos < cap, );
		u8 identifierByte = (static_cast<u8>(classOfTag) << 6) | (static_cast<u8>(constructed) << 5);
		if (identifier >= 0b00011111) {
			data[pos++] = identifierByte | 0b00011111;
			write_variable_length_integer(identifier);
		} else {
			data[pos++] = identifierByte | identifier;
		}
	}

	// Doesn't actually write any sort of length, just puts a dummy byte there for patch_length to use
	u32 write_length() {
		ASN1_CHECK_ERROR_RETURN(0);
		ASN1_VERIFY_ERROR_RETURN(pos < cap, 0);
		u32 oldPos = pos;
		data[pos++] = 0;
		return oldPos;
	}

	void patch_length(u32 oldPos) {
		ASN1_CHECK_ERROR_RETURN();
		u32 length = pos - (oldPos + 1);
		if (length <= 0b01111111) {
			// Lucky, fits in 7 bits
			data[oldPos] = length;
		} else {
			// Unlucky, have to use long form
			data[oldPos] |= 0b10000000;
			u32 bytesUsed = ((32 - _lzcnt_u32(length)) + 7) / 8;
			ASN1_VERIFY_ERROR_RETURN(pos + bytesUsed <= cap, );
			data[oldPos] |= bytesUsed;
			// Have to shift everything over now to make room for the variable length encoding
			memmove(data + oldPos + 1 + bytesUsed, data + oldPos + 1, length);
			for (u32 i = 0; i < bytesUsed; i++) {
				data[oldPos + 1 + i] = (length >> ((bytesUsed - i - 1) * 8)) & 0xFF;
			}
			pos += bytesUsed;
		}
	}

	u32 write_identifier_length(ASN1ClassOfTag classOfTag, u32 identifier, bool constructed) {
		write_identifier(classOfTag, identifier, constructed);
		return write_length();
	}

	u32 begin_sequence() {
		return write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_SEQUENCE_SEQUENCE_OF, true);
	}

	void end_sequence(u32 lengthPatchPos) {
		patch_length(lengthPatchPos);
	}

	u32 begin_set() {
		write_identifier(ASN1_CLASS_UNIVERSAL, ASN1_SET_SET_OF, true);
		return write_length();
	}

	void end_set(u32 lengthPatchPos) {
		patch_length(lengthPatchPos);
	}

	void write_integer(i32 val) {
		ASN1_CHECK_ERROR_RETURN();
		u32 lengthPatch = write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_INTEGER, false);
		char* twosComplimentBytes = reinterpret_cast<char*>(&val);
		i32 byteIdx = 3;
		// Skip bytes that would make the first 9 bits all 1s or zeros, because they are unnecessary in two's compliment encoding
		for (; byteIdx > 0; byteIdx--) {
			u32 first9Bits = (twosComplimentBytes[byteIdx] << 1) | (twosComplimentBytes[byteIdx - 1] >> 7);
			if (first9Bits != 0 && first9Bits != 0b111111111) {
				break;
			}
		}
		for (; byteIdx >= 0; byteIdx--) {
			ASN1_VERIFY_ERROR_RETURN(pos < cap, );
			data[pos++] = twosComplimentBytes[byteIdx];
		}
		patch_length(lengthPatch);
	}

	void write_big_integer(BigInteger& val) {
		ASN1_CHECK_ERROR_RETURN();
		u32 lengthPatch = write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_INTEGER, false);
		u32 bytesToWrite = (BigInteger::bitlength(val) + 7) / 8;
		ASN1_VERIFY_ERROR_RETURN(pos + bytesToWrite <= cap, );

		if (val.negative) {
			if (BigInteger::is_zero(val)) {
				val.negative = false;
			} else {
				// Conversion from sign bit to two's complement
				BigInteger::sub_single_impl(val, val, 1);
				BigInteger::invert_bits(val);
			}
		}

		// Since it's two's compliment, we have to make sure the first bit is set for negative values and isn't set for positive values
		i32 i = bytesToWrite - 1;
		u32 wordIdx = i >> 3;
		u32 byteIdx = i & 7;
		u8 firstByte = (val.data[wordIdx] >> (byteIdx * 8)) & 0xFF;
		if (val.negative && (firstByte & 0b10000000) != 0b10000000) {
			ASN1_VERIFY_ERROR_RETURN(pos + bytesToWrite + 1 <= cap, );
			data[pos++] = 0xFF;
		} else if (!val.negative && (firstByte & 0b10000000) == 0b10000000) {
			ASN1_VERIFY_ERROR_RETURN(pos + bytesToWrite + 1 <= cap, );
			data[pos++] = 0;
		}

		for (; i >= 0; i--) {
			wordIdx = i >> 3;
			byteIdx = i & 7;
			data[pos++] = (val.data[wordIdx] >> (byteIdx * 8)) & 0xFF;
		}

		if (val.negative) {
			BigInteger::invert_bits(val);
			BigInteger::add_single_impl(val, val, 1);
		}

		patch_length(lengthPatch);
	}

	void write_raw_bytes(const void* bytes, u32 length) {
		ASN1_CHECK_ERROR_RETURN();
		ASN1_VERIFY_ERROR_RETURN(pos + length <= cap, );
		memcpy(data + pos, bytes, length);
		pos += length;
	}

	void write_raw_byte(const u8 octet) {
		ASN1_CHECK_ERROR_RETURN();
		ASN1_VERIFY_ERROR_RETURN(pos < cap, );
		data[pos++] = octet;
	}

	void write_oid(const u32* oid, u32 numSubidentifiers) {
		ASN1_CHECK_ERROR_RETURN();
		ASN1_VERIFY_ERROR_RETURN(numSubidentifiers >= 2, );
		ASN1_VERIFY_ERROR_RETURN(pos < cap, );
		u32 lengthPatch = write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_OBJECT_IDENTIFIER, false);
		data[pos++] = oid[0] * 40 + oid[1];
		for (u32 i = 2; i < numSubidentifiers; i++) {
			write_variable_length_integer(oid[i]);
		}
		patch_length(lengthPatch);
	}

	void write_raw_oid(const void* oidData, u32 oidDataLength) {
		u32 lengthPatch = write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_OBJECT_IDENTIFIER, false);
		write_raw_bytes(oidData, oidDataLength);
		patch_length(lengthPatch);
	}

	void write_utf8_string(const char* str) {
		u32 lengthPatch = write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_UTF8_STRING, false);
		while (pos < cap && str[0] != '\0') {
			data[pos++] = str[0];
			str++;
		}
		ASN1_VERIFY_ERROR_RETURN(str[0] == '\0', );
		patch_length(lengthPatch);
	}

	void write_printable_string(const char* str) {
		// Same as write_utf8_string because the encoding differences don't matter for the ASCII strings I'm writing, but I still thought it was worth making them explicitly different
		u32 lengthPatch = write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_PRINTABLE_STRING, false);
		while (pos < cap && str[0] != '\0') {
			data[pos++] = str[0];
			str++;
		}
		ASN1_VERIFY_ERROR_RETURN(str[0] == '\0', );
		patch_length(lengthPatch);
	}

	void write_null() {
		u32 lengthPatch = write_identifier_length(ASN1_CLASS_UNIVERSAL, ASN1_NULL, false);
		patch_length(lengthPatch);
	}

	// Name = SEQUENCE OF RelativeDistinguishedName
	// RelativeDistinguishedName = SET OF AttributeTypeAndDistinguishedValue
	// AttributeTypeAndDistinguishedValue = SEQUENCE {
	//  type ATTRIBUTE.&id (an OID as far as I care)
	//  val ATTRIBUTE.&Type (a UTF8String as far as I care)
	//  <two optional values I don't care about>
	// }
	void write_x501_name(const char** names, const void** oidData, const u32* oidLengths, u32 numNames) {
		u32 relativeDistinguishedNameSequence = begin_sequence();
		for (u32 i = 0; i < numNames; i++) {
			u32 set = begin_set();
			u32 typeAndValueSequence = begin_sequence();
			write_raw_oid(oidData[i], oidLengths[i]);
			if (oidData[i] == ASN1_RAW_OID_COUNTRY_NAME) {
				// Writing countryName as a PrintableString is recommended in RFC 5280, and some programs won't accept it as anything else
				write_printable_string(names[i]);
			} else {
				write_utf8_string(names[i]);
			}
			end_sequence(typeAndValueSequence);
			end_set(set);
		}
		end_sequence(relativeDistinguishedNameSequence);
	}
};

#undef ASN1_VERIFY_ERROR_RETURN
#undef ASN1_CHECK_ERROR_RETURN