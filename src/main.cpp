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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void processInput(GLFWwindow* window);
void WindowSizeChangedHandler(Window* window, int width, int height);
Camera* CreateCamera();

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGT = 600;


int main()
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
	std::vector<MeshRenderer*> renderers;
    std::vector<Matrix4x4> matrixList;
	for (int i = 0; i < 10; ++i)
	{
		Entity* entity = World::ActiveWorld()->CreateEntity();
		MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
		renderers.push_back(renderer);
		renderer->material = &material;
		renderer->mesh = &mesh;

		Transform* tr = entity->GetComponent<Transform>();
		tr->SetLocalPosition(Vector3(cubePositions[i]));
		float radian = 20.0f * i * Mathf::Deg2Rad;
		Quaternion q = Quaternion::AngleAxis(radian, Vector3(1.0f, 0.3f, 0.5f));
		tr->SetLocalRotation(q);
        matrixList.push_back(tr->LocalToWorldMatrix());
	}

	// main loop
	window->FrameLoop([&] {
		processInput(window->RawGLFWwindow());
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Transform* cameraTransform = camera->GetComponent<Transform>();
		//Quaternion r = Quaternion::AngleAxis(glfwGetTime(), Vector3(0, 1, 0));
		//cameraTransform->SetLocalRotation(r);
		//Vector3 p = -cameraTransform->Forward() * 10;
		//camera->GetComponent<Transform>()->SetLocalPosition(p);
		for (int i = 0; i < 10; ++i)
		{
			MeshRenderer* renderer = renderers[i];
			Quaternion r = Quaternion::AngleAxis(glfwGetTime(), Vector3(0, 1, 0));
			Transform* transform = renderer->GetComponent<Transform>();

			Matrix4x4 newMatrix = Matrix4x4::Rotate(r) * matrixList[i];
			transform->SetTrsMatrix(newMatrix);
		}

		Matrix4x4 viewMatrix = camera->WorldToCameraMatrix();
		Matrix4x4 projectionMatrix = camera->ProjectionMatrix();
		for (MeshRenderer* renderer : World::ActiveWorld()->GetComponentsInEnities<MeshRenderer>())
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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


void WindowSizeChangedHandler(Window* window, int width, int height)
{
	glViewport(0, 0, width, height);
	for (auto camera : World::ActiveWorld()->GetComponentsInEnities<Camera>())
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

	return camera;
}