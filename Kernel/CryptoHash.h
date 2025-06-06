#pragma once

#include "CLibDefs.h"
#include "Util.h"

static constexpr U64 keccak_iota_rc(U32 t) {
	// The iota linear feedback shift register
	U32 r = 0b10000000;
	for (U32 i = 0; i < (t & 0xFF); i++) {
		U32 r8Mask = (r >> 7) * 0xFF;
		r <<= 1;
		r8Mask &= 0b1110001;
		r = (r ^ r8Mask) & 0xFF;
	}
	return r & 1;
}

static constexpr U64 keccak_iota(U32 i) {
	U64 rc = 0;
	for (U32 j = 0; j <= 6; j++) {
		U32 bitIdx = (1 << j) - 1;
		rc |= keccak_iota_rc(j + 7 * i) << bitIdx;
	}
	return rc;
}

struct Keccak {
	static constexpr U32 RHO_ROTATE_TABLE[]{
		0,  1,  62, 28, 27,
		36, 44, 6,  55, 20,
		3,  10, 43, 25, 39,
		41, 45, 15, 21, 8,
		18, 2,  61, 56, 14
	};
	static constexpr U32 NUM_ROUNDS = 24;
	static constexpr U64 IOTA_TABLE[NUM_ROUNDS]{
		0x0000000000000001ui64,
		keccak_iota(0),
		keccak_iota(1),
		keccak_iota(2),
		keccak_iota(3),
		keccak_iota(4),
		keccak_iota(5),
		keccak_iota(6),
		keccak_iota(7),
		keccak_iota(8),
		keccak_iota(9),
		keccak_iota(10),
		keccak_iota(11),
		keccak_iota(12),
		keccak_iota(13),
		keccak_iota(14),
		keccak_iota(15),
		keccak_iota(16),
		keccak_iota(17),
		keccak_iota(18),
		keccak_iota(19),
		keccak_iota(20),
		keccak_iota(21),
		keccak_iota(22),
	};
	static constexpr U32 KECCAK_SIZE = 1600;
	static constexpr U32 CONSTANT_BITS = 512;
	static constexpr U32 BLOCK_SIZE = KECCAK_SIZE - CONSTANT_BITS;
	static constexpr U32 BLOCK_SIZE_BYTES = BLOCK_SIZE / 8;
	static constexpr U32 BLOCK_SIZE_INT64 = BLOCK_SIZE_BYTES / 8;
	// 3d array of internal state. Each uint64 is a z axis strip, with a 5x5 bit cross section
	U64 state[25];
	U32 squeezeMarker;

	void init() {
		for (U32 i = 0; i < 25; i++) {
			state[i] = 0;
		}
		squeezeMarker = 0;
	}

	void add_data(const U8 data[BLOCK_SIZE_BYTES]) {
		for (U32 i = 0; i < BLOCK_SIZE_BYTES; i++) {
			reinterpret_cast<U8*>(state)[i] ^= data[i];
		}
	}

