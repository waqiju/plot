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
    Camera* camera = Application::MainCamera();
    Vector3 min = CameraHelper::ViewportToXyPlane(camera, Vector3(-1, -1, 0));
    Vector3 max = CameraHelper::ViewportToXyPlane(camera, Vector3(1, 1, 0));
    switch (kind)
    {
        case Kind::Full:
        {
            bounds = Bounds(min, max);
            // std::cout << min.ToString() << " " << max.ToString() << std::endl;
            break;
        }
        case Kind::Region:
        {
            float height = max.y - min.y;
            Vector3 start = Vector3(min.x, min.y + height * verticalStart, min.z);
            Vector3 end = Vector3(max.x, min.y + height * verticalEnd, max.z);
            bounds = Bounds(start, end);
            Matrix4x4 matrix = this->GetComponent<Transform>()->WorldToLocalMatrix();
            localBounds = Bounds(matrix.MultiplyPoint3x4(bounds.min), matrix.MultiplyPoint3x4(bounds.max));
            break;
        }
    }
    // algin y
    Bounds screenBoundsInWorld = CameraHelper::VisibleAreaInXyPlane(Application::MainCamera());
    Bounds screenBoundsInLocal;
    screenBoundsInLocal.min = thisTransform->WorldToLocalMatrix().MultiplyPoint3x4(screenBoundsInWorld.min);
    screenBoundsInLocal.max = thisTransform->WorldToLocalMatrix().MultiplyPoint3x4(screenBoundsInWorld.max);
    Bounds plotBoundsInLocal(Vector3(screenBoundsInWorld.min.x, 1e8f, 0), Vector3(screenBoundsInWorld.max.x, -1e8f, 0));
    PlotHelper::CollectBoundsInChildren(this->OwnerEntity(), plotBoundsInLocal);
    // set scale.y
    scale.y = bounds.Size().y * 0.9f / plotBoundsInLocal.Size().y;
    thisTransform->SetLocalScale(scale);
    // set position.y
    Vector3 position = thisTransform->LocalPosition();
    position.y = bounds.Center().y - plotBoundsInLocal.Center().y * scale.y;
    thisTransform->SetLocalPosition(position);

    // update local bounds
    Matrix4x4 matrix = this->GetComponent<Transform>()->WorldToLocalMatrix();
    localBounds = Bounds(matrix.MultiplyPoint3x4(bounds.min), matrix.MultiplyPoint3x4(bounds.max));
}


void FloatingPanel::Render()
{
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