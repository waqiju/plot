#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"
#include "core_component/core_component.h"
#include "graphic/graphic.h"
#include <string>
#include "asset_loader/prefab_convertor.h"



class TextComponent: public Component, public IBoundsGetter, public IPrefabConvertor
{
public:
    static TextComponent* Create(Transform* parent, std::string text, float fontSize, LayoutAlignment alignment);
    static int GenerateMesh(const Bounds& bounds, const std::string& text, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors, std::vector<Vector2>& uvs);
    static void BatchRender(std::vector<TextComponent*> textList);

    TextComponent(Entity* owner): Component(owner) {}

    std::string text;
    float fontSize;
    LayoutAlignment alignment;
    Color color = Color::white;
    Bounds bounds;

    void AlignBounds();
    int GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors, std::vector<Vector2>& uvs);
    void Render();
    Bounds& GetBounds() { return bounds; }

    // Prefab
    void Deserialize(Entity& entity, const pb::WorldObject& pbComponentObj, PrefabLoader& loader) override;
};