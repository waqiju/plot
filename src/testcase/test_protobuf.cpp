#include <iostream>
#include "pb/prophet.pb.h"
#include "plot/segment.h"

using namespace chimera;

int main()
{
    KChart kChart;
    kChart.set_open(1.3);

    ::Segment seg;
    chimera::Segment seg2(NULL);

    std::cout << kChart.open() << std::endl;

    std::cin.get();
    return 0;
}