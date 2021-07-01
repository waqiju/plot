#pragma once
#include "common/common.h"
#include "camera.h"
#include "geometry/geometry.h"
#include "entity/ec.h"
#include "mesh.h"


class CursorSight : public Singleton<CursorSight>
{
public:
    static const Color kDefaultColor;

    enum class SightStyle
    {
        None,
        Horizontal,
        Vertical,
        Cross,
    };

    CursorSight();
    ~CursorSight();
    void Render();
    void RenderAsixReadout();

    // fields
    SightStyle style = SightStyle::None;
    Color color = kDefaultColor;
private:
    unsigned int m_VAO;
    unsigned int m_VertexBufferID;
    unsigned int m_DirectionBufferID;
    Material* m_Material;
};