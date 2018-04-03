#include "biginteger.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

//
// Class implementation
//

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(long long value) : m_negative(value < 0)
{
    if (m_negative) {
        value = -value;
    }

    m_digits = 0;

    do {
        addDigit(value % Base);
        value /= Base;
    } while (value);
}

template<typename T, size_t Base>
template<typename NewT, size_t NewBase>
BigInteger<T, Base>::operator BigInteger<NewT, NewBase>() const
{
    vector<T> sparse_digits;
    BigInteger temp(*this);

    while (!temp.isZero()) {
        T remainder = 0;
        temp.divide_with_remainder(NewBase, &remainder);
        sparse_digits.push_back(remainder);
    }

    BigInteger<NewT, NewBase> result(sparse_digits);
    result.setNegative(m_negative);
    return result;
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(string value)
{
    bool negative = false;

    if (!value.empty() && (value[0] == '-' || value[0] == '+')) {
        negative = (value[0] == '-');
        value = value.substr(1);
    }

    if (value.empty()) {
        value = "0";
    }

    for (auto c : value) {
        if (c < '0' || c > '9') {
            throw std::ios_base::failure("Provided string is not a number");
        }
    }

    // Trim leading zeros
    value.erase(0, std::min(value.find_first_not_of('0'), value.size()-1));

    // 0 (zero) is not negative
    if (negative && value == "0") {
        negative = false;
    }

    vector<T> sparse_digits;
    for (int i = value.length(); i > 0; i -= 9) {
        int32_t x = std::stoi(value.substr(std::max(0, i - 9), i - std::max(0, i - 9)));
        sparse_digits.push_back(x);
    }

    BigInteger<uint32_t, 1000000000> temp(sparse_digits);
    temp.setNegative(negative);

    *this = static_cast<BigInteger<T, Base>>(temp);
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(vector<T> sparse_values)  : m_digits(0), m_negative(false)
{
    for (auto i : sparse_values) {
        addDigit(i % Base);
    }
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(const BigInteger<T, Base> & bigint)
{
    m_negative = bigint.m_negative;
    m_number = bigint.m_number;
    m_digits = bigint.m_digits;
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(BigInteger<T, Base> && bigint)
{
    m_negative = bigint.m_negative;
    m_number = std::move(bigint.m_number);
    m_digits = bigint.m_digits;
}

template<typename T, size_t Base>
BigInteger<T, Base>& BigInteger<T, Base>::operator =(const BigInteger<T, Base> & other)
{
    m_negative = other.m_negative;
    m_number = other.m_number;
    m_digits = other.m_digits;
    return *this;
}

template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator +=(const BigInteger<T, Base> &other)
{
    if (m_negative == other.m_negative) {
        // neg + neg or pos + pos;
        T carry = 0;

        for (size_t i = 0; i < other.size() || carry; ++i) {
            T a = i < size() ? this->operator [](i) : 0;
            T b = i < other.size() ? other[i] : 0;

            T current;
            if (a > Base - b - carry) {
                current = std::max(a, b) - Base + std::min(a, b) + carry;
                carry = 1;
            } else {
                current = a + b + carry;
                carry = 0;
            }

            setDigit(i, current);
        }
    } else if (m_negative && !other.m_negative) {
        // neg + pos = pos + neg
        *this = (other) + (*this);
    } else if (m_number.size() < other.m_number.size()
               || (m_number.size() == other.m_number.size()
                   && m_number.back() < other.m_number.back())) {
        // pos + neg, where abs(pos) < abs(neg)
        // Swap to substract smaller from bigger
        *this = -(-other + -(*this));
    } else {
        // pos + neg = subtraction
        T carry = 0;

        for (size_t i = 0; i < other.size() || carry; ++i) {
            T a = this->operator [](i);
            T b = i < other.size() ? other[i] : 0;

            if (carry > a) {
                a += Base - carry;
                carry = 1;
            } else {
                a -= carry;
                carry = 0;
            }

            T current;

            if (a < b) {
                current = a + (Base - b);
                carry = 1;
            } else {
                current = a - b;
            }

            setDigit(i, current);
        }

        // Trim leading zeros
        for (size_t i = size() - 1; i >= 0 && this->operator [](i) == 0; --i) {
            --m_digits;
        }


        size_t usedCells = std::ceil(double(size()) / digitsInT());
        if (usedCells < m_number.size()) {
            m_number.resize(usedCells);
        }
    }

    return *this;
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator +(const BigInteger<T, Base> &other) const
{
    return BigInteger<T, Base>(*this) += other;
}

template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator -=(const BigInteger<T, Base> &other)
{
    return *this += (-other);
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator -(const BigInteger<T, Base> &other) const
{
    return BigInteger(*this) -= other;
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator -() const
{
    BigInteger temp = *this;
    temp.m_negative = !(m_negative);
    return temp;
}

template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator *=(const T &other)
{
    T carry = 0;

    for (size_t i = 0; i < size() || carry; ++i) {
        T temp = this->operator [](i) + carry;
        temp *= other;
        carry = temp / Base;
        setDigit(i, temp % Base);
    }

    return *this;
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator *(const T &other) const
{
    return BigInteger(*this) *= other;
}

template<typename T>
void longMultiply(T a, T b, T &resH, T &resL)
{
    size_t half = sizeof(T) * 4;
    T aH = a >> half;
    T bH = b >> half;
    T aL = a - (aH << half);
    T bL = b - (bH << half);
    resH = aH * bH;
    resL = (aH * bL) << half + (aL * bH) << half + aL * bL;
}

template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::divide_with_remainder(const T &other, T * remainder)
{
    if (other == 0) {
        throw std::overflow_error("Division by zero");
    }

    size_t collect = 0;
    vector<T> sparse_digits;

    for (int i = size() - 1; i >= 0; --i) {
        collect = collect * Base + this->operator [](i);

        if (collect >= other) {
            sparse_digits.push_back(collect / other);
            collect %= other;
        }
    }

    if (sparse_digits.empty()) {
        sparse_digits.push_back(0);
    }

    if (remainder != nullptr) {
        *remainder = collect;
    }

    std::reverse(sparse_digits.begin(), sparse_digits.end());
    BigInteger temp(sparse_digits);
    temp.setNegative(m_negative);

    return *this = temp;
}

template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator /=(const T &other)
{
    return divide_with_remainder(other);
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator /(const T &other) const
{
    return BigInteger(*this) /= other;
}

template<typename T, size_t Base>
size_t BigInteger<T, Base>::size() const
{
    return m_digits;
}

template<typename T, size_t Base>
T BigInteger<T, Base>::operator[](size_t i) const
{
    T result = m_number[i / digitsInT()];
    result >>= (i % digitsInT()) * bitsPerDigit();
    return result % Base;
}

template<typename T, size_t Base>
void BigInteger<T, Base>::setDigit(size_t i, T digit)
{
    if (i >= size()) {
        for (size_t t = size(); t <= i; ++t) {
            addDigit(0);
        }
    }

    size_t j = i / digitsInT();
    size_t shift = i % digitsInT() * bitsPerDigit();

    if (digitsInT() == 1) {
        m_number[j] = 0;
    } else {
        T t1 = 1 << bitsPerDigit();
        T t2 = t1 - 1;
        T t3 = t2 << shift;
        T clearMask = ~t3;
        //T clearMask = ~(((1 << bitsPerDigit()) - 1) << shift);
        m_number[j] &= clearMask;
    }
    m_number[j] |= digit << shift;
}

template<typename T, size_t Base>
void BigInteger<T, Base>::addDigit(T digit)
{
    if (size() % digitsInT() == 0) {
        m_number.push_back(digit);
    } else {
        setDigit(size(), digit);
    }
    ++m_digits;
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::isNegative() const
{
    return m_negative;
}

template<typename T, size_t Base>
void BigInteger<T, Base>::setNegative(bool negative)
{
    if (!isZero()) {
        m_negative = negative;
    }
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::isZero() const
{
    return size() == 1 && this->operator [](0) == 0;
}

template<typename T, size_t Base>
int BigInteger<T, Base>::digitsInT()
{
    return (sizeof(T) * 8) / bitsPerDigit();
}

template<typename T, size_t Base>
int BigInteger<T, Base>::bitsPerDigit()
{
    size_t temp = Base;
    int bits = 0;
    while (temp) {
        temp >>= 1;
        ++bits;
    }

    return bits;
}

//
// Input-output stream implementations
//

template<typename T, size_t Base>
ostream & operator << (ostream &stream, const BigInteger<T, Base> &bigint)
{
    if (bigint.m_negative) {
        stream << '-';
    }

    BigInteger<uint32_t, 1000000000> temp = static_cast<BigInteger<uint32_t, 1000000000>>(bigint);

    stream << temp[temp.size() - 1];

    for (int i = temp.size() - 2; i >= 0; --i) {
        stream << std::setfill('0') << std::setw(9) << temp[i];
    }

    return stream;
}

template<typename T, size_t Base>
istream & operator >>(istream &stream, BigInteger<T, Base> &bigint)
{
    string temp;
    stream >> temp;
    bigint = BigInteger<T, Base>(temp);
    return stream;
}
