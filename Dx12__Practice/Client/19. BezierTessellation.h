#pragma once
#include "IExecute.h"

class GameObject; 

class BezierTessellation : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;
};

