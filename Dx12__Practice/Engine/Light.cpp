#include "pch.h"
#include "Light.h"

Light::Light() : Super(ComponentType::Light)
{
}

Light::~Light()
{
}

void Light::Init(LightType lighttype)
{
	_type = lighttype;
}

void Light::Update()
{
	_desc.position = GetTransform()->GetPosition();
}
