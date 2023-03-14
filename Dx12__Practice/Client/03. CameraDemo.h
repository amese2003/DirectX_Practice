#pragma once
#include "IExecute.h"

class GameObject; 

class CameraDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	shared_ptr<GameObject> _gameObject;
	shared_ptr<GameObject> _camera;

	Matrix _world = Matrix::Identity;
};

