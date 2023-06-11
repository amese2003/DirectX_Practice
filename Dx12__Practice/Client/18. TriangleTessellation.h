#pragma once
#include "IExecute.h"

class GameObject; 

class TriangleTessellation : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;
};

