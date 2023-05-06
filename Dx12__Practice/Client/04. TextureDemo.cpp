#include "pch.h"
#include "04. TextureDemo.h"
#include "Camera.h"
#include "CameraScript.h"

void TextureDemo::Init()
{
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(L"..\\Shaders\\04. Texture.fx");

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	{
		mesh->Init();
		mesh->CreateCube();
	}

	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->Init(L"..\\Resources\\Hoshino.png");

	_gameObject = make_shared<GameObject>();
	_gameObject->GetOrAddTransform();
	_gameObject->GetTransform()->SetPosition(Vec3(0.5f, 0.f, 10.f));
	_gameObject->AddComponent(make_shared<MeshRenderer>());
	_gameObject->GetMeshRenderer()->SetMesh(mesh);
	_gameObject->GetMeshRenderer()->SetShader(shader);
	_gameObject->GetMeshRenderer()->SetTexture(texture);

	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void TextureDemo::Update()
{
	_camera->Update();
	_gameObject->Update();
}

void TextureDemo::Render()
{
}