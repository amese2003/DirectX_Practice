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
	
	
	/*shared_ptr<Light> dirLight = make_shared<Light>();
	dirLight->Init(LightType::Dirlight);
	shared_ptr<Material> dirmaterial = make_shared<Material>();
	{
		dirmaterial->Init();

		Vec4 ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);;
		Vec4 diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);;
		Vec4 specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

		dirmaterial->SetAmbient(ambient);
		dirmaterial->SetDiffuse(diffuse);
		dirmaterial->SetSpecular(specular);
	}
	dirLight->SetDir(Vec3(0.57735f, -0.57735f, 0.57735f));
	dirLight->SetMaterial(dirmaterial);
	


	shared_ptr<Light> pointLight = make_shared<Light>();
	pointLight->Init(LightType::PointLight);
	shared_ptr<Material> pointMat = make_shared<Material>();
	{
		pointMat->Init();

		Vec4 ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		Vec4 diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
		Vec4 specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);

		pointMat->SetAmbient(ambient);
		pointMat->SetDiffuse(diffuse);
		pointMat->SetSpecular(specular);
	}
	pointLight->SetRange(25.f);
	pointLight->SetMaterial(pointMat);
	pointLight->SetAtt(Vec3(0.f, 0.1f, 0.f));
	

	shared_ptr<Light> spotLight = make_shared<Light>();
	spotLight->Init(LightType::SpotLight);
	shared_ptr<Material> spotMat = make_shared<Material>();
	{
		spotMat->Init();

		Vec4 ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		Vec4 specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		spotMat->SetAmbient(ambient);
		spotMat->SetDiffuse(diffuse);
		spotMat->SetSpecular(specular);
	}
	spotLight->SetSpot(1.f);
	spotLight->SetRange(10000.f);
	spotLight->SetMaterial(spotMat);
	spotLight->SetAtt(Vec3(1.f, 0.f, 0.f));*/


	/*{
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
		grid->GetMeshRenderer()->SetDir(dirLight);
		grid->GetMeshRenderer()->SetSpot(spotLight);
		grid->GetMeshRenderer()->SetPoint(pointLight);

		_gameObjects.push_back(grid);
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
		wave->SetDir(dirLight);
		wave->SetSpot(spotLight);
		wave->SetPoint(pointLight);
		wave->SetMesh(waveMesh);

		grid->AddComponent(wave);

		_gameObjects.push_back(grid);
	}

	{
		shared_ptr<GameObject> dirLightObject = make_shared<GameObject>();
		dirLightObject->GetOrAddTransform();
		dirLightObject->AddComponent(dirLight);
		_dirLight = dirLightObject;
	}

	{
		shared_ptr<GameObject> spotLightObject = make_shared<GameObject>();
		spotLightObject->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.f, 0.f));
		spotLightObject->AddComponent(spotLight);
		_spotLight = spotLightObject;
	}

	{
		shared_ptr<GameObject> pointLightObject = make_shared<GameObject>();
		pointLightObject->GetOrAddTransform()->SetPosition(Vec3(0.f, 2.f, -30.f));
		pointLightObject->AddComponent(pointLight);
		_pointLight = pointLightObject;
	}*/

	

	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	GRAPHICS->GetCommandQueue()->WaitSync();
}

void LightingDemo::Update()
{
	_camera->Update();


	//_pointLight->GetTransform()->SetPosition(Vec3(70.0f * cosf(0.2f * TIME->TotalTime()), 70.0f * sinf(0.2f * TIME->TotalTime()), MathHelper::Max(GetHillHeight(_pointLight->GetTransform()->GetPosition().x, _pointLight->GetTransform()->GetPosition().z), -3.0f) + 10.0f));
	_pointLight->Update();
	
	/*Vec3 dir = Vec3(0.f, 0.f, 0.f) - Camera::S_Eyepos;
	dir.Normalize();
	_spotLight->GetLight()->SetDir(dir);
	_spotLight->Update();
	*/

	_dirLight->Update();

	for (auto gameObject : _gameObjects)
		gameObject->Update();

	

}

void LightingDemo::Render()
{
}