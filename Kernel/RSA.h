#pragma once

#include "BigInteger.h"

inline void rsa_keygen_2048(BigInteger* publicKey, BigInteger* privateKey, BigInteger* modulus) {
	BigInteger p; p.init(64);
	BigInteger q; q.init(64);
	BigInteger& n = *modulus;
	BigInteger lambda; lambda.init(64);
	BigInteger& e = *publicKey;
	BigInteger& d = *privateKey;

	// Wikipedia (and the original RSA paper) says the primes should differ in length by a few digits to make sure a fermat factorization attack can't happen.
	// Thomas Pornin (the guy who wrote BearSSL) doesn't seem to think much of this, so I'll ignore it.
	// 
	// "This "don't make P and Q the same length" rule seems to be the remnants of some old advice of dubious value.
	// The risk of having P and Q close enough to each other to allow Fermat's factorization method is much lower than the risk of
	// making P and Q with a "special" format that allows any other factorization algorithm to become more efficient."
	// - Thomas Pornin on Stack Exchange
	BigInteger::gen_large_prime(p, 1024 / 8);
	BigInteger::gen_large_prime(q, 1024 / 8);
	BigInteger::mul(n, p, q);
	p.data[0]--;
	q.data[0]--;
	BigInteger::least_common_multiple(lambda, p, q);
	BigInteger::assign(e, 65537);
	BigInteger::assign(d, e);
	BigInteger::modular_inverse(d, lambda);
}