#pragma once
#include "RenderStates.h"

class VertexShader;
class PixelShader;
struct ShaderInfo;


class Shader : public ResourceBase
{
	using Super = ResourceBase;

public:
	Shader();
	virtual ~Shader();

	void Init(const wstring& path, ShaderInfo info = ShaderInfo(), ShaderArg args = ShaderArg());
	void Update();
	D3D12_PRIMITIVE_TOPOLOGY& GetTopology() { return _topology; }
	ShaderInfo& GetInfo() { return _info; }
	ComPtr<ID3D12PipelineState> GetPipelineState() { return _pipelineState; }

private:
	void CreateGraphicShader(const wstring& path);
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreateGeometryShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);


private:
	void CreateComputeShader(const wstring& path);

	

private:
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_gsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};
	D3D12_PRIMITIVE_TOPOLOGY			_topology;

private:
	ComPtr<ID3DBlob>					_csBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC   _computePipelineDesc = {};

private:
	ShaderInfo _info;
	ShaderArg _args;
};

