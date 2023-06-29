#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include <filesystem>
#include "TextureMultiple.h"
#include "FrameResource.h"

void ResourceManager::Init()
{
	//CreateDefaultMesh();
	CreateDefaultTexture();
	LoadMeshs();
	
	BuildFrameResources();
	CreateDefaultMaterials();
}

std::shared_ptr<Texture> ResourceManager::GetOrAddTexture(const wstring& key, const wstring& path)
{
	shared_ptr<Texture> texture = Get<Texture>(key);

	if (filesystem::exists(filesystem::path(path)) == false)
		return nullptr;

	texture = Load<Texture>(key, path);

	if (texture == nullptr)
	{
		texture = make_shared<Texture>();
		texture->Load(path);
		Add(key, texture);
	}

	return texture;
}

void ResourceManager::CreateDefaultMesh()
{
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateRectangle();
		Add(L"Quad", mesh);
	}
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateCube();
		Add(L"Cube", mesh);
	}
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateSphere();
		Add(L"Sphere", mesh);
	}
}

void ResourceManager::CreateDefaultTexture()
{
	GRAPHICS->GetCmdQueue()->GetResourceCmdList()->Reset(GRAPHICS->GetCmdQueue()->GetResourceAlloc().Get(), nullptr);

	shared_ptr<Texture> brickTex = make_shared<Texture>();
	brickTex->Load(L"..\\Resources\\Textures\\bricks.dds");
	Add(L"brickTex", brickTex);

	GRAPHICS->GetCmdQueue()->GetResourceCmdList()->Reset(GRAPHICS->GetCmdQueue()->GetResourceAlloc().Get(), nullptr);

	shared_ptr<Texture> stoneTex = make_shared<Texture>();
	stoneTex->Load(L"..\\Resources\\Textures\\stone.dds");
	Add(L"stoneTex", stoneTex);

	GRAPHICS->GetCmdQueue()->GetResourceCmdList()->Reset(GRAPHICS->GetCmdQueue()->GetResourceAlloc().Get(), nullptr);

	shared_ptr<Texture> tileTex = make_shared<Texture>();
	tileTex->Load(L"..\\Resources\\Textures\\tile.dds");
	Add(L"tileTex", tileTex);

	GRAPHICS->GetCmdQueue()->GetResourceCmdList()->Reset(GRAPHICS->GetCmdQueue()->GetResourceAlloc().Get(), nullptr);

	shared_ptr<Texture> woodcreateTex = make_shared<Texture>();
	woodcreateTex->Load(L"..\\Resources\\Textures\\WoodCrate01.dds");
	Add(L"woodcreateTex", woodcreateTex);

	GRAPHICS->GetCmdQueue()->GetResourceCmdList()->Reset(GRAPHICS->GetCmdQueue()->GetResourceAlloc().Get(), nullptr);

	shared_ptr<Texture> iceTex = make_shared<Texture>();
	iceTex->Load(L"..\\Resources\\Textures\\ice.dds");
	Add(L"waterTex", iceTex);

	GRAPHICS->GetCmdQueue()->GetResourceCmdList()->Reset(GRAPHICS->GetCmdQueue()->GetResourceAlloc().Get(), nullptr);

	shared_ptr<Texture> grassTex = make_shared<Texture>();
	grassTex->Load(L"..\\Resources\\Textures\\grass.dds");
	Add(L"grassTex", grassTex);

	GRAPHICS->GetCmdQueue()->GetResourceCmdList()->Reset(GRAPHICS->GetCmdQueue()->GetResourceAlloc().Get(), nullptr);

	shared_ptr<Texture> whiteTex = make_shared<Texture>();
	whiteTex->Load(L"..\\Resources\\Textures\\white1x1.dds");
	Add(L"whiteTex", whiteTex);


	shared_ptr<TextureMultiple> multiTex = make_shared<TextureMultiple>();
	multiTex->Init();
	Add(L"multiTex", multiTex);
}

void ResourceManager::LoadMeshs()
{
	shared_ptr<Mesh> skullMesh = make_shared<Mesh>();
	skullMesh->Init();
	skullMesh->CreateSkull();
	Add(L"skullMesh", skullMesh);
}

void ResourceManager::CreateDefaultMaterials()
{
	auto bricks0 = make_shared<Material>();
	bricks0->SetName("bricks0");
	bricks0->Setindex(0);
	bricks0->SetSrvindex(0);
	bricks0->SetDiffuseAlbedo(Color(1.f, 1.f, 1.f, 1.f));
	bricks0->Setfresnel(Vec3(0.02f, 0.02f, 0.02f));
	bricks0->SetRoughness(0.1f);
	Add<Material>(L"bricks0", bricks0);

	auto stone0 = make_shared<Material>();
	stone0->SetName("stone0");
	stone0->Setindex(1);
	stone0->SetSrvindex(1);
	stone0->SetDiffuseAlbedo(Color(1.f, 1.f, 1.f, 1.f));
	stone0->Setfresnel(Vec3(0.05f, 0.05f, 0.05f));
	stone0->SetRoughness(0.1f);
	Add<Material>(L"stone0", stone0);

	auto tile0 = make_shared<Material>();
	tile0->SetName("tile0");
	tile0->Setindex(2);
	tile0->SetSrvindex(2);
	tile0->SetDiffuseAlbedo(Color(1.f, 1.f, 1.f, 1.f));
	tile0->Setfresnel(Vec3(0.02f, 0.02f, 0.02f));
	tile0->SetRoughness(0.3f);
	Add<Material>(L"tile0", tile0);

	auto crate0 = make_shared<Material>();
	crate0->SetName("crate0");
	crate0->Setindex(3);
	crate0->SetSrvindex(3);
	crate0->SetDiffuseAlbedo(Color(1.f, 1.f, 1.f, 1.f));
	crate0->Setfresnel(Vec3(0.05f, 0.05f, 0.05f));
	crate0->SetRoughness(0.2f);
	Add<Material>(L"crate0", crate0);

	auto ice0 = make_shared<Material>();
	ice0->SetName("ice0");
	ice0->Setindex(4);
	ice0->SetSrvindex(4);
	ice0->SetDiffuseAlbedo(Color(1.f, 1.f, 1.f, 1.f));
	ice0->Setfresnel(Vec3(0.1f, 0.1f, 0.1f));
	ice0->SetRoughness(0.0f);
	Add<Material>(L"ice0", ice0);

	auto grass0 = make_shared<Material>();
	grass0->SetName("grass0");
	grass0->Setindex(5);
	grass0->SetSrvindex(5);
	grass0->SetDiffuseAlbedo(Color(1.f, 1.f, 1.f, 1.f));
	grass0->Setfresnel(Vec3(0.05f, 0.05f, 0.05f));
	grass0->SetRoughness(0.2f);
	Add<Material>(L"grass0", grass0);

	auto skullMat = make_shared<Material>();
	skullMat->SetName("skullMat");
	skullMat->Setindex(6);
	skullMat->SetSrvindex(6);
	skullMat->SetDiffuseAlbedo(Color(1.f, 1.f, 1.f, 1.f));
	skullMat->Setfresnel(Vec3(0.05f, 0.05f, 0.05f));
	skullMat->SetRoughness(0.5f);
	Add<Material>(L"skullMat", skullMat);
}

void ResourceManager::BuildFrameResources()
{
	for (int i = 0; i < _numframeResouces; ++i)
	{
		auto resource = make_shared<FrameResource>();
		resource->Init(DEVICE.Get(), 1);
		resource->InitMaterialData(ResourceSize<Material>());
		_frameResources.push_back(resource);
	}
}
