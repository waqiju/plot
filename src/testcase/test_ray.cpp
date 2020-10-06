#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.h"
#include "window.h"
#include "entity/ec.h"
#include "geometry/geometry.h"
#include "application/application.h"

Camera* CreateCamera();


int main()
{
	CreateCamera();

    auto plane = Plane(Vector3(0, 0, 0), Vector3(0, 0, -1));
	// 左上
	auto ray = Application::MainCamera()->ViewportPointToRay(Vector3(-1, -1, -1));
	Vector3 hitPoint;
	if (Physics::Raycast(ray, plane, hitPoint))
	{
		std::cout << "左下: " << hitPoint.ToString() << std::endl;
	}
	ray = Application::MainCamera()->ViewportPointToRay(Vector3(-1, 1, -1));
	if (Physics::Raycast(ray, plane, hitPoint))
	{
		std::cout << "左上: " << hitPoint.ToString() << std::endl;
	}
	ray = Application::MainCamera()->ViewportPointToRay(Vector3(1, -1, -1));
	if (Physics::Raycast(ray, plane, hitPoint))
	{
		std::cout << "右下: " << hitPoint.ToString() << std::endl;
	}
	ray = Application::MainCamera()->ViewportPointToRay(Vector3(1, 1, -1));
	if (Physics::Raycast(ray, plane, hitPoint))
	{
		std::cout << "右上: " << hitPoint.ToString() << std::endl;
	}
	ray = Application::MainCamera()->ViewportPointToRay(Vector3(0, 0, -1));
	if (Physics::Raycast(ray, plane, hitPoint))
	{
		std::cout << "中间: " << hitPoint.ToString() << std::endl;
	}
    
    std::cin.get();
    return 0;
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