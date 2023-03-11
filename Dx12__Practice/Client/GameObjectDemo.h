#pragma once
#include "IExecute.h"

class GameObject;

class GameObjectDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	shared_ptr<GameObject> _gameObject;
};

