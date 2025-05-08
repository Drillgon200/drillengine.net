#pragma once

#include "BigInteger.h"

#define SECP256R1_KEY_SIZE 32

#pragma pack(push, 1)
struct SECP256R1PrivateKey {
	U8 n[SECP256R1_KEY_SIZE];
};

struct SECP256R1PublicKey {
	U8 x[SECP256R1_KEY_SIZE];
	U8 y[SECP256R1_KEY_SIZE];
};

struct SECP256R1Key {
	SECP256R1PublicKey publicKey;
	SECP256R1PrivateKey privateKey;
};
#pragma pack(pop)

struct ECParams {
	BigInteger curveA;
	BigInteger curveB;
	BigInteger order;
	BigInteger generatorX;
	BigInteger generatorY;
	BigInteger prime;

	void init() {
		curveA.init(64);
		curveB.init(64);
		order.init(64);
		generatorX.init(64);
		generatorY.init(64);
		prime.init(64);
	}
};

bool initialized;

ECParams EC_SECP256r1_PARAMS;
ECParams EC_SECP384r1_PARAMS;
ECParams EC_SECP521r1_PARAMS;

// SEC 2: Recommended Elliptic Curve Domain Parameters
// https://www.secg.org/SEC2-Ver-1.0.pdf
inline void ec_init() {
	if (initialized) {
		return;
	}

	EC_SECP256r1_PARAMS.init();
	BigInteger::assign_hex(EC_SECP256r1_PARAMS.curveA, "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC");
	BigInteger::assign_hex(EC_SECP256r1_PARAMS.curveB, "5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B");
	BigInteger::assign_hex(EC_SECP256r1_PARAMS.order, "FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551");
	BigInteger::assign_hex(EC_SECP256r1_PARAMS.generatorX, "6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296");
	BigInteger::assign_hex(EC_SECP256r1_PARAMS.generatorY, "4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5");
	BigInteger::assign_hex(EC_SECP256r1_PARAMS.prime, "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF");

	EC_SECP384r1_PARAMS.init();
	BigInteger::assign_hex(EC_SECP384r1_PARAMS.curveA, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC");
	BigInteger::assign_hex(EC_SECP384r1_PARAMS.curveB, "B3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A2ED19D2A85C8EDD3EC2AEF");
	BigInteger::assign_hex(EC_SECP384r1_PARAMS.order, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973");
	BigInteger::assign_hex(EC_SECP384r1_PARAMS.generatorX, "AA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF55296C3A545E3872760AB7");
	BigInteger::assign_hex(EC_SECP384r1_PARAMS.generatorY, "3617DE4A96262C6F5D9E98BF9292DC29F8F41DBD289A147CE9DA3113B5F0B8C00A60B1CE1D7E819D7A431D7C90EA0E5F");
	BigInteger::assign_hex(EC_SECP384r1_PARAMS.prime, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF");

	EC_SECP521r1_PARAMS.init();
	BigInteger::assign_hex(EC_SECP521r1_PARAMS.curveA, "01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC");
	BigInteger::assign_hex(EC_SECP521r1_PARAMS.curveB, "0051953EB9618E1C9A1F929A21A0B68540EEA2DA725B99B315F3B8B489918EF109E156193951EC7E937B1652C0BD3BB1BF073573DF883D2C34F1EF451FD46B503F00");
	BigInteger::assign_hex(EC_SECP521r1_PARAMS.order, "01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA51868783BF2F966B7FCC0148F709A5D03BB5C9B8899C47AEBB6FB71E91386409");
	BigInteger::assign_hex(EC_SECP521r1_PARAMS.generatorX, "00C6858E06B70404E9CD9E3ECB662395B4429C648139053FB521F828AF606B4D3DBAA14B5E77EFE75928FE1DC127A2FFA8DE3348B3C1856A429BF97E7E31C2E5BD66");
	BigInteger::assign_hex(EC_SECP521r1_PARAMS.generatorY, "011839296A789A3BC0045C8A5FB42C7D1BD998F54449579B446817AFBD17273E662C97EE72995EF42640C550B9013FAD0761353C7086A272C24088BE94769FD16650");
	BigInteger::assign_hex(EC_SECP521r1_PARAMS.prime, "01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

	initialized = true;
}

// Verify that y^2 = x^3 + a*x + b
inline bool ec_check_point_on_curve(BigInteger& x, BigInteger& y, BigInteger& a, BigInteger& b, BigInteger& modulo) {
	BigInteger leftSide; leftSide.init(x.cap);
	BigInteger rightSide; rightSide.init(x.cap);
	BigInteger tmp; tmp.init(x.cap);

	BigInteger::mul(leftSide, y, y);

	BigInteger::mul(tmp, x, x);
	BigInteger::mul(rightSide, tmp, x);
	BigInteger::mul(tmp, a, x);
	BigInteger::add(rightSide, rightSide, tmp);
	BigInteger::add(rightSide, rightSide, b);

	BigInteger::div(leftSide, modulo, nullptr);
	BigInteger::div(rightSide, modulo, nullptr);

	return BigInteger::cmp(leftSide, rightSide) == 0;
}

inline void ec_convert_from_homogenous_coords(BigInteger& x, BigInteger& y, BigInteger& z, BigInteger& mod) {
	BigInteger tmp; tmp.init(x.cap);
	BigInteger::modular_inverse(z, mod);
	

	BigInteger::mul(tmp, x, z);
	BigInteger::div(tmp, mod, nullptr);
	BigInteger::assign(x, tmp);

	BigInteger::mul(tmp, y, z);
	BigInteger::div(tmp, mod, nullptr);
	BigInteger::assign(y, tmp);
}

// RFC6090, Appendix F.2
// Not necessarily the fastest way to do it, there are ones that use fewer multiplications here
// http://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian.html
// coordinates are in homogenous space, with each number in montgomery form
inline void ec_point_mul(BigInteger& x1, BigInteger& y1, BigInteger& z1, BigInteger& x2, BigInteger& y2, BigInteger& z2, BigInteger& curveA, BigInteger& mod, U64 inverse) {
	if (BigInteger::is_zero(z2)) {
		return;
	}

	if (BigInteger::is_zero(z1)) {
		BigInteger::assign(x1, x2);
		BigInteger::assign(y1, y2);
		BigInteger::assign(z1, z2);
		return;
	}

	BigInteger u; u.init(x1.cap);
	BigInteger v; v.init(x1.cap);
	BigInteger tmp; tmp.init(x1.cap);

	BigInteger::montgomery_mul(u, y2, z1, mod, inverse);
	BigInteger::montgomery_mul(tmp, y1, z2, mod, inverse);
	BigInteger::montgomery_sub(u, u, tmp, mod);

	BigInteger::montgomery_mul(v, x2, z1, mod, inverse);
	BigInteger::montgomery_mul(tmp, x1, z2, mod, inverse);
	BigInteger::montgomery_sub(v, v, tmp, mod);

	if (!BigInteger::is_zero(u) && BigInteger::is_zero(v)) {
		BigInteger::zero(x1);
		BigInteger::assign(x2, 1);
		BigInteger::zero(z1);
		return;
	}

	BigInteger xOut; xOut.init(x1.cap);
	BigInteger yOut; yOut.init(x1.cap);
	BigInteger zOut; zOut.init(x1.cap);
	BigInteger inner; inner.init(x1.cap);
	if (!BigInteger::is_zero(u) && !BigInteger::is_zero(v)) {
		BigInteger vSquared; vSquared.init(x1.cap);
		BigInteger vCubed; vCubed.init(x1.cap);
		BigInteger uSquared; uSquared.init(x1.cap);
		BigInteger uCubed; uCubed.init(x1.cap);

		// Generate powers of u/v variables
		BigInteger::montgomery_mul(vSquared, v, v, mod, inverse);
		BigInteger::montgomery_mul(vCubed, vSquared, v, mod, inverse);
		BigInteger::montgomery_mul(uSquared, u, u, mod, inverse);
		BigInteger::montgomery_mul(uCubed, uSquared, u, mod, inverse);

		// inner = Z1 * u^2 - 2 * X1 * V^2
		BigInteger::montgomery_mul(inner, z1, uSquared, mod, inverse);
		BigInteger::montgomery_mul(tmp, x1, vSquared, mod, inverse);
		BigInteger::montgomery_lshift(tmp, 1, mod);
		BigInteger::montgomery_sub(inner, inner, tmp, mod);

		// X3 = v * (Z2 * inner - v^3)
		BigInteger::montgomery_mul(tmp, inner, z2, mod, inverse);
		BigInteger::montgomery_sub(tmp, tmp, vCubed, mod);
		BigInteger::montgomery_mul(xOut, tmp, v, mod, inverse);

		// inner = 3 * x1 * u * v^2 - y1 * v^3 - z1 * u^3
		BigInteger::montgomery_mul(tmp, u, vSquared, mod, inverse);
		BigInteger::montgomery_mul(inner, tmp, x1, mod, inverse);
		BigInteger::assign(tmp, inner);
		BigInteger::montgomery_lshift(inner, 1, mod);
		BigInteger::montgomery_add(inner, inner, tmp, mod);
		BigInteger::montgomery_mul(tmp, y1, vCubed, mod, inverse);
		BigInteger::montgomery_sub(inner, inner, tmp, mod);
		BigInteger::montgomery_mul(tmp, z1, uCubed, mod, inverse);
		BigInteger::montgomery_sub(inner, inner, tmp, mod);

		// Y3 = Z2 * inner + u * v^3
		BigInteger::montgomery_mul(tmp, inner, z2, mod, inverse);
		BigInteger::montgomery_mul(inner, u, vCubed, mod, inverse);
		BigInteger::montgomery_add(yOut, tmp, inner, mod);

		// Z3 = v^3 * z1 * z2
		BigInteger::montgomery_mul(inner, vCubed, z1, mod, inverse);
		BigInteger::montgomery_mul(zOut, inner, z2, mod, inverse);
	} else {
		// Else P2 == P1, point doubling

		BigInteger w; w.init(x1.cap);
		BigInteger y1Squared; y1Squared.init(x1.cap);
		// w = 3 * x1^2 + a * z1 ^ 2
		BigInteger::montgomery_mul(tmp, x1, x1, mod, inverse);
		BigInteger::assign(w, tmp);
		BigInteger::montgomery_lshift(w, 1, mod);
		BigInteger::montgomery_add(w, w, tmp, mod);
		BigInteger::montgomery_mul(inner, z1, z1, mod, inverse);
		BigInteger::montgomery_mul(tmp, inner, curveA, mod, inverse);
		BigInteger::montgomery_add(w, w, tmp, mod);

		// inner = w^2 - 8 * X1 * Y1^2 * Z1
		BigInteger::montgomery_mul(y1Squared, y1, y1, mod, inverse);
		BigInteger::montgomery_mul(inner, x1, y1Squared, mod, inverse);
		BigInteger::montgomery_mul(tmp, inner, z1, mod, inverse);
		BigInteger::montgomery_lshift(tmp, 3, mod);
		BigInteger::montgomery_mul(inner, w, w, mod, inverse);
		BigInteger::montgomery_sub(inner, inner, tmp, mod);

		// X3 = 2 * Y1 * Z1 * inner
		BigInteger::montgomery_mul(tmp, inner, z1, mod, inverse);
		BigInteger::montgomery_mul(xOut, tmp, y1, mod, inverse);
		BigInteger::montgomery_lshift(xOut, 1, mod);

		// inner = 3 * w * X1 - 2 * Y1^2 * Z1
		BigInteger::montgomery_mul(tmp, w, x1, mod, inverse);
		BigInteger::assign(inner, tmp);
		BigInteger::montgomery_lshift(inner, 1, mod);
		BigInteger::montgomery_add(inner, inner, tmp, mod);
		BigInteger::montgomery_mul(tmp, y1Squared, z1, mod, inverse);
		BigInteger::montgomery_lshift(tmp, 1, mod);
		BigInteger::montgomery_sub(inner, inner, tmp, mod);

		// Y3 = 4 * Y1^2 * Z1 * inner - w^3
		BigInteger::montgomery_mul(tmp, inner, z1, mod, inverse);
		BigInteger::montgomery_mul(yOut, tmp, y1Squared, mod, inverse);
		BigInteger::montgomery_lshift(yOut, 2, mod);
		BigInteger::montgomery_mul(tmp, w, w, mod, inverse);
		BigInteger::montgomery_mul(inner, tmp, w, mod, inverse);
		BigInteger::montgomery_sub(yOut, yOut, inner, mod);

		// Z3 = 8 * (Y1 * Z1)^3
		BigInteger::montgomery_mul(tmp, y1, z1, mod, inverse);
		BigInteger::montgomery_mul(inner, tmp, tmp, mod, inverse);
		BigInteger::montgomery_mul(zOut, inner, tmp, mod, inverse);
		BigInteger::montgomery_lshift(zOut, 3, mod);
	}

	BigInteger::assign(x1, xOut);
	BigInteger::assign(y1, yOut);
	BigInteger::assign(z1, zOut);
}

inline bool ec_power(BigInteger& px, BigInteger& py, BigInteger& curveA, BigInteger& power, BigInteger& modulo) {
	// Convert to montgomery form
	U64 inverse = BigInteger::digit_negative_inverse(modulo.data[0]);
	BigInteger::word_shift_left(px, modulo.size);
	BigInteger::div(px, modulo, nullptr);
	BigInteger::word_shift_left(py, modulo.size);
	BigInteger::div(py, modulo, nullptr);

	BigInteger curveAMontgomery; curveAMontgomery.init(curveA.cap);
	BigInteger::assign(curveAMontgomery, curveA);
	BigInteger::word_shift_left(curveAMontgomery, modulo.size);
	BigInteger::div(curveAMontgomery, modulo, nullptr);

	BigInteger one; one.init(64);
	BigInteger::assign(one, 1);
	BigInteger::word_shift_left(one, modulo.size);
	BigInteger::div(one, modulo, nullptr);

	BigInteger runningPointX; runningPointX.init(px.cap);
	BigInteger runningPointY; runningPointY.init(px.cap);
	BigInteger runningPointZ; runningPointZ.init(px.cap);
	BigInteger::assign(runningPointX, px);
	BigInteger::assign(runningPointY, py);
	BigInteger::assign(runningPointZ, one);


	U32 bitStart = 63 - _lzcnt_u64(power.data[power.size - 1]);
	// Assignment does the first one
	bitStart--;
	for (I32 wordIdx = power.size - 1; wordIdx >= 0; wordIdx--) {
		U64 word = power.data[wordIdx];
		for (I32 bitIdx = bitStart; bitIdx >= 0; bitIdx--) {
			// Square
			ec_point_mul(runningPointX, runningPointY, runningPointZ, runningPointX, runningPointY, runningPointZ, curveAMontgomery, modulo, inverse);

			U64 bit = (word >> bitIdx) & 1;
			if (bit) {
				// Multiply
				ec_point_mul(runningPointX, runningPointY, runningPointZ, px, py, one, curveAMontgomery, modulo, inverse);
			}
		}
		bitStart = 63;
	}

	// Convert out of montgomery form
	BigInteger::montgomery_redc(runningPointX, modulo, inverse);
	BigInteger::montgomery_redc(runningPointY, modulo, inverse);
	BigInteger::montgomery_redc(runningPointZ, modulo, inverse);

	if (BigInteger::is_zero(runningPointZ)) {
		return false;
	}

	ec_convert_from_homogenous_coords(runningPointX, runningPointY, runningPointZ, modulo);
	BigInteger::assign(px, runningPointX);
	BigInteger::assign(py, runningPointY);

	return true;
}

// Computes pointA ^ powerA * pointB ^ powerB
// This is the Strauss-Shamir trick
inline bool ec_two_power_mul(BigInteger& pxA, BigInteger& pyA, BigInteger& pxB, BigInteger& pyB, BigInteger& curveA, BigInteger& powerA, BigInteger& powerB, BigInteger& modulo) {
	// Convert to montgomery form
	U64 inverse = BigInteger::digit_negative_inverse(modulo.data[0]);
	BigInteger::word_shift_left(pxA, modulo.size);
	BigInteger::div(pxA, modulo, nullptr);
	BigInteger::word_shift_left(pyA, modulo.size);
	BigInteger::div(pyA, modulo, nullptr);
	BigInteger pxBMont; pxBMont.init(pxB.cap);
	BigInteger::assign(pxBMont, pxB);
	BigInteger pyBMont; pyBMont.init(pyB.cap);
	BigInteger::assign(pyBMont, pyB);
	BigInteger::word_shift_left(pxBMont, modulo.size);
	BigInteger::div(pxBMont, modulo, nullptr);
	BigInteger::word_shift_left(pyBMont, modulo.size);
	BigInteger::div(pyBMont, modulo, nullptr);

	BigInteger curveAMontgomery; curveAMontgomery.init(curveA.cap);
	BigInteger::assign(curveAMontgomery, curveA);
	BigInteger::word_shift_left(curveAMontgomery, modulo.size);
	BigInteger::div(curveAMontgomery, modulo, nullptr);

	BigInteger one; one.init(64);
	BigInteger::assign(one, 1);
	BigInteger::word_shift_left(one, modulo.size);
	BigInteger::div(one, modulo, nullptr);

	BigInteger aTimesBX; aTimesBX.init(pxA.cap);
	BigInteger aTimesBY; aTimesBY.init(pxA.cap);
	BigInteger aTimesBZ; aTimesBZ.init(pxA.cap);
	BigInteger::assign(aTimesBX, pxA);
	BigInteger::assign(aTimesBY, pyA);
	BigInteger::assign(aTimesBZ, one);
	ec_point_mul(aTimesBX, aTimesBY, aTimesBZ, pxBMont, pyBMont, one, curveA, modulo, inverse);

	BigInteger runningPointX; runningPointX.init(pxA.cap);
	BigInteger runningPointY; runningPointY.init(pxA.cap);
	BigInteger runningPointZ; runningPointZ.init(pxA.cap);
	U32 lengthPowerA = BigInteger::bitlength(powerA);
	U32 lengthPowerB = BigInteger::bitlength(powerB);
	U32 bitStart;
	if (lengthPowerA > lengthPowerB) {
		BigInteger::assign(runningPointX, pxA);
		BigInteger::assign(runningPointY, pyA);
		BigInteger::assign(runningPointZ, one);
		bitStart = 63 - _lzcnt_u64(powerA.data[powerA.size - 1]);
	} else if (lengthPowerA < lengthPowerB) {
		BigInteger::assign(runningPointX, pxBMont);
		BigInteger::assign(runningPointY, pyBMont);
		BigInteger::assign(runningPointZ, one);
		bitStart = 63 - _lzcnt_u64(powerB.data[powerB.size - 1]);
	} else {
		BigInteger::assign(runningPointX, aTimesBX);
		BigInteger::assign(runningPointY, aTimesBY);
		BigInteger::assign(runningPointZ, aTimesBZ);
		bitStart = 63 - _lzcnt_u64(powerA.data[powerA.size - 1]);
	}

	// Assignment does the first one
	bitStart--;
	for (I32 wordIdx = max(powerA.size - 1, powerB.size - 1); wordIdx >= 0; wordIdx--) {
		U64 wordA = wordIdx < powerA.size ? powerA.data[wordIdx] : 0;
		U64 wordB = wordIdx < powerB.size ? powerB.data[wordIdx] : 0;
		for (I32 bitIdx = bitStart; bitIdx >= 0; bitIdx--) {
			// Square
			ec_point_mul(runningPointX, runningPointY, runningPointZ, runningPointX, runningPointY, runningPointZ, curveAMontgomery, modulo, inverse);

			U64 bitA = (wordA >> bitIdx) & 1;
			U64 bitB = (wordB >> bitIdx) & 1;
			// Multiply
			if (bitA == 1 && bitB == 1) {
				ec_point_mul(runningPointX, runningPointY, runningPointZ, aTimesBX, aTimesBY, aTimesBZ, curveAMontgomery, modulo, inverse);
			} else if (bitA == 1 && bitB == 0) {
				ec_point_mul(runningPointX, runningPointY, runningPointZ, pxA, pyA, one, curveAMontgomery, modulo, inverse);
			} else if (bitA == 0 && bitB == 1) {
				ec_point_mul(runningPointX, runningPointY, runningPointZ, pxBMont, pyBMont, one, curveAMontgomery, modulo, inverse);
			}
		}
		bitStart = 63;
	}

	// Convert out of montgomery form
	BigInteger::montgomery_redc(runningPointX, modulo, inverse);
	BigInteger::montgomery_redc(runningPointY, modulo, inverse);
	BigInteger::montgomery_redc(runningPointZ, modulo, inverse);

	if (BigInteger::is_zero(runningPointZ)) {
		return false;
	}

	ec_convert_from_homogenous_coords(runningPointX, runningPointY, runningPointZ, modulo);
	BigInteger::assign(pxA, runningPointX);
	BigInteger::assign(pyA, runningPointY);

	return true;
}

inline bool secp256r1_load_uncompressed(BigInteger& x, BigInteger& y, void* vbytes, U32 bytesLength) {
	if (bytesLength != 1 + 32 + 32) {
		return false;
	}
	U8* bytes = reinterpret_cast<U8*>(vbytes);
	// 4 means uncompressed form
	if (bytes[0] != 0x04) {
		return false;
	}
	BigInteger::assign_bytes_big_endian(x, bytes + 1, 32);
	BigInteger::assign_bytes_big_endian(y, bytes + 1 + 32, 32);
	return true;
}

//TODO refactor these void* into a named struct
inline void secp256r1_generate_keypair(Keccak& random, void* publicKey, void* privateKey) {
	BigInteger& curveOrder = EC_SECP256r1_PARAMS.order;
	BigInteger randomKey; randomKey.init(64);
	BigInteger& curveA = EC_SECP256r1_PARAMS.curveA;
	BigInteger& curveB = EC_SECP256r1_PARAMS.curveB;
	BigInteger& curvePrime = EC_SECP256r1_PARAMS.prime;

	randomKey.size = 4;
	do {
		random.squeeze(randomKey.data, 32);
	} while (BigInteger::cmp(randomKey, curveOrder) >= 0);
	BigInteger::find_new_size(randomKey, 4);

	BigInteger x; x.init(64);
	BigInteger y; y.init(64);
	BigInteger::assign(x, EC_SECP256r1_PARAMS.generatorX);
	BigInteger::assign(y, EC_SECP256r1_PARAMS.generatorY);

	//bool onCurve1 = ec_check_point_on_curve(x, y, curveA, curveB, curvePrime);

	//BigInteger::assign(randomKey, 1);
	ec_power(x, y, curveA, randomKey, curvePrime);

	//bool onCurve = ec_check_point_on_curve(x, y, curveA, curveB, curvePrime);

	BigInteger::write_bytes_big_endian(publicKey, 32, x);
	BigInteger::write_bytes_big_endian(reinterpret_cast<U8*>(publicKey) + 32, 32, y);
	BigInteger::write_bytes_big_endian(privateKey, 32, randomKey);

}

inline bool secp256r1_ecdhe(void* output, const U8* publicX, const U8* publicY, const U8* privateKey) {
	BigInteger x; x.init(64);
	BigInteger y; y.init(64);
	BigInteger power; power.init(64);
	BigInteger::assign_bytes_big_endian(x, publicX, 32);
	BigInteger::assign_bytes_big_endian(y, publicY, 32);
	BigInteger::assign_bytes_big_endian(power, privateKey, 32);

	BigInteger& curveA = EC_SECP256r1_PARAMS.curveA;
	BigInteger& curveB = EC_SECP256r1_PARAMS.curveB;
	BigInteger& curvePrime = EC_SECP256r1_PARAMS.prime;

	if (!ec_check_point_on_curve(x, y, curveA, curveB, curvePrime)) {
		return false;
	}

	bool isPointAtInfinity = !ec_power(x, y, curveA, power, curvePrime);

	if (isPointAtInfinity) {
		return false;
	}

	if (!ec_check_point_on_curve(x, y, curveA, curveB, curvePrime)) {
		return false;
	}

	BigInteger::write_bytes_big_endian(output, 32, x);

	return true;
}