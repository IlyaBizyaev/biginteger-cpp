#include <iostream>
#include "biginteger.hpp"

int main()
{
    BigInteger<uint32_t> a, b;
    std::cin >> a >> b;

    const int checkBase = 3;

    BigInteger<unsigned short, checkBase> a2 = static_cast< BigInteger<unsigned short, checkBase> >(a);
    BigInteger<unsigned short, checkBase> b2 = static_cast< BigInteger<unsigned short, checkBase> >(b);

    std::cout << "a + b: " << a2 + b2 << std::endl;
    std::cout << "a - b: " << a2 - b2 << std::endl;

    return 0;
}
