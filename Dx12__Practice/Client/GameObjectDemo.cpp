#include "pch.h"
#include "GameObjectDemo.h"

#include "MeshRenderer.h"

void GameObjectDemo::Init()
{
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(L"..\\Shaders\\02. ConstantBuffer.fx");

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init();

	_gameObject = make_shared<GameObject>();
	_gameObject->GetOrAddTransform();
	_gameObject->GetTransform()->SetPosition(Vec3(0.5f, 0.f, 0.f));
	_gameObject->AddComponent(make_shared<MeshRenderer>());
	_gameObject->GetMeshRenderer()->SetMesh(mesh);
	_gameObject->GetMeshRenderer()->SetShader(shader);

	
}

void GameObjectDemo::Update()
{
	_gameObject->Update();
}

void GameObjectDemo::Render()
{
}
