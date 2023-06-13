#include "pch.h"
#include "09. SkullDemo.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Waves.h"
#include "Material.h"
#include "Light.h"
#include "MathHelper.h"

void SkullDemo::Init()
{
	
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());
	CUR_SCENE->Add(_camera);


	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Shaders\\05. Color.fx");

		

		shared_ptr<Material> material = make_shared<Material>();
		{
			material->Init();

			Vec4 ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
			Vec4 diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
			Vec4 specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

			material->SetAmbient(ambient);
			material->SetDiffuse(diffuse);
			material->SetSpecular(specular);

			material->SetShader(shader);
		}	

		shared_ptr<Mesh> skullMesh = make_shared<Mesh>();
		{
			skullMesh->Init();
			skullMesh->CreateMesh(L"../Resources/Models/skull.txt");
		}

		shared_ptr<GameObject> skull = make_shared<GameObject>();
		skull->GetOrAddTransform();
		skull->AddComponent(make_shared<MeshRenderer>());
		skull->GetMeshRenderer()->SetMesh(skullMesh);
		skull->GetMeshRenderer()->SetShader(shader);

		skull->GetMeshRenderer()->SetMateral(material);

		//_gameObjects.push_back(skull);
		CUR_SCENE->Add(skull);

	}

	

	

	
	GRAPHICS->GetCommandQueue()->WaitSync();
}

void SkullDemo::Update()
{
	/*_camera->Update();



	for (auto gameObject : _gameObjects)
		gameObject->Update();*/

	

}

void SkullDemo::Render()
{
}