	void do_round(U32 round) {
		U64 tempState[25];
		// Theta function
		U64 colXors[5];
		for (U32 x = 0; x < 5; x++) {
			colXors[x] = 0;
			for (U32 z = 0; z < 64; z++) {
				U64 colXor = (state[x] >> z) & 1;
				colXor ^= (state[5 + x] >> z) & 1;
				colXor ^= (state[10 + x] >> z) & 1;
				colXor ^= (state[15 + x] >> z) & 1;
				colXor ^= (state[20 + x] >> z) & 1;
				colXors[x] |= colXor << z;
			}
		}
		for (I32 x = 0; x < 5; x++) {
			U64 colorXorsLeft = colXors[(x + 4) % 5];
			U64 colorXorsRight = colXors[(x + 1) % 5];
			for (U32 y = 0; y < 5; y++) {
				U32 stateIdx = y * 5 + x;
				U64 oldState = state[stateIdx];
				U64 newState = 0;
				for (U32 z = 0; z < 64; z++) {
					U64 bit = (oldState >> z) & 1;
					U64 sumLeftBit = (colorXorsLeft >> z) & 1;
					U64 sumRightFrontBit = (colorXorsRight >> ((z - 1) & 63)) & 1;
					newState |= (bit ^ sumLeftBit ^ sumRightFrontBit) << z;
				}
				state[stateIdx] = newState;
			}
		}

		// Rho function
		for (U32 i = 1; i < 25; i++) {
			U64 oldState = state[i];
			U32 rot = RHO_ROTATE_TABLE[i];
			state[i] = (oldState << rot) | (oldState >> (64 - rot));
		}

		// Pi function
		for (U32 y = 0; y < 5; y++) {
			for (U32 x = 0; x < 5; x++) {
				U32 newY = x;
				U32 newX = (x + y * 3) % 5;
				tempState[y * 5 + x] = state[newY * 5 + newX];
			}
		}

		// Chi function
		for (U32 y = 0; y < 25; y += 5) {
			state[y] = tempState[y] ^ (~tempState[y + 1] & tempState[y + 2]);
			state[y + 1] = tempState[y + 1] ^ (~tempState[y + 2] & tempState[y + 3]);
			state[y + 2] = tempState[y + 2] ^ (~tempState[y + 3] & tempState[y + 4]);
			state[y + 3] = tempState[y + 3] ^ (~tempState[y + 4] & tempState[y + 0]);
			state[y + 4] = tempState[y + 4] ^ (~tempState[y] & tempState[y + 1]);
		}

		// Iota function
		state[0] ^= IOTA_TABLE[round];
	}

	void do_stage() {
		for (U32 i = 0; i < NUM_ROUNDS; i++) {
			do_round(i);
		}
	}

	void absorb(const U8* data, U32 size) {
		while (size > BLOCK_SIZE_BYTES) {
			add_data(data);
			do_stage();
			data += BLOCK_SIZE_BYTES;
			size -= BLOCK_SIZE_BYTES;
		}
		if (size != 0) {
			alignas(8) U8 paddedData[BLOCK_SIZE_BYTES];
			U32 i;
			for (i = 0; i < size; i++) {
				paddedData[i] = data[i];
			}
			for (; i < BLOCK_SIZE_BYTES; i++) {
				paddedData[i] = 0;
			}
			paddedData[BLOCK_SIZE_BYTES - 1] = 0x80;
			paddedData[size] |= 0x06;
			add_data(paddedData);
			do_stage();
		}
	}

	void squeeze(void* vout, U32 size) {
		U8* out = reinterpret_cast<U8*>(vout);
		U32 outptr = 0;
		while (size != 0) {
			if (squeezeMarker == BLOCK_SIZE_BYTES) {
				do_stage();
				squeezeMarker = 0;
			}
			U32 end = squeezeMarker + size;
			if (end > BLOCK_SIZE_BYTES) {
				end = BLOCK_SIZE_BYTES;
			}
			size -= end - squeezeMarker;
			while (squeezeMarker != end) {
				*(out++) = reinterpret_cast<U8*>(state)[squeezeMarker++];
			}
		}
	}

	void make_secure_random() {
		U64 randData[4];
		while (_rdrand64_step(&randData[0]) == 0);
		while (_rdrand64_step(&randData[1]) == 0);
		while (_rdrand64_step(&randData[2]) == 0);
		while (_rdrand64_step(&randData[3]) == 0);

		init();
		absorb(reinterpret_cast<const U8*>(randData), 4 * sizeof(U64));
	}
};

inline U32 rotate_right(U32 x, U32 amount) {
	return (x >> amount) | (x << (32 - amount));
}

inline U32 rotate_left(U32 x, U32 amount) {
	return (x << amount) | (x >> (32 - amount));
}

#define SHA1_INPUT_BLOCK_SIZE 64

struct SHA1 {
	U32 state[5];
	U32 inputBufferPos;
	U8 inputBuffer[SHA1_INPUT_BLOCK_SIZE];
	U64 messageLength;

