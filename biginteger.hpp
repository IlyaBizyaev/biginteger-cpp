#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <iostream>
#include <string>
using std::istream;
using std::ostream;
using std::string;

class BigInteger {
    friend ostream & operator <<(ostream &, const BigInteger &);
    friend istream & operator >>(istream &, BigInteger &);
public:
    explicit BigInteger(long long value = 0);
    BigInteger(const BigInteger &);
    BigInteger(BigInteger &&);
    BigInteger& operator =(const BigInteger &);

    BigInteger & operator +=(const BigInteger &);
    BigInteger operator +(const BigInteger &) const;

    BigInteger & operator -=(const BigInteger &);
    BigInteger operator -(const BigInteger &) const;
    BigInteger operator -() const;

private:
    int getDigit(size_t i) const;
    BigInteger(string digits, bool isNegative = false);
    string m_number;
    bool m_negative;
};

ostream & operator <<(ostream &, const BigInteger &);
istream & operator >>(istream &, BigInteger &);

#endif // BIGINTEGER_H
