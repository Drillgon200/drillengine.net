#pragma once
#include "CryptoHash.h"

struct BigInteger;

struct BigInteger {
	static constexpr u32 MAX_DIGIT_COUNT = 128;

	// Plus 4, since division is nicer to deal with if there's some extra space at the end
	u64 data[MAX_DIGIT_COUNT + 4];
	u32 cap;
	u32 size;
	bool negative;

	void init(u32 digitCount) {
		cap = digitCount;
		size = 1;
		data[0] = 0;
		negative = false;
	}

	static void zero(BigInteger& num) {
		num.size = 1;
		num.data[0] = 0;
	}

	static bool is_zero(const BigInteger& num) {
		return num.size == 1 && num.data[0] == 0;
	}

	static bool is_equal(const BigInteger& num, u64 digit) {
		return num.size == 1 && num.data[0] == digit;
	}

	static void assign(BigInteger& dst, const BigInteger& src) {
		u32 end = min(src.size, dst.cap);
		for (u32 i = 0; i != end; i++) {
			dst.data[i] = src.data[i];
		}
		dst.size = end;
		dst.negative = src.negative;
	}

	static void assign(BigInteger& dst, const char* base10Input) {
		zero(dst);
		if (base10Input[0] == '\0') {
			return;
		}
		bool neg = false;
		if (base10Input[0] == '-') {
			neg = true;
			base10Input++;
		}
		while (base10Input[0] >= '0' && base10Input[0] <= '9') {
			u64 num = base10Input[0] - '0';
			mul_single(dst, dst, 10);
			add_single(dst, dst, num);
			base10Input++;
		}
		dst.negative = neg;
	}

	static void assign_hex(BigInteger& dst, const char* hexInput) {
		bool neg = false;
		if (hexInput[0] == '-') {
			neg = true;
			hexInput++;
		}
		i32 size = 0;
		while (hexInput[size] != '\0' && ((hexInput[size] >= '0' && hexInput[size] <= '9') || ((hexInput[size] | 0b100000) >= 'a' && (hexInput[size] | 0b100000) <= 'f'))) {
			size++;
		}

		i32 newSize = min(size, static_cast<i32>(dst.cap * 16));
		hexInput += size - newSize;
		size = newSize;

		dst.size = (size + 15) >> 4;
		for (u32 i = 0; i < dst.size; i++) {
			dst.data[i] = 0;
		}
		for (i32 i = size - 1; i >= 0; i--) {
			u32 idx = size - i - 1;
			// Convert any lowercacse letters to uppo
			char digit = hexInput[i];
			u64 quartet = digit > '9' ? (digit | 0b100000) - 'a' + 10 : digit - '0';
			u32 wordIdx = idx >> 4;
			dst.data[wordIdx] |= quartet << ((idx & 15) * 4);
		}

		dst.negative = neg;
	}

	static void assign_bytes_big_endian(BigInteger& dst, const void* vbytes, u32 size) {
		const u8* bytes = reinterpret_cast<const u8*>(vbytes);
		u32 words = (size + 7) >> 3;
		dst.size = words;
		for (u32 i = 0; i < words; i++) {
			dst.data[i] = 0;
		}
		for (i32 i = size - 1; i >= 0; i--) {
			u32 dataIdx = size - i - 1;
			dst.data[dataIdx >> 3] |= static_cast<u64>(bytes[i]) << ((dataIdx & 7) * 8);
		}
		find_new_size(dst, words);
	}

	static void assign(BigInteger& dst, u64 src) {
		dst.size = 1;
		dst.data[0] = src;
		dst.negative = false;
	}

	static void write_bytes_big_endian(void* dst, u32 outSize, const BigInteger& src) {
		u8* out = reinterpret_cast<u8*>(dst);
		for (i32 i = outSize - 1; i >= 0; i--) {
			u32 dataIdx = outSize - i - 1;
			u32 wordIdx = dataIdx >> 3;
			out[i] = wordIdx >= src.size ? 0 : (src.data[wordIdx] >> ((dataIdx & 7) * 8)) & 0xFF;
		}
	}

	static u64 bitlength(BigInteger& val) {
		return (val.size - 1) * 64 + (64 - _lzcnt_u64(val.data[val.size - 1]));
	}

	static u64 count_trailing_zeros(BigInteger& val) {
		if (is_zero(val)) {
			return 0;
		}
		u32 idx = 0;
		while (val.data[idx] == 0) {
			idx++;
		}
		return idx * 64 + _tzcnt_u64(val.data[idx]);
	}

	static void invert_bits(BigInteger& val) {
		for (u32 i = 0; i < val.size; i++) {
			val.data[i] = ~val.data[i];
		}
	}

	static void find_new_size(BigInteger& val, u32 sizeMax) {
		sizeMax--;
		for (; sizeMax >= 1; sizeMax--) {
			if (val.data[sizeMax] != 0) {
				break;
			}
		}
		val.size = sizeMax + 1;
	}

	// Left greater -> 1, right greater -> -1, equal -> 0
	static i32 cmp(BigInteger& left, BigInteger& right) {
		if (left.size > right.size) {
			return 1;
		} else if (left.size < right.size) {
			return -1;
		}
		for (i32 i = left.size - 1; i >= 0; i--) {
			if (left.data[i] > right.data[i]) {
				return 1;
			} else if (left.data[i] < right.data[i]) {
				return -1;
			}
		}
		return 0;
	}

