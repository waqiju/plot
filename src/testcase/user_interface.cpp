#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"
#include "material.h"
#include "mesh.h"
#include "mesh_renderer.h"
#include "camera.h"
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void ProcessInput();
void WindowSizeChangedHandler(Window* window, int width, int height);
void MouseScrollHandler(Window* window, double xoffset, double yoffset);
Camera* CreateCamera();


int main_1()
{
	Window* window = Window::CreateWindow("Chimera", Application::screenWidth, Application::screenHeight);
	window->onWindowSizeChanged = WindowSizeChangedHandler;
    window->onMouseScrollChanged = MouseScrollHandler;

	// camera
	Camera* camera = CreateCamera();
	glEnable(GL_DEPTH_TEST);

	// shader
	Shader shader("src/shader/color.vs", "src/shader/color.fs");
	Material material(&shader);
	
	// vao
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3 (2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
	Mesh mesh;
	mesh.SetVertices(vertices, sizeof(vertices)/sizeof(vertices[0]));
	// renderer
    Entity* rendererRoot = World::ActiveWorld()->CreateEntity();
    rendererRoot->name = "RendererRoot";
	for (int i = 0; i < 10; ++i)
	{
		Entity* entity = World::ActiveWorld()->CreateEntity();
		entity->GetComponent<Transform>()->SetParent(rendererRoot->GetComponent<Transform>());
		MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
		renderer->material = &material;
		renderer->mesh = &mesh;

		Transform* tr = entity->GetComponent<Transform>();
		tr->SetLocalPosition(Vector3(cubePositions[i]));
		float radian = 20.0f * i * Mathf::Deg2Rad;
		Quaternion q = Quaternion::AngleAxis(radian, Vector3(1.0f, 0.3f, 0.5f));
		tr->SetLocalRotation(q);
	}

	// main loop
	window->FrameLoop([&] {
		ProcessInput();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Quaternion r = Quaternion::AngleAxis((float)glfwGetTime(), Vector3(0, 1, 0));
        rendererRoot->GetComponent<Transform>()->SetLocalRotation(r);
		World::ActiveWorld()->FlushTransform();

		Matrix4x4 viewMatrix = camera->WorldToCameraMatrix();
		Matrix4x4 projectionMatrix = camera->ProjectionMatrix();
		for (MeshRenderer* renderer : World::ActiveWorld()->GetComponentsInRootEnities<MeshRenderer>())
		{
			Matrix4x4 modelMatrix = renderer->GetComponent<Transform>()->LocalToWorldMatrix();
			Matrix4x4 MVP = projectionMatrix * viewMatrix * modelMatrix;
			renderer->material->SetMatrix("MVP", MVP);
			renderer->Render();
		}
	});

	window->Close();
	return 0;
}

void ProcessInput()
{
    // 键盘移动
    Vector3 direction = Vector3::zero;
    if (Input::GetKey(GLFW_KEY_UP))
    {
        direction += Vector3(0, 1, 0);
    }
    if (Input::GetKey(GLFW_KEY_DOWN))
    {
        direction += Vector3(0, -1, 0);
    }
    if (Input::GetKey(GLFW_KEY_LEFT))
    {
        direction += Vector3(-1, 0, 0);
    }
    if (Input::GetKey(GLFW_KEY_RIGHT))
    {
        direction += Vector3(1, 0, 0);
    }
    if (direction != Vector3::zero)
    {
        auto tr = Application::MainCamera()->GetComponent<Transform>();
        tr->SetLocalPosition(tr->LocalPosition() + direction * Time::DeltaTime());
    }
    // 鼠标移动
    // TODO 用 ScreenPointToRay 实现是更通用的策略，直接 ScreenToViewport 是近截面
    static Vector3 panStartWorldPosition;
    static Vector3 panStartCameraPosition;
    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_1))
    {
        std::cout << "mouse down" << std::endl;
        Vector3 mousePosition = Input::MousePosition();
        panStartWorldPosition = Application::MainCamera()->ScreenToWorldPoint(mousePosition);
        panStartCameraPosition = Application::MainCamera()->GetComponent<Transform>()->LocalPosition();
    }
    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_1))
    {
        std::cout << "mouse press" << std::endl;
        Vector3 mousePosition = Input::MousePosition();
        Vector3 worldPosition = Application::MainCamera()->ScreenToWorldPoint(mousePosition);
        Vector3 panWorldDistance = worldPosition - panStartWorldPosition;
        Application::MainCamera()->GetComponent<Transform>()->SetLocalPosition(panStartCameraPosition - panWorldDistance);
    }
}

void WindowSizeChangedHandler(Window* window, int width, int height)
{
	glViewport(0, 0, width, height);
	for (auto camera : World::ActiveWorld()->GetComponentsInRootEnities<Camera>())
	{
        Application::screenWidth = width;
        Application::screenHeight = height;
		camera->aspect = (float)width / height;
	}
}

void MouseScrollHandler(Window* window, double xoffset, double yoffset)
{
    // 鼠标滚轮 zoom
    auto tr = Application::MainCamera()->GetComponent<Transform>();
    tr->SetLocalPosition(tr->LocalPosition() + tr->Forward() * yoffset);
}

Camera* CreateCamera()
{
	Entity* entity = World::ActiveWorld()->CreateEntity();
	Transform* transform = entity->GetComponent<Transform>();
	transform->SetLocalPosition(Vector3(0, 0, -10));
	Camera* camera = entity->AddComponent<Camera>();
	camera->fieldOfView = 45.0f * Mathf::Deg2Rad;
	camera->aspect = (float)Application::screenWidth / Application::screenHeight;
    camera->nearClipPlane = 5;
    camera->farClipPlane = 15;
	// 设置为主相机
	Application::SetMainCamera(camera);

	return camera;
}