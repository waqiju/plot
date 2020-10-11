#include <iostream>
#include "pb/prophet.pb.h"


int main()
{
    pb::KChart kChart;
    kChart.set_open(1.3);

    std::cout << kChart.open() << std::endl;

    std::cin.get();
    return 0;
}