	void init() {
		state[0] = 0x67452301;
		state[1] = 0xEFCDAB89;
		state[2] = 0x98BADCFE;
		state[3] = 0x10325476;
		state[4] = 0xC3D2E1F0;

		inputBufferPos = 0;
		messageLength = 0;
	}

	static constexpr U32 ROUND_CONSTANT_1 = 0x5A827999;
	static U32 round_func_1(U32 b, U32 c, U32 d) {
		return (b & c) | (~b & d);
	}

	static constexpr U32 ROUND_CONSTANT_2 = 0x6ED9EBA1;
	static U32 round_func_2(U32 b, U32 c, U32 d) {
		return b ^ c ^ d;
	}

	static constexpr U32 ROUND_CONSTANT_3 = 0x8F1BBCDC;
	static U32 round_func_3(U32 b, U32 c, U32 d) {
		return (b & c) | (b & d) | (c & d);
	}

	static constexpr U32 ROUND_CONSTANT_4 = 0xCA62C1D6;
	static U32 round_func_4(U32 b, U32 c, U32 d) {
		return b ^ c ^ d;
	}

	void do_round(U32 inputWord, U32 roundConstant, U32(*func)(U32, U32, U32)) {
		U32 aRot5 = (state[0] << 5) | (state[0] >> (32 - 5));
		U32 bRot30 = (state[1] << 30) | (state[1] >> (32 - 30));

		U32 e = state[4];
		e += func(state[1], state[2], state[3]);
		e += aRot5;
		e += inputWord;
		e += roundConstant;

		state[4] = state[3];
		state[3] = state[2];
		state[2] = bRot30;
		state[1] = state[0];
		state[0] = e;
	}

	void do_stage(U32 roundConstant, U32(*func)(U32, U32, U32), U32 stageMessageSchedule[20]) {
		for (U32 i = 0; i < 20; i++) {
			do_round(stageMessageSchedule[i], roundConstant, func);
		}
	}

	void hash_block() {
		U32 messageSchedule[80];
		for (U32 i = 0; i < 16; i++) {
			// It's 3 AM right now, and I've just figured out the whole reason this wasn't working was because I didn't do this endian swap
			messageSchedule[i] =
				(inputBuffer[i * 4 + 0] << 24) |
				(inputBuffer[i * 4 + 1] << 16) |
				(inputBuffer[i * 4 + 2] << 8) |
				(inputBuffer[i * 4 + 3]);
		}
		for (U32 i = 16; i < 80; i++) {
			U32 messageXor = messageSchedule[i - 16] ^ messageSchedule[i - 14] ^ messageSchedule[i - 8] ^ messageSchedule[i - 3];
			messageSchedule[i] = (messageXor << 1) | (messageXor >> (32 - 1));
		}

		U32 originalState[5];
		memcpy(originalState, state, 5 * sizeof(U32));

		do_stage(ROUND_CONSTANT_1, round_func_1, messageSchedule + 0);
		do_stage(ROUND_CONSTANT_2, round_func_2, messageSchedule + 20);
		do_stage(ROUND_CONSTANT_3, round_func_3, messageSchedule + 40);
		do_stage(ROUND_CONSTANT_4, round_func_4, messageSchedule + 60);

		state[0] += originalState[0];
		state[1] += originalState[1];
		state[2] += originalState[2];
		state[3] += originalState[3];
		state[4] += originalState[4];
	}

	void update(const void* vData, U32 length) {
		const U8* data = reinterpret_cast<const U8*>(vData);
		messageLength += length;
		while (length > 0) {
			U32 amountToCopy = min(length, SHA1_INPUT_BLOCK_SIZE - inputBufferPos);
			memcpy(inputBuffer + inputBufferPos, data, amountToCopy);

			length -= amountToCopy;
			data += amountToCopy;
			inputBufferPos += amountToCopy;

			if (inputBufferPos == SHA1_INPUT_BLOCK_SIZE) {
				hash_block();
				inputBufferPos = 0;
			}
		}
	}

