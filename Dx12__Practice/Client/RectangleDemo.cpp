#include "pch.h"
#include "RectangleDemo.h"
#include "Mesh.h"

void RectangleDemo::Init()
{
	_shader = make_shared<Shader>();
	_shader->Init(L"..\\Shaders\\default.fx");

	//_meshBuffer = make_shared<Mesh>();

	_meshBuffer = make_shared<Mesh>();
	vector<VertexColorData> vec(3);
	vec[0].position = Vec3(0.f, 0.5f, 0.5f);
	vec[1].position = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].position = Vec3(-0.5f, -0.5f, 0.5f);
	_meshBuffer->Init(vec);

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
