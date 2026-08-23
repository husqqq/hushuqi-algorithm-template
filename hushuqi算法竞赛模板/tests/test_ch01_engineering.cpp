#include "../include/engineering.hpp"

int main()
{
    speedRounds = 10000000;
    speedBlocks = 0;
    assert(speedTest() == 0);
    speedBlocks = 1;
    assert(speedTest() == 0);
    cout << "OK\n";
}
