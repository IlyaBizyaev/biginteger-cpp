#include <iostream>
#include "biginteger.hpp"

int main()
{
    BigInteger a(0), b(0);
    std::cin >> a >> b;
    std::cout << a + b << std::endl;
    return 0;
}