	// Left greater -> 1, right greater -> -1, equal -> 0
	static i32 cmp(BigInteger& left, u64 right) {
		if (left.size > 1 || left.data[0] > right) {
			return 1;
		} else if (left.data[0] < right) {
			return -1;
		}
		return 0;
	}

	static void word_shift_right(BigInteger& val, u32 amount) {
		amount = min(val.size, amount);
		val.size -= amount;
		for (u32 i = 0; i < val.size; i++) {
			val.data[i] = val.data[i + amount];
		}
		if (val.size == 0) {
			val.size = 1;
			val.data[0] = 0;
		}
	}

	static void word_shift_left(BigInteger& val, u32 amount) {
		i32 i = min(amount + val.size - 1, val.cap - 1);
		val.size = min(val.size + amount, val.cap);
		for (; i >= amount; i--) {
			val.data[i] = val.data[i - amount];
		}
		for (; i >= 0; i--) {
			val.data[i] = 0;
		}
	}

	static void bit_shift_right(BigInteger& val, u64 amount) {
		if (amount >= 64) {
			word_shift_right(val, amount >> 6);
		}
		amount &= 63;
		for (u32 i = 0; i < (val.size - 1); i++) {
			val.data[i] = (val.data[i] >> amount) | (val.data[i + 1] << (64 - amount));
		}
		val.data[val.size - 1] >>= amount;
		if (val.data[val.size - 1] == 0) {
			val.size = max(val.size - 1, 1ui32);
		}
	}

	static u64 bit_shift_left(BigInteger& val, u64 amount) {
		if (amount >= 64) {
			word_shift_left(val, amount >> 6);
		}
		amount &= 63;
		u64 carryOver = val.data[val.size - 1] >> (64 - amount);
		for (u32 i = val.size - 1; i > 0; i--) {
			val.data[i] = (val.data[i] << amount) | (val.data[i - 1] >> (64 - amount));
		}
		val.data[0] <<= amount;
		if (carryOver != 0 && val.size < val.cap) {
			val.data[val.size++] = carryOver;
			return 0;
		} else {
			return carryOver;
		}
	}

	static u64 add_impl(BigInteger& result, BigInteger& left, BigInteger& right) {
		u32 firstEnd = min(min(left.size, right.size), result.cap);
		u32 lastEnd = min(max(left.size, right.size), result.cap);
		u64 carryBit = 0;
		u32 i = 0;
		for (; i != firstEnd; i++) {
			u64 oldCarry = carryBit;
			u64 res = left.data[i] + right.data[i];
			carryBit = res < left.data[i];
			res += oldCarry;
			carryBit |= res < left.data[i];
			result.data[i] = res;
		}
		BigInteger& data = (left.size > right.size) ? left : right;
		for (; i != lastEnd; i++) {
			u64 res = data.data[i] + carryBit;
			carryBit = res < data.data[i];
			result.data[i] = res;
		}
		result.size = i;
		if (carryBit&& result.size < result.cap) {
			result.data[result.size++] = carryBit;
			carryBit = 0;
		}
		result.negative = left.negative;
		find_new_size(result, result.size);
		return carryBit;
	}

	static u64 sub_impl(BigInteger& result, BigInteger& left, BigInteger& right) {
		u32 firstEnd = min(min(left.size, right.size), result.cap);
		u32 lastEnd = min(max(left.size, right.size), result.cap);
		u64 borrowBit = 0;
		u32 i = 0;
		for (; i != firstEnd; i++) {
			u64 oldBorrow = borrowBit;
			u64 res = left.data[i] - right.data[i];
			borrowBit = res > left.data[i];
			res -= oldBorrow;
			borrowBit |= res > left.data[i];
			result.data[i] = res;
		}
		bool chooseLeft = left.size > right.size;
		for (; i != lastEnd; i++) {
			u64 oldData = (chooseLeft ? left.data[i] : 0);
			u64 oldBorrow = borrowBit;
			u64 res = (chooseLeft ? left.data[i] : (0ui64 - right.data[i]));
			borrowBit = res > oldData;
			res -= oldBorrow;
			borrowBit |= res > oldData;
			result.data[i] = res;
		}
		result.negative = left.negative;
		result.size = i;
		if (borrowBit) {
			result.negative = !result.negative;
			invert_bits(result);
			add_single_impl(result, result, 1);
		}
		find_new_size(result, result.size);
		return borrowBit;
	}

	static u64 add(BigInteger& result, BigInteger& left, BigInteger& right) {
		if (left.negative && !right.negative) {
			return sub_impl(result, right, left);
		} else if (!left.negative && right.negative) {
			return sub_impl(result, left, right);
		} else {
			return add_impl(result, left, right);
		}
	}

	static u64 sub(BigInteger& result, BigInteger& left, BigInteger& right) {
		if (!left.negative && right.negative) {
			return add_impl(result, left, right);
		} else if (left.negative && right.negative) {
			return sub_impl(result, right, left);
		} else if (!left.negative && !right.negative) {
			return sub_impl(result, left, right);
		} else {
			u64 res = add_impl(result, left, right);
			result.negative = true;
			return res;
		}
	}

	static u64 add_single_impl(BigInteger& result, BigInteger& left, u64 right) {
		u64 res = left.data[0] + right;
		u64 carryBit = res < left.data[0];
		left.data[0] = res;
		u32 end = min(left.size, result.cap);
		u32 i = 1;
		for (; i != end; i++) {
			res = left.data[i] + carryBit;
			carryBit = res < left.data[i];
			result.data[i] = res;
			if (carryBit == 0) {
				break;
			}
		}
		if (result.data != left.data) {
			for (; i != end; i++) {
				result.data[i] = left.data[i];
			}
		}
		if (carryBit&& result.size < result.cap) {
			result.data[result.size++] = 1;
		}
		result.negative = left.negative;
		return carryBit;
	}

