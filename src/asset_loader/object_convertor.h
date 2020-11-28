#pragma once
#include "prefab_loader.h"
#include "pb/prefab.pb.h"

class Vector3;
class Bounds;
class Color;
class StockMetadata;


void ConvertVector3(const pb::WorldObject& object, Vector3& vector, const pb::Prefab& prefab);
void ConvertBounds(const pb::WorldObject& object, Bounds& bounds, const pb::Prefab& prefab);
void ConvertColor(const pb::WorldObject& object, Color& color, const pb::Prefab& prefab);
void ConvertStockMetadata(const pb::WorldObject& object, StockMetadata& metadata, const pb::Prefab& prefab);

