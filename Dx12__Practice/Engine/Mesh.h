#pragma once
struct Material;


class Mesh
{

public:
	Mesh();
	virtual ~Mesh();

	void Init();
	void CreateRectangle();
	void CreateQuad();
	void CreateTriangle();
	void CreateCube();
	void CreateHillDemo();
	void CreateGrid();
	void CreateSphere();
	void CreateCylinder();
	void CreateSkull();
	void CreateFloor();
	void CreateWall();
	void CreateMirror();
	void CreateTreeSprites();


	void SetMateral(shared_ptr<Material> material) { _material = material; }
	shared_ptr<Material> GetMaterial() { return _material; }

	void SetShadow(shared_ptr<Material> material) { _shadowMaterial = material; }
	shared_ptr<Material> GetShadowMaterial() { return _shadowMaterial; }

	void SetReflect(shared_ptr<Material> material) { _reflectMaterial = material; }
	shared_ptr<Material> GetReflectMaterial() { return _reflectMaterial; }






private:
	ComPtr<ID3D12Device> _device;

	//shared_ptr<Geometry<VertexTextureNormalTangentData>> _geometry;\

private:
	shared_ptr<Material> _material;
	shared_ptr<Material> _shadowMaterial;
	shared_ptr<Material> _reflectMaterial;


};

