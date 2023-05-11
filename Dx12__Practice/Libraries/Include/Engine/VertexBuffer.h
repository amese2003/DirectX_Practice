#pragma once
class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D12Device> device);
	~VertexBuffer();

	ComPtr<ID3D12Resource> GetComPtr() { return _vertexBuffer; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() { return _vertexBufferView; }

	template<typename T>
	void CreateTexture(const vector<T>& vertices)
	{
		_count = static_cast<uint32>(vertices.size());
		uint32 bufferSize = _count * sizeof(T);

		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC desc;
		ZeroMemory(&desc, sizeof(D3D12_RESOURCE_DESC));

		desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
		DEVICE->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_vertexBuffer));

		PushData(vertices);

		// Initialize the vertex buffer view.
		_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
		_vertexBufferView.StrideInBytes = sizeof(T); // 정점 1개 크기
		_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
	}

	template<typename T>
	void PushData(const vector<T>& vertices)
	{
		_count = static_cast<uint32>(vertices.size());
		uint32 bufferSize = _count * sizeof(T);

		void* vertexDataBuffer = nullptr;
		CD3DX12_RANGE readRange(0, 0);

		_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
		::memcpy(vertexDataBuffer, &vertices[0], bufferSize);
		_vertexBuffer->Unmap(0, nullptr);
	}

private:
	friend class MeshRenderer;
	ComPtr<ID3D12Resource>	_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