	static u64 sub_single_impl(BigInteger& result, BigInteger& left, u64 right) {
		u64 res = left.data[0] - right;
		u64 borrowBit = res > left.data[0];
		result.data[0] = res;
		u32 end = min(left.size, result.cap);
		u32 i = 1;
		for (; i != end; i++) {
			u64 res = left.data[i] - borrowBit;
			borrowBit = res > left.data[i];
			result.data[i] = res;
			if (borrowBit == 0) {
				break;
			}
		}
		if (result.data != left.data) {
			for (; i != end; i++) {
				result.data[i] = left.data[i];
			}
		}
		result.negative = left.negative;
		find_new_size(result, end);
		if (borrowBit) {
			result.negative = !result.negative;
			invert_bits(result);
			add_single_impl(result, result, 1);
		}
		return borrowBit;
	}

	static u64 sub_single(BigInteger& result, BigInteger& left, u64 right) {
		if (left.negative) {
			return add_single_impl(result, left, right);
		}
		return sub_single_impl(result, left, right);
	}

	static u64 add_single(BigInteger& result, BigInteger& left, u64 right) {
		if (left.negative) {
			return sub_single_impl(result, left, right);
		}
		return add_single_impl(result, left, right);
	}

	static bool try_expand_size(BigInteger& val, u32 size) {
		u32 toGrow = min(val.cap, max(val.size, size));
		for (u32 i = val.size; i < toGrow; i++) {
			val.data[i] = 0;
		}
		val.size = toGrow;
		return toGrow >= size;
	}

	// result = left + (rightLo + rightHigh * base) * (base ^ pos);
	static void add128(BigInteger& result, u64 rightLo, u64 rightHi, u32 pos) {
		try_expand_size(result, pos + 1 + (rightHi != 0));
		u64 res;
		u64 carryBit = 0;

		// Add the high and low parts of the 128 bit integer
		if (pos < result.size) {
			res = result.data[pos] + rightLo;
			carryBit = res < result.data[pos];
			result.data[pos] = res;
		}

		if (pos + 1 < result.size) {
			res = result.data[pos + 1] + rightHi + carryBit;
			carryBit = res < result.data[pos + 1];
			result.data[pos + 1] = res;
		}

		// Propagate any carry bits
		for (u32 i = pos + 2; i < result.size; i++) {
			res = result.data[i] + carryBit;
			carryBit = res < result.data[i];
			result.data[i] = res;
			if (carryBit == 0) {
				break;
			}
		}
		if (carryBit == 1 && result.size < result.cap) {
			result.data[result.size++] = 1;
		}
	}

	// result = left * right
	static void mul(BigInteger& result, BigInteger& left, BigInteger& right) {
		zero(result);
		for (u32 i = 0; i < left.size; i++) {
			for (u32 j = 0; j < right.size; j++) {
				if (i + j >= result.cap) {
					break;
				}
				u64 high;
				u64 low = _umul128(left.data[i], right.data[j], &high);
				// TODO don't do a full add here, partial add and carry
				add128(result, low, high, i + j);
			}
		}
		result.negative = left.negative ^ right.negative;
	}

	// result = left * right
	static void mul_single(BigInteger& result, BigInteger& left, u64 right) {
		if (right == 0 || (left.size == 1 && left.data[0] == 0)) {
			zero(result);
			return;
		}
		u32 size = left.size;
		try_expand_size(result, size);
		u64 prevHigh = 0;
		for (u32 i = 0; i < min(result.size, left.size); i++) {
			u64 high;
			u64 low = _umul128(left.data[i], right, &high);
			u64 sum = low + prevHigh;
			// Carry
			high += sum < prevHigh;
			result.data[i] = sum;
			prevHigh = high;
		}
		if (prevHigh != 0) {
			if (try_expand_size(result, size + 1)) {
				result.data[size] = prevHigh;
			}
		}
	}

	// result += left * right
	static u64 fmadd_single(BigInteger& result, BigInteger& left, u64 right, u32 offset = 0) {
		if (right == 0 || is_zero(left)) {
			return 0;
		}
		u32 size = left.size + offset;
		try_expand_size(result, size);
		u64 carry = 0;
		u32 i = offset;
		for (; i < min(result.size, size); i++) {
			u64 high;
			u64 low = _umul128(left.data[i - offset], right, &high);
			u64 sum = low + carry;
			// Carry
			high += sum < carry;
			u64 prevSum = sum;
			sum = result.data[i] + sum;
			high += sum < prevSum;
			result.data[i] = sum;
			carry = high;
		}
		for (; carry && i < result.size; i++) {
			u64 oldDigit = result.data[i];
			result.data[i] += carry;
			carry = result.data[i] < oldDigit;
		}
		if (carry && result.size < result.cap) {
			result.data[result.size++] = carry;
			carry = 0;
		}
		return carry;
	}

	static u64 div_single_rem(BigInteger& dividend, u64 divisor) {
		if (dividend.size == 1 && dividend.data[0] < divisor) {
			return dividend.data[0];
		}

		// https://janmr.com/blog/2012/11/basic-multiple-precision-short-division/
		u64 remain = 0;
		for (i32 k = i32(dividend.size - 1); k >= 0; k--) {
			_udiv128(remain, dividend.data[k], divisor, &remain);
		}
		return remain;
	}

