#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"
#include "material.h"
#include "mesh.h"
#include "mesh_renderer.h"
#include "camera.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
Camera* CreateCamera();

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGT = 600;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGT, "Plot 3D", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// 监听窗口size改变
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize GLAD" << std::endl;
		return -1;
	}

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
	for (int i = 0; i < 10; ++i)
	{
		Entity* entity = World::ActiveWorld()->CreateEntity();
		MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
		renderer->material = &material;
		renderer->mesh = &mesh;

		Transform* tr = entity->GetComponent<Transform>();
		tr->SetLocalPosition(Vector3(cubePositions[i]));
		float radian = 20.0f * i * Mathf::Deg2Rad;
		Quaternion q = Quaternion::AngleAxis(radian, Vector3(1.0f, 0.3f, 0.5f));
		tr->SetLocalRotation(q);
	}

	Matrix4x4 projectionMatrix = camera->ProjectionMatrix();

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		// Matrix4x4 viewMatrix = Matrix4x4::LookAt(Vector3(camX, 0.0f, camZ), Vector3::zero, Vector3(0.0f, 1.0f, 0.0f));

		Quaternion r = Quaternion::LookAt(-Vector3(camX, 0.0f, camZ), Vector3(0.0f, 1.0f, 0.0f));
		camera->GetComponent<Transform>()->SetLocalRotation(r);
		camera->GetComponent<Transform>()->SetLocalPosition(Vector3(camX, 0.0f, camZ));
		Matrix4x4 viewMatrix = camera->WorldToCameraMatrix();

		for (MeshRenderer* renderer : World::ActiveWorld()->GetComponentsInEnities<MeshRenderer>())
		{
			Matrix4x4 modelMatrix = renderer->GetComponent<Transform>()->LocalToWorldMatrix();
			Matrix4x4 MVP = projectionMatrix * viewMatrix * modelMatrix;
			renderer->material->SetMatrix("MVP", MVP);
			renderer->Render();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Camera* CreateCamera()
{
	Entity* entity = World::ActiveWorld()->CreateEntity();
	Transform* transform = entity->GetComponent<Transform>();
	transform->SetLocalPosition(Vector3(0, 0, 3));
	Camera* camera = entity->AddComponent<Camera>();
	camera->fieldOfView = 45.0f * Mathf::Deg2Rad;
	camera->aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGT;

	return camera;
}