#pragma once
#include "VertexData.h"

class VertexBuffer;
class ConstTransform;


class Mesh : public ResourceBase
{
	using Super = ResourceBase;

public:
	Mesh();
	virtual ~Mesh();

	void Init();
	void CreateRectangle();
	void CreateCube();
	void CreateHillDemo();
	void CreateGrid();
	void CreateSphere();
	void CreateCylinder();

	shared_ptr<VertexBuffer> GetVertexBuffer() { return _vertexBuffer; }
	shared_ptr<IndexBuffer> GetIndexBuffer() { return _indexBuffer; }

	void SetVertexBuffer(shared_ptr<VertexBuffer> vertexBuffer) { _vertexBuffer = vertexBuffer; }
	void SetIndexBuffer(shared_ptr<IndexBuffer> indexBuffer) { _indexBuffer = indexBuffer; }


private:
	ComPtr<ID3D12Device> _device;

	shared_ptr<Geometry<VertexTextureNormalTangentData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
};

