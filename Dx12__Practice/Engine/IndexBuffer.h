#pragma once
class IndexBuffer
{
public:
	IndexBuffer(ComPtr<ID3D12Device> device);
	~IndexBuffer();

	ComPtr<ID3D12Resource> GetComPtr() { return _indexBuffer; }

	template<typename T>
	void Create(const vector<T>& vertices)
	{
		_count = static_cast<uint32>(vertices.size());
		uint32 bufferSize = _count * sizeof(uint32);

		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

		DEVICE->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_indexBuffer));

		void* indexDataBuffer = nullptr;
		CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
		_indexBuffer->Map(0, &readRange, &indexDataBuffer);
		::memcpy(indexDataBuffer, &vertices[0], bufferSize);
		_indexBuffer->Unmap(0, nullptr);

		_indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
		_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		_indexBufferView.SizeInBytes = bufferSize;
	}

private:
	friend class MeshRenderer;
	ComPtr<ID3D12Resource> _indexBuffer;
	D3D12_INDEX_BUFFER_VIEW _indexBufferView = {};

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