	void digest(void* out) {
		// Pad and do final hash
		inputBuffer[inputBufferPos++] = 0x80;
		if (inputBufferPos > (SHA1_INPUT_BLOCK_SIZE - 8)) {
			memset(inputBuffer + inputBufferPos, 0, SHA1_INPUT_BLOCK_SIZE - inputBufferPos);
			hash_block();
			inputBufferPos = 0;
		}
		memset(inputBuffer + inputBufferPos, 0, SHA1_INPUT_BLOCK_SIZE - 8 - inputBufferPos);

		// Append length in big endian
		messageLength *= 8;
		U8* lengthBuf = inputBuffer + (SHA1_INPUT_BLOCK_SIZE - 8);
		lengthBuf[0] = (messageLength >> 56) & 0xFF;
		lengthBuf[1] = (messageLength >> 48) & 0xFF;
		lengthBuf[2] = (messageLength >> 40) & 0xFF;
		lengthBuf[3] = (messageLength >> 32) & 0xFF;
		lengthBuf[4] = (messageLength >> 24) & 0xFF;
		lengthBuf[5] = (messageLength >> 16) & 0xFF;
		lengthBuf[6] = (messageLength >> 8) & 0xFF;
		lengthBuf[7] = messageLength & 0xFF;

		hash_block();

		if (out) {
			// Copy hash to output, making sure to byteswap to big endian
			for (U32 i = 0; i < 5; i++) {
				reinterpret_cast<U8*>(out)[i * 4 + 0] = (state[i] >> 24) & 0xFF;
				reinterpret_cast<U8*>(out)[i * 4 + 1] = (state[i] >> 16) & 0xFF;
				reinterpret_cast<U8*>(out)[i * 4 + 2] = (state[i] >> 8) & 0xFF;
				reinterpret_cast<U8*>(out)[i * 4 + 3] = state[i] & 0xFF;
			}
		}
	}
};

#define SHA256_INPUT_BLOCK_SIZE 64
#define SHA256_HASH_SIZE 32

// https://datatracker.ietf.org/doc/html/rfc4634
struct SHA256 {
	U32 state[8];
	U32 inputBufferPos;
	U8 inputBuffer[SHA256_INPUT_BLOCK_SIZE];
	U64 messageLength;

	void init() {
		state[0] = 0x6a09e667;
		state[1] = 0xbb67ae85;
		state[2] = 0x3c6ef372;
		state[3] = 0xa54ff53a;
		state[4] = 0x510e527f;
		state[5] = 0x9b05688c;
		state[6] = 0x1f83d9ab;
		state[7] = 0x5be0cd19;

		inputBufferPos = 0;
		messageLength = 0;
	}

	void copy_from(SHA256& other) {
		memcpy(state, other.state, 8 * sizeof(U32));
		inputBufferPos = other.inputBufferPos;
		memcpy(inputBuffer, other.inputBuffer, SHA256_INPUT_BLOCK_SIZE);
		messageLength = other.messageLength;
	}

