#pragma once
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "entity/ec.h"
#include <vector>
#include "pb/prophet.pb.h"


class StockGlyph : public Component
{
public:
    static StockGlyph* Create(Transform* parent, float x, const KChart& chart);
    static void BatchRender(std::vector<StockGlyph*>& stockGlyph);

    StockGlyph(Entity* owner): Component(owner) {}

    Vector3 leftBottom = Vector3::zero, rightTop = Vector3::zero;
    Color color = Color::white;
    float x;
    KChart chart;


    void Reset(float inX, const KChart& inKChart);
    void Render();
private:
    void RenderRectangle();
    void RenderSegment();
};