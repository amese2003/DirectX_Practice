#pragma once
#include "Component.h"
#include "Material.h"

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

	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	shared_ptr<Material> GetMaterial() { return _material; }

	void SetDir(Vec3 dir) { _dir = dir; }
	void SetRange(float range) { _range = range; }
	void SetSpot(float spot) { _spot = spot; }
	void SetAtt(Vec3 pos) { _att = pos; }

	Vec3 GetDir() { return _dir; }
	float GetRange() { return _range; }
	float GetSpot() { return _spot; }
	Vec3 GetAtt() { return _att; }

	virtual void Update() override;

private:
	LightType _type;
	shared_ptr<Material> _material;

	Vec3 _dir = { 0, 0, 0 };
	float _range = 25.f;
	float _spot = 90.f;

	Vec3 _att = { 0, 0, 0 };
	
};