	// "These words represent the first thirty-two bits of the fractional parts of the cube roots of the first sixty-four prime numbers."
	static constexpr U32 SHA256_CONSTANTS[64]{
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
		0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
		0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	static U32 ch(U32 x, U32 y, U32 z) {
		return (x & y) ^ (~x & z);
	}

	static U32 maj(U32 x, U32 y, U32 z) {
		return (x & y) ^ (x & z) ^ (y & z);
	}

	static U32 bsig0(U32 x) {
		return rotate_right(x, 2) ^ rotate_right(x, 13) ^ rotate_right(x, 22);
	}

	static U32 bsig1(U32 x) {
		return rotate_right(x, 6) ^ rotate_right(x, 11) ^ rotate_right(x, 25);
	}

	static U32 ssig0(U32 x) {
		return rotate_right(x, 7) ^ rotate_right(x, 18) ^ (x >> 3);
	}

	static U32 ssig1(U32 x) {
		return rotate_right(x, 17) ^ rotate_right(x, 19) ^ (x >> 10);
	}

	void do_round(U32 inputWord, U32 roundConstant) {
		U32 t1 = state[7] + bsig1(state[4]) + ch(state[4], state[5], state[6]) + roundConstant + inputWord;
		U32 t2 = bsig0(state[0]) + maj(state[0], state[1], state[2]);

		state[7] = state[6];
		state[6] = state[5];
		state[5] = state[4];
		state[4] = state[3] + t1;
		state[3] = state[2];
		state[2] = state[1];
		state[1] = state[0];
		state[0] = t1 + t2;
	}

	void hash_block() {
		// Compute the message schedule
		U32 messageSchedule[64];
		for (U32 i = 0; i < 16; i++) {
			messageSchedule[i] =
				(inputBuffer[i * 4 + 0] << 24) |
				(inputBuffer[i * 4 + 1] << 16) |
				(inputBuffer[i * 4 + 2] << 8) |
				inputBuffer[i * 4 + 3];
		}
		for (U32 i = 16; i < 64; i++) {
			messageSchedule[i] = ssig1(messageSchedule[i - 2]) + messageSchedule[i - 7] + ssig0(messageSchedule[i - 15]) + messageSchedule[i - 16];
		}

		U32 oldState[8];
		memcpy(oldState, state, 8 * sizeof(U32));

		// Compute the hash
		for (U32 i = 0; i < 64; i++) {
			do_round(messageSchedule[i], SHA256_CONSTANTS[i]);
		}

		// Add back to original words
		for (U32 i = 0; i < 8; i++) {
			state[i] += oldState[i];
		}
	}

	void update(const void* vData, U32 length) {
		const U8* data = reinterpret_cast<const U8*>(vData);
		messageLength += length;
		while (length > 0) {
			U32 amountToCopy = min(length, SHA256_INPUT_BLOCK_SIZE - inputBufferPos);
			memcpy(inputBuffer + inputBufferPos, data, amountToCopy);

			length -= amountToCopy;
			data += amountToCopy;
			inputBufferPos += amountToCopy;

			if (inputBufferPos == SHA256_INPUT_BLOCK_SIZE) {
				hash_block();
				inputBufferPos = 0;
			}
		}
	}

	void digest(void* out = nullptr, U32 outputLength = SHA256_HASH_SIZE) {
		// Pad and do final hash (same as sha1)
		inputBuffer[inputBufferPos++] = 0x80;
		if (inputBufferPos > (SHA256_INPUT_BLOCK_SIZE - 8)) {
			memset(inputBuffer + inputBufferPos, 0, SHA256_INPUT_BLOCK_SIZE - inputBufferPos);
			hash_block();
			inputBufferPos = 0;
		}
		memset(inputBuffer + inputBufferPos, 0, SHA256_INPUT_BLOCK_SIZE - 8 - inputBufferPos);

		// Append length in big endian
		messageLength *= 8;
		U8* lengthBuf = inputBuffer + (SHA256_INPUT_BLOCK_SIZE - 8);
		lengthBuf[0] = (messageLength >> 56) & 0xFF;
		lengthBuf[1] = (messageLength >> 48) & 0xFF;
		lengthBuf[2] = (messageLength >> 40) & 0xFF;
		lengthBuf[3] = (messageLength >> 32) & 0xFF;
		lengthBuf[4] = (messageLength >> 24) & 0xFF;
		lengthBuf[5] = (messageLength >> 16) & 0xFF;
		lengthBuf[6] = (messageLength >> 8) & 0xFF;
		lengthBuf[7] = messageLength & 0xFF;

		hash_block();

		// Byte output is in big endian
		for (U32 i = 0; i < 8; i++) {
			state[i] = bswap32(state[i]);
		}

		if (out) {
			memcpy(out, state, outputLength);
		}
	}
};

inline void sha256(void* out, const void* data, U32 len) {
	SHA256 sha;
	sha.init();
	sha.update(data, len);
	sha.digest(out);
}

struct HMACSHA256 {
	static constexpr U8 IPAD = 0x36;
	static constexpr U8 OPAD = 0x5C;

