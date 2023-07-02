#pragma once

class MeshGeometry;
struct Material;


enum class RenderLayer : int
{
	Opaque = 0,
	Highlight,
	Sky,
	OpaqueDynamicReflectors,
	Count
};


class RenderObject
{
public:
	RenderObject();

	//void Get(Matrix mat) { _worldMatrix = mat; }
	void SetWorldMatrix(Matrix mat) { _worldMatrix = mat; }
	void SetTexMatrix(Matrix mat) { _texMatrix = mat; }
	void SetDirty(int dirty) { _numframesDirty = dirty; }
	void SetConstantIndex(int idx) { _objCbIndex = idx; }
	void SetMaterial(shared_ptr<Material> mat) { _mat = mat; }
	void SetGeometry(shared_ptr<MeshGeometry> geo) { _geo = geo; }
	void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY type) { _primitiveType = type; }
	

	void SetIndexCount(UINT count) { _indexCount = count; }
	void SetStartIndexLocation(UINT num) { _startIndexLocation = num; }
	void SetBaseIndexLocation(UINT num) { _baseVertexLocation = num; }
	void SetBoundingBox(BoundingBox box) { Bounds = box; }

	void AddInstanceData(InstanceData data) { Instances.push_back(data); }
	void SetInstanceCount(int idx) { _instanceCount = idx; }
	void SetInstanceSize(int idx) { Instances.resize(idx); }
	void SetInstanceData(int idx, InstanceData data) { Instances[idx] = data; }
	void SetVisible(bool visible) { _visible = visible; }

	Matrix GetTexMatrix() { return _texMatrix; }
	Matrix GetWorldMatrix() { return _worldMatrix; }

	int& GetnumframedIrty() { return _numframesDirty; }
	UINT& GetConstantIndex() { return _objCbIndex; }

	shared_ptr<Material> GetMaterial() { return _mat; }
	shared_ptr<MeshGeometry> GetGeometry() { return _geo; }

	D3D12_PRIMITIVE_TOPOLOGY& GetPrimitiveType() { return _primitiveType; }

	BoundingBox& GetBoundingBox() { return Bounds; }

	InstanceData GetInstanceData(int num) { return Instances[num]; }
	vector<InstanceData> GetInstanceData() { return Instances; }
	
	bool GetVisible() { return _visible; }
	

	UINT GetIndexCount() { return _indexCount; }
	UINT GetInstanceCount() { return _instanceCount; }
	UINT GetStartIndexLocation() { return _startIndexLocation; }
	int GetBaseVertexLocation() { return _baseVertexLocation; }
	
	


private:
	Matrix _worldMatrix = MathHelper::Identity4x4();
	Matrix _texMatrix = MathHelper::Identity4x4();

	int _numframesDirty = 3;
	UINT _objCbIndex = -1;

	shared_ptr<Material> _mat = nullptr;
	shared_ptr<MeshGeometry> _geo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY _primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	BoundingBox Bounds;
	std::vector<InstanceData> Instances;

	bool _visible = true;

	// DrawIndexedInstanced parameters.
	UINT _indexCount = 0;
	UINT _instanceCount = 0;
	UINT _startIndexLocation = 0;
	int _baseVertexLocation = 0;
};

