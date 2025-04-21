#pragma once

#include <immintrin.h>
#include <intrin.h>

extern "C" {
	void* __cdecl memcpy(void* _Dst, void const* _Src, unsigned long long _Size);
	void* __cdecl memmove( void* _Dst, void const* _Src, unsigned long long _Size);
	void* __cdecl memset(void* _Dst, int _Val, unsigned long long _Size);
	int __cdecl memcmp(void const* _Buf1, void const* _Buf2, unsigned long long _Size);
	int __cdecl strcmp(char const* _Str1,char const* _Str2);
	unsigned long long __cdecl strlen(char const* _Str);
	char* strcpy(char* _Destination, char const* _Source);
};

// For some reason these functions fail to link in release mode but not in debug mode?
#ifdef NDEBUG

#pragma function(memcpy)
void* __cdecl memcpy(void* dst, const void* src, unsigned long long count) {
	const unsigned char* bSrc = reinterpret_cast<const unsigned char*>(src);
	unsigned char* bDst = reinterpret_cast<unsigned char*>(dst);
	/*while (count--) {
		*bDst++ = *bSrc++;
	}*/
	__movsb(bDst, bSrc, count);
	return dst;
}

#pragma function(memmove)
void* __cdecl memmove(void* dst, const void* src, unsigned long long count) {
	const unsigned char* bSrc = reinterpret_cast<const unsigned char*>(src);
	unsigned char* bDst = reinterpret_cast<unsigned char*>(dst);
	if (reinterpret_cast<unsigned long long>(bDst) - reinterpret_cast<unsigned long long>(bSrc) >= count) {
		__movsb(bDst, bSrc, count);
	} else {
		bSrc += count;
		bDst += count;
		while (count --> 0) {
			*(--bDst) = *(--bSrc);
		}
	}
	return dst;
}

#pragma function(memset)
void* __cdecl memset(void* dst, int val, unsigned long long count) {
	unsigned char* bDst = reinterpret_cast<unsigned char*>(dst);
	/*while (count--) {
		*bDst++ = val;
	}*/
	__stosb(bDst, unsigned char(val), count);
	return dst;
}

#pragma function(strcmp)
int __cdecl strcmp(const char* s1, const char* s2) {
	const unsigned char* bS1 = reinterpret_cast<const unsigned char*>(s1);
	const unsigned char* bS2 = reinterpret_cast<const unsigned char*>(s2);
	unsigned char b1 = 0;
	unsigned char b2 = 0;
	while ((b1 = *bS1++) == (b2 = *bS2++)) {
		if (b1 == '\0') {
			break;
		}
	}
	return b1 - b2;
}

#pragma function(strlen)
unsigned long long __cdecl strlen(const char* str) {
	unsigned long long result = 0;
	if (str != nullptr) {
		while (*str++) {
			result++;
		}
	}
	return result;
}

#pragma function(strcpy)
char* strcpy(char* dst, const char* src) {
	while(*(dst++) = *(src++));
	return --dst;
}

#pragma function(memcmp)
int __cdecl memcmp(const void* m1, const void* m2, unsigned long long n) {
	int diff = 0;
	const unsigned char* bM1 = reinterpret_cast<const unsigned char*>(m1);
	const unsigned char* bM2 = reinterpret_cast<const unsigned char*>(m2);
	while (n--) {
		if (*bM1++ != *bM2++) {
			diff = *--bM1 - *--bM2;
			break;
		}
	}
	return diff;
}

#endif