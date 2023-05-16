#include "pch.h"
#include "06. ShapesDemo.h"
#include "Camera.h"
#include "CameraScript.h"

void ShapesDemo::Init()
{
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(L"..\\Shaders\\05. Color.fx", true);

	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		{
			mesh->Init();
			mesh->CreateGrid();
		}

		shared_ptr<GameObject> grid = make_shared<GameObject>();
		grid->GetOrAddTransform();
		grid->AddComponent(make_shared<MeshRenderer>());
		grid->GetMeshRenderer()->SetMesh(mesh);
		grid->GetMeshRenderer()->SetShader(shader);
		_gameObjects.push_back(grid);
		CUR_SCENE->Add(grid);
	}

	

	{
		shared_ptr<Shader> shader1 = make_shared<Shader>();
		shader1->Init(L"..\\Shaders\\05. Color.fx");

		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		{
			mesh->Init();
			mesh->CreateCube();
		}

		shared_ptr<GameObject> box = make_shared<GameObject>();
		box->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.5f, 0.f));
		box->GetTransform()->SetLocalScale(Vec3(2.f, 1.f, 2.f));
		box->AddComponent(make_shared<MeshRenderer>());
		box->GetMeshRenderer()->SetMesh(mesh);
		box->GetMeshRenderer()->SetShader(shader1);
		_gameObjects.push_back(box);
		CUR_SCENE->Add(box);
	}

	{
		shared_ptr<Mesh> sphere = make_shared<Mesh>();
		{
			sphere->Init();
			sphere->CreateSphere();
		}

		shared_ptr<GameObject> centerSphere = make_shared<GameObject>();
		centerSphere->GetOrAddTransform()->SetPosition(Vec3(0.f, 2.f, 0.f));
		centerSphere->GetTransform()->SetLocalScale(Vec3(2.0f, 2.0f, 2.0f));
		centerSphere->AddComponent(make_shared<MeshRenderer>());
		centerSphere->GetMeshRenderer()->SetMesh(sphere);
		centerSphere->GetMeshRenderer()->SetShader(shader);
		_gameObjects.push_back(centerSphere);
		CUR_SCENE->Add(centerSphere);
	}


	vector<shared_ptr<GameObject>> cyl;
	cyl.resize(10);

	vector<shared_ptr<GameObject>> spheres;
	spheres.resize(10);

	for (int32 i = 0; i < 5; i++)
	{
		{
			shared_ptr<Mesh> boxMesh = make_shared<Mesh>();
			boxMesh->Init();
			boxMesh->CreateCylinder();

			shared_ptr<GameObject> box = make_shared<GameObject>();
			box->GetOrAddTransform()->SetPosition(Vec3(-5.0f, 1.5f, -10.0f + i * 5.0f));
			box->AddComponent(make_shared<MeshRenderer>());
			box->GetMeshRenderer()->SetMesh(boxMesh);
			box->GetMeshRenderer()->SetShader(shader);
			cyl[i * 2 + 0] = box;
		}

		{
			shared_ptr<Mesh> boxMesh = make_shared<Mesh>();
			boxMesh->Init();
			boxMesh->CreateCylinder();

			shared_ptr<GameObject> box = make_shared<GameObject>();
			box->GetOrAddTransform()->SetPosition(Vec3(5.0f, 1.5f, -10.0f + i * 5.0f));
			box->AddComponent(make_shared<MeshRenderer>());
			box->GetMeshRenderer()->SetMesh(boxMesh);
			box->GetMeshRenderer()->SetShader(shader);
			cyl[i * 2 + 1] = box;
		}

		{
			shared_ptr<Mesh> sphereMesh = make_shared<Mesh>();
			sphereMesh->Init();
			sphereMesh->CreateSphere();

			shared_ptr<GameObject> sphere = make_shared<GameObject>();
			sphere->GetOrAddTransform()->SetPosition(Vec3(-5.0f, 3.5f, -10.0f + i * 5.0f));
			sphere->AddComponent(make_shared<MeshRenderer>());
			sphere->GetMeshRenderer()->SetMesh(sphereMesh);
			sphere->GetMeshRenderer()->SetShader(shader);
			spheres[i * 2 + 0] = sphere;
		}
		
		{
			shared_ptr<Mesh> sphereMesh = make_shared<Mesh>();
			sphereMesh->Init();
			sphereMesh->CreateSphere();

			shared_ptr<GameObject> sphere = make_shared<GameObject>();
			sphere->GetOrAddTransform()->SetPosition(Vec3(+5.0f, 3.5f, -10.0f + i * 5.0f));
			sphere->AddComponent(make_shared<MeshRenderer>());
			sphere->GetMeshRenderer()->SetMesh(sphereMesh);
			sphere->GetMeshRenderer()->SetShader(shader);
			spheres[i * 2 + 1] = sphere;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		_gameObjects.push_back(cyl[i]);
		_gameObjects.push_back(spheres[i]);

		CUR_SCENE->Add(cyl[i]);
		CUR_SCENE->Add(spheres[i]);
	}



	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());
	CUR_SCENE->Add(_camera);

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void ShapesDemo::Update()
{
	_camera->Update();

	for (auto gameObject : _gameObjects)
		gameObject->Update();

}

void ShapesDemo::Render()
{
}