#pragma once

#include "ResourceBase.h"

class Shader;
class Texture;
class Mesh;
class Material;
class TextureMultiple;
class FrameResource;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);
public:
	void Init();

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& key);

	template<typename T>
	int ResourceSize();

	shared_ptr<Texture> GetOrAddTexture(const wstring& key, const wstring& path);

	template<typename T>
	ResourceType GetResourceType();

	int GetnumframeResource() { return _numframeResouces; };
	shared_ptr<FrameResource> GetFrameResources(int num) { return _frameResources[num]; }

private:
	void CreateDefaultMesh();
	void CreateDefaultTexture();
	void LoadMeshs();
	void CreateDefaultMaterials();
	void BuildFrameResources();

private:
	wstring _resourcePath;

private:
	using KeyObjMap = map<wstring/*key*/, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;

private:
	int _numframeResouces = 3;
	vector<shared_ptr<FrameResource>> _frameResources;

};

template<typename T>
shared_ptr<T>
ResourceManager::Load(const wstring& key, const wstring& path)
{
	auto objectType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);


	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

template<typename T>
bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;
	return true;
}

template<typename T>
shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

template<typename T>
ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;
	if (std::is_same_v<T, Material>)
		return ResourceType::Material;
	if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;
	if (std::is_same_v<T, TextureMultiple>)
		return ResourceType::TextureMultiple;

	assert(false);
	return ResourceType::None;
}

template<typename T>
inline int ResourceManager::ResourceSize()
{
	ResourceType resourceType = GetResourceType<T>();
	return  _resources[static_cast<uint8>(resourceType)].size();
}
