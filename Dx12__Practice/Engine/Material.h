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

	void SetShader(const wstring& path, ShaderInfo info = ShaderInfo());
	void SetShader(shared_ptr<Shader> shader);
	void UpdateShader(int pass = 0);
	shared_ptr<Shader> GetShader(int pass = 0) { return _shaders[pass]; }

private:
	Vec4 _ambient = { 1,1,1,1 };
	Vec4 _diffuse = { 1,1,1,1 };
	Vec4 _specular = { 1, 1, 1, 1 };


	vector<shared_ptr<Shader>> _shaders;
};

