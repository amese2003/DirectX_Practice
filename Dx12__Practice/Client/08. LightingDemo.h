#pragma once
#include "IExecute.h"

class GameObject; 

class LightingDemo : public IExecute
{


public:
	void Init() override;
	void Update() override;
	void Render() override;

	float GetHillHeight(float x, float z) { return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z)); }
	XMFLOAT3 GetHillNormal(float x, float z) const;

	vector<shared_ptr<GameObject>> _gameObjects;
	shared_ptr<GameObject> _camera;

	shared_ptr<GameObject> _dirLight;
	shared_ptr<GameObject> _pointLight;
	shared_ptr<GameObject> _spotLight;
};

