#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
using std::istream;
using std::ostream;
using std::string;
using std::vector;

template<typename T, size_t Base = std::numeric_limits<T>::max()>
class BigInteger;

template<typename T, size_t Base>
ostream & operator <<(ostream &, const BigInteger<T, Base> &);
template<typename T, size_t Base>
istream & operator >>(istream &, BigInteger<T, Base> &);

template<typename T, size_t Base>
class BigInteger {
    friend ostream & operator << <>(ostream &, const BigInteger<T, Base> &);
    friend istream & operator >> <>(istream &, BigInteger<T, Base> &);
    template<typename T1, size_t Base1> friend class BigInteger;
public:
    explicit BigInteger(long long value = 0);
    explicit BigInteger(string value);
    template<typename NewT, size_t NewBase>
    explicit operator BigInteger<NewT, NewBase>() const;

    BigInteger(const BigInteger &);
    BigInteger(BigInteger &&);
    BigInteger& operator =(const BigInteger &);

    BigInteger & operator +=(const BigInteger &);
    BigInteger operator +(const BigInteger &) const;

    BigInteger & operator -=(const BigInteger &);
    BigInteger operator -(const BigInteger &) const;
    BigInteger operator -() const;

    BigInteger & operator *=(const T &);
    BigInteger operator *(const T &) const;

    BigInteger & operator /=(const T &);
    BigInteger operator /(const T &) const;

    size_t size() const;
    T getDigit(size_t i) const;
    T operator[](size_t i) const;
    bool isNegative() const;
    void setNegative(bool negative = true);
    bool isZero() const;

private:
    explicit BigInteger(vector<T> sparse_values);
    BigInteger & divide_with_remainder(const T &, T * remainder = nullptr);
    constexpr static int digitsInT();
    constexpr static int bitsPerDigit();
    void setDigit(size_t i, const T &digit);
    void addDigit(const T &digit);

    vector<T> m_number;
    size_t m_digits;
    bool m_negative;
};

#include "biginteger.tpp"

#endif // BIGINTEGER_H
