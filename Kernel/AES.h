#pragma once

#include "Util.h"

// x^8 + x^4 + x^3 + x + 1
#define AES_GALOIS_POLY 0b100011011
// x^128 + x^7 + x^2 + x + 1
#define GCM_GALOIS_POLY 0b10000111ui64

constexpr U32 AES_SUB_TABLE[256]{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

constexpr U32 AES_SUB_TABLE_INVERSE[256]{
	0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
	0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
	0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
	0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
	0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
	0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
	0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
	0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
	0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
	0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
	0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
	0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
	0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
	0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
	0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

constexpr U32 AES_ROUND_CONSTANTS[10]{
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

inline U32 galois_mulx(U32 n) {
	// Basically multiply by x, if the high bit is set, subtract the modulo polynomial
	return (n << 1) ^ (((n >> 7) & 1) * AES_GALOIS_POLY);
}

inline U32 galois_mul2(U32 n) {
	// Multiply n(x); which is just multiplying by x
	return galois_mulx(n);
}

inline U32 galois_mul3(U32 n) {
	// Multiply n(x + 1); factoring to (n)x + n;
	return galois_mulx(n) ^ n;
}

inline U32 galois_mul9(U32 n) {
	// Multiply n(x^3 + 1); factoring to (((n)x)x)x + n
	return galois_mulx(galois_mulx(galois_mulx(n))) ^ n;
}

inline U32 galois_mul11(U32 n) {
	// Multiply n(x^3 + x + 1); factoring to (((n)x)x + n)x + n
	return galois_mulx(galois_mulx(galois_mulx(n)) ^ n) ^ n;
}

inline U32 galois_mul13(U32 n) {
	// Multiply n(x^3 + x^2 + 1); factoring to (((n)x + n)x)x + n
	return galois_mulx(galois_mulx(galois_mulx(n) ^ n)) ^ n;
}

inline U32 galois_mul14(U32 n) {
	// Multiply n(x^3 + x^2 + x); factoring to (((n)x + n)x + n)x
	return galois_mulx(galois_mulx(galois_mulx(n) ^ n) ^ n);
}

inline U32 galois_mul(U32 x, U32 y) {
	U32 result = 0;
	for (U32 i = 0; i < 8; i++) {
		if ((y >> i) & 1) {
			result ^= x << i;
		}
	}
	return result;
}

inline U32 galois_aes_reduce(U32 n) {
	for (U32 i = 15; i >= 8; i--) {
		if ((n >> i) & 1) {
			n ^= AES_GALOIS_POLY << (i - 8);
		}
	}
	return n;
}

inline U32 galois_aes_mul(U32 x, U32 y) {
	U32 res = galois_mul(x, y);
	return galois_aes_reduce(res);
}

inline void galois_aes_matmul4x4(U32* arr1, U32* arr2, U32* result) {
	for (U32 i = 0; i < 4; i++) {
		for (U32 j = 0; j < 4; j++) {
			result[i * 4 + j] =
				galois_aes_mul(arr1[i * 4 + 0], arr2[0 * 4 + j]) ^
				galois_aes_mul(arr1[i * 4 + 1], arr2[1 * 4 + j]) ^
				galois_aes_mul(arr1[i * 4 + 2], arr2[2 * 4 + j]) ^
				galois_aes_mul(arr1[i * 4 + 3], arr2[3 * 4 + j]);
		}
	}
}

#define AES_BLOCK_SIZE_BYTES 16
#define AES_BLOCK_SIZE_WORD32 4

#define AES_KEY_SIZE_BYTES 16
#define AES_KEY_SIZE_WORD32 4

#define AES_NUM_ROUNDS 10
#define AES_NUM_ROUND_KEYS (AES_NUM_ROUNDS + 1)

struct AES {
	U32 keySchedule[AES_BLOCK_SIZE_WORD32 * AES_NUM_ROUND_KEYS];

	U32 rot_word(U32 word) {
		return (word >> 8) | (word << 24);
	}

	U32 sub_word(U32 word) {
		U32 w0 = AES_SUB_TABLE[(word) & 0xFF];
		U32 w1 = AES_SUB_TABLE[(word >> 8) & 0xFF];
		U32 w2 = AES_SUB_TABLE[(word >> 16) & 0xFF];
		U32 w3 = AES_SUB_TABLE[(word >> 24) & 0xFF];
		return w0 | (w1 << 8) | (w2 << 16) | (w3 << 24);
	}

	void init(const void* key) {
		memcpy(keySchedule, key, AES_KEY_SIZE_BYTES);
		for (U32 i = AES_KEY_SIZE_WORD32; i < (4 * AES_NUM_ROUND_KEYS); i++) {
			if ((i % AES_KEY_SIZE_WORD32) == 0) {
				keySchedule[i] = keySchedule[i - AES_KEY_SIZE_WORD32] ^ sub_word(rot_word(keySchedule[i - 1])) ^ AES_ROUND_CONSTANTS[(i / AES_KEY_SIZE_WORD32) - 1];
			} else if (AES_KEY_SIZE_WORD32 > 6 && (i % AES_KEY_SIZE_WORD32) == 4) {
				keySchedule[i] = keySchedule[i - AES_KEY_SIZE_WORD32] ^ sub_word(keySchedule[i - 1]);
			} else {
				keySchedule[i] = keySchedule[i - AES_KEY_SIZE_WORD32] ^ keySchedule[i - 1];
			}
		}
	}

	void encrypt_round(U32 round, U8* block) {
		U8* key = reinterpret_cast<U8*>(&keySchedule[(round + 1) * AES_BLOCK_SIZE_WORD32]);

		// Byte sub
		// The code to generate this substitution is at the bottom of this file
		// For each byte it's the modular inverse with respect to the AES polynomial, multiplied by a bit matrix, then added to a constant
		for (U32 i = 0; i < AES_BLOCK_SIZE_BYTES; i++) {
			block[i] = AES_SUB_TABLE[block[i]];
		}

		// Shift row
		// Each row in the 16 bytes treated as a column major 4x4 matrix gets rotated
		// This (elements are indices):
		// 
		// 0 4 8 12
		// 1 5 9 13
		// 2 6 10 14
		// 3 7 11 15
		// 
		// Goes to this:
		//
		// 0 4 8 12 (no rotate)
		// 5 9 13 1 (rotate 1 left)
		// 10 14 2 6 (rotate 2 left)
		// 15 3 7 11 (rotate 3 left)
		//

		U8 shiftedBlock[AES_BLOCK_SIZE_BYTES]{
			block[0], block[5], block[10], block[15],
			block[4], block[9], block[14], block[3],
			block[8], block[13], block[2], block[7],
			block[12], block[1], block[6], block[11],
		};

		if (round != (AES_NUM_ROUNDS - 1)) {
			// Mix col
			for (U32 i = 0; i < 4; i++) {
				U8* col = &shiftedBlock[i * 4];
				U8* outCol = &block[i * 4];

				// This is matrix multiplication of the column with this 4x4 matrix:
				// 
				// 2 3 1 1
				// 1 2 3 1
				// 1 1 2 3
				// 3 1 1 2
				//
				outCol[0] = galois_mul2(col[0]) ^ galois_mul3(col[1]) ^ col[2] ^ col[3];
				outCol[1] = col[0] ^ galois_mul2(col[1]) ^ galois_mul3(col[2]) ^ col[3];
				outCol[2] = col[0] ^ col[1] ^ galois_mul2(col[2]) ^ galois_mul3(col[3]);
				outCol[3] = galois_mul3(col[0]) ^ col[1] ^ col[2] ^ galois_mul2(col[3]);
			}
		} else {
			memcpy(block, shiftedBlock, AES_BLOCK_SIZE_BYTES);
		}

		// Key add
		for (U32 i = 0; i < AES_BLOCK_SIZE_BYTES; i++) {
			block[i] ^= key[i];
		}
	}

	void decrypt_round(U32 round, U8* block) {
		U8* key = reinterpret_cast<U8*>(&keySchedule[(round + 1) * AES_BLOCK_SIZE_WORD32]);

		// Inverse key add
		for (U32 i = 0; i < AES_BLOCK_SIZE_BYTES; i++) {
			block[i] ^= key[i];
		}

		U8 shiftedBlock[AES_BLOCK_SIZE_BYTES];

		if (round != (AES_NUM_ROUNDS - 1)) {
			// Inverse mix col
			for (U32 i = 0; i < 4; i++) {
				U8* col = &block[i * 4];
				U8* outCol = &shiftedBlock[i * 4];

				// This is matrix multiplication of the column with this 4x4 matrix:
				// 
				// 14 11 13 9
				// 9  14 11 13
				// 13 9  14 11
				// 11 13 9  14
				//
				// As it happens, the original matrix to the fourth power in AES galois space is the identity matrix.
				// So, it reasonably follows that the original matrix to the third power is the inverse, because M^3 * M == M^4 == I == M^-1 * M
				// Taking the third power, this is the matrix you get.
				// See print_mixcol_matrix_inverse for code that generates the inverse from the mixcols matrix
				outCol[0] = galois_mul14(col[0]) ^ galois_mul11(col[1]) ^ galois_mul13(col[2]) ^ galois_mul9(col[3]);
				outCol[1] = galois_mul9(col[0]) ^ galois_mul14(col[1]) ^ galois_mul11(col[2]) ^ galois_mul13(col[3]);
				outCol[2] = galois_mul13(col[0]) ^ galois_mul9(col[1]) ^ galois_mul14(col[2]) ^ galois_mul11(col[3]);
				outCol[3] = galois_mul11(col[0]) ^ galois_mul13(col[1]) ^ galois_mul9(col[2]) ^ galois_mul14(col[3]);
			}
		} else {
			memcpy(shiftedBlock, block, AES_BLOCK_SIZE_BYTES);
		}

		// Inverse shift row
		block[0] = shiftedBlock[0];   block[1] = shiftedBlock[13]; block[2] = shiftedBlock[10]; block[3] = shiftedBlock[7];
		block[4] = shiftedBlock[4];   block[5] = shiftedBlock[1];  block[6] = shiftedBlock[14]; block[7] = shiftedBlock[11];
		block[8] = shiftedBlock[8];   block[9] = shiftedBlock[5];  block[10] = shiftedBlock[2]; block[11] = shiftedBlock[15];
		block[12] = shiftedBlock[12]; block[13] = shiftedBlock[9]; block[14] = shiftedBlock[6]; block[15] = shiftedBlock[3];

		// Inverse sub bytes
		for (U32 i = 0; i < AES_BLOCK_SIZE_BYTES; i++) {
			block[i] = AES_SUB_TABLE_INVERSE[block[i]];
		}
	}

	void encrypt_block(U8* block) {
		// Initial key add
		U8* key = reinterpret_cast<U8*>(keySchedule);
		for (U32 i = 0; i < 16; i++) {
			block[i] ^= key[i];
		}

		for (U32 i = 0; i < AES_NUM_ROUNDS; i++) {
			encrypt_round(i, block);
		}
	}

	void decrypt_block(U8* block) {
		for (I32 i = (AES_NUM_ROUNDS - 1); i >= 0; i--) {
			decrypt_round(i, block);
		}
		// Inverse initial key add
		U8* key = reinterpret_cast<U8*>(keySchedule);
		for (U32 i = 0; i < 16; i++) {
			block[i] ^= key[i];
		}
	}

	U32 get_aes_output_size(U32 length) {
		// Lowest multiple of AES_BLOCK_SIZE_BYTES (16) greater than or equal to length
		return (length + AES_BLOCK_SIZE_BYTES - 1) & ~(AES_BLOCK_SIZE_BYTES - 1);
	}

	void encrypt(U8* data, U32 length) {
		U32 numBlocks = (length + AES_BLOCK_SIZE_BYTES - 1) >> 4;
		for (U32 i = 0; i < numBlocks - 1; i++) {
			encrypt_block(data + i * AES_BLOCK_SIZE_BYTES);
			length -= AES_BLOCK_SIZE_BYTES;
		}
		memset(data + (numBlocks - 1) * AES_BLOCK_SIZE_BYTES + (AES_BLOCK_SIZE_BYTES - length), 0, length);
		encrypt_block(data + (numBlocks - 1) * AES_BLOCK_SIZE_BYTES);
	}

	void decrypt(U8* data, U32 length) {
		U32 numBlocks = (length + AES_BLOCK_SIZE_BYTES - 1) >> 4;
		for (U32 i = 0; i < numBlocks; i++) {
			decrypt_block(data + i * AES_BLOCK_SIZE_BYTES);
		}
	}
};

void gcm_galois_mul(U64 result[2], U64 x[2], U64 y[2]) {
	result[0] = result[1] = 0;

	// Double and add algorithm
	// Loop over y in reverse, multiply by x for each bit, add x if the bit is 1
	for (I32 word = 1; word >= 0; word--) {
		for (I32 bit = 63; bit >= 0; bit--) {
			// Double
			U64 needsReduction = (result[1] >> 63) & 1;
			result[1] = (result[1] << 1) | (result[0] >> 63);
			result[0] <<= 1;
			if (needsReduction) {
				result[0] ^= GCM_GALOIS_POLY;
			}
			// Add
			U64 shouldAddX = (y[word] >> bit) & 1;
			if (shouldAddX) {
				result[0] ^= x[0];
				result[1] ^= x[1];
			}
		}
	}
}

struct GHASH {
	U64 hash[2];
	U64 hashSubkey[2];

	void init(U64 key[2]) {
		hash[0] = hash[1] = 0;
		hashSubkey[0] = bitswap_bytes64(key[0]);
		hashSubkey[1] = bitswap_bytes64(key[1]);
	}

	// Data will be padded with zeros to a multiple of 16 bytes
	void update(const void* vdata, U32 length) {
		const U8* data = reinterpret_cast<const U8*>(vdata);
		while (length > 0) {
			U32 blockLength = min(length, 16ui32);
			U64 x[2]{};
			memcpy(x, data, blockLength);
			if (blockLength != 16) {
				memset(reinterpret_cast<U8*>(x) + blockLength, 0, 16 - blockLength);
			}
			x[0] = bitswap_bytes64(x[0]);
			x[1] = bitswap_bytes64(x[1]);
			hash[0] ^= x[0];
			hash[1] ^= x[1];

			gcm_galois_mul(x, hash, hashSubkey);
			U64 h[2]{ bitswap_bytes64(x[0]), bitswap_bytes64(x[1]) };

			hash[0] = x[0];
			hash[1] = x[1];

			data += blockLength;
			length -= blockLength;
		}
	}

	void get_hash(void* out) {
		U64 h[2]{ bitswap_bytes64(hash[0]), bitswap_bytes64(hash[1]) };
		// Possibly need to endian swap here
		memcpy(out, h, 16);
	}
};

#define AES_GCM_IV_SIZE 12

struct AESGCM {
	AES aes;
	U8 counterBlock[16];

	void init(const void* key, const void* counterIV) {
		aes.init(key);
		memcpy(counterBlock, counterIV, 12);
		counterBlock[12] = counterBlock[13] = counterBlock[14] = 0;
		counterBlock[15] = 1;
	}

	void inc32(void* data) {
		STORE_BE32(data, LOAD_BE32(data) + 1);
	}

	void gctr(void* vout, const void* vdata, U32 dataLength) {
		U8* out = reinterpret_cast<U8*>(vout);
		const U8* data = reinterpret_cast<const U8*>(vdata);
		U8 cipherBuf[16];
		while (dataLength > 0) {
			U32 processSize = min(dataLength, 16ui32);
			memcpy(cipherBuf, counterBlock, 16);

			aes.encrypt_block(cipherBuf);
			
			for (U32 i = 0; i < processSize; i++) {
				out[i] = data[i] ^ cipherBuf[i];
			}

			inc32(counterBlock + 12);

			data += processSize;
			out += processSize;
			dataLength -= processSize;
		}
	}

	void encrypt(void* authTagOut, void* cipherOut, const void* vdata, U32 dataLength, const void* authenticatedData, U32 authenticatedDataLength) {
		U64 zeroBlock[2]{};
		aes.encrypt_block(reinterpret_cast<U8*>(zeroBlock));

		// Encrypt
		inc32(counterBlock + 12);
		gctr(cipherOut, vdata, dataLength);

		/// Reset counter block to zero before authentication
		counterBlock[12] = counterBlock[13] = counterBlock[14] = 0;
		counterBlock[15] = 1;
		// Authenticate
		GHASH ghash;
		ghash.init(zeroBlock);
		ghash.update(authenticatedData, authenticatedDataLength);
		ghash.update(cipherOut, dataLength);

		U64 dataLengths[2]{ bswap64(authenticatedDataLength * 8), bswap64(dataLength * 8) };
		ghash.update(dataLengths, 16);
		ghash.get_hash(authTagOut);
		memcpy(zeroBlock, counterBlock, 16);
		aes.encrypt_block(reinterpret_cast<U8*>(zeroBlock));
		for (U32 i = 0; i < 16; i++) {
			reinterpret_cast<U8*>(authTagOut)[i] ^= reinterpret_cast<U8*>(zeroBlock)[i];
		}
	}

	B32 decrypt(void* dataOut, const void* authTag, const void* vCipherData, U32 dataLength, const void* authenticatedData, U32 authenticatedDataLength) {
		U64 zeroBlock[2]{};
		aes.encrypt_block(reinterpret_cast<U8*>(zeroBlock));

		// Authenticate
		GHASH ghash;
		ghash.init(zeroBlock);
		ghash.update(authenticatedData, authenticatedDataLength);
		ghash.update(vCipherData, dataLength);
		U64 dataLengths[2]{ bswap64(authenticatedDataLength * 8), bswap64(dataLength * 8) };
		ghash.update(dataLengths, 16);
		U8 newAuthTag[16];
		ghash.get_hash(newAuthTag);

		// Reset counter block to zero before decrypt
		counterBlock[12] = counterBlock[13] = counterBlock[14] = 0;
		counterBlock[15] = 1;

		memcpy(zeroBlock, counterBlock, 16);
		aes.encrypt_block(reinterpret_cast<U8*>(zeroBlock));
		for (U32 i = 0; i < 16; i++) {
			newAuthTag[i] ^= reinterpret_cast<U8*>(zeroBlock)[i];
		}

		// Decrypt (same as Encrypt)
		inc32(counterBlock + 12);
		gctr(dataOut, vCipherData, dataLength);

		return memcmp(authTag, newAuthTag, 16) == 0;
	}
};


// Code for generating the sbox constants below

// The regular sbox matrix, numbers are LSB first
constexpr U32 GALOIS_SBOX_MATRIX[8] = {
	0b10001111,
	0b11000111,
	0b11100011,
	0b11110001,
	0b11111000,
	0b01111100,
	0b00111110,
	0b00011111
};

// We actually need the bit reversed version because C++ numbers are MSB first as you'd expect
constexpr U32 GALOIS_SBOX_MATRIX_BIT_REVERSED[8] = {
	0b11110001,
	0b11100011,
	0b11000111,
	0b10001111,
	0b00011111,
	0b00111110,
	0b01111100,
	0b11111000
};

inline U32 galois_div(U32 x, U32 y) {
	U32 ySize = _lzcnt_u32(y) - 23;
	U32 quotient = 0;
	for (U32 i = 0; i <= ySize; i++) {
		U32 idx = 8 - i;
		if ((x >> idx) & 1) {
			x ^= y << (ySize - i);
			quotient |= 1 << (ySize - i);
		}
	}
	return quotient;
}

inline U32 galois_modular_invert(U32 n) {
	if (n == 0) {
		return 0;
	}
	// Extended Euclidian again, but with the galois polynomials
	U32 t = 0;
	U32 newT = 1;
	U32 r = AES_GALOIS_POLY;
	U32 newR = n;
	while (newR != 0) {
		U32 quotient = galois_div(r, newR);
		t ^= galois_mul(quotient, newT);
		swap(&t, &newT);
		U32 test = galois_mul(quotient, newR);
		r ^= galois_mul(quotient, newR);
		swap(&r, &newR);
	}
	return t;
}

inline U32 bitwise_8bit_dot(U32 x, U32 y) {
	U32 mul = x & y;
	U32 result = 0;
	for (U32 i = 0; i < 8; i++) {
		result ^= (mul >> i) & 1;
	}
	return result;
}

inline void print_galois_sbox_arrays() {
	U32 inverseMapping[256];
	for (U32 i = 0; i < 256; i++) {
		U32 num = galois_modular_invert(i);

		num = bitwise_8bit_dot(num, GALOIS_SBOX_MATRIX_BIT_REVERSED[0]) |
			(bitwise_8bit_dot(num, GALOIS_SBOX_MATRIX_BIT_REVERSED[1]) << 1) |
			(bitwise_8bit_dot(num, GALOIS_SBOX_MATRIX_BIT_REVERSED[2]) << 2) |
			(bitwise_8bit_dot(num, GALOIS_SBOX_MATRIX_BIT_REVERSED[3]) << 3) |
			(bitwise_8bit_dot(num, GALOIS_SBOX_MATRIX_BIT_REVERSED[4]) << 4) |
			(bitwise_8bit_dot(num, GALOIS_SBOX_MATRIX_BIT_REVERSED[5]) << 5) |
			(bitwise_8bit_dot(num, GALOIS_SBOX_MATRIX_BIT_REVERSED[6]) << 6) |
			(bitwise_8bit_dot(num, GALOIS_SBOX_MATRIX_BIT_REVERSED[7]) << 7);

		num ^= 0x63;

		inverseMapping[num] = i;

		/*std::cout << "0x";
		if (num <= 0xF) {
			std::cout << '0';
		}
		std::cout << std::hex << std::uppercase << num << ", ";
		if ((i & 15) == 15) {
			std::cout << '\n';
		}*/
	}

	/*std::cout << '\n';

	for (u32 i = 0; i < 256; i++) {
		std::cout << "0x";
		if (inverseMapping[i] <= 0xF) {
			std::cout << '0';
		}
		std::cout << std::hex << std::uppercase << inverseMapping[i] << ", ";
		if ((i & 15) == 15) {
			std::cout << '\n';
		}
	}*/
}

inline void print_mixcol_matrix_inverse() {
	U32 mixcols[16]{
		2, 3, 1, 1,
		1, 2, 3, 1,
		1, 1, 2, 3,
		3, 1, 1, 2
	};
	U32 res[16];
	U32 res2[16];
	galois_aes_matmul4x4(mixcols, mixcols, res);
	galois_aes_matmul4x4(mixcols, res, res2);
	galois_aes_matmul4x4(mixcols, res2, res);

	// M^4, the identity matrix
	/*std::cout << "M^4: \n";
	for (u32 i = 0; i < 4; i++) {
		for (u32 j = 0; j < 4; j++) {
			std::cout << res[i * 4 + j] << ", ";
		}
		std::cout << std::endl;
	}*/

	// M^3, the inverse matrix
	/*std::cout << "M^3: \n";
	for (u32 i = 0; i < 4; i++) {
		for (u32 j = 0; j < 4; j++) {
			std::cout << res2[i * 4 + j] << ", ";
		}
		std::cout << std::endl;
	}*/
}