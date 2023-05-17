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
	PushLightData();

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

	LightParams cbParams;
	cbParams.lightCount = 0;
	cbParams.eyePosition = GetMainCamera()->GetCamera()->GetTransform()->GetPosition();
	
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

	GRAPHICS->GetConstantBuffer(CBV_REGISTER::b0)->SetGlobalData(&cbParams, sizeof(cbParams));
}
