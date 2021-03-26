#include "space_grid.h"
#include "geometry/geometry.h"
#include "graphic/graphic.h"
#include "mesh_renderer.h"
#include "rectangle.h"
#include "text_2d.h"
#include "camera_helper.h"
#include "application/application.h"
#include "floating_panel.h"
#include "entity/ec.h"
#include "camera_helper.h"
#include "plot_helper.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <resource/resource_manager.h>
using namespace chimera;


Color FloatingPanel::kBackgroundColor = Color(1,1,1,0.15f);


void FloatingPanel::Update()
{
    // follow scale.x with main panel
    Transform* mainPanelTr = ObjectID::Find(ObjectID::PlotMainPanelRoot)->ToTransform();
    Transform* thisTransform = this->GetTransform();
    Vector3 scale = thisTransform->LocalScale();
    scale.x = mainPanelTr->LocalScale().x;
    thisTransform->SetLocalScale(scale);
    // update bounds
    Bounds screenBoundsInWorld = CameraHelper::VisibleAreaInXyPlane(Application::MainCamera());
    // Full 通过取 MaxEnd 转换为 Region 处理
    if (kind == Kind::OverlayMainPanel)
    {
        verticalStart = PlotHelper::GetFloatingPanelMaxEnd() + 0.05f;
        verticalEnd = 1 - 0.05f;
    }
    {
        Vector3 min = screenBoundsInWorld.min;
        Vector3 max = screenBoundsInWorld.max;
        float height = max.y - min.y;
        // bounds
        Vector3 start = Vector3(min.x, min.y + height * verticalStart, min.z);
        Vector3 end = Vector3(max.x, min.y + height * verticalEnd, max.z);
        bounds = Bounds(start, end);
    }
    // algin y
    Bounds screenBoundsInLocal;
    screenBoundsInLocal.min = thisTransform->WorldToLocalMatrix().MultiplyPoint3x4(screenBoundsInWorld.min);
    screenBoundsInLocal.max = thisTransform->WorldToLocalMatrix().MultiplyPoint3x4(screenBoundsInWorld.max);
    Bounds itemBoundsInLocal(Vector3(screenBoundsInLocal.min.x, 1e8f, 0), Vector3(screenBoundsInLocal.max.x, -1e8f, 0));
    PlotHelper::CollectBoundsInChildren(this->OwnerEntity(), itemBoundsInLocal);
    // Fix bug. 表现为如果把浮动面板内的 item 全部移出屏幕，再移动会来 item 全部消失了。
    // 如果为空，调整 itemBoundsInLocal 到合理值。猜测到问题出在这里，形成原因还没有想明白。
    if (std::abs(itemBoundsInLocal.Size().y) > 1e8f)
    {
        itemBoundsInLocal.min.y = 0;
        itemBoundsInLocal.max.y = 1;
    }
    // set scale.y
    if (kind == Kind::OverlayMainPanel)
    {
        // Full 不需要内部留白了
        scale.y = bounds.Size().y / itemBoundsInLocal.Size().y;
    }
    else if (kind == Kind::Region)
    {
        scale.y = bounds.Size().y * 0.9f / itemBoundsInLocal.Size().y;
    }
    thisTransform->SetLocalScale(scale);
    // set position.y
    Vector3 position = thisTransform->LocalPosition();
    position.y = bounds.Center().y - itemBoundsInLocal.Center().y * scale.y;
    thisTransform->SetLocalPosition(position);

    // update local bounds
    Matrix4x4 matrix = this->GetComponent<Transform>()->WorldToLocalMatrix();
    localBounds = Bounds(matrix.MultiplyPoint3x4(bounds.min), matrix.MultiplyPoint3x4(bounds.max));
}


void FloatingPanel::Render()
{
    // Full 是叠加到主面板，不需要渲染 Title 和 Background 了
    // Full 这个名字取的不够贴切
    if (kind == Kind::OverlayMainPanel)
    {
        return;
    }
    RenderTitle();
    RenderBackground();
}

void FloatingPanel::RenderTitle()
{
    Vector3 textPosition = GetTitlePosition();
    Text2D::DrawInWorld(panelName, textPosition, 18);
}


void FloatingPanel::RenderBackground()
{
    // Mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    int capcity = chimera::Rectangle::VerticesCount() * 2;
    vertices.reserve(capcity);
    colors.reserve(capcity);

    // Main Background
    // 可以直接用 bounds 的，计算量更小。这里用 localBounds 是为了验证下 localBounds 的值是否正确
    //Rectangle::GenerateMesh(bounds, kBackgroundColor, vertices, colors);
    Rectangle::GenerateMesh(localBounds, kBackgroundColor, vertices, colors);
    Matrix4x4Helper::ApplyMatrixForEach(this->GetTransform()->LocalToWorldMatrix(), vertices, 0, vertices.size());
    // Title Background
    Vector3 textPosition = GetTitlePosition();
    Bounds textBackgroundBounds = Text2D::CalculateBounds(panelName, textPosition, 18);
    textBackgroundBounds.Expand(Vector3(1.2f, 1, 1));
    Rectangle::GenerateMesh(textBackgroundBounds, kBackgroundColor, vertices, colors);

    Mesh mesh;
    mesh.SetVertices(vertices);
    mesh.SetColors(colors);
    // material
    auto shader = ResourceManager::LoadShader("src/shader/color_vertex.vs", "src/shader/color_vertex.fs", "", "color_vertex");
    Material material = Material(shader);
    material.SetColor("ColorTint", Color::white);
    // renderer
    auto renderer = World::OriginEntity()->GetOrAddComponent<MeshRenderer>();
    renderer->mesh = &mesh;
    renderer->material = &material;
    renderer->camera = Application::MainCamera();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderer->Render();
}

Vector3 FloatingPanel::GetTitlePosition()
{
    float x = bounds.min.x + (bounds.max.x - bounds.min.x) * 0.05f;
    Vector3 textPosition = Vector3(x, bounds.max.y, bounds.min.z);
    return textPosition;
}