#include "biginteger.hpp"
#include <iostream>
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
}

BigInteger::BigInteger(string digits, bool isNegative) : m_negative(isNegative)
{
    for (auto c : digits) {
        if (c < '0' || c > '9') {
            throw std::ios_base::failure("Provided string is not a number");
        }
    }
    m_number = digits;
}

BigInteger::BigInteger(const BigInteger & bigint)
{
    m_number = bigint.m_number;
    m_negative = bigint.m_negative;
}

BigInteger::BigInteger(BigInteger && bigint)
{
    m_number = string(std::move(bigint.m_number));
    m_negative = bigint.m_negative;
}

BigInteger& BigInteger::operator =(const BigInteger & other)
{
    m_number = other.m_number;
    m_negative = other.m_negative;
    return *this;
}

BigInteger & BigInteger::operator +=(const BigInteger &other)
{
    return *this = *this + other;
}

BigInteger BigInteger::operator +(const BigInteger &other) const
{
    size_t n = std::max(m_number.length(), other.m_number.length());

    if (m_negative && other.m_negative) {
        // neg + neg = neg(sum);
        return -(-other + -(*this));
    } else if (m_negative && !other.m_negative) {
        // neg + pos = pos + neg
        return (other)+(*this);
    } else if (!m_negative && !other.m_negative) {
        // pos + pos = sum
        string result(n + 1, '0');
        int carry = 0;
        for (size_t i = 0; i < n; ++i) {
            int sum = getDigit(i) + other.getDigit(i) + carry;
            if (sum >= 10) {
                sum -= 10;
                carry = 1;
            } else {
                carry = 0;
            }
            result[result.length()-1-i] = '0' + sum;
        }

        if (carry) {
            result[0] = '0' + carry;
            return BigInteger(result);
        } else {
            return BigInteger(result.substr(1));
        }
    } else {
        // pos + neg = subtraction
        if (m_number.length() < other.m_number.length()
                || (m_number.length() == other.m_number.length() && m_number[0] < other.m_number[0])) {
            // Swap to substract smaller from bigger
            return -(-other + -(*this));
        }

        int carry = 0;
        string result(n, '0');

        for (size_t i = 0; i < n; ++i) {
            int a = getDigit(i) - carry;
            int b = other.getDigit(i);
            carry = 0;

            if (a < b) {
                a += 10;
                carry = 1;
            }

            result[result.length()-1-i] = '0' + (a - b);
        }

        result.erase(0, std::min(result.find_first_not_of('0'), result.size()-1));
        return BigInteger(result);
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
    temp.m_negative = !(m_negative);
    return temp;
}

int BigInteger::getDigit(size_t i) const
{
    return i < m_number.length() ? m_number.at(m_number.length() - i - 1) - '0' : 0;
}

//
// Input-output stream implementations
//

ostream & operator <<(ostream &stream, const BigInteger &bigint)
{
    if (bigint.m_negative) {
        stream << '-';
    }
    stream << bigint.m_number;
    return stream;
}

istream & operator >>(istream &stream, BigInteger &bigint)
{
    string temp;
    stream >> temp;
    bool isNegative = false;
    if (!temp.length()) {
        temp = "0";
    } else if(temp[0] == '-' || temp[0] == '+') {
        isNegative = (temp[0] == '-');
        temp = temp.substr(1);
    }

    temp.erase(0, std::min(temp.find_first_not_of('0'), temp.size()-1));

    // 0 (zero) is not negative
    if (isNegative && temp == "0") {
        isNegative = false;
    }
    bigint = BigInteger(temp, isNegative);
    return stream;
}
