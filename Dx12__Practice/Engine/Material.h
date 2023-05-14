#pragma once
#include "ResourceBase.h"

class Material : public ResourceBase
{
	using Super = ResourceBase;

public:
	Material();
	virtual ~Material();

	void Init();

	void SetAmbient(Vec4 vec) { _ambient = vec; }
	void SetDiffuse(Vec4 vec) { _diffuse = vec; }
	void SetSpecular(Vec4 vec) { _specular = vec; }

	Vec4 GetAmbient() { return _ambient; }
	Vec4 GetDiffuse() { return _diffuse; }
	Vec4 GetSpecular() { return _specular; }

private:
	Vec4 _ambient = { 1,1,1,1 };
	Vec4 _diffuse = { 1,1,1,1 };
	Vec4 _specular = { 1, 1, 1, 1 };
};

