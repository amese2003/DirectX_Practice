#include "pch.h"
#include "06. ShapesDemo.h"
#include "Camera.h"
#include "CameraScript.h"

void ShapesDemo::Init()
{
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(L"..\\Shaders\\05. Color.fx");

	

	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		{
			mesh->Init();
			mesh->CreateCube();
		}

		shared_ptr<GameObject> box = make_shared<GameObject>();
		box->GetOrAddTransform()->SetPosition(Vec3(0.f, 5.f, 0.f));
		box->GetTransform()->SetLocalScale(Vec3(2.f, 1.f, 2.f));
		box->AddComponent(make_shared<MeshRenderer>());
		box->GetMeshRenderer()->SetMesh(mesh);
		box->GetMeshRenderer()->SetShader(shader);
		_gameObjects.push_back(box);
	}

	{
		shared_ptr<Mesh> sphere = make_shared<Mesh>();
		{
			sphere->Init();
			sphere->CreateSphere();
		}

		shared_ptr<GameObject> centerSphere = make_shared<GameObject>();
		centerSphere->GetOrAddTransform()->SetPosition(Vec3(1.f, 0.5f, 0.f));
		centerSphere->GetTransform()->SetLocalScale(Vec3(2.0f, 2.0f, 2.0f));
		centerSphere->AddComponent(make_shared<MeshRenderer>());
		centerSphere->GetMeshRenderer()->SetMesh(sphere);
		centerSphere->GetMeshRenderer()->SetShader(shader);
		_gameObjects.push_back(centerSphere);
	}

	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void ShapesDemo::Update()
{
	_camera->Update();

	for (auto gameObject : _gameObjects)
		gameObject->Update();

}

void ShapesDemo::Render()
{
}