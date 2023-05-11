#include "pch.h"
#include "07. WavesDemo.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Waves.h"

void WavesDemo::Init()
{
	

	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Shaders\\05. Color.fx");

		shared_ptr<Mesh> landMesh = make_shared<Mesh>();
		{
			landMesh->Init();
			landMesh->CreateHillDemo();
		}

		shared_ptr<GameObject> grid = make_shared<GameObject>();
		grid->GetOrAddTransform();
		grid->AddComponent(make_shared<MeshRenderer>());
		grid->GetMeshRenderer()->SetMesh(landMesh);
		grid->GetMeshRenderer()->SetShader(shader);
		_gameObjects.push_back(grid);
	}

	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Shaders\\05. Color.fx", true);

		shared_ptr<Mesh> waveMesh = make_shared<Mesh>();
		waveMesh->Init();

		shared_ptr<GameObject> grid = make_shared<GameObject>();
		grid->GetOrAddTransform();

		shared_ptr<Waves> wave = make_shared<Waves>();
		wave->Init(waveMesh, shader, 200, 200, 0.8f, 0.03f, 3.25f, 0.4f);
		grid->AddComponent(wave);

		_gameObjects.push_back(grid);
	}

	




	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void WavesDemo::Update()
{
	_camera->Update();

	for (auto gameObject : _gameObjects)
		gameObject->Update();

}

void WavesDemo::Render()
{
}