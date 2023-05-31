#include "pch.h"
#include "10. CrateDemo.h"
#include "Camera.h"
#include "CameraScript.h"

void CrateDemo::Init()
{
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Shaders\\07. Basic.fx");

		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		{
			mesh->Init();
			mesh->CreateCube();


			shared_ptr<Material> material = make_shared<Material>();
			material->SetAmbient(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
			material->SetDiffuse(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			material->SetSpecular(Vec4(0.6f, 0.6f, 0.6f, 16.0f));
			mesh->SetMateral(material);
		}

		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Load(L"..\\Resources\\Textures\\WoodCrate01.dds");

		_gameObject = make_shared<GameObject>();
		_gameObject->GetOrAddTransform();
		_gameObject->GetTransform()->SetPosition(Vec3(0.5f, 0.f, 5.f));
		_gameObject->AddComponent(make_shared<MeshRenderer>());
		_gameObject->GetMeshRenderer()->SetMesh(mesh);
		_gameObject->GetMeshRenderer()->SetShader(shader);
		_gameObject->GetMeshRenderer()->SetTexture(texture);

		CUR_SCENE->Add(_gameObject);
	}


	{
		shared_ptr<GameObject> dirLightObject = make_shared<GameObject>();
		dirLightObject->GetOrAddTransform();

		shared_ptr<Light> dirLight = make_shared<Light>();
		dirLight->Init(LightType::Dirlight);

		LightDesc dirDesc;
		dirDesc.ambient = Vec4(0.3f, 0.3f, 0.3f, 1.0f);
		dirDesc.diffuse = Vec4(0.8f, 0.8f, 0.8f, 1.0f);
		dirDesc.specular = Vec4(0.6f, 0.6f, 0.6f, 16.0f);
		dirDesc.direction = Vec3(0.707f, -0.707f, 0.0f);

		dirLight->SetLightDesc(dirDesc);
		dirLightObject->AddComponent(dirLight);
		CUR_SCENE->Add(dirLightObject);
	}

	{
		shared_ptr<GameObject> dirLightObject = make_shared<GameObject>();
		dirLightObject->GetOrAddTransform();

		shared_ptr<Light> dirLight = make_shared<Light>();
		dirLight->Init(LightType::Dirlight);

		LightDesc dirDesc;
		dirDesc.ambient = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
		dirDesc.diffuse = Vec4(1.4f, 1.4f, 1.4f, 1.0f);
		dirDesc.specular = Vec4(0.3f, 0.3f, 0.3f, 16.0f);
		dirDesc.direction = Vec3(-0.707f, 0.0f, 0.707f);

		dirLight->SetLightDesc(dirDesc);
		dirLightObject->AddComponent(dirLight);
		CUR_SCENE->Add(dirLightObject);
	}


	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	CUR_SCENE->Add(_camera);
	GRAPHICS->GetCommandQueue()->WaitSync();
}

void CrateDemo::Update()
{

}

void CrateDemo::Render()
{
}