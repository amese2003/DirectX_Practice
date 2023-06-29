#pragma once

template<typename T>
class UploadBuffer
{
public:
	UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : _isConstantBuffer(isConstantBuffer)
	{
		_elementByteSize = sizeof(T);

		if (_isConstantBuffer)
			_elementByteSize = (_elementByteSize + 255) & ~255;


		auto heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto totalSize = _elementByteSize * elementCount;
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(totalSize);
		

		CHECK(
			device->CreateCommittedResource(
				&heapProperty,
				D3D12_HEAP_FLAG_NONE,
				&buffer,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&_uploadBuffer))
		);

		CHECK(_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));
	}

	~UploadBuffer()
	{
		if (_uploadBuffer != nullptr)
			_uploadBuffer->Unmap(0, nullptr);

		_mappedData = nullptr;
	}

	ID3D12Resource* Resource()const
	{
		return _uploadBuffer.Get();
	}

	void CopyData(int elementIndex, const T& data)
	{
		memcpy(&_mappedData[elementIndex * _elementByteSize], &data, sizeof(T));
	}

private:
	ComPtr<ID3D12Resource> _uploadBuffer;
	BYTE* _mappedData = nullptr;

	UINT _elementByteSize = 0;
	bool _isConstantBuffer = false;
};

