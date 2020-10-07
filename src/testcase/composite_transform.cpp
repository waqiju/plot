#include "gl_headers.h"
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
Camera* CreateCamera();

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGT = 600;


int main_3()
{
	Window* window = Window::CreateWindow("Chimera", SCREEN_WIDTH, SCREEN_HEIGT);
	window->onWindowSizeChanged = WindowSizeChangedHandler;

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
    if (Input::GetKey(GLFW_KEY_UP))
    {
        std::cout<<"up"<<std::endl;
        auto tr = Application::MainCamera()->GetComponent<Transform>();
        tr->SetLocalPosition(tr->LocalPosition() + Vector3(0, 1 * Time::DeltaTime(), 0));
    }
    if (Input::GetKey(GLFW_KEY_DOWN))
    {
        std::cout<<"down"<<std::endl;
        auto tr = Application::MainCamera()->GetComponent<Transform>();
        tr->SetLocalPosition(tr->LocalPosition() + Vector3(0, -1 * Time::DeltaTime(), 0));
    }
}

void WindowSizeChangedHandler(Window* window, int width, int height)
{
	glViewport(0, 0, width, height);
	for (auto camera : World::ActiveWorld()->GetComponentsInRootEnities<Camera>())
	{
		camera->aspect = (float)width / height;
	}
}

Camera* CreateCamera()
{
	Entity* entity = World::ActiveWorld()->CreateEntity();
	Transform* transform = entity->GetComponent<Transform>();
	transform->SetLocalPosition(Vector3(0, 0, -10));
	Camera* camera = entity->AddComponent<Camera>();
	camera->fieldOfView = 45.0f * Mathf::Deg2Rad;
	camera->aspect = (float)SCREEN_WIDTH / SCREEN_HEIGT;
	// 设置为主相机
	Application::SetMainCamera(camera);

	return camera;
}