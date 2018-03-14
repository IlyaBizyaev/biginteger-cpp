#include "biginteger.hpp"
#include <iostream>
#include <iomanip>

//
// Class implementation
//

const uint32_t MODULE = 1000000000;

BigInteger::BigInteger(long long value) : m_negative(value < 0)
{
    if (m_negative) {
        value = -value;
    }

    do {
        m_number.push_back(value % MODULE);
        value /= MODULE;
    } while (value);
}

BigInteger::BigInteger(string value) : m_negative(false)
{
    if (!value.empty() && (value[0] == '-' || value[0] == '+')) {
        m_negative = (value[0] == '-');
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
    if (m_negative && value == "0") {
        m_negative = false;
    }

    for (int i = value.length(); i > 0; i -= 9) {
        int32_t x = std::stoi(value.substr(std::max(0, i - 9), i - std::max(0, i - 9)));
        m_number.push_back(x);
    }
}

BigInteger::BigInteger(const BigInteger & bigint)
{
    m_negative = bigint.m_negative;
    m_number = bigint.m_number;
}

BigInteger::BigInteger(BigInteger && bigint)
{
    m_negative = bigint.m_negative;
    m_number = std::move(bigint.m_number);
}

BigInteger& BigInteger::operator =(const BigInteger & other)
{
    m_negative = other.m_negative;
    m_number = other.m_number;
    return *this;
}

BigInteger & BigInteger::operator +=(const BigInteger &other)
{
    if (m_negative == other.m_negative) {
        // neg + neg or pos + pos;
        uint32_t carry = 0;

        for (size_t i = 0; i < other.m_number.size() || carry; ++i) {
            if (i >= m_number.size()) {
                m_number.push_back(0);
            }

            int64_t a = m_number.at(i);
            a += i < other.m_number.size() ? other.m_number.at(i) : 0;
            a += carry;

            carry = a / MODULE;
            m_number[i] = a % MODULE;
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
        uint32_t carry = 0;

        for (size_t i = 0; i < other.m_number.size() || carry; ++i) {
            int64_t a = m_number.at(i);
            a -= carry;
            carry = 0;
            int64_t b = i < other.m_number.size() ? other.m_number.at(i) : 0;

            if (a < b) {
                a += MODULE;
                carry = 1;
            }

            m_number[i] = a - b;
        }

        // Trim leading zeros
        while (m_number.back() == 0 && m_number.size() > 1) {
            m_number.pop_back();
        }
    }

    return *this;
}

BigInteger BigInteger::operator +(const BigInteger &other) const
{
    return BigInteger(*this) += other;
}

BigInteger & BigInteger::operator -=(const BigInteger &other)
{
    return *this += (-other);
}

BigInteger BigInteger::operator -(const BigInteger &other) const
{
    return BigInteger(*this) -= other;
}

BigInteger BigInteger::operator -() const
{
    BigInteger temp = *this;
    temp.m_negative = !(m_negative);
    return temp;
}

//
// Input-output stream implementations
//

ostream & operator <<(ostream &stream, const BigInteger &bigint)
{
    if (bigint.m_negative) {
        stream << '-';
    }

    stream << bigint.m_number.back();

    for (int i = bigint.m_number.size() - 2; i >= 0; --i) {
        stream << std::setfill('0') << std::setw(9) << bigint.m_number.at(i);
    }

    return stream;
}

istream & operator >>(istream &stream, BigInteger &bigint)
{
    string temp;
    stream >> temp;
    bigint = BigInteger(temp);
    return stream;
}
