#include "pch.h"
#include "RectangleDemo.h"
#include "Mesh.h"

void RectangleDemo::Init()
{
	_shader = make_shared<Shader>();
	_shader->Init(L"..\\Shaders\\default.fx");

	//_meshBuffer = make_shared<Mesh>();

	_meshBuffer = make_shared<Mesh>();
	_meshBuffer->Init();

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void RectangleDemo::Update()
{
}

void RectangleDemo::Render()
{
	GRAPHICS->RenderBegin();

	_shader->Update();
	_meshBuffer->Render();

	GRAPHICS->RenderEnd();
}
