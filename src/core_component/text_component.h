#pragma once
#include "entity/ec.h"
#include "geometry/bounds.h"
#include "core_component/layout_define.h"
#include "graphic/graphic.h"
#include <string>


class TextComponent: public Component
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
};