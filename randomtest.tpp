#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <ctime>
#include "randomtest.hpp"

using std::cin;
using std::cout;
using std::endl;

/*
 * Helper functions
 */

inline int randomInt(int min, int max)
{
    return min + (rand() % static_cast<int>(max - min + 1));
}

string randomBIString()
{
    bool negative = randomInt(0, 1);
    int length = randomInt(1, 1000);
    if (negative) {
        ++length;
    }
    string result(length, '0');

    int i = 0;
    if (negative) {
        result[0] = '-';
        ++i;
    }

    for (; i < length; ++i) {
        result[i] = randomInt('0', '9');
    }
    return result;
}

/*
 * Class implementation
 */

template<typename T, size_t Base>
bool RandomTest<T, Base>::compareResult(BigInteger<T, Base> &biResult, mpz_class &gmpResult)
{
    string biResultStr = biResult.str();
    string gmpResultStr = gmpResult.get_str(10);;

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

template<typename T, size_t Base>
bool RandomTest<T, Base>::testValue(const string &a, const string &b)
{
    typedef unsigned long long initialType;

    static int compareNumber = 1;
    cout << "Test #" << compareNumber++ << ":" << endl;

    BigInteger<initialType> biA, biB;
    mpz_class gmpA, gmpB;

    bool inputIsValid = true;

    try {
        biA = BigInteger<initialType>(a);
        biB = BigInteger<initialType>(b);
    } catch(...) {
        inputIsValid = false;
    }

    inputIsValid &= (gmpA.set_str(a, 10) == 0);
    inputIsValid &= (gmpB.set_str(b, 10) == 0);

    if (!inputIsValid) {
        cout << "Invalid input!" << endl;
        return false;
    }

    BigInteger<T, Base> biAConv = static_cast< BigInteger<T, Base> >(biA);
    BigInteger<T, Base> biBConv = static_cast< BigInteger<T, Base> >(biB);

    bool success = true;
    cout << "Addition:\t";
    BigInteger<T, Base> biResult = biAConv + biBConv;
    mpz_class gmpResult = gmpA + gmpB;
    success &= compareResult(biResult, gmpResult);

    cout << "Substraction:\t";
    biResult = biAConv - biBConv;
    gmpResult = gmpA - gmpB;
    success &= compareResult(biResult, gmpResult);

    cout << "Multiplication:\t";
    biResult = biAConv * biBConv;
    gmpResult = gmpA * gmpB;
    success &= compareResult(biResult, gmpResult);

    return success;
}

template<typename T, size_t Base>
bool RandomTest<T, Base>::run()
{
    std::srand(std::time(0));
    bool success = true;

    std::clock_t begin = clock();
    for (int i = 0; i < 100; ++i) {
        string a = randomBIString();
        string b = randomBIString();
        success &= testValue(a, b);
    }
    std::clock_t end = clock();

    double elapsed = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Finished in " << std::setprecision(2) << elapsed << "s" << endl;

    return success;
}

