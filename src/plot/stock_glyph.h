#pragma once
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "entity/ec.h"
#include <vector>
#include <utility>
#include "stock_metadata.h"


class StockGlyph : public Component
{
public:
    static StockGlyph* Create(Transform* parent, float x, const StockMetadata& metadata);
    static void BatchRender(std::vector<StockGlyph*>& stockGlyph);
	static std::pair<float, float> MeshVertexPairX(float x);

    StockGlyph(Entity* owner): Component(owner) {}

    Bounds bounds;
    Color color = Color::white;
    float x;
    StockMetadata metadata;


    void Reset(float inX, const StockMetadata& inMetadata);
    void Render();
private:
    void RenderRectangle();
    void RenderSegment();
};