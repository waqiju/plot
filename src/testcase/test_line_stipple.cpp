#include "test_line_stipple.h"
#include "plot/plot.h"
#include <iostream>
using namespace chimera;


void TestLineStipple()
{
	auto mainPanelTr = ObjectID::Find(ObjectID::PlotMainPanelRoot)->ToTransform();
    // 1
	auto segment1 = Segment::Create(mainPanelTr, Vector3(0, 0.95f, 0), Vector3(1, 0.95f, 0), Color::white);
    segment1->stipplePattern = 0xf0f0;
	auto boundsCp = segment1->AddComponent<BoundsComponent>();
	boundsCp->localBounds = segment1->bounds;
    // 2
    auto segment2 = Segment::Create(mainPanelTr, Vector3::zero, Vector3(0.5f, 0.5f, 0), Color::white);
    segment2->stipplePattern = 0xff00;
    auto boundsCp2 = segment2->AddComponent<BoundsComponent>();
    boundsCp2->localBounds = segment2->bounds;
}
