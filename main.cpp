#include <iostream>
#include "randomtest.hpp"

int main()
{
    RandomTest<unsigned short, 3> test;
    bool result = test.run();
    std::cout << "TESTS " << (result ? "PASSED" : "FAILED") << std::endl;
    return 0;
}
