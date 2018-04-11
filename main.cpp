#include <iostream>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <gmp.h>
#include "biginteger.hpp"

using std::cin;
using std::cout;
using std::endl;

template<typename T>
bool initialize(BigInteger<T> & x, mpz_t & gmp_x, string value)
{
    try {
        x = BigInteger<T>(value);
    } catch(...) {
        return false;
    }

    mpz_init(gmp_x);
    return mpz_set_str(gmp_x, value.c_str(), 10) == 0;
}

template<typename T, size_t Base>
bool compare(BigInteger<T, Base> &biResult, mpz_t &gmpResult)
{
    string biResultStr = biResult.str();

    char* gmpResultChar = new char[mpz_sizeinbase(gmpResult, 10) + 2];
    mpz_get_str(gmpResultChar, 10, gmpResult);
    string gmpResultStr(gmpResultChar);
    delete[] gmpResultChar;

    if (biResultStr == gmpResultStr) {
        cout << "PASS" << endl;
        return true;
    } else {
        cout << "FAIL" << endl;
        cout << "GMP: " << gmpResultStr << endl;
        cout << "BI:  " << biResultStr << endl;
        return false;
    }
}

bool test(const string &a, const string &b)
{
    typedef unsigned long long initialType;
    typedef unsigned short testType;
    const testType testBase = 3;

    static int compareNumber = 1;
    cout << "Test #" << compareNumber++ << ":" << endl;

    BigInteger<initialType> biA, biB;
    mpz_t gmpA, gmpB;

    bool inputIsValid = initialize<initialType>(biA, gmpA, a);
    inputIsValid &= initialize<initialType>(biB, gmpB, b);

    if (!inputIsValid) {
        cout << "Invalid input!" << endl;
        return false;
    }

    BigInteger<testType, testBase> biAConv = static_cast< BigInteger<testType, testBase> >(biA);
    BigInteger<testType, testBase> biBConv = static_cast< BigInteger<testType, testBase> >(biB);

    bool success = true;
    cout << "Addition:\t";
    BigInteger<testType, testBase> biResult = biAConv + biBConv;
    mpz_t gmpResult;
    mpz_init(gmpResult);
    mpz_add(gmpResult, gmpA, gmpB);
    success &= compare<testType, testBase>(biResult, gmpResult);

    cout << "Substraction:\t";
    biResult = biAConv - biBConv;
    mpz_sub(gmpResult, gmpA, gmpB);
    success &= compare<testType, testBase>(biResult, gmpResult);

    /*cout << "Multiplication:\t";
    biResult = biAConv * biBConv;
    mpz_mul(gmpResult, gmpA, gmpB);
    success &= compare<testType, testBase>(biResult, gmpResult);*/

    mpz_clear(gmpA);
    mpz_clear(gmpB);
    mpz_clear(gmpResult);
    return success;
}

inline int randomInt(int min, int max)
{
    return min + (rand() % static_cast<int>(max - min + 1));
}

string randomBIString()
{
    int length = randomInt(1, 1000);
    string result(length, '0');
    for (int i = 0; i < length; ++i) {
        result[i] = randomInt('0', '9');
    }
    return result;
}

int main()
{
    std::srand(std::time(0));
    bool success = true;

    for (int i = 0; i < 100; ++i) {
        string a = randomBIString();
        string b = randomBIString();
        success &= test(a, b);
    }

    cout << "TESTS " << (success ? "PASSED" : "FAILED") << endl;
    return 0;
}
