#pragma once
#include "IExecute.h"

class GameObject; 

class TreeBillboardDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;
};

