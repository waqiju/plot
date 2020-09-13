#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "material.h"
#include "mesh.h"
#include "mesh_renderer.h"
#include "camera.h"
#include "entity/ec.h"
#include "geometry/geometry.h"


void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
Camera* CreateCamera();


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Plot 3D", NULL, NULL);
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

	// shader
	Shader shader("src/shader/color.vs", "src/shader/color.fs");
	Material material(&shader);
	
	// vao
	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};
	Mesh mesh;
	mesh.SetVertices(vertices, sizeof(vertices));
	mesh.SetIndices(indices, sizeof(indices), MeshTopology::Triangles);
	// renderer
	Entity* entity = World::ActiveWorld()->CreateEntity();
	MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
	renderer->material = &material;
	renderer->mesh = &mesh;

	// mvp
	Matrix4x4 modelMatrix = entity->GetComponent<Transform>()->WorldToLocalMatrix();
	Matrix4x4 viewMatrix = Matrix4x4::LookAt(camera->GetComponent<Transform>()->LocalPosition(), 
		entity->GetComponent<Transform>()->LocalPosition(), 
		Vector3(0, 1, 0));
	Matrix4x4 projectMatrix = camera->ProjectionMatrix();
	Matrix4x4 mvp = projectMatrix * viewMatrix * modelMatrix;

	renderer->material->SetMatrix("MVP", mvp);
	renderer->material->SetVector3("Color", Vector3(1, 1, 1));

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		for (MeshRenderer* renderer : World::ActiveWorld()->GetComponentsInEnities<MeshRenderer>())
		{
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

	return camera;
}