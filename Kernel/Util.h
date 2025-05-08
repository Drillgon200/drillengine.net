#pragma once

#define UTIL_USE_INTRINSICS

#include "intrin.h"
#include "CLibDefs.h"

#define VERIFY_RETURN(cond, val) if (!(cond)) { return val; }
#define VERIFY_CLEANUP(cond, cleanupLabel) if(!(cond)) { goto cleanupLabel; }

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))
#define ALIGN_LOW(num, alignment) ((num) & ~((alignment) - 1))
#define ALIGN_HIGH(num, alignment) (((num) + (static_cast<decltype(num)>(alignment) - 1)) & ~(static_cast<decltype(num)>(alignment) - 1))
#define SIGN_EXTEND64(num, bits) (U64(I64(U64(num) << 64 - (bits)) >> 64 - (bits)))
#define SIGN_EXTEND32(num, bits) (U32(I32(U32(num) << 32 - (bits)) >> 32 - (bits)))
#define OFFSET_OF(type, member) __builtin_offsetof(type, member) //(reinterpret_cast<uptr>(&reinterpret_cast<type*>(uptr(0))->member))
#define MAKE_IP(a, b, c, d) (U32(a) << 0 | U32(b) << 8 | U32(c) << 16 | U32(d) << 24)

typedef signed __int8 I8;
typedef unsigned __int8 U8;
typedef signed __int16 I16;
typedef unsigned __int16 U16;
typedef signed __int32 I32;
typedef unsigned __int32 U32;
typedef signed __int64 I64;
typedef unsigned __int64 U64;
typedef U64 UPtr;
typedef U8 Byte;
typedef float F32;
typedef double F64;
typedef U32 B32;
typedef U8 B8;
typedef U32 Flags32;
typedef U16 Flags16;
typedef U8 Flags8;

#define U8_MAX 0xFF
#define U16_MAX 0xFFFF
#define U32_MAX 0xFFFFFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFFULL
#define I8_MAX 0x7F
#define I16_MAX 0x7FFF
#define I32_MAX 0x7FFFFFFF
#define I64_MAX 0x7FFFFFFFFFFFFFFFULL
#define I8_MIN I8(0x80)
#define I16_MIN I16(0x8000)
#define I32_MIN I32(0x80000000)
#define I64_MIN I64(0x8000000000000000LL)
#define F32_SMALL (__builtin_bit_cast(F32, 0x00800000u))
#define F32_LARGE (__builtin_bit_cast(F32, 0x7F7FFFFFu))
#define F32_INF (__builtin_bit_cast(F32, 0x7F800000u))
#define F32_QNAN (__builtin_bit_cast(F32, 0x7FFFFFFFu))
#define F32_SNAN (__builtin_bit_cast(F32, 0x7FBFFFFFu))
#define F64_SMALL (__builtin_bit_cast(F64, 0x0010000000000000ull))
#define F64_LARGE (__builtin_bit_cast(F64, 0x7FEFFFFFFFFFFFFFull))
#define F64_INF (__builtin_bit_cast(F64, 0x7FF0000000000000ull))
#define F64_QNAN (__builtin_bit_cast(F64, 0x7FFFFFFFFFFFFFFFull))
#define F64_SNAN (__builtin_bit_cast(F64, 0x7FF7FFFFFFFFFFFFull))
#define B32_TRUE 1
#define B32_FALSE 0
#define B8_TRUE 1
#define B8_FALSE 0

#define FINLINE __forceinline

extern "C" int _fltused = 0;

FINLINE U16 load_le16(void* ptr) {
	U16 result;
	memcpy(&result, ptr, sizeof(U16));
	return result;
}
FINLINE U32 load_le32(void* ptr) {
	U32 result;
	memcpy(&result, ptr, sizeof(U32));
	return result;
}
FINLINE U64 load_le64(void* ptr) {
	U64 result;
	memcpy(&result, ptr, sizeof(U64));
	return result;
}
FINLINE void store_le16(void* ptr, U16 val) {
	memcpy(ptr, &val, sizeof(U16));
}
FINLINE void store_le32(void* ptr, U32 val) {
	memcpy(ptr, &val, sizeof(U32));
}
FINLINE void store_le64(void* ptr, U64 val) {
	memcpy(ptr, &val, sizeof(U64));
}

