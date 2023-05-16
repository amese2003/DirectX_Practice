#pragma once

class GameObject;

class Scene
{
public:
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	virtual void Render();

	virtual void Add(shared_ptr<GameObject> object);
	virtual void Remove(shared_ptr<GameObject> object);
	

	unordered_set<shared_ptr<GameObject>>& GetObjects() { return _objects; }
	shared_ptr<GameObject> GetMainCamera();
	shared_ptr<GameObject> GetUICamera();
	shared_ptr<GameObject> GetLight() { return _lights.empty() ? nullptr : *_lights.begin(); }

	void PushLightData();

private:
	unordered_set<shared_ptr<GameObject>> _objects;
	unordered_set<shared_ptr<GameObject>> _cameras;
	unordered_set<shared_ptr<GameObject>> _lights;
};

