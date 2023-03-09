#include "pch.h"
#include "RectangleDemo.h"
#include "Mesh.h"

void TriangleDemo::Init()
{
	_shader = make_shared<Shader>();
	_shader->Init(L"..\\Shaders\\default.fx");

	//_meshBuffer = make_shared<Mesh>();

	_mesh = make_shared<Mesh>();
	_mesh->Init();

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void TriangleDemo::Update()
{
}

void TriangleDemo::Render()
{
	GRAPHICS->RenderBegin();

	_shader->Update();
	_mesh->Render();

	GRAPHICS->RenderEnd();
}
