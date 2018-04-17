#ifndef RANDOMTEST_H
#define RANDOMTEST_H

#include <string>
#include <limits>
#include <gmpxx.h>
#include "biginteger.hpp"
using std::string;

template<typename T, size_t Base = std::numeric_limits<T>::max()>
class RandomTest {
public:
    bool run();

private:
    bool compareResult(BigInteger<T, Base> &biResult, mpz_class &gmpResult);
    bool testValue(const string &a, const string &b);
};

#include "randomtest.tpp"

#endif // RANDOMTEST_H

