#include "stock_vertical_layout.h"
#include "stock_layout_item.h"
#include "stock_glyph.h"
#include "core_component/core_component.h"
#include <vector>


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
    StockGlyph* stockGlyph = this->GetComponent<StockGlyph>();
    if (stockGlyph == NULL)
        return;
	// up
    float startY = stockGlyph->bounds.max.y;
	for (auto item : upList)
	{
		BoundsComponent* boundsCp = item->GetComponent<BoundsComponent>();
        // TODO spacing in pixel
        startY += this->spacing;
        Vector3 position = item->GetTransform()->LocalPosition();
        position.y = startY + boundsCp->localBounds.Size().y / 2 - boundsCp->localBounds.Center().y;
        item->GetTransform()->SetLocalPosition(position);
	}
	// down
    startY = stockGlyph->bounds.min.y;
    for (auto item : downList)
    {
        BoundsComponent* boundsCp = item->GetComponent<BoundsComponent>();
        startY -= this->spacing;
        Vector3 position = item->GetTransform()->LocalPosition();
        position.y = startY - boundsCp->localBounds.Size().y / 2 - boundsCp->localBounds.Center().y;
        item->GetTransform()->SetLocalPosition(position);
    }
}