#include "pch.h"
#include "05. HillsDemo.h"
#include "Camera.h"
#include "CameraScript.h"

void HillsDemo::Init()
{
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(L"..\\Shaders\\05. Color.fx");

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	{
		mesh->Init();
		mesh->CreateHillDemo();
	}


	_gameObject = make_shared<GameObject>();
	_gameObject->GetOrAddTransform();
	_gameObject->GetTransform()->SetPosition(Vec3(0.5f, 0.f, 5.f));
	_gameObject->AddComponent(make_shared<MeshRenderer>());
	_gameObject->GetMeshRenderer()->SetMesh(mesh);
	_gameObject->GetMeshRenderer()->SetShader(shader);
	CUR_SCENE->Add(_gameObject);

	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());
	CUR_SCENE->Add(_camera);

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void HillsDemo::Update()
{
	_camera->Update();
	_gameObject->Update();
}

void HillsDemo::Render()
{
}