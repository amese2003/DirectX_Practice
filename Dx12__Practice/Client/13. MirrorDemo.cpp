#include "pch.h"
#include "13. MirrorDemo.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Waves.h"
#include "Material.h"
#include "Light.h"
#include "MathHelper.h"
#include "SkullScript.h"

void MirrorDemo::Init()
{
	
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());
	CUR_SCENE->Add(_camera);

	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		{
			mesh->Init();
			mesh->CreateMirror();


			shared_ptr<Material> material = make_shared<Material>();
			material->SetAmbient(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
			material->SetDiffuse(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			material->SetSpecular(Vec4(0.6f, 0.6f, 0.6f, 16.0f));
			mesh->SetMateral(material);

			shared_ptr<Shader> shader = make_shared<Shader>();
			{
				ShaderInfo desc;
				desc.blendType = BLEND_TYPE::NoRenderTargetWritesBS;
				desc.depthStencilType = DEPTH_STENCIL_TYPE::MarkMirrorDSS;

				shader->Init(L"..\\Shaders\\08. Basic.fx", desc);
			}



			material->SetShader(shader);
		}

		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Load(L"..\\Resources\\Textures\\ice.dds");

		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->GetOrAddTransform();
		gameObject->GetTransform()->SetPosition(Vec3(0.5f, 0.f, 0.f));
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(mesh);
		gameObject->GetMeshRenderer()->SetShader(nullptr);
		gameObject->GetMeshRenderer()->SetTexture(texture);

		CUR_SCENE->Add(gameObject);
	}


	{
		shared_ptr<Material> material = make_shared<Material>();
		{
			material->Init();

			shared_ptr<Shader> shader = make_shared<Shader>();
			{
				shader->Init(L"..\\Shaders\\08. Basic.fx");
			}

			Vec4 ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
			Vec4 diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
			Vec4 specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

			material->SetAmbient(ambient);
			material->SetDiffuse(diffuse);
			material->SetSpecular(specular);

			material->SetShader(shader);
		}

		shared_ptr<Material> shadowMaterial = make_shared<Material>();
		{
			shadowMaterial->Init();

			shared_ptr<Shader> shader = make_shared<Shader>();
			{
				ShaderInfo info;
				info.depthStencilType = DEPTH_STENCIL_TYPE::NoDoubleBlendDSS;
				shader->Init(L"..\\Shaders\\08. Basic.fx");
			}

			Vec4 ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);;
			Vec4 diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);;
			Vec4 specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);

			shadowMaterial->SetAmbient(ambient);
			shadowMaterial->SetDiffuse(diffuse);
			shadowMaterial->SetSpecular(specular);

			shadowMaterial->SetShader(shader);
		}

		shared_ptr<Material> reflectMaterial = make_shared<Material>();
		{
			reflectMaterial->Init();

			shared_ptr<Shader> shader = make_shared<Shader>();
			{
				ShaderInfo info;
				info.rasterizerType = RASTERIZER_TYPE::CullClockwiseRS;
				info.depthStencilType = DEPTH_STENCIL_TYPE::DrawReflectionDSS;
				shader->Init(L"..\\Shaders\\08. Basic.fx", info);
			}

			Vec4 ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);;
			Vec4 diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);;
			Vec4 specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

			reflectMaterial->SetAmbient(ambient);
			reflectMaterial->SetDiffuse(diffuse);
			reflectMaterial->SetSpecular(specular);

			reflectMaterial->SetShader(shader);
		}

		shared_ptr<Mesh> skullMesh = make_shared<Mesh>();
		{
			skullMesh->Init();
			skullMesh->CreateMesh(L"../Resources/Models/skull.txt");
			skullMesh->SetMateral(material);
			skullMesh->SetShadow(shadowMaterial);
			skullMesh->SetReflect(reflectMaterial);
		}

		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Load(L"..\\Resources\\Textures\\checkboard.dds");

		shared_ptr<GameObject> skull = make_shared<GameObject>();
		skull->GetOrAddTransform()->SetPosition(Vec3(0.0f, 1.f, -3.f));
		//skull->GetTransform()->SetRotation(Vec3(0,0,0) * 0.5f * MathHelper::Pi);
		skull->GetTransform()->SetRotation(Vec3(0,90,0));
		skull->GetTransform()->SetScale(Vec3(0.45f, 0.45f, 0.45f));
		skull->AddComponent(make_shared<MeshRenderer>());
		skull->GetMeshRenderer()->SetMesh(skullMesh);
		skull->GetMeshRenderer()->SetShader(nullptr);
		skull->GetMeshRenderer()->SetTexture(texture);


		skull->AddComponent(make_shared<SkullScript>());

		//_gameObjects.push_back(skull);
		CUR_SCENE->Add(skull);

	}


	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		{
			mesh->Init();
			mesh->CreateFloor();


			shared_ptr<Material> material = make_shared<Material>();
			material->SetAmbient(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
			material->SetDiffuse(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			material->SetSpecular(Vec4(0.6f, 0.6f, 0.6f, 16.0f));
			

			shared_ptr<Shader> shader = make_shared<Shader>();
			{
				shader->Init(L"..\\Shaders\\08. Basic.fx");
			}


			material->SetShader(shader);
			mesh->SetMateral(material);
		}

		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Load(L"..\\Resources\\Textures\\checkboard.dds");

		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->GetOrAddTransform();
		gameObject->GetTransform()->SetPosition(Vec3(0.5f, 0.f, 0.f));
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(mesh);
		gameObject->GetMeshRenderer()->SetShader(nullptr);
		gameObject->GetMeshRenderer()->SetTexture(texture);

		CUR_SCENE->Add(gameObject);
	}

	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		{
			mesh->Init();
			mesh->CreateWall();


			shared_ptr<Material> material = make_shared<Material>();
			material->SetAmbient(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
			material->SetDiffuse(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			material->SetSpecular(Vec4(0.6f, 0.6f, 0.6f, 16.0f));
			mesh->SetMateral(material);

			shared_ptr<Shader> shader = make_shared<Shader>();
			{
				shader->Init(L"..\\Shaders\\08. Basic.fx");
			}



			material->SetShader(shader);
		}

		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Load(L"..\\Resources\\Textures\\brick01.dds");

		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->GetOrAddTransform();
		gameObject->GetTransform()->SetPosition(Vec3(0.5f, 0.f, 0.f));
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(mesh);
		gameObject->GetMeshRenderer()->SetShader(nullptr);
		gameObject->GetMeshRenderer()->SetTexture(texture);

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

	

	
	GRAPHICS->GetCommandQueue()->WaitSync();
}

void MirrorDemo::Update()
{


	

}

void MirrorDemo::Render()
{
}