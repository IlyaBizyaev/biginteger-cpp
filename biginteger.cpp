#include "biginteger.hpp"
#include <stdlib.h>
#include <sstream>

//
// Class implementation
//

BigInteger::BigInteger(long long value) : m_negative(value < 0)
{
    if (m_negative) {
        value = -value;
    }
    std::stringstream ss;
    ss << value;
    m_number = ss.str();
    std::cout << "Constructed: " << (m_negative ? '-' : '+') << m_number << std::endl;
}

BigInteger::BigInteger(const BigInteger & bigint)
{
    m_number = bigint.m_number;
    m_negative = bigint.m_negative;
}

BigInteger::BigInteger(BigInteger && bigint)
{
    m_number = std::string(std::move(bigint.m_number));
    m_negative = bigint.m_negative;
}

BigInteger& BigInteger::operator =(const BigInteger & other)
{
    this->m_number = other.m_number;
    this->m_negative = other.m_negative;
    return *this;
}

BigInteger & BigInteger::operator +=(const BigInteger &other)
{
    return *this = *this + other;
}
BigInteger BigInteger::operator +(const BigInteger &other) const
{
    // size_t n = std::max(this->m_number.length(), other.m_number.length())+1;

    if (this->m_negative && other.m_negative) {
        // neg + neg = neg(sum);
        return -(-(*this) + (-other));
    } else if (this->m_negative && !other.m_negative) {
        // neg + pos = -subtraction
        return -(-(*this)+(-other));
    } else if (!this->m_negative && !other.m_negative) {
        // pos + pos = sum
        // TODO
        int carry = 0;
        return BigInteger(42);
    } else {
        // pos + neg = subtraction
        // TODO
        int carry = 0;
        return BigInteger(42);
    }
}

BigInteger & BigInteger::operator -=(const BigInteger &other)
{
    return *this = *this - other;
}

BigInteger BigInteger::operator -(const BigInteger &other) const
{
    return *this + (-other);
}

BigInteger BigInteger::operator -() const
{
    BigInteger temp = *this;
    temp.m_negative = !(this->m_negative);
    return temp;
}

//
// Input-output stream implementations
//

std::ostream & operator <<(std::ostream &stream, const BigInteger &bigint)
{
    stream << bigint.m_number;
    return stream;
}

std::istream & operator >>(std::istream &stream, BigInteger &bigint)
{
    stream >> bigint.m_number;
    return stream;
}