#define LOAD_LE8(ptr) (*reinterpret_cast<U8*>(ptr))
#define LOAD_LE16(ptr) (load_le16((ptr)))
#define LOAD_LE32(ptr) (load_le32((ptr)))
#define LOAD_LE64(ptr) (load_le64((ptr)))
#define STORE_LE8(ptr, val) (*reinterpret_cast<U8*>(ptr) = (val))
#define STORE_LE16(ptr, val) (store_le16((ptr), (val)))
#define STORE_LE32(ptr, val) (store_le32((ptr), (val)))
#define STORE_LE64(ptr, val) (store_le64((ptr), (val)))
#define LOAD_BE8(ptr) (*reinterpret_cast<U8*>(ptr))
#define LOAD_BE16(ptr) (_load_be_u16((ptr)))
#define LOAD_BE32(ptr) (_load_be_u32((ptr)))
#define LOAD_BE64(ptr) (_load_be_u64((ptr)))
#define STORE_BE8(ptr, val) (*reinterpret_cast<U8*>(ptr) = (val))
#define STORE_BE16(ptr, val) (_store_be_u16((ptr), (val)))
#define STORE_BE32(ptr, val) (_store_be_u32((ptr), (val)))
#define STORE_BE64(ptr, val) (_store_be_u64((ptr), (val)))

template<typename To, typename From>
FINLINE constexpr To bitcast(const From& val) {
	return __builtin_bit_cast(To, val);
}

namespace SerializeTools {
void serialize_f64(char* dstBuffer, U32* dstBufferSize, F64 startValue);
bool is_whitespace(char c);
}

template<typename T>
FINLINE T max(T a, T b) {
	return a > b ? a : b;
}

template<typename T>
FINLINE T min(T a, T b) {
	return a < b ? a : b;
}

template<typename T>
FINLINE T abs(T val) {
	return val < T(0) ? -val : val;
}

