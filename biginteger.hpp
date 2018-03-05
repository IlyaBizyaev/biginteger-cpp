#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <iostream>
#include <string>

class BigInteger {
    friend std::ostream & operator <<(std::ostream &, const BigInteger &);
    friend std::istream & operator >>(std::istream &, BigInteger &);
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
    std::string m_number;
    bool m_negative;
};

std::ostream & operator <<(std::ostream &, const BigInteger &);
std::istream & operator >>(std::istream &, BigInteger &);

#endif // BIGINTEGER_H
