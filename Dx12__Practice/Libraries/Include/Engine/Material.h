#pragma once
#include "ResourceBase.h"

class Shader;

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


	void SetColor(Color color) { _color = color; }
	Color& GetColor() { return _color; }

public:
	void SetName(string name) { _name = name; }
	string GetName() { return _name; }

	void Setindex(int idx) { _cbindex = idx; }
	int GetIndex() { return _cbindex; }

	void SetSrvindex(int idx) { _srvheapIndex = idx; }
	int GetSrvindex() { return _srvheapIndex; }

	void SetDiffuseAlbedo(Color color) { _diffuseAlbedo = color; }
	Color GetdiffuseAlbedo() { return _diffuseAlbedo; }

	void Setfresnel(Vec3 vec) { _fresnelR0 = vec; }
	Vec3 Getfresnel() { return _fresnelR0; }

	void SetRoughness(float roughness) { _roughness = roughness; }
	float GetRoughness() { return _roughness; }

private:
	Color _color = { 0, 0, 0 ,1 };

	Vec4 _ambient = { 1,1,1,1 };
	Vec4 _diffuse = { 1,1,1,1 };
	Vec4 _specular = { 1, 1, 1, 1 };

private:
	string _name = "";
	int _cbindex = 0;
	int _srvheapIndex = 0;
	Color _diffuseAlbedo = Color(1.f, 1.f, 1.f, 1.f);
	Vec3 _fresnelR0 = Vec3(0.2f, 0.2f, 0.2f);
	float _roughness = 0.1f;
};