	// TODO test this
	static u64 div_single(BigInteger& dividendAndQuotient, u64 divisor) {
		if (dividendAndQuotient.size == 1 && dividendAndQuotient.data[0] < divisor) {
			u64 rem = dividendAndQuotient.data[0];
			zero(dividendAndQuotient);
			return rem;
		}

		// https://janmr.com/blog/2012/11/basic-multiple-precision-short-division/
		u64 remain = 0;
		for (i32 k = i32(dividendAndQuotient.size - 1); k >= 0; k--) {
			u64 quotient = _udiv128(remain, dividendAndQuotient.data[k], divisor, &remain);
			dividendAndQuotient.data[k] = quotient;
		}
		find_new_size(dividendAndQuotient, dividendAndQuotient.size);
		return remain;
	}

	static void div(BigInteger& dividendAndRemainderOut, BigInteger& divisor, BigInteger* quotientOut) {
		BigInteger& dividend = dividendAndRemainderOut;
		i32 compare = cmp(dividend, divisor);
		// Special cases, the arguments are the same or dividing a smaller number by a larger number
		if (compare == 0) {
			// Same number
			if (quotientOut) {
				zero(*quotientOut);
				quotientOut->data[0] = 1;
			}
			zero(dividend);
			return;
		} else if (compare == -1) {
			// Dividing by a greater number
			if (quotientOut) {
				zero(*quotientOut);
			}
			return;
		}

		i32 divisorSize = divisor.size;

		// Special case, divide by zero
		if (divisorSize == 1 && divisor.data[0] == 0) {
			return;
		}

		// Special case, the divisor only has one digit
		if (divisorSize == 1) {
			// https://janmr.com/blog/2012/11/basic-multiple-precision-short-division/
			u64 remain = 0;
			for (i32 k = i32(dividend.size - 1); k >= 0; k--) {
				u64 quot = _udiv128(remain, dividend.data[k], divisor.data[0], &remain);
				if (quotientOut && k < quotientOut->cap) {
					quotientOut->data[k] = quot;
				}
			}
			if (quotientOut) {
				find_new_size(*quotientOut, dividend.size);
				quotientOut->negative = false;
			}
			assign(dividendAndRemainderOut, remain);
			return;
		}

		// General multi digit by multi digit division
		// https://janmr.com/blog/2014/04/basic-multiple-precision-long-division/
		// Helpful resource
		// https://ridiculousfish.com/blog/posts/labor-of-division-episode-iv.html
		// Page 272 of Knuth's The Art of Computer Programming, Volume 2.
		// https://seriouscomputerist.atariverse.com/media/pdf/book/Art%20of%20Computer%20Programming%20-%20Volume%202%20(Seminumerical%20Algorithms).pdf

		i32 dividendSize = is_zero(dividend) ? 0 : dividend.size;
		dividend.data[dividendSize] = 0;

		// Normalization step, shift both numbers left so that the divisor has a 1 in the high bit
		// This way the quotient estimate will be off by at most 2
		u64 shift = __lzcnt64(divisor.data[divisorSize - 1]);
		if (shift != 0) {
			for (i32 i = i32(divisorSize - 1); i > 0; i--) {
				divisor.data[i] = (divisor.data[i] << shift) | (divisor.data[i - 1] >> (64 - shift));
			}
			divisor.data[0] = divisor.data[0] << shift;


			for (i32 i = i32(dividendSize); i > 0; i--) {
				dividend.data[i] = (dividend.data[i] << shift) | (dividend.data[i - 1] >> (64 - shift));
			}
			dividend.data[0] = dividend.data[0] << shift;
			if (dividend.data[dividendSize] != 0) {
				dividendSize++;
				dividend.data[dividendSize] = 0;
			}
		}

		if (quotientOut) {
			zero(*quotientOut);
		}

		// Do long division
		for (u32 i = dividendSize; i >= divisorSize; i--) {
			// Take a guess at the quotient digit by dividing the leading digits of the dividend and divisor
			// Because of the normalization, it's guaranteed to be at most 2 greater than the real quotient digit, and not less.
			u64 remainderHat;
			u64 quotientHat;
			if (dividend.data[i] == divisor.data[divisorSize - 1]) {
				// udiv128 will throw an overflow exception if the quotient is greater than max uint64, so we have to handle the quotient == base step here
				quotientHat = 0xFFFFFFFFFFFFFFFF;
				u64 remainderHigh = dividend.data[i];
				remainderHat = dividend.data[i - 1] + divisor.data[divisorSize - 1];
			} else {
				quotientHat = _udiv128(dividend.data[i], dividend.data[i - 1], divisor.data[divisorSize - 1], &remainderHat);
			}

			// Refine the tentative quotient digit using the next digits, this will get it within 1 of the real quotient digit
			// The second article linked does a good job of explaining this
			u64 qHatDHi;
			// qHat * v[n-2] (in Algo D)
			u64 qHatDLo = _umul128(quotientHat, divisor.data[divisorSize - 2], &qHatDHi);
			u64 oldRemainderHat = remainderHat;
			// qHat * v[n-2] > b * rHat + u[j + n - 2] (in Algo D)
			if (qHatDHi > remainderHat || (qHatDHi == remainderHat && qHatDLo > dividend.data[i - 2])) {
				// Actually I have no idea if this is correct or not since I was not able to find a number that triggers it
				// and I don't feel like downloading a real big integer library to do automated random tests
				quotientHat--;
				remainderHat += divisor.data[divisorSize - 1];
			}
			// "repeat this test if rHat < b"
			if (oldRemainderHat < remainderHat) {
				qHatDLo = _umul128(quotientHat, divisor.data[divisorSize - 2], &qHatDHi);
				if (qHatDHi > remainderHat || (qHatDHi == remainderHat && qHatDLo > dividend.data[i - 2])) {
					quotientHat--;
					remainderHat += divisor.data[divisorSize - 1];
				}
			}

			// At this point the tentative quotient digit is at most 1 greater (which is a rare case)
			// Long division step, dividend = dividend - quotientDigit * divisor;
			u64 multCarry = 0;
			u64 borrow = 0;
			for (u32 j = 0; j < divisorSize; j++) {
				u64 mulHi;
				u64 mulLo = _umul128(quotientHat, divisor.data[j], &mulHi);
				u64 newMulLo = mulLo + multCarry;
				if (newMulLo < mulLo) {
					mulHi++;
				}
				u32 dividendIdx = i - divisorSize + j;

				u64 subtraction = dividend.data[dividendIdx] - newMulLo;
				u64 oldBorrow = borrow;
				borrow = subtraction > dividend.data[dividendIdx];
				subtraction -= oldBorrow;
				borrow |= subtraction > dividend.data[dividendIdx];
				dividend.data[dividendIdx] = subtraction;
				multCarry = mulHi;
			}
			u64 subtraction = dividend.data[i] - multCarry;
			u64 oldBorrow = borrow;
			borrow = subtraction > dividend.data[i];
			subtraction -= oldBorrow;
			borrow |= subtraction > dividend.data[i];
			dividend.data[i] = subtraction;

			if (borrow != 0) {
				// Ended up negative, quotient was 1 too high, so add a divisor again
				quotientHat--;
				// I hope this add back is correct now.
				u64 carry = 0;
				for (u32 j = 0; j <= divisorSize; j++) {
					u32 dividendIdx = i - divisorSize + j;
					u64 divisorData = j < divisor.size ? divisor.data[j] : 0;
					u64 oldCarry = carry;
					u64 sum = dividend.data[dividendIdx] + divisorData;
					carry = sum < dividend.data[dividendIdx];
					sum += oldCarry;
					carry |= sum < dividend.data[dividendIdx];
					dividend.data[dividendIdx] = sum;
				}
			}
			if (quotientOut) {
				u32 idx = i - divisorSize;
				if (idx < quotientOut->cap) {
					quotientOut->data[idx] = quotientHat;
					if (quotientHat != 0) {
						quotientOut->size = max(quotientOut->size, idx + 1);
					}
				}
			}
		}
		// Shift remainder and divisor back
		if (shift != 0) {
			for (u32 i = 0; i < divisorSize - 1; i++) {
				divisor.data[i] = (divisor.data[i] >> shift) | (divisor.data[i + 1] << (64 - shift));
			}
			divisor.data[divisorSize - 1] >>= shift;

			for (u32 i = 0; i < dividendSize - 1; i++) {
				dividend.data[i] = (dividend.data[i] >> shift) | (dividend.data[i + 1] << (64 - shift));
			}
			dividend.data[dividendSize - 1] >>= shift;
			if (dividend.data[dividendSize - 1] == 0) {
				dividendSize--;
			}
		}

		find_new_size(dividendAndRemainderOut, dividendAndRemainderOut.size);
		// Quotient, if present, now contains the quotient, while dividend contains the remainder. Long division finished!
	}