template<typename T>
FINLINE void swap(T* a, T* b) {
	// All my structs are POD, no need to mess around with move semantics or anything
	T tmp = *b;
	*b = *a;
	*a = tmp;
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
	U32 start;
	U32 end;

	DataRange data_range(const char* data);

	FINLINE U32 length() {
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
	U32 start;
	U32 end;

	FINLINE Range range() {
		return Range{ start, end };
	}

	FINLINE U32 length() {
		return end - start;
	}

	FINLINE bool operator==(const DataRange& other) const {
		if ((end - start) != (other.end - other.start)) {
			return false;
		}
		for (U32 pos = 0; pos < (end - start); pos++) {
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
		U32 pos = 0;
		while (start + pos < end && str[pos] != '\0' && data[start + pos] == str[pos]) {
			pos++;
		}
		return str[pos] == '\0' && (start + pos) == end;
	}

	FINLINE bool operator!=(const char* str) const {
		return !(*this == str);
	}

	FINLINE U32 parse_uint(U32 maxVal = U32_MAX) {
		U64 val = 0;
		for (U32 i = start; i < end && is_digit(data[i]); i++) {
			val = val * 10 + data[i] - '0';
			if (val > maxVal) {
				return maxVal;
			}
		}
		return static_cast<U32>(val);
	}
};

FINLINE DataRange Range::data_range(const char* data) {
	return DataRange{ data, start, end };
}

// An ASCII string
struct StrA {
	const char* str;
	U64 length;

	FINLINE bool operator==(const StrA& other) const {
		return length == other.length && memcmp(str, other.str, length) == 0;
	}
	FINLINE bool operator!=(const StrA& other) const {
		return length != other.length || memcmp(str, other.str, length) != 0;
	}
	FINLINE char operator[](I64 pos) const {
		return str[pos < 0 ? length + pos : pos];
	}
	FINLINE StrA operator++(int) {
		StrA prev = *this;
		str++, length--;
		return prev;
	}
	FINLINE StrA& operator++() {
		str++, length--;
		return *this;
	}
	FINLINE bool is_empty() const {
		return length == 0;
	}
	FINLINE I64 find(StrA other) const {
		if (other.length > length) {
			return -1;
		}
		for (U64 i = 0; i <= length - other.length; i++) {
			if (memcmp(str + i, other.str, other.length) == 0) {
				return I64(i);
			}
		}
		return -1;
	}
	FINLINE I64 find(char c) const {
		for (U64 i = 0; i < length; i++) {
			if (str[i] == c) {
				return I64(i);
			}
		}
		return -1;
	}
	FINLINE bool contains(StrA other) {
		return find(other) != -1;
	}
	FINLINE bool contains(char c) {
		return find(c) != -1;
	}
	FINLINE I64 rfind(StrA other) const {
		if (other.length > length) {
			return -1;
		}
		for (I64 i = I64(length - other.length); i >= 0; i--) {
			if (memcmp(str + i, other.str, other.length) == 0) {
				return i;
			}
		}
		return -1;
	}
	FINLINE I64 rfind(char c) const {
		for (I64 i = I64(length - 1); i >= 0; i--) {
			if (str[i] == c) {
				return i;
			}
		}
		return -1;
	}
	FINLINE bool starts_with(StrA other) const {
		return other.length <= length && memcmp(str, other.str, other.length) == 0;
	}
	FINLINE bool ends_with(StrA other) const {
		return other.length <= length && memcmp(str + (length - other.length), other.str, other.length) == 0;
	}
	FINLINE StrA slice(I64 begin, I64 end) const {
		if (begin < 0) {
			begin = max(I64(length) + begin, 0LL);
		}
		if (end < 0) {
			end = max(I64(length) + end, 0LL);
		}
		if (end < begin) {
			return StrA{};
		}
		U64 first = min(U64(begin), length);
		U64 last = min(U64(end), length);
		return StrA{ str + first, U64(last - first) };
	}
	FINLINE StrA prefix(I64 amount) const {
		return slice(0, amount);
	}
	FINLINE StrA suffix(I64 amount) const {
		return slice(amount < 0 ? -amount : I64(length) - amount, I64_MAX);
	}
	FINLINE StrA skip(I64 amount) const {
		return slice(amount, I64_MAX);
	}
	FINLINE StrA substr(U64 begin, U64 newLength) const {
		U64 start = min(begin, length);
		return StrA{ str + start, min(newLength, length - start) };
	}
	FINLINE StrA strip() {
		StrA result = *this;
		while (result.length && SerializeTools::is_whitespace(*result.str)) {
			result.str++, result.length--;
		}
		while (result.length && SerializeTools::is_whitespace(result.str[result.length - 1])) {
			result.length--;
		}
		return result;
	}
	bool consume(StrA beginning) {
		if (starts_with(beginning)) {
			str += beginning.length;
			length -= beginning.length;
			return true;
		}
		return false;
	}
	const char* begin() const {
		return str;
	}
	const char* end() const {
		return str + length;
	}
	char front() const {
		return str[0];
	}
	char back() const {
		return str[length - 1];
	}
};

template<typename T>
struct Hasher;

template<>
struct Hasher<StrA> {
	FINLINE U32 operator()(StrA str) {
		// djb2
		// http://www.cse.yorku.ca/~oz/hash.html
		// It's unclear whether this is actually a good string hashing algorithm these days, but I don't care to research it right now.
		// Having at least a 3 cycle latency chain on each Byte can't be good for performance in any case
		U32 hash = 5381;
		for (U32 i = 0; i < str.length; i++) {
			hash = ((hash << 5) + hash) ^ str[i]; // hash * 33 + c
		}
		return hash;
	}
};

// Here's one of the C++ features I'll be using, should make string literals much nicer
// Technically user defined literal suffixes that don't start with an underscore are reserved, but I don't really care.
// If it becomes an issue, I'll change it then
FINLINE constexpr StrA operator""a(const char* lit, U64 len) {
	return StrA{ lit, len };
}

FINLINE U64 total_stralen(StrA str) {
	return str.length;
}
template<typename... Others>
FINLINE U64 total_stralen(StrA str, Others... others) {
	return str.length + total_stralen(others...);
}
FINLINE void copy_strings_to_buffer(char* out, StrA str) {
	memcpy(out, str.str, str.length);
}
template<typename... Others>
FINLINE void copy_strings_to_buffer(char* out, StrA str, Others... others) {
	memcpy(out, str.str, str.length);
	copy_strings_to_buffer(out + str.length, others...);
}

void hex2data(void* out, U32 outSize, const char* c) {
	I32 i = outSize - 1;
	for (; i >= 0; i--) {
		if (c[0] == '\0' || c[1] == '\0') {
			break;
		}
		U8 quartet1 = c[0] > '9' ? (c[0] | 0b100000) - 'a' + 10 : c[0] - '0';
		U8 quartet2 = c[1] > '9' ? (c[1] | 0b100000) - 'a' + 10 : c[1] - '0';
		U8 octet = (quartet1 << 4) | quartet2;
		reinterpret_cast<U8*>(out)[i] = octet;
		c += 2;
	}
	i++;
	if (i != 0) {
		memmove(out, reinterpret_cast<U8*>(out) + i, outSize - i);
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

FINLINE U16 bswap16(U16 n) {
#ifdef UTIL_USE_INTRINSICS
	return _byteswap_ushort(n);
#else
	return (n << 8) | (n >> 8);
#endif
}

FINLINE U32 bswap32(U32 n) {
#ifdef UTIL_USE_INTRINSICS
	return _byteswap_ulong(n);
#else
	n = (n >> 16) | (n << 16);
	return ((n & 0xFF00FF00) >> 8) | ((n & 0x00FF00FF) << 8);
#endif
}

FINLINE U64 bswap64(U64 n) {
#ifdef UTIL_USE_INTRINSICS
	return _byteswap_uint64(n);
#else
	n = (n >> 32) | (n << 32);
	n = ((n & 0xFFFF0000FFFF0000ui64) >> 16) | ((n & 0x0000FFFF0000FFFFui64) << 16);
	return ((n & 0xFF00FF00FF00FF00ui64) >> 8) | ((n & 0x00FF00FF00FF00FFui64) << 8);
#endif
}

// Reverse the order of bits in the uint64
FINLINE U64 bitswap64(U64 n) {
	n = (n >> 32) | (n << 32);
	n = ((n & 0xFFFF0000FFFF0000ui64) >> 16) | ((n & 0x0000FFFF0000FFFFui64) << 16);
	n = ((n & 0xFF00FF00FF00FF00ui64) >> 8) | ((n & 0x00FF00FF00FF00FFui64) << 8);
	n = ((n & 0xF0F0F0F0F0F0F0F0ui64) >> 4) | ((n & 0x0F0F0F0F0F0F0F0Fui64) << 4);
	n = ((n & 0xCCCCCCCCCCCCCCCCui64) >> 2) | ((n & 0x3333333333333333ui64) << 2);
	return ((n & 0xAAAAAAAAAAAAAAAAui64) >> 1) | ((n & 0x5555555555555555ui64) << 1);
}

// Reverse the order of bits in each Byte, but not the Bytes themselves
FINLINE U64 bitswap_bytes64(U64 n) {
	n = ((n & 0xF0F0F0F0F0F0F0F0ui64) >> 4) | ((n & 0x0F0F0F0F0F0F0F0Fui64) << 4);
	n = ((n & 0xCCCCCCCCCCCCCCCCui64) >> 2) | ((n & 0x3333333333333333ui64) << 2);
	return ((n & 0xAAAAAAAAAAAAAAAAui64) >> 1) | ((n & 0x5555555555555555ui64) << 1);
}

// Base64 URL safe encoding
// Section 5 of RFC 4648
FINLINE char base64url_encode_6bits(const U8 bits) {
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

FINLINE U8 base64url_decode_6bits(const char charIn) {
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

FINLINE void base64url_encode_24bits(char* dataOut, const U8* dataIn) {
	dataOut[0] = base64url_encode_6bits(dataIn[0] >> 2);
	dataOut[1] = base64url_encode_6bits(((dataIn[0] & 0b11) << 4) | (dataIn[1] >> 4));
	dataOut[2] = base64url_encode_6bits(((dataIn[1] & 0b1111) << 2) | (dataIn[2] >> 6));
	dataOut[3] = base64url_encode_6bits(dataIn[2] & 0b111111);
}

FINLINE bool base64url_decode_24bits(U8* dataOut, const char* dataIn) {
	U8 bits[4];
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

FINLINE U32 base64url_encoded_size(U32 inSize) {
	return (inSize * 8 + 5) / 6;
}

U32 base64url_encode(void* vdataOut, U32 dataOutCap, const void* vdataIn, U32 dataInSize) {
	const U8* dataIn = reinterpret_cast<const U8*>(vdataIn);
	char* dataOut = reinterpret_cast<char*>(vdataOut);
	U32 dataOutSize = base64url_encoded_size(dataInSize);
	if (dataOutCap < dataOutSize) {
		return 0;
	}
	U32 inIdx = 0;
	U32 outIdx = 0;
	for (; (outIdx + 4) <= dataOutSize; inIdx += 3, outIdx += 4) {
		base64url_encode_24bits(&dataOut[outIdx], &dataIn[inIdx]);
	}
	if (outIdx != dataOutSize) {
		U8 extraIn[3]{};
		char extraOut[4]{};
		for (U32 i = 0; i < dataInSize - inIdx; i++) {
			extraIn[i] = dataIn[inIdx + i];
		}
		base64url_encode_24bits(extraOut, extraIn);
		for (U32 i = 0; i < dataOutSize - outIdx; i++) {
			dataOut[outIdx + i] = extraOut[i];
		}
	}
	return dataOutSize;
}

FINLINE U32 base64url_decoded_size(U32 inSize) {
	return (inSize * 6) / 8;
}

U32 base64url_decode(void* vdataOut, U32 dataOutCap, const void* vdataIn, U32 dataInSize) {
	const char* dataIn = reinterpret_cast<const char*>(vdataIn);
	U8* dataOut = reinterpret_cast<U8*>(vdataOut);
	U32 dataOutSize = base64url_decoded_size(dataInSize);
	if (dataOutCap < dataOutSize) {
		return 0;
	}
	U32 inIdx = 0;
	U32 outIdx = 0;
	for (; (outIdx + 3) <= dataOutSize; inIdx += 4, outIdx += 3) {
		VERIFY_RETURN(base64url_decode_24bits(&dataOut[outIdx], &dataIn[inIdx]), 0);
	}
	if (outIdx != dataOutSize) {
		char extraIn[4]{};
		U8 extraOut[3]{};
		for (U32 i = 0; i < dataInSize - inIdx; i++) {
			extraIn[i] = dataIn[inIdx + i];
		}
		VERIFY_RETURN(base64url_decode_24bits(extraOut, extraIn), 0);
		for (U32 i = 0; i < dataOutSize - outIdx; i++) {
			dataOut[outIdx + i] = extraOut[i];
		}
	}
	return dataOutSize;
}

I32 ascii_base10_encode_u32(char* buffer, U32 bufferSize, U32 num) {
	char encodeBuf[32];
	char* ptr = &encodeBuf[32];
	if (!num) {
		*(--ptr) = '0';
	}
	while (num) {
		*(--ptr) = num % 10 + '0';
		num /= 10;
	}
	I32 result = -1;
	if (&encodeBuf[32] - ptr <= bufferSize) {
		result = 0;
		while (ptr != &encodeBuf[32]) {
			buffer[result++] = *(ptr++);
		}
	}
	return result;
}

I32 ascii_base10_encode_u64(char* buffer, U32 bufferSize, U64 num) {
	char encodeBuf[64];
	char* ptr = &encodeBuf[64];
	if (!num) {
		*(--ptr) = '0';
	}
	while (num) {
		*(--ptr) = num % 10 + '0';
		num /= 10;
	}
	I32 result = -1;
	if (&encodeBuf[64] - ptr <= bufferSize) {
		result = 0;
		while (ptr != &encodeBuf[64]) {
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

struct BigEndianByteBuf {
	Byte* bytes;
	U32 offset;
	U32 capacity;
	B32 failed;

	void wrap(void* buffer, U32 size) {
		bytes = reinterpret_cast<Byte*>(buffer);
		offset = 0;
		capacity = size;
		failed = false;
	}

	B32 has_data_left(U32 size) {
		return I64(capacity) - I64(offset) >= I64(size);
	}

	void skip_bytes(U32 length) {
		if (capacity - offset < length) {
			failed = true;
		} else {
			offset += length;
		}
	}

	U8 read_u8() {
		U8 result = 0;
		if (capacity - offset < sizeof(U8)) {
			failed = true;
		} else {
			result = bytes[offset];
			offset += sizeof(U8);
		}
		return result;
	}

	U16 read_u16() {
		U16 result = 0;
		if (capacity - offset < sizeof(U16)) {
			failed = true;
		} else {
			result = LOAD_BE16(bytes + offset);
			offset += sizeof(U16);
		}
		return result;
	}

	U32 read_u24() {
		U32 result = 0;
		if (capacity - offset < 3) {
			failed = true;
		} else {
			result =
				(U32(bytes[offset + 0]) << 16) |
				(U32(bytes[offset + 1]) << 8) |
				(U32(bytes[offset + 2]) << 0);
			offset += 3;
		}
		return result;
	}

	U32 read_u32() {
		U32 result = 0;
		if (capacity - offset < sizeof(U32)) {
			failed = true;
		} else {
			result = LOAD_BE32(bytes + offset);
			offset += sizeof(U32);
		}
		return result;
	}

	U64 read_u64() {
		U64 result = 0;
		if (capacity - offset < sizeof(U32)) {
			failed = true;
		} else {
			result = LOAD_BE64(bytes + offset);
			offset += sizeof(U64);
		}
		return result;
	}

	F32 read_f32() {
		U32 floatAsInt = read_u32();
		F32 result = 0.0F;
		memcpy(&result, &floatAsInt, sizeof(F32));
		return result;
	}

	void read_bytes(void* dst, U32 length) {
		if (capacity - offset < length) {
			failed = true;
		} else {
			memcpy(dst, bytes + offset, length);
			offset += length;
		}
	}

	U32 write_u8(U8 v) {
		U32 oldOffset = offset;
		if (capacity - offset < sizeof(U8)) {
			failed = true;
		} else {
			bytes[offset++] = v;
		}
		return oldOffset;
	}

	U32 write_u16(U16 v) {
		U32 oldOffset = offset;
		if (capacity - offset < sizeof(U16)) {
			failed = true;
		} else {
			STORE_BE16(bytes + offset, v);
			offset += sizeof(U16);
		}
		return oldOffset;
	}

	U32 write_u24(U32 v) {
		U32 oldOffset = offset;
		if (capacity - offset < 3) {
			failed = true;
		} else {
			bytes[offset++] = (v >> 16) & 0xFF;
			bytes[offset++] = (v >> 8) & 0xFF;
			bytes[offset++] = v & 0xFF;
		}
		return oldOffset;
	}

	U32 write_u32(U32 v) {
		U32 oldOffset = offset;
		if (capacity - offset < sizeof(U32)) {
			failed = true;
		} else {
			STORE_BE32(bytes + offset, v);
			offset += sizeof(U32);
		}
		return oldOffset;
	}

	U32 write_u64(U64 v) {
		U32 oldOffset = offset;
		if (capacity - offset < sizeof(U64)) {
			failed = true;
		} else {
			STORE_BE64(bytes + offset, v);
			offset += sizeof(U64);
		}
		return oldOffset;
	}

	U32 write_f32(F32 v) {
		U32 floatAsInt;
		memcpy(&floatAsInt, &v, sizeof(F32));
		return write_u32(floatAsInt);
	}

	U32 write_bytes(const void* src, U32 length) {
		U32 oldOffset = offset;
		if (capacity - offset < length) {
			failed = true;
		} else {
			memcpy(bytes + offset, src, length);
			offset += length;
		}
		return oldOffset;
	}

	void patch_u8(U32 pos, U8 v) {
		if (capacity - pos < sizeof(U8)) {
			failed = true;
		} else {
			bytes[pos] = v;
		}
	}

	void patch_u16(U32 pos, U16 v) {
		if (capacity - pos < sizeof(U16)) {
			failed = true;
		} else {
			STORE_BE16(bytes + pos, v);
		}
	}

	void patch_u24(U32 pos, U32 v) {
		if (capacity - pos < 3) {
			failed = true;
		} else {
			bytes[pos + 0] = (v >> 16) & 0xFF;
			bytes[pos + 1] = (v >> 8) & 0xFF;
			bytes[pos + 2] = (v >> 0) & 0xFF;
		}
	}

	void patch_u32(U32 pos, U32 v) {
		if (capacity - pos < sizeof(U32)) {
			failed = true;
		} else {
			STORE_BE32(bytes + pos, v);
		}
	}

	void patch_u64(U32 pos, U64 v) {
		if (capacity - pos < sizeof(U64)) {
			failed = true;
		} else {
			STORE_BE64(bytes + pos, v);
		}
	}

	void patch_f32(U32 pos, F32 v) {
		U32 floatAsInt;
		memcpy(&floatAsInt, &v, sizeof(F32));
		patch_u32(pos, floatAsInt);
	}
};

#include "SerializeTools.h"