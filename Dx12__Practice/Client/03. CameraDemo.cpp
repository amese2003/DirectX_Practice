#include "pch.h"
#include "03. CameraDemo.h"
#include "Camera.h"
#include "CameraScript.h"

void CameraDemo::Init()
{
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(L"..\\Shaders\\03. World.fx");

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init();

	_gameObject = make_shared<GameObject>();
	
	_gameObject->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.f, 0.f));
	_gameObject->AddComponent(make_shared<MeshRenderer>());
	_gameObject->GetMeshRenderer()->SetMesh(mesh);
	_gameObject->GetMeshRenderer()->SetShader(shader);

	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.f, 5.f));
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void CameraDemo::Update()
{
	_camera->Update();
	_gameObject->Update();
	
}

void CameraDemo::Render()
{
	GRAPHICS->RenderBegin();
	GRAPHICS->RenderEnd();
}