	SHA256 sha;
	U8 paddedKey[64];

	void init(const void* key, U32 length) {
		sha.init();
		if (length > 64) {
			sha.update(key, length);
			sha.digest(paddedKey);
			length = 8 * sizeof(U32);
			sha.init();
		}

		U32 i = 0;
		for (; i < length; i++) {
			paddedKey[i] = reinterpret_cast<const U8*>(key)[i] ^ IPAD;
		}
		for (; i < 64; i++) {
			paddedKey[i] = IPAD;
		}
		sha.update(paddedKey, 64);
	}

	void update(const void* data, U32 length) {
		sha.update(data, length);
	}

	void generate(void* output) {
		U32 result[8];
		sha.digest(result);

		// Outer hash
		for (U32 i = 0; i < 64; i++) {
			// Remove the ipad xor before applying opad
			paddedKey[i] ^= IPAD;
			paddedKey[i] ^= OPAD;
		}

		sha.init();
		sha.update(paddedKey, 64);
		sha.update(result, 8 * sizeof(U32));
		sha.digest(output);
	}
};

inline void sha256_hmac(void* output, const void* key, U32 keyLength, const void* data, U32 size) {
	HMACSHA256 hmac;
	hmac.init(key, keyLength);
	hmac.update(data, size);
	hmac.generate(output);
}

inline void sha256_hkdf_extract(void* output, const void* salt, U32 saltLength, const void* data, U32 size) {
	sha256_hmac(output, salt, saltLength, data, size);
}

inline void sha256_hkdf_expand(void* output, U32 outputLength, const void* key, U32 keyLength, const void* info, U32 infoLength) {
	HMACSHA256 hmac;
	U8 t[32];
	U8 tNumber = 0;
	while (outputLength > 0) {
		tNumber++;
		hmac.init(key, keyLength);
		if (tNumber > 1) {
			hmac.update(t, 32);
		}
		hmac.update(info, infoLength);
		hmac.update(&tNumber, 1);
		hmac.generate(t);

		U32 generatedSize = min(outputLength, 32ui32);
		memcpy(output, t, generatedSize);
		outputLength -= generatedSize;
		output = &reinterpret_cast<U8*>(output)[generatedSize];
	}
}

inline void sha256_hkdf_expand_label(void* output, U16 outputLength, const void* secret, U32 secretLength, const void* label, U8 labelLength, const void* context, U8 contextLength) {
	HMACSHA256 hmac;
	U8 t[32];
	U8 tNumber = 0;
	while (outputLength > 0) {
		tNumber++;
		hmac.init(secret, secretLength);
		if (tNumber > 1) {
			hmac.update(t, 32);
		}

		// HkdfLabel
		U16 bigEndianLength = (outputLength << 8) | (outputLength >> 8);
		hmac.update(&bigEndianLength, sizeof(U16));
		U8 labelLengthField = labelLength + 6;
		hmac.update(&labelLengthField, 1);
		hmac.update("tls13 ", 6);
		hmac.update(label, labelLength);
		hmac.update(&contextLength, 1);
		hmac.update(context, contextLength);

		hmac.update(&tNumber, 1);
		hmac.generate(t);

		U32 generatedSize = min(outputLength, 32ui16);
		memcpy(output, t, generatedSize);
		outputLength -= generatedSize;
		output = &reinterpret_cast<U8*>(output)[generatedSize];
	}
}

inline void sha256_derive_secret(void* output, U32 outputLength, const void* secret, U32 secretLength, const void* label, U32 labelLength, const void* messagesHash, U32 messageHashLength) {
	sha256_hkdf_expand_label(output, outputLength, secret, secretLength, label, labelLength, messagesHash, messageHashLength);
}

void print_first_hash(const void* data, U32 size) {
	Byte hash[SHA256_HASH_SIZE];
	sha256(hash, data, size);
	print_num(LOAD_LE64(hash));
}