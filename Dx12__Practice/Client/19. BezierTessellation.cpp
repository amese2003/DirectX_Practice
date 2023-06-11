#include "pch.h"
#include "19. BezierTessellation.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Waves.h"

void BezierTessellation::Init()
{
	shared_ptr<Texture> grassTex = make_shared<Texture>();
	grassTex->Load(L"..\\Resources\\Textures\\grass.dds");
	RESOURCES->Add(L"grassTex", grassTex);



	


	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		{
			ShaderInfo info;
			info.rasterizerType = RASTERIZER_TYPE::WireframeOnly;
			info.topologyType = TOPOLOGY_TYPE::PATCH;

			ShaderArg args;
			args.hs = "HS_Main";
			args.ds = "DS_Main";

			shader->Init(L"..\\Shaders\\17. BezierTessellation.fx", info, args);;
		}
		

		

		shared_ptr<Mesh> landMesh = make_shared<Mesh>();
		{
			landMesh->Init();
			landMesh->CreateQuad();

			shared_ptr<Material> material = make_shared<Material>();
			material->SetAmbient(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
			material->SetDiffuse(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			material->SetSpecular(Vec4(0.6f, 0.6f, 0.6f, 16.0f));
			material->SetShader(shader);
			landMesh->SetMateral(material);
		}

		shared_ptr<GameObject> grid = make_shared<GameObject>();
		grid->GetOrAddTransform();
		grid->AddComponent(make_shared<MeshRenderer>());
		grid->GetMeshRenderer()->SetMesh(landMesh);
		grid->GetMeshRenderer()->SetTexture(grassTex);
		CUR_SCENE->Add(grid);

		GRAPHICS->GetBlurFilter()->_test = grid;
		GRAPHICS->_test = grid;
	}

	



	{
		shared_ptr<GameObject> dirLightObject = make_shared<GameObject>();
		dirLightObject->GetOrAddTransform();

		shared_ptr<Light> dirLight = make_shared<Light>();
		dirLight->Init(LightType::Dirlight);

		LightDesc dirDesc;
		dirDesc.ambient = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
		dirDesc.diffuse = Vec4(0.5f, 0.5f, 0.5f, 1.0f);
		dirDesc.specular = Vec4(0.5f, 0.5f, 0.5f, 1.0f);
		dirDesc.direction = Vec3(0.57735f, -0.57735f, 0.57735f);

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
		dirDesc.ambient = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		dirDesc.diffuse = Vec4(0.20f, 0.20f, 0.20f, 1.0f);
		dirDesc.specular = Vec4(0.25f, 0.25f, 0.25f, 1.0f);
		dirDesc.direction = Vec3(-0.57735f, -0.57735f, 0.57735f);

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
		dirDesc.ambient = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		dirDesc.diffuse = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
		dirDesc.specular = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		dirDesc.direction = Vec3(0.0f, -0.707f, -0.707f);

		dirLight->SetLightDesc(dirDesc);
		dirLightObject->AddComponent(dirLight);
		CUR_SCENE->Add(dirLightObject);
	}

	shared_ptr<GameObject> cameraObject = make_shared<GameObject>();
	cameraObject->GetOrAddTransform();
	cameraObject->AddComponent(make_shared<Camera>());
	cameraObject->AddComponent(make_shared<CameraScript>());
	CUR_SCENE->Add(cameraObject);

	
	GRAPHICS->GetCommandQueue()->WaitSync();
}

void BezierTessellation::Update()
{


}

void BezierTessellation::Render()
{
}