	static void modular_inverse(BigInteger& num, BigInteger& mod) {
		// Basically straight out of the wikipedia code for the extended euclidian modular inverse
		// https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm

		// r = mod, newR = num
		BigInteger r; r.init(mod.cap);
		assign(r, mod);
		BigInteger& newR = num;
		// t = 0, newT = 1
		BigInteger t; r.init(num.cap);
		BigInteger newT; newT.init(num.cap);
		newT.data[0] = 1;
		// temp storage vars
		BigInteger quotient; quotient.init(num.cap);
		BigInteger tmp; tmp.init(num.cap);

		BigInteger* rPtr = &r;
		BigInteger* newRPtr = &newR;
		BigInteger* tPtr = &t;
		BigInteger* newTPtr = &newT;

		while (!is_zero(*newRPtr)) {
			assign(tmp, *rPtr);
			div(tmp, *newRPtr, &quotient);

			mul(tmp, quotient, *newTPtr);
			sub(*tPtr, *tPtr, tmp);
			swap(tPtr, newTPtr);

			mul(tmp, quotient, *newRPtr);
			sub(*rPtr, *rPtr, tmp);
			swap(rPtr, newRPtr);
		}
		// Check if r is greater than 1
		if (rPtr->size > 1 || rPtr->data[0] > 1) {
			// No inverse
			zero(num);
			return;
		}
		if (tPtr->negative) {
			add(num, *tPtr, mod);
		} else {
			assign(num, *tPtr);
		}
	}

	static void greatest_common_divisor(BigInteger& result, BigInteger& aIn, BigInteger& bIn) {
		BigInteger a; a.init(64);
		BigInteger b; b.init(64);
		BigInteger* pA = &a;
		BigInteger* pB = &b;
		assign(a, aIn);
		assign(b, bIn);

		while (!is_zero(*pB)) {
			div(*pA, *pB, nullptr);
			swap(pA, pB);
		}
		assign(result, *pA);
	}

	static void least_common_multiple(BigInteger& result, BigInteger& a, BigInteger& b) {
		BigInteger gcd; gcd.init(result.cap);
		BigInteger ab; ab.init(result.cap);
		greatest_common_divisor(gcd, a, b);
		mul(ab, a, b);
		div(ab, gcd, &result);
	}

	// Clever little algorithm from Colin Plumb, found here. The number of modulo bits doubles each iteration
	// https://groups.google.com/g/sci.crypt/c/UI-UMbUnYGk/m/hX2-wQVyE3oJ?pli=1
	// This basically follows the identity ax === 1 (mod 2^k) implies ax(2 - ax) === 1 (mod 2^(2k))\
	// 
	// Derivation for this:
	// ax === 1 (mod 2^k)
	// ax == 1 + b(2^k)
	// ax - 1 == b(2^k)
	// a^2x^2 - 2ax + 1 == b^2(2^(2k))
	// -a^2x^2 + 2ax == 1 - b^2(2^(2k))
	// ax(-ax + 2) == 1 - b^2(2^(2k))
	// ax(2 - ax) === 1 (mod 2^(2k))
	// 
	// Doing this iteratively, with x = 2x - ax^2, doubles the number of modulo power of 2 bits each time
	static u64 digit_negative_inverse(u64 digit) {
		// 4 bit inverse
		u64 x = ((((digit << 1) ^ digit) & 4) << 1) ^ digit;
		// 8 bit inverse
		x += x - digit * x * x;
		// 16 bit inverse
		x += x - digit * x * x;
		// 32 bit inverse
		x += x - digit * x * x;
		// 64 bit inverse
		x += x - digit * x * x;
		return U64_MAX - x + 1;
	}

	static u32 digit_negative_inverse32(u32 digit) {
		// 4 bit inverse
		u32 x = ((((digit << 1) ^ digit) & 4) << 1) ^ digit;
		// 8 bit inverse
		x += x - digit * x * x;
		// 16 bit inverse
		x += x - digit * x * x;
		// 32 bit inverse
		x += x - digit * x * x;
		return U32_MAX - x + 1;
	}

	static u64 bit_at(BigInteger& val, u32 idx) {
		u32 wordIdx = idx >> 6;
		u32 bitIdx = idx & 0b111111;
		return (val.data[wordIdx] >> bitIdx) & 1;
	}

	// I think I've finally found a big number library that doesn't suck and is commented (thank you, seems to be the only one around that has good comments), good resource that helped understanding this
	// http://philzimmermann.com/EN/bnlib/index.html
	// Cool paper that talks a little about why the single digit inverse works
	// https://link.springer.com/content/pdf/10.1007/3-540-46877-3_21.pdf
	// This implementation is essentially the "Improvement 1" algorithm from the paper.
	// It's a lot simpler and looks cleaner than the later improvements, and I'm not going for an optimized library here
	static void montgomery_redc(BigInteger& toReduce, BigInteger& modulo, u64 inverse) {
		u32 rLength = modulo.size;
		u32 modLength = modulo.size;

		// Do the T += mod * negativeModInverse * T[i] step
		u64 carry = 0;
		for (u32 i = 0; i < modLength; i++) {
			carry = fmadd_single(toReduce, modulo, inverse * (i < toReduce.size ? toReduce.data[i] : 0), i);
		}

		// Divide by r
		word_shift_right(toReduce, rLength);
		if (carry != 0) {
			toReduce.data[toReduce.size++] = carry;
		}

		// Make sure we're still in modular space
		if (cmp(toReduce, modulo) >= 0) {
			sub(toReduce, toReduce, modulo);
		}
	}

	static void montgomery_add(BigInteger& result, BigInteger& a, BigInteger& b, BigInteger& mod) {
		add(result, a, b);
		if (result.negative) {
			add(result, result, mod);
		} else if (cmp(result, mod) >= 0) {
			sub(result, result, mod);
		}
	}

	static void montgomery_sub(BigInteger& result, BigInteger& a, BigInteger& b, BigInteger& mod) {
		sub(result, a, b);
		if (result.negative) {
			add(result, result, mod);
		} else if (cmp(result, mod) >= 0) {
			sub(result, result, mod);
		}
	}

	static void montgomery_mul(BigInteger& result, BigInteger& a, BigInteger& b, BigInteger& mod, u64 inv) {
		mul(result, a, b);
		montgomery_redc(result, mod, inv);
	}

	static void montgomery_lshift(BigInteger& n, u32 amount, BigInteger& mod) {
		amount &= 63;
		for (u32 i = 0; i < amount; i++) {
			bit_shift_left(n, 1);
			if (cmp(n, mod) >= 0) {
				sub(n, n, mod);
			}
		}
	}

	// pow_mod but specficitally for a power of 2. Used for the fermat test in the prime number generator
	// I probably don't need a specialized version just for this, but what the heck, might as well
	static void pow_mod_power_of_2(BigInteger& result, BigInteger& power, BigInteger& mod) {
		if (is_zero(power)) {
			assign(result, 1);
			return;
		}

		/*BigInteger localPower{power.cap};
		assign(localPower, power);

		u64 shiftCount = bitlength(mod) - 1;
		if (shiftCount > 0) {
			shiftCount = min(shiftCount-1, localPower.size > 1 ? 0xFFFFFFFFFFFFFFFF : localPower.data[0]);
			u64 idx = 0;
			u64 dataShift = shiftCount;
			while (dataShift >= 64) {
				result.data[idx] = 0;
				dataShift -= 64;
				idx++;
			}
			result.data[idx] = 1ui64 << dataShift;
			result.size = idx + 1;
			sub_single(localPower, localPower, shiftCount);
		}

		if (is_zero(localPower)) {
			return;
		}*/

		// Convert to montgomery form
		assign(result, 2);
		word_shift_left(result, mod.size);
		div(result, mod, nullptr);
		u64 inverse = digit_negative_inverse(mod.data[0]);

		BigInteger tmp; tmp.init(result.cap);
		BigInteger* pResult = &result;
		BigInteger* pTmp = &tmp;

		u32 bitStart = 63 - _lzcnt_u64(power.data[power.size - 1]);
		bitStart--;
		for (i32 i = power.size - 1; i >= 0; i--) {
			u64 word = power.data[i];
			for (i32 bitIdx = bitStart; bitIdx >= 0; bitIdx--) {
				// Square
				montgomery_mul(*pTmp, *pResult, *pResult, mod, inverse);
				swap(pTmp, pResult);
				// Multiply
				u64 bit = (word >> bitIdx) & 1;
				if (bit) {
					u64 carry = bit_shift_left(*pResult, 1);
					if (carry || cmp(*pResult, mod) == 1) {
						sub(*pResult, *pResult, mod);
					}
				}
			}
			bitStart = 63;
		}

		// Convert back to regular form
		montgomery_redc(*pResult, mod, inverse);
		if (pResult != &result) {
			assign(result, *pResult);
		}
	}

	// Faster version of pow_mod using montgomery form to avoid heavy divisions. Modulo must be odd
	// Still as fast as it could be. A couple "easy" opetimizations would be to use windowed multiplication and optimized squaring
	static void pow_mod_montgomery(BigInteger& num, BigInteger& power, BigInteger& mod) {
		if (is_zero(power)) {
			assign(num, 1);
			return;
		}
		// Convert to montgomery form
		word_shift_left(num, mod.size);
		div(num, mod, nullptr);

		u64 inverse = digit_negative_inverse(mod.data[0]);

		BigInteger result; result.init(num.cap);
		assign(result, num);
		BigInteger tmp; tmp.init(num.cap);

		BigInteger* pResult = &result;
		BigInteger* pNum = &num;
		BigInteger* pTmp = &tmp;

		u32 bitStart = 63 - _lzcnt_u64(power.data[power.size - 1]);
		// First iteration is done by the assignment
		bitStart--;
		for (i32 i = power.size - 1; i >= 0; i--) {
			u64 digit = power.data[i];
			for (i32 j = bitStart; j >= 0; j--) {
				montgomery_mul(*pTmp, *pResult, *pResult, mod, inverse);
				swap(pResult, pTmp);

				u64 bit = (digit >> j) & 1;
				if (bit) {
					montgomery_mul(*pTmp, *pNum, *pResult, mod, inverse);
					swap(pResult, pTmp);
				}
			}
			bitStart = 63;
		}
		// Convert back to regular form
		montgomery_redc(*pResult, mod, inverse);
		//std::cout << "MRS: " << *pResult << '\n';
		assign(num, *pResult);
	}

	static void pow_mod(BigInteger& num, BigInteger& power, BigInteger& mod) {
		BigInteger squares; squares.init(num.cap);
		assign(squares, num);
		num.size = 1;
		num.data[0] = 1;
		BigInteger tmp; tmp.init(num.cap);

		BigInteger* tmpPtr = &tmp;
		BigInteger* squaresPtr = &squares;
		BigInteger* numPtr = &num;

		for (u32 i = 0; i < power.size; i++) {
			u64 exp = power.data[i];
			for (u32 j = 0; j < 64; j++) {
				if (i == (power.size - 1) && exp == 0) {
					break;
				}
				if (exp & 1) {
					mul(*tmpPtr, *squaresPtr, *numPtr);
					div(*tmpPtr, mod, nullptr);
					swap(tmpPtr, numPtr);
				}
				mul(*tmpPtr, *squaresPtr, *squaresPtr);
				div(*tmpPtr, mod, nullptr);
				swap(tmpPtr, squaresPtr);
				exp >>= 1;
			}
		}
		if (numPtr != &num) {
			assign(num, *numPtr);
		}
	}

	static void get_crypto_random(BigInteger& num, u32 byteCount) {
		u64 randData[4];
		// 256 bits of real random data ought to be enough, right?
		while (_rdrand64_step(&randData[0]) == 0);
		while (_rdrand64_step(&randData[1]) == 0);
		while (_rdrand64_step(&randData[2]) == 0);
		while (_rdrand64_step(&randData[3]) == 0);
		Keccak k{};
		k.init();
		k.absorb(reinterpret_cast<const u8*>(randData), 4 * sizeof(u64));
		k.squeeze(reinterpret_cast<u8*>(num.data), byteCount);
		num.size = (byteCount + 7) / sizeof(u64);
	}

	static constexpr u32 NUM_TEST_PRIMES = 99;
	static constexpr u64 SMALL_PRIME_NUMBERS[NUM_TEST_PRIMES]{
		3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
		73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
		179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
		283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
		419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541
	};
	static constexpr u64 RABIN_MILLER_ITERATIONS = 8;
	static constexpr u64 RABIN_MILLER_BASES[RABIN_MILLER_ITERATIONS]{ 2, 3, 5, 7, 11, 13, 17, 19 };

	static bool primality_test(BigInteger& num) {
		if (!(num.data[0] & 1)) {
			return false;
		}

		BigInteger base; base.init(num.cap);
		BigInteger millerNum; millerNum.init(num.cap);
		BigInteger millerTmp; millerTmp.init(num.cap);
		BigInteger& fermatResult{ millerNum };
		BigInteger& fermatSub1{ millerTmp };

		BigInteger* basePtr = &base;
		BigInteger* millerTmpPtr = &millerTmp;

		// Test first few primes up to a couple hundred
		// TODO replace with a sieve
		for (u32 i = 0; i < NUM_TEST_PRIMES; i++) {
			if (div_single_rem(num, SMALL_PRIME_NUMBERS[i]) == 0) {
				return false;
			}
		}

		//std::cout << "Small prime pass\n";

		// Fermat test
		sub_single(fermatSub1, num, 1);
		pow_mod_power_of_2(fermatResult, fermatSub1, num);
		if (!is_equal(fermatResult, 1)) {
			return false;
		}
		//std::cout << "Fermat pass\n";

		// Miller rabin iterations
		for (u32 i = 0; i < RABIN_MILLER_ITERATIONS; i++) {
			assign(millerNum, num);
			millerNum.data[0] -= 1;
			u64 trailingZeros = count_trailing_zeros(millerNum);
			bit_shift_right(millerNum, trailingZeros);

			assign(*basePtr, RABIN_MILLER_BASES[i]);
			pow_mod_montgomery(*basePtr, millerNum, num);
			for (u32 s = 0; s < trailingZeros; s++) {
				mul(*millerTmpPtr, *basePtr, *basePtr);
				div(*millerTmpPtr, num, nullptr);

				if (is_equal(*millerTmpPtr, 1) && !is_equal(*basePtr, 1)) {
					num.data[0]--;
					if (cmp(*basePtr, num) != 0) {
						num.data[0]++;
						return false;
					}
					num.data[0]++;
				}

				swap(basePtr, millerTmpPtr);
			}
			if (!is_equal(*basePtr, 1)) {
				return false;
			}
			//std::cout << "Miller pass iteration " << i << '\n';
		}

		// If all of the above passed it's probably a prime
		return true;
	}

	static void gen_large_prime(BigInteger& num, u32 bytes) {
		Keccak k;
		k.make_secure_random();
		k.squeeze(reinterpret_cast<u8*>(num.data), bytes);
		num.size = (bytes + 7) / 8;
		num.data[0] |= 1;

		BigInteger base; base.init(num.cap);
		BigInteger millerNum; millerNum.init(num.cap);
		BigInteger millerTmp; millerTmp.init(num.cap);
		BigInteger& fermatResult{ millerNum };
		BigInteger& fermatSub1{ millerTmp };

		BigInteger* basePtr = &base;
		BigInteger* millerTmpPtr = &millerTmp;

		while (true) {
			k.squeeze(reinterpret_cast<u8*>(num.data), bytes);
			// Make odd
			num.data[0] |= 1;
			// Set top two bits so that this number will always be the correct number of bits long (if a 512 bit prime was requested, it will be a 512 bit number, not a 511 or less bit number)
			// Also set the next bit so multiplying two of them will result in the top bit being set (useful for RSA where the primes are multiplied)
			num.data[num.size - 1] |= 0b11000000ui64 << (((bytes - 1) & 7) * 8);
			// Test first few primes up to a couple hundred
			// TODO replace with a sieve
			for (u32 i = 0; i < NUM_TEST_PRIMES; i++) {
				if (div_single_rem(num, SMALL_PRIME_NUMBERS[i]) == 0) {
					goto composite;
				}
			}

			//std::cout << "Small prime pass\n";

			// Fermat test
			sub_single(fermatSub1, num, 1);
			pow_mod_power_of_2(fermatResult, fermatSub1, num);
			if (!is_equal(fermatResult, 1)) {
				goto composite;
			}
			//std::cout << "Fermat pass\n";

			// Miller rabin iterations
			for (u32 i = 0; i < RABIN_MILLER_ITERATIONS; i++) {
				assign(millerNum, num);
				millerNum.data[0] -= 1;
				u64 trailingZeros = count_trailing_zeros(millerNum);
				bit_shift_right(millerNum, trailingZeros);

				assign(*basePtr, RABIN_MILLER_BASES[i]);
				pow_mod_montgomery(*basePtr, millerNum, num);
				for (u32 s = 0; s < trailingZeros; s++) {
					mul(*millerTmpPtr, *basePtr, *basePtr);
					div(*millerTmpPtr, num, nullptr);

					if (is_equal(*millerTmpPtr, 1) && !is_equal(*basePtr, 1)) {
						num.data[0]--;
						if (cmp(*basePtr, num) != 0) {
							num.data[0]++;
							goto composite;
						}
						num.data[0]++;
					}

					swap(basePtr, millerTmpPtr);
				}
				if (!is_equal(*basePtr, 1)) {
					goto composite;
				}
				//std::cout << "Miller pass iteration " << i << '\n';
			}

			// If all of the above passed it's probably a prime
			break;

		composite:;
		}
	}

	bool operator==(u64 ui) {
		return !negative && size == 1 && ui == data[0];
	}

	bool operator!=(u64 ui) {
		return negative || size != 1 || ui != data[0];
	}
};
