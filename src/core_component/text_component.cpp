#include "text_component.h"
#include "mesh.h"
#include "material.h"
#include "resource/resource_manager.h"
#include "mesh_renderer.h"
#include "application/application.h"
#include "entity/ec.h"
#include "camera_helper.h"


TextComponent* TextComponent::Create(Transform* parent, std::string text, float fontSize, LayoutAlignment alignment)
{
    auto entity = World::ActiveWorld()->CreateEntity();
    auto textCp = entity->AddComponent<TextComponent>();
    textCp->text = text;
    textCp->fontSize = fontSize;
    textCp->alignment = alignment;

    entity->GetComponent<Transform>()->SetParent(parent);
    return textCp;
}


int TextComponent::GenerateMesh(const Bounds& bounds, const std::string& text, const Color& inColor, std::vector<Vector3>& vertices, std::vector<Color>& colors, std::vector<Vector2>& uvs)
{
    int verticesCount = text.length() * 4;

    float characterSpacing = bounds.max.x - bounds.min.x;
    float fontWidthInLocal = characterSpacing / 0.6f;
    float fontHeightInLocal = bounds.max.y - bounds.min.y;
    // 起点
    float x = bounds.min.x;
    float y = bounds.min.y;
    float z = bounds.min.z;

    for (size_t i = 0; i < text.length(); ++i)
    {
        // vertices
        Vector3 vertexUpLeft = Vector3(x + i * characterSpacing, y + fontHeightInLocal, z);
        Vector3 vertexUpRight = Vector3(x + i * characterSpacing + fontWidthInLocal, y + fontHeightInLocal, z);
        Vector3 vertexDownRight = Vector3(x + i * characterSpacing + fontWidthInLocal, y, z);
        Vector3 vertexDownLeft = Vector3(x + i * characterSpacing, y, z);

        vertices.push_back(vertexDownLeft);
        vertices.push_back(vertexUpRight);
        vertices.push_back(vertexUpLeft);

        vertices.push_back(vertexDownRight);
        vertices.push_back(vertexUpRight);
        vertices.push_back(vertexDownLeft);

        // uvs
        char character = text[i];
        float uvX = (character % 16) / 16.0f;
        float uvY = (character / 16) / 16.0f;

        float uvOffset = 1.0f / 16.0f;
        Vector2 uvUpLeft = Vector2(uvX, uvY);
        Vector2 uvUpRight = Vector2(uvX + uvOffset, uvY);
        Vector2 uvDownRight = Vector2(uvX + uvOffset, uvY+uvOffset);
        Vector2 uvDownLeft = Vector2(uvX, uvY+uvOffset);

        uvs.push_back(uvDownLeft);
        uvs.push_back(uvUpRight);
        uvs.push_back(uvUpLeft);

        uvs.push_back(uvDownRight);
        uvs.push_back(uvUpRight);
        uvs.push_back(uvDownLeft);

        // colors
        for (int i = 0;i < 6;++i)
        {
            colors.push_back(inColor);
        }
    }

    return verticesCount;
}

int TextComponent::GenerateMesh(std::vector<Vector3>& vertices, std::vector<Color>& colors, std::vector<Vector2>& uvs)
{
    return GenerateMesh(bounds, text, color, vertices, colors, uvs);
}

void TextComponent::AlignBounds()
{
    Transform* transform = this->GetTransform();
    Vector3 onePixelSizeInLocal = CameraHelper::OnePixelSizeInLocal(Application::MainCamera(), static_cast<float>(Application::screenHeight), transform->LocalToWorldMatrix());
    float fontWidthInLocal = onePixelSizeInLocal.x * fontSize;
    float characterSpacing = fontWidthInLocal * 0.6f;
    float fontHeightInLocal = onePixelSizeInLocal.y * fontSize;
    // 起点和 bounds
    const Vector3& position = transform->LocalPosition();
    float totalWidth = characterSpacing * text.length();
    switch (alignment)
    {
        case LayoutAlignment::Left:
        {
            bounds.min = position;
            bounds.max = position + Vector3(totalWidth, fontHeightInLocal, 0);
            break;
        }
        case LayoutAlignment::Center:
        {
            bounds.min = Vector3(position.x - totalWidth * 0.5f, position.y, position.z);
            bounds.max = bounds.min + Vector3(totalWidth, fontHeightInLocal, 0);
            break;
        }
        case LayoutAlignment::Right:
        {
            bounds.min = Vector3(position.x - totalWidth, position.y, position.z);
            bounds.max = bounds.min + Vector3(totalWidth, fontHeightInLocal, 0);
            break;
        }
    }
}

void TextComponent::Render()
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    std::vector<Vector2> uvs;
    GenerateMesh(vertices, colors, uvs);
    Mesh mesh;
    mesh.SetVertices(vertices);
    mesh.SetColors(colors);
    mesh.SetUVs(uvs);
    // material
    auto shader = ResourceManager::LoadShader("src/shader/text_in_world.vs", "src/shader/text_in_world.fs", "", "text_in_world");
    auto texture2D = ResourceManager::LoadTexture("art/consolas.png", true, "art/consolas.png");
    Material material = Material(shader);
    material.SetTexture("FontTextureSampler", texture2D->ID);
    material.SetColor("ColorTint", Color::white);
    // renderer
    auto renderer = m_OwerEntity->GetOrAddComponent<MeshRenderer>();
    renderer->mesh = &mesh;
    renderer->material = &material;
    renderer->camera = Application::MainCamera();
    glEnable(GL_BLEND);
    renderer->Render();
    glDisable(GL_BLEND);
}

void TextComponent::BatchRender(std::vector<TextComponent*> textList)
{
    // mesh
    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    std::vector<Vector2> uvs;

    for (TextComponent* item:textList)
    {
        int count = item->GenerateMesh(vertices, colors, uvs);
        Matrix4x4Helper::ApplyMatrixForEach(item->GetTransform()->LocalToWorldMatrix(), vertices, vertices.size() - count, vertices.size());
    }

    Mesh mesh;
    mesh.SetVertices(vertices);
    mesh.SetColors(colors);
    mesh.SetUVs(uvs);
    // material
    auto shader = ResourceManager::LoadShader("src/shader/text_in_world.vs", "src/shader/text_in_world.fs", "", "text_in_world");
    auto texture2D = ResourceManager::LoadTexture("art/consolas.png", true, "art/consolas.png");
    Material material = Material(shader);
    material.SetTexture("FontTextureSampler", texture2D->ID);
    material.SetColor("ColorTint", Color::white);
    // renderer
    auto renderer = World::OriginEntity()->GetOrAddComponent<MeshRenderer>();
    renderer->mesh = &mesh;
    renderer->material = &material;
    renderer->camera = Application::MainCamera();
    glEnable(GL_BLEND);
    renderer->Render();
    glDisable(GL_BLEND);
}
