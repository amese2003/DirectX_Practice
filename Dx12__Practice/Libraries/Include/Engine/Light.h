#pragma once
#include "Component.h"


enum class LightType
{
	Dirlight = 0,
	PointLight,
	SpotLight
};

class Light : public Component
{
	using Super = Component;

public:
	Light();
	virtual ~Light();

	void Init(LightType lighttype); 
	virtual void Update() override;

public:
	LightDesc& GetLightDesc() { return _desc; }
	LightType GetLightType() { return _type; }

	void SetLightDesc(LightDesc& desc) { _desc = desc; }
	void SetAmbient(const Color& color) { _desc.ambient = color; }
	void SetDiffuse(const Color& color) { _desc.diffuse = color; }
	void SetSpecular(const Color& color) { _desc.specular = color; }
	void SetEmissive(const Color& color) { _desc.emissive = color; }
	void SetLightDirection(Vec3 direction) { _desc.direction = direction; }


private:
	LightType _type;
	LightDesc _desc;
	
};

