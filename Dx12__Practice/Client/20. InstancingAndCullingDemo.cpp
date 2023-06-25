#include "pch.h"
#include "20. InstancingAndCullingDemo.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Waves.h"
#include "Material.h"
#include "Light.h"
#include "MathHelper.h"
#include "TextureMultiple.h"

void InstancingAndCullingDemo::Init()
{
	
	//_camera = make_shared<GameObject>();
	//_camera->GetOrAddTransform();
	//_camera->AddComponent(make_shared<Camera>());
	//_camera->AddComponent(make_shared<CameraScript>());
	//CUR_SCENE->Add(_camera);


	//vector<shared_ptr<Texture>> texarr;


	//shared_ptr<Texture> grassTex = make_shared<Texture>();
	//grassTex->Load(L"..\\Resources\\Textures\\grass.dds");
	//RESOURCES->Add(L"grassTex", grassTex);
	//texarr.push_back(grassTex);

	//shared_ptr<Texture> waterTex = make_shared<Texture>();
	//waterTex->Load(L"..\\Resources\\Textures\\water2.dds");
	//RESOURCES->Add(L"waterTex", waterTex);
	//texarr.push_back(waterTex);

	//shared_ptr<Texture> wireTex = make_shared<Texture>();
	//wireTex->Load(L"..\\Resources\\Textures\\WireFence.dds");
	//RESOURCES->Add(L"wireTex", wireTex);
	//texarr.push_back(wireTex);

	//shared_ptr< TextureMultiple> texMult = make_shared<TextureMultiple>();
	//texMult->Init(texarr.size(), texarr);


	//shared_ptr<Shader> shader = make_shared<Shader>();
	//shader->Init(L"..\\Shaders\\19. InstancedBasic.fx");
	//RESOURCES->Add(L"texShader", shader);

	//shared_ptr<Mesh> skullMesh = make_shared<Mesh>();
	//{
	//	skullMesh->Init();
	//	skullMesh->CreateMesh(L"../Resources/Models/skull.txt");
	//}

	//shared_ptr<Material> material = make_shared<Material>();
	//{
	//	Vec4 ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	//	Vec4 diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	//	Vec4 specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	//	material->SetAmbient(ambient);
	//	material->SetDiffuse(diffuse);
	//	material->SetSpecular(specular);
	//	material->SetColor(Vec4(1, 1, 1, 1));
	//}


	//const int n = 5;
	//float width = 200.0f;
	//float height = 200.0f;
	//float depth = 200.0f;

	//float x = -0.5f * width;
	//float y = -0.5f * height;
	//float z = -0.5f * depth;
	//float dx = width / (n - 1);
	//float dy = height / (n - 1);
	//float dz = depth / (n - 1);
	//for (int k = 0; k < n; ++k)
	//{
	//	for (int i = 0; i < n; ++i)
	//	{
	//		for (int j = 0; j < n; ++j)
	//		{

	//			shared_ptr<GameObject> gameObject = make_shared<GameObject>();
	//			gameObject->GetOrAddTransform()->SetPosition(Vec3(x + j * dx, y + i * dy, z + k * dz));
	//			gameObject->GetOrAddTransform()->SetScale(Vec3(2.0f, 2.0f, 1.0f));
	//			

	//			shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
	//			renderer->SetMateral(material);
	//			renderer->SetShader(shader);
	//			renderer->SetMesh(skullMesh);
	//			renderer->SetTexture(texMult);

	//			gameObject->AddComponent(renderer);
	//			
	//			CUR_SCENE->Add(gameObject);
	//			//skullRitem->Instances[index].MaterialIndex = index % mMaterials.size();
	//		}
	//	}
	//}
	//

	//
	//GRAPHICS->GetCommandQueue()->WaitSync();
}

void InstancingAndCullingDemo::Update()
{
	/*_camera->Update();



	for (auto gameObject : _gameObjects)
		gameObject->Update();*/

	

}

void InstancingAndCullingDemo::Render()
{
}