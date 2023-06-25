#pragma once
#include "VertexData.h"

class VertexBuffer;
class ConstTransform;
class Material;


class Mesh : public ResourceBase
{
	using Super = ResourceBase;

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

	shared_ptr<VertexBuffer> GetVertexBuffer() { return _vertexBuffer; }
	shared_ptr<IndexBuffer> GetIndexBuffer() { return _indexBuffer; }

	void SetVertexBuffer(shared_ptr<VertexBuffer> vertexBuffer) { _vertexBuffer = vertexBuffer; }
	void SetIndexBuffer(shared_ptr<IndexBuffer> indexBuffer) { _indexBuffer = indexBuffer; }

	void SetMateral(shared_ptr<Material> material) { _material = material; }
	shared_ptr<Material> GetMaterial() { return _material; }

	void SetShadow(shared_ptr<Material> material) { _shadowMaterial = material; }
	shared_ptr<Material> GetShadowMaterial() { return _shadowMaterial; }

	void SetReflect(shared_ptr<Material> material) { _reflectMaterial = material; }
	shared_ptr<Material> GetReflectMaterial() { return _reflectMaterial; }






private:
	ComPtr<ID3D12Device> _device;

	shared_ptr<Geometry<VertexTextureNormalTangentData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

private:
	shared_ptr<Material> _material;
	shared_ptr<Material> _shadowMaterial;
	shared_ptr<Material> _reflectMaterial;


};

