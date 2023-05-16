#include "pch.h"
#include "08. LightingDemo.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Waves.h"
#include "Material.h"
#include "Light.h"
#include "MathHelper.h"

void LightingDemo::Init()
{
	


	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Shaders\\06. Lighting.fx");

		

		shared_ptr<Material> material = make_shared<Material>();
		{
			material->Init();

			Vec4 ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
			Vec4 diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
			Vec4 specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

			material->SetAmbient(ambient);
			material->SetDiffuse(diffuse);
			material->SetSpecular(specular);
		}	

		shared_ptr<Mesh> landMesh = make_shared<Mesh>();
		{
			landMesh->Init();
			landMesh->CreateHillDemo();
			landMesh->SetMateral(material);
		}


		shared_ptr<GameObject> grid = make_shared<GameObject>();
		grid->GetOrAddTransform()->SetPosition(Vec3(0.f, -3.f, 0.f));
		grid->AddComponent(make_shared<MeshRenderer>());
		grid->GetMeshRenderer()->SetMesh(landMesh);
		grid->GetMeshRenderer()->SetShader(shader);
		grid->GetMeshRenderer()->GetMesh()->SetMateral(material);

		CUR_SCENE->Add(grid);
	}

	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Shaders\\06. Lighting.fx");

		shared_ptr<Material> material = make_shared<Material>();
		{
			material->Init();

			Vec4 ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
			Vec4 diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
			Vec4 specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

			material->SetAmbient(ambient);
			material->SetDiffuse(diffuse);
			material->SetSpecular(specular);
		}

		shared_ptr<Mesh> waveMesh = make_shared<Mesh>();
		waveMesh->Init();
		waveMesh->SetMateral(material);

		shared_ptr<GameObject> grid = make_shared<GameObject>();
		grid->GetOrAddTransform()->SetPosition(Vec3(0.f, -10.f, 0.f));

		shared_ptr<Waves> wave = make_shared<Waves>();
		wave->Init(waveMesh, shader, 200, 200, 0.8f, 0.03f, 3.25f, 0.4f);
		wave->SetMesh(waveMesh);
		grid->AddComponent(wave);
		CUR_SCENE->Add(grid);
	}

	{
		shared_ptr<GameObject> dirLightObject = make_shared<GameObject>();
		dirLightObject->GetOrAddTransform();

		shared_ptr<Light> dirLight = make_shared<Light>();
		dirLight->Init(LightType::Dirlight);

		LightDesc dirDesc;
		dirDesc.ambient = Vec4(0.2f, 0.2f, 0.2f, 1.0f);;
		dirDesc.diffuse = Vec4(0.5f, 0.5f, 0.5f, 1.0f);;
		dirDesc.specular = Vec4(0.5f, 0.5f, 0.5f, 1.0f);
		dirDesc.direction = Vec3(0.57735f, -0.57735f, 0.57735f);

		dirLight->SetLightDesc(dirDesc);
		dirLightObject->AddComponent(dirLight);
		CUR_SCENE->Add(dirLightObject);
	}

	{
		shared_ptr<GameObject> spotLightObject = make_shared<GameObject>();
		spotLightObject->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.f, 10.f));

		shared_ptr<Light> spotLight = make_shared<Light>();
		spotLight->Init(LightType::SpotLight);

		LightDesc spotDesc;
		spotDesc.ambient = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		spotDesc.diffuse = Vec4(1.0f, 1.0f, 0.0f, 1.0f);
		spotDesc.specular = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
		spotDesc.Att = Vec3(1.f, 0.f, 0.f);
		spotDesc._spot = 96.f;
		spotDesc.range = 10000.f;

		spotLight->SetLightDesc(spotDesc);
		spotLightObject->AddComponent(spotLight);
		CUR_SCENE->Add(spotLightObject);
	}

	{
		shared_ptr<GameObject> pointLightObject = make_shared<GameObject>();
		pointLightObject->GetOrAddTransform()->SetPosition(Vec3(10.f, 2.f, 10.f));

		shared_ptr<Light> pointLight = make_shared<Light>();
		pointLight->Init(LightType::PointLight);

		LightDesc pointDesc;
		pointDesc.ambient = Vec4(0.3f, 0.3f, 0.3f, 1.0f);
		pointDesc.diffuse = Vec4(0.7f, 0.7f, 0.7f, 1.0f);
		pointDesc.specular = Vec4(0.7f, 0.7f, 0.7f, 1.0f);
		pointDesc.range = 25.f;
		pointDesc.Att = Vec3(0.f, 0.1f, 0.f);

		pointLight->SetLightDesc(pointDesc);
		pointLightObject->AddComponent(pointLight);
		CUR_SCENE->Add(pointLightObject);
	}


	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());
	CUR_SCENE->Add(_camera);

	CUR_SCENE->PushLightData();
	GRAPHICS->GetCommandQueue()->WaitSync();
}

void LightingDemo::Update()
{


	

}

void LightingDemo::Render()
{
}