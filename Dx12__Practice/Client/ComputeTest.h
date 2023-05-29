#pragma once
#include "MonoBehaviour.h"

struct Data
{
	XMFLOAT3 v1;
	XMFLOAT2 v2;
};

class ComputeTest : public MonoBehaviour
{
public:
	virtual void Start() override;
	virtual void Update() override;

	void BuildBuffers();
	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
	void Dispatch();

	ComPtr<ID3D12Resource> CreateDefaultBuffer(const void* initData, UINT64 byteSize, ComPtr<ID3D12Resource>& uploadBuffer);

private:
	shared_ptr<Shader> _shader;

	const int NumDataElements = 32;

	ComPtr<ID3D12Resource> mInputBufferA = nullptr;
	ComPtr<ID3D12Resource> mInputUploadBufferA = nullptr;
	ComPtr<ID3D12Resource> mInputBufferB = nullptr;
	ComPtr<ID3D12Resource> mInputUploadBufferB = nullptr;
	ComPtr<ID3D12Resource> mOutputBuffer = nullptr;
	ComPtr<ID3D12Resource> mReadBackBuffer = nullptr;
};

