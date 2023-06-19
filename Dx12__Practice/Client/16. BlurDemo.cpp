#include "pch.h"
#include "16. BlurDemo.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Waves.h"

void BlurDemo::Init()
{
	shared_ptr<Texture> grassTex = make_shared<Texture>();
	grassTex->Load(L"..\\Resources\\Textures\\grass.dds");
	RESOURCES->Add(L"grassTex", grassTex);

	shared_ptr<Texture> waterTex = make_shared<Texture>();
	waterTex->Load(L"..\\Resources\\Textures\\water2.dds");
	RESOURCES->Add(L"waterTex", waterTex);

	shared_ptr<Texture> wireTex = make_shared<Texture>();
	wireTex->Load(L"..\\Resources\\Textures\\WireFence.dds");
	RESOURCES->Add(L"wireTex", wireTex);


	shared_ptr<Texture> treearrayTex = make_shared<Texture>();
	treearrayTex->Load(L"..\\Resources\\Textures\\treearray.dds");
	RESOURCES->Add(L"treearrayTex", treearrayTex);



	shared_ptr<Shader> horizonblurShader = make_shared<Shader>();
	{
		ShaderInfo info;
		info.shaderType = SHADER_TYPE::COMPUTE;

		ShaderArg arg;
		arg.cs = "HorzBlurCS";

		horizonblurShader->Init(L"..\\Shaders\\14. Blur.fx", info, arg);
	}


	shared_ptr<Shader> vertblurShader = make_shared<Shader>();
	{
		ShaderInfo info;
		info.shaderType = SHADER_TYPE::COMPUTE;

		ShaderArg arg;
		arg.cs = "VertBlurCS";

		vertblurShader->Init(L"..\\Shaders\\14. Blur.fx", info, arg);
	}

	RESOURCES->Add<Shader>(L"HorzBlur", horizonblurShader);
	RESOURCES->Add<Shader>(L"VertBlur", vertblurShader);



	GRAPHICS->GetBlurFilter()->BuildExampleResource();
	


	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Shaders\\07. Basic.fx");

		

		shared_ptr<Mesh> landMesh = make_shared<Mesh>();
		{
			landMesh->Init();
			landMesh->CreateHillDemo();

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
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Shaders\\07. Basic.fx");

		shared_ptr<Material> material = make_shared<Material>();
		material->SetAmbient(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
		material->SetDiffuse(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		material->SetSpecular(Vec4(0.6f, 0.6f, 0.6f, 16.0f));
		material->SetShader(shader);

		shared_ptr<Mesh> waveMesh = make_shared<Mesh>();
		{
			waveMesh->Init();
			waveMesh->SetMateral(material);
		}

		shared_ptr<MeshRenderer> render = make_shared<MeshRenderer>();
		{
			render->SetMesh(waveMesh);
			render->SetTexture(waterTex);
			render->SetMateral(material);
		}

		shared_ptr<Waves> wave = make_shared<Waves>();
		{
			wave->Init(waveMesh, shader, 160, 160, 1.f, 0.03f, 3.25f, 0.4f);
			wave->SetTexture(waterTex);
		}
		
		

		shared_ptr<GameObject> grid = make_shared<GameObject>();
		grid->GetOrAddTransform();
		grid->AddComponent(render);
		grid->AddComponent(wave);

		CUR_SCENE->Add(grid);
	}

	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		{
			ShaderInfo info;
			info.rasterizerType = RASTERIZER_TYPE::NoCullRS;
			shader->Init(L"..\\Shaders\\08. Basic.fx", info);
		}


		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		{
			mesh->Init();
			mesh->CreateCube();


			shared_ptr<Material> material = make_shared<Material>();
			material->SetAmbient(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
			material->SetDiffuse(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			material->SetSpecular(Vec4(0.6f, 0.6f, 0.6f, 16.0f));
			material->SetShader(shader);
			mesh->SetMateral(material);
		}

		

		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->GetOrAddTransform();
		gameObject->GetTransform()->SetPosition(Vec3(8.0f, 5.0f, -15.0f));
		gameObject->GetTransform()->SetLocalScale(Vec3(15.0f, 15.0f, 15.0f));
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(mesh);
		gameObject->GetMeshRenderer()->SetShader(shader);
		gameObject->GetMeshRenderer()->SetTexture(wireTex);

		CUR_SCENE->Add(gameObject);
	}

	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		{
			ShaderInfo info;
			info.blendType = BLEND_TYPE::AlphaToCoverageBS;
			info.topologyType = TOPOLOGY_TYPE::POINT;

			ShaderArg args;
			args.gs = "GS_Main";
			shader->Init(L"..\\Shaders\\12. TreeSprite.fx", info, args);
		}


		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		{
			mesh->Init();
			mesh->CreateTreeSprites();


			shared_ptr<Material> material = make_shared<Material>();
			material->SetAmbient(Vec4(1.f, 1.f, 1.f, 1.0f));
			material->SetDiffuse(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			material->SetSpecular(Vec4(0.6f, 0.6f, 0.6f, 16.0f));
			material->SetShader(shader);
			mesh->SetMateral(material);
		}

		

		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->GetOrAddTransform();
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(mesh);
		gameObject->GetMeshRenderer()->SetShader(shader);
		gameObject->GetMeshRenderer()->SetTexture(treearrayTex);

		CUR_SCENE->Add(gameObject);
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

void BlurDemo::Update()
{


}

void BlurDemo::Render()
{
}