#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <iostream>

class BigInteger {
public:
	explicit BigInteger(long long value = 0);
	BigInteger(const BigInteger &);
	BigInteger(BigInteger &&);

	BigInteger & operator +=(const BigInteger &);
	BigInteger operator +(const BigInteger &) const;

	BigInteger & operator -=(const BigInteger &);
	BigInteger operator -(const BigInteger &) const;

private:
	friend std::ostream & operator <<(std::ostream &, const BigInteger &);
	friend std::istream & operator >>(std::ostream &, BigInteger &);
};

std::ostream & operator <<(std::ostream &, const BigInteger &);
std::istream & operator >>(std::ostream &, BigInteger &);

#endif // BIGINTEGER_H
