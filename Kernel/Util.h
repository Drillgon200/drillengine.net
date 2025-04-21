#pragma once

#define UTIL_USE_INTRINSICS

#include "CLibDefs.h"

#ifdef _DEBUG
#define VERIFY_RETURN(cond, val) if (!(cond)) { __debugbreak(); return val; }
#define VERIFY_CLEANUP(cond, cleanupLabel) if(!(cond)) { __debugbreak(); goto cleanupLabel; }
#else
#define VERIFY_RETURN(cond, val) if (!(cond)) { return val; }
#define VERIFY_CLEANUP(cond, cleanupLabel) if(!(cond)) { goto cleanupLabel; }
#endif

typedef signed __int8 i8;
typedef unsigned __int8 u8;
typedef signed __int16 i16;
typedef unsigned __int16 u16;
typedef signed __int32 i32;
typedef unsigned __int32 u32;
typedef signed __int64 i64;
typedef unsigned __int64 u64;
typedef u64 uptr;
typedef u8 byte;
typedef float f32;
typedef double f64;
typedef u32 b32;
typedef u32 flags32;

#define U8_MAX 0xFF
#define U16_MAX 0xFFFF
#define U32_MAX 0xFFFFFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFFULL
#define I8_MAX 0x7F
#define I16_MAX 0x7FFF
#define I32_MAX 0x7FFFFFFF
#define I64_MAX 0x7FFFFFFFFFFFFFFFULL

#define F32_MAX 3.402823466e+38F
#define F32_MIN 1.175494351e-38F
#define F32_INFINITY ((f32)(1e+300 * 1e+300))
#define F32_NAN ((f32)(INFINITY * 0.0F))

#define FINLINE __forceinline

template<typename T>
FINLINE T max(T a, T b) {
	return a > b ? a : b;
}

template<typename T>
FINLINE T min(T a, T b) {
	return a < b ? a : b;
}

FINLINE bool is_whitespace(char c) {
	return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f';
}

FINLINE bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

FINLINE bool is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

FINLINE bool is_numeric(char c) {
	return c >= '0' && c <= '9';
}

FINLINE bool is_alphanumeric(char c) {
	return is_alpha(c) || is_numeric(c);
}

FINLINE bool is_hex_digit(char c) {
	return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

struct DataRange;

struct Range {
	u32 start;
	u32 end;

	DataRange data_range(const char* data);

	FINLINE u32 length() {
		return end - start;
	}

	FINLINE void reset() {
		start = 0;
		end = 0;
	}

	FINLINE bool operator==(const Range& other) const {
		return other.start == start && other.end == end;
	}

	FINLINE bool operator!=(const Range& other) const {
		return !(*this == other);
	}
};

struct DataRange {
	const char* data;
	u32 start;
	u32 end;

	FINLINE Range range() {
		return Range{ start, end };
	}

	FINLINE u32 length() {
		return end - start;
	}

	FINLINE bool operator==(const DataRange& other) const {
		if ((end - start) != (other.end - other.start)) {
			return false;
		}
		for (u32 pos = 0; pos < (end - start); pos++) {
			if (data[start + pos] != other.data[other.start + pos]) {
				return false;
			}
		}
		return true;
	}

	FINLINE bool operator!=(const DataRange& other) const {
		return !(*this == other);
	}

	FINLINE bool operator==(const char* str) const {
		u32 pos = 0;
		while (start + pos < end && str[pos] != '\0' && data[start + pos] == str[pos]) {
			pos++;
		}
		return str[pos] == '\0' && (start + pos) == end;
	}

	FINLINE bool operator!=(const char* str) const {
		return !(*this == str);
	}

	FINLINE u32 parse_uint(u32 maxVal = U32_MAX) {
		u64 val = 0;
		for (u32 i = start; i < end && is_digit(data[i]); i++) {
			val = val * 10 + data[i] - '0';
			if (val > maxVal) {
				return maxVal;
			}
		}
		return static_cast<u32>(val);
	}
};

FINLINE DataRange Range::data_range(const char* data) {
	return DataRange{ data, start, end };
}

void hex2data(void* out, u32 outSize, const char* c) {
	i32 i = outSize - 1;
	for (; i >= 0; i--) {
		if (c[0] == '\0' || c[1] == '\0') {
			break;
		}
		u8 quartet1 = c[0] > '9' ? (c[0] | 0b100000) - 'a' + 10 : c[0] - '0';
		u8 quartet2 = c[1] > '9' ? (c[1] | 0b100000) - 'a' + 10 : c[1] - '0';
		u8 octet = (quartet1 << 4) | quartet2;
		reinterpret_cast<u8*>(out)[i] = octet;
		c += 2;
	}
	i++;
	if (i != 0) {
		memmove(out, reinterpret_cast<u8*>(out) + i, outSize - i);
	}
}

FINLINE int popcnt(int n) {
#ifdef UTIL_USE_INTRINSICS
	return _mm_popcnt_u32(n);
#else
	// Sum bits
	n = ((n & 0xAAAAAAAA) >> 1) + (n & 0x55555555);
	n = ((n & 0xCCCCCCCC) >> 2) + (n & 0x33333333);
	n = ((n & 0xF0F0F0F0) >> 4) + (n & 0x0F0F0F0F);
	n = ((n & 0xFF00FF00) >> 8) + (n & 0x00FF00FF);
	n = ((n & 0xFFFF0000) >> 16) + (n & 0x0000FFFF);
	return n;
#endif
}

FINLINE u32 bswap32(u32 n) {
#ifdef UTIL_USE_INTRINSICS
	return _byteswap_ulong(n);
#else
	n = (n >> 16) | (n << 16);
	return ((n & 0xFF00FF00) >> 8) | ((n & 0x00FF00FF) << 8);
#endif
}

FINLINE u64 bswap64(u64 n) {
#ifdef UTIL_USE_INTRINSICS
	return _byteswap_uint64(n);
#else
	n = (n >> 32) | (n << 32);
	n = ((n & 0xFFFF0000FFFF0000ui64) >> 16) | ((n & 0x0000FFFF0000FFFFui64) << 16);
	return ((n & 0xFF00FF00FF00FF00ui64) >> 8) | ((n & 0x00FF00FF00FF00FFui64) << 8);
#endif
}

// Reverse the order of bits in the uint64
FINLINE u64 bitswap64(u64 n) {
	n = (n >> 32) | (n << 32);
	n = ((n & 0xFFFF0000FFFF0000ui64) >> 16) | ((n & 0x0000FFFF0000FFFFui64) << 16);
	n = ((n & 0xFF00FF00FF00FF00ui64) >> 8) | ((n & 0x00FF00FF00FF00FFui64) << 8);
	n = ((n & 0xF0F0F0F0F0F0F0F0ui64) >> 4) | ((n & 0x0F0F0F0F0F0F0F0Fui64) << 4);
	n = ((n & 0xCCCCCCCCCCCCCCCCui64) >> 2) | ((n & 0x3333333333333333ui64) << 2);
	return ((n & 0xAAAAAAAAAAAAAAAAui64) >> 1) | ((n & 0x5555555555555555ui64) << 1);
}

// Reverse the order of bits in each byte, but not the bytes themselves
FINLINE u64 bitswap_bytes64(u64 n) {
	n = ((n & 0xF0F0F0F0F0F0F0F0ui64) >> 4) | ((n & 0x0F0F0F0F0F0F0F0Fui64) << 4);
	n = ((n & 0xCCCCCCCCCCCCCCCCui64) >> 2) | ((n & 0x3333333333333333ui64) << 2);
	return ((n & 0xAAAAAAAAAAAAAAAAui64) >> 1) | ((n & 0x5555555555555555ui64) << 1);
}

FINLINE u32 load32_bigendian(const void* data) {
#ifdef UTIL_USE_INTRINSICS
	return _load_be_u32(data);
#else
	const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data);
	return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
#endif
}

FINLINE void store32_bigendian(void* out, u32 data) {
#ifdef UTIL_USE_INTRINSICS
	return _store_be_u32(out, data);
#else
	uint8_t* bytes = reinterpret_cast<uint8_t*>(out);
	bytes[0] = (data >> 24) & 0xFF;
	bytes[1] = (data >> 16) & 0xFF;
	bytes[2] = (data >> 8) & 0xFF;
	bytes[3] = data & 0xFF;
#endif
}

// Base64 URL safe encoding
// Section 5 of RFC 4648
FINLINE char base64url_encode_6bits(const u8 bits) {
	if (bits <= 25) {
		return 'A' + bits;
	} else if (bits <= 51) {
		return 'a' + (bits - 26);
	} else if (bits <= 61) {
		return '0' + (bits - 52);
	} else if (bits == 62) {
		return '-';
	} else { // bits == 63
		return '_';
	}
}

FINLINE u8 base64url_decode_6bits(const char charIn) {
	if (charIn == '_') {
		return 63;
	} else if (charIn == '-') {
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

FINLINE void base64url_encode_24bits(char* dataOut, const u8* dataIn) {
	dataOut[0] = base64url_encode_6bits(dataIn[0] >> 2);
	dataOut[1] = base64url_encode_6bits(((dataIn[0] & 0b11) << 4) | (dataIn[1] >> 4));
	dataOut[2] = base64url_encode_6bits(((dataIn[1] & 0b1111) << 2) | (dataIn[2] >> 6));
	dataOut[3] = base64url_encode_6bits(dataIn[2] & 0b111111);
}

FINLINE bool base64url_decode_24bits(u8* dataOut, const char* dataIn) {
	u8 bits[4];
	bits[0] = base64url_decode_6bits(dataIn[0]);
	bits[1] = base64url_decode_6bits(dataIn[1]);
	bits[2] = base64url_decode_6bits(dataIn[2]);
	bits[3] = base64url_decode_6bits(dataIn[3]);
	dataOut[0] = (bits[0] << 2) | (bits[1] >> 4);
	dataOut[1] = (bits[1] << 4) | (bits[2] >> 2);
	dataOut[2] = (bits[2] << 6) | bits[3];
	// If any of the 2 high bits are set, decode error
	return !((bits[0] | bits[1] | bits[2] | bits[3]) & 0b11000000);
}

FINLINE u32 base64url_encoded_size(u32 inSize) {
	return (inSize * 8 + 5) / 6;
}

u32 base64url_encode(void* vdataOut, u32 dataOutCap, const void* vdataIn, u32 dataInSize) {
	const u8* dataIn = reinterpret_cast<const u8*>(vdataIn);
	char* dataOut = reinterpret_cast<char*>(vdataOut);
	u32 dataOutSize = base64url_encoded_size(dataInSize);
	if (dataOutCap < dataOutSize) {
		return 0;
	}
	u32 inIdx = 0;
	u32 outIdx = 0;
	for (; (outIdx + 4) <= dataOutSize; inIdx += 3, outIdx += 4) {
		base64url_encode_24bits(&dataOut[outIdx], &dataIn[inIdx]);
	}
	if (outIdx != dataOutSize) {
		u8 extraIn[3]{};
		char extraOut[4]{};
		for (u32 i = 0; i < dataInSize - inIdx; i++) {
			extraIn[i] = dataIn[inIdx + i];
		}
		base64url_encode_24bits(extraOut, extraIn);
		for (u32 i = 0; i < dataOutSize - outIdx; i++) {
			dataOut[outIdx + i] = extraOut[i];
		}
	}
	return dataOutSize;
}

FINLINE u32 base64url_decoded_size(u32 inSize) {
	return (inSize * 6) / 8;
}

u32 base64url_decode(void* vdataOut, u32 dataOutCap, const void* vdataIn, u32 dataInSize) {
	const char* dataIn = reinterpret_cast<const char*>(vdataIn);
	u8* dataOut = reinterpret_cast<u8*>(vdataOut);
	u32 dataOutSize = base64url_decoded_size(dataInSize);
	if (dataOutCap < dataOutSize) {
		return 0;
	}
	u32 inIdx = 0;
	u32 outIdx = 0;
	for (; (outIdx + 3) <= dataOutSize; inIdx += 4, outIdx += 3) {
		VERIFY_RETURN(base64url_decode_24bits(&dataOut[outIdx], &dataIn[inIdx]), 0);
	}
	if (outIdx != dataOutSize) {
		char extraIn[4]{};
		u8 extraOut[3]{};
		for (u32 i = 0; i < dataInSize - inIdx; i++) {
			extraIn[i] = dataIn[inIdx + i];
		}
		VERIFY_RETURN(base64url_decode_24bits(extraOut, extraIn), 0);
		for (u32 i = 0; i < dataOutSize - outIdx; i++) {
			dataOut[outIdx + i] = extraOut[i];
		}
	}
	return dataOutSize;
}

FINLINE i32 ascii_base10_encode_u32(char* buffer, u32 bufferSize, u32 num) {
	char encodeBuf[32];
	char* ptr = &encodeBuf[32];
	if (!num) {
		*(--ptr) = '0';
	}
	while (num) {
		*(--ptr) = num % 10 + '0';
		num /= 10;
	}
	i32 result = -1;
	if (&encodeBuf[32] - ptr <= bufferSize) {
		result = 0;
		while (ptr != &encodeBuf[32]) {
			buffer[result++] = *(ptr++);
		}
	}
	return result;
}

template<typename T>
FINLINE T clamp(T val, T low, T high) {
	return min(high, max(low, val));
}

template<typename T>
FINLINE T clamp01(T val) {
	return clamp(val, static_cast<T>(0.0), static_cast<T>(1.0));
}

template<typename T>
FINLINE void swap2(T& a, T& b) {
	T c = a;
	a = b;
	b = c;
}