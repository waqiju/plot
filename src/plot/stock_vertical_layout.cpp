#include "stock_vertical_layout.h"
#include "stock_layout_item.h"
#include "price_digest_glyph.h"
#include "core_component/core_component.h"
#include <vector>
#include <algorithm>


StockVerticalLayout::StockVerticalLayout(Entity* owner) : Component(owner)
{
    name = "StockVerticalLayout";
}

void StockVerticalLayout::ApplyLayout()
{
	std::vector<StockLayoutItem*> itemList = this->GetComponentsInChildren<StockLayoutItem>();
	decltype(itemList) upList, downList;
	for (StockLayoutItem* item : itemList)
	{
		if (item->GetComponent<BoundsComponent>() == NULL)
			continue;

		if (item->priority >= 0)
		{
			upList.push_back(item);
		}
		else
		{
			downList.push_back(item);
		}
	}
    BoundsComponent* rootBoundsCp = this->GetComponent<BoundsComponent>();
    if (rootBoundsCp == NULL)
        return;
	// up
    std::sort(upList.begin(), upList.end()
        , [&](StockLayoutItem* lhs, StockLayoutItem* rhs) {return lhs->priority <= rhs->priority; });
    float startY = rootBoundsCp->WorldBounds().max.y;
    float layoutSpacing = CameraHelper::CalculateLength(this->spacing, this->unit);
	for (auto item : upList)
	{
        startY += layoutSpacing;

        BoundsComponent* boundsCp = item->GetComponent<BoundsComponent>();
        Bounds worldBounds = boundsCp->WorldBounds();
        float boundsHeight = worldBounds.Size().y;
        Vector3 position = item->GetTransform()->Position();
        position.y = startY + boundsHeight / 2 - boundsCp->localBounds.Center().y;
        item->GetTransform()->SetPosition(position);
        startY += boundsHeight;
	}
	// down
    std::sort(downList.begin(), downList.end()
        , [&](StockLayoutItem* lhs, StockLayoutItem* rhs) {return lhs->priority >= rhs->priority; });
    startY = rootBoundsCp->WorldBounds().min.y;
    for (auto item : downList)
    {
        startY -= layoutSpacing;

        BoundsComponent* boundsCp = item->GetComponent<BoundsComponent>();
        Bounds worldBounds = boundsCp->WorldBounds();
        float boundsHeight = worldBounds.Size().y;
        Vector3 position = item->GetTransform()->Position();
        position.y = startY - boundsHeight / 2 - boundsCp->localBounds.Center().y;
        item->GetTransform()->SetPosition(position);
        startY -= boundsHeight;
    }
}