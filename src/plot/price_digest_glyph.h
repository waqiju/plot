#pragma once
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "core_component/core_component.h"
#include "entity/ec.h"
#include <vector>
#include <utility>
#include "stock_price_digest.h"


class PriceDigestGlyph : public Component, public IBoundsGetter
{
public:
    static PriceDigestGlyph* Create(Transform* parent, float x, const StockPriceDigest& metadata);
    static void BatchRender(std::vector<PriceDigestGlyph*>& stockGlyph);
	static std::pair<float, float> MeshVertexPairX(float x);

    PriceDigestGlyph(Entity* owner): Component(owner) {}

    Bounds bounds;
    Color color = Color::white;
    float x;
    StockPriceDigest metadata;


    void Reset(float inX, const StockPriceDigest& inMetadata);
    void Render();
    Bounds& GetBounds() { return bounds; }
private:
    void RenderRectangle();
    void RenderSegment();
};