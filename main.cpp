#include <iostream>
#include "biginteger.hpp"

int main()
{
    BigInteger<uint32_t> a, b;
    std::cin >> a >> b;

    std::cout << "a + b: " << a + b << std::endl;
    std::cout << "a - b: " << a - b << std::endl;
    return 0;
}
