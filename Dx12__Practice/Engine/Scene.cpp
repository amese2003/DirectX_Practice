#include "pch.h"
#include "Scene.h"

void Scene::Start()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->Start();
	}
}

void Scene::Update()
{
	//PushLightData();

	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->Update();
	}

}

void Scene::LateUpdate()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->LateUpdate();
	}

}

void Scene::Render()
{
	for (auto& camera : _cameras)
	{
	}
}

void Scene::Add(shared_ptr<GameObject> object)
{
	_objects.insert(object);
	_gameObjects.push_back(object);

	if (object->GetCamera() != nullptr)
	{
		_cameras.insert(object);
	}

	if (object->GetLight() != nullptr)
	{
		_lights.insert(object);
	}
}

void Scene::Remove(shared_ptr<GameObject> object)
{
}

shared_ptr<GameObject> Scene::GetMainCamera()
{
	for (auto& camera : _cameras)
	{
		return camera;
	}

	return nullptr;
}

shared_ptr<GameObject> Scene::GetUICamera()
{
	return shared_ptr<GameObject>();
}

void Scene::PushLightData()
{
	CHECK(_lights.size() > 50);

	shared_ptr<Camera> camera = GetMainCamera()->GetCamera();

	GlobalParams cbParams;
	
	XMMATRIX view = Camera::S_MatView;
	XMMATRIX proj = Camera::S_MatProjection;
	XMMATRIX viewProj = view * proj;

	cbParams.matView = view;
	auto viewPos = XMMatrixDeterminant(view);
	cbParams.invView = XMMatrixInverse(&viewPos, view);

	cbParams.matProj = proj;
	auto projPos = XMMatrixDeterminant(proj);
	cbParams.invProj = XMMatrixInverse(&projPos, proj);

	cbParams.viewProj = viewProj;
	auto viewProjPos = XMMatrixDeterminant(viewProj);
	cbParams.invViewProj = XMMatrixInverse(&viewProjPos, viewProj);

	cbParams.eyePosition = camera->GetTransform()->GetPosition();
	cbParams.lightCount = 0;

	for (shared_ptr<GameObject> lightObject : _lights)
	{
		shared_ptr<Light> light = lightObject->GetLight();

		LightDesc lightDesc;
		
		lightDesc = light->GetLightDesc();
		LightType type = light->GetLightType();
		//cbParams.lights[static_cast<int>(type)] = lightDesc;
		cbParams.lights[cbParams.lightCount] = lightDesc;
		cbParams.lightCount++;
	}

	cbParams.renderTargetSize = {GRAPHICS->GetViewport().Width, GRAPHICS->GetViewport().Height};
	cbParams.invRenderTargetSize = {1.f/ GRAPHICS->GetViewport().Width, 1.f / GRAPHICS->GetViewport().Height};

	cbParams.nearZ = 1.f;
	cbParams.farZ = 1000.f;

	cbParams.totalTime = TIME->TotalTime();
	cbParams.deltaTime = TIME->DeltaTime();

	cbParams.ambientLight = { 0.25f, 0.25f, 0.35f, 1.0f };

	//cbParams.gFogColor = Color(0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f);
	//cbParams.gFogColor = Color(0.f,0.f,0.f, 1.000000000f);
	//cbParams.gFogStart = 2.0f;
	//cbParams.gFogRange = 40.0f;

	//GRAPHICS->GetConstantBuffer(CBV_REGISTER::b0)->SetGlobalData(&cbParams, sizeof(cbParams));
}
