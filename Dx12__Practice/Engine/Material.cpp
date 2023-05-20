#include "pch.h"
#include "Material.h"

Material::Material() : Super(ResourceType::Material)
{
}

Material::~Material()
{
}

void Material::Init()
{
}

void Material::SetShader(const wstring& path, ShaderInfo info)
{
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(path, info);
	_shaders.push_back(shader);
}

void Material::SetShader(shared_ptr<Shader> shader)
{
	_shaders.push_back(shader);
}

void Material::UpdateShader(int pass)
{
	_shaders[pass]->Update();
}
