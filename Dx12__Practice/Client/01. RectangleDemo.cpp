#include "pch.h"
#include "01. RectangleDemo.h"
#include "Mesh.h"

void RectangleDemo::Init()
{
	_shader = make_shared<Shader>();
	_shader->Init(L"..\\Shaders\\01. Rectangle.fx");

	//_meshBuffer = make_shared<Mesh>();

	_mesh = make_shared<Mesh>();
	_mesh->Init();

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void RectangleDemo::Update()
{
}

void RectangleDemo::Render()
{
	GRAPHICS->RenderBegin();

	_shader->Update();
	//_mesh->Render();

	GRAPHICS->RenderEnd();
}
