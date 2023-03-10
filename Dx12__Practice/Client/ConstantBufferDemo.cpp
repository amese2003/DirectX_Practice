#include "pch.h"
#include "ConstantBufferDemo.h"
#include "Mesh.h"

void ConstantBufferDemo::Init()
{
	_shader = make_shared<Shader>();
	_shader->Init(L"..\\Shaders\\ConstantBufferDemo.fx");

	//_meshBuffer = make_shared<Mesh>();

	_mesh = make_shared<Mesh>();
	_mesh->Init();

	Transform transform;
	transform.offset = Vec4(1, 0, 0, 0);
	_mesh->SetTransform(transform);

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void ConstantBufferDemo::Update()
{
}

void ConstantBufferDemo::Render()
{
	GRAPHICS->RenderBegin();

	_shader->Update();
	_mesh->Render();

	GRAPHICS->RenderEnd();
}
