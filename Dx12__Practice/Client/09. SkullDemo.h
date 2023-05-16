#pragma once
#include "IExecute.h"

class GameObject; 

class SkullDemo : public IExecute
{


public:
	void Init() override;
	void Update() override;
	void Render() override;

	vector<shared_ptr<GameObject>> _gameObjects;
	shared_ptr<GameObject> _camera;
};

