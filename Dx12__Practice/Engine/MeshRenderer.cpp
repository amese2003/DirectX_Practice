#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "MathHelper.h"
#include "Shader.h"

MeshRenderer::MeshRenderer() : Super(ComponentType::MeshRenderer)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{
	Render();

	if (_mesh->GetShadowMaterial() != nullptr)
		RenderShadow();

	if (_mesh->GetReflectMaterial() != nullptr)
		RenderReflect();
	
}

void MeshRenderer::Render()
{
	shared_ptr<Material> material = _mesh->GetMaterial();

	if (material)
	{
		auto temp = material->GetShader()->GetTopology();
		CMD_LIST->IASetPrimitiveTopology(temp);
	}
	else
		CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CMD_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexBuffer()->GetVertexBufferView()); // Slot: (0~15)

	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->IASetIndexBuffer(&_mesh->GetIndexBuffer()->_indexBufferView);


	TransformData ctransformbuffer;
	ctransformbuffer.position = GetTransform()->GetPosition();
	ctransformbuffer.pad = 1.f;
	ctransformbuffer.world = GetTransform()->GetWorldMatrix();
	ctransformbuffer.matView = Camera::S_MatView;
	ctransformbuffer.matProjection = Camera::S_MatProjection;
	ctransformbuffer.worldnvTranspose = MathHelper::InverseTranspose(ctransformbuffer.world);
	ctransformbuffer.worldViewProj = ctransformbuffer.world * ctransformbuffer.matView * ctransformbuffer.matProjection;
	ctransformbuffer.ViewProj = ctransformbuffer.matView * ctransformbuffer.matProjection;

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b1)->PushData(&ctransformbuffer, sizeof(ctransformbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b1);


	MaterialDesc pushDesc;
	

	material->UpdateShader();

	if (material)
	{
		pushDesc.ambient = material->GetAmbient();
		pushDesc.diffuse = material->GetDiffuse();
		pushDesc.specular = material->GetSpecular();
		pushDesc.emissive = Color(1.f, 1.f, 1.f, 1.f);
	}
	
	
	MaterialData cbuffer;
	cbuffer.mat = pushDesc;
	cbuffer.texTransform = Matrix::Identity;

	if (_texture)
	{
		cbuffer.texTransform *= 5;
		GRAPHICS->GetTableDescHeap()->SetShaderResourceView(_texture->GetCpuHandle(), SRV_REGISTER::t0);
	}


	D3D12_CPU_DESCRIPTOR_HANDLE transformhandle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b2)->PushData(&cbuffer, sizeof(cbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(transformhandle, CBV_REGISTER::b2);
	

	GRAPHICS->GetTableDescHeap()->CommitTable();
	
	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->DrawIndexedInstanced(_mesh->GetIndexBuffer()->_count, 1, 0, 0, 0);
	else
		CMD_LIST->DrawInstanced(_mesh->GetVertexBuffer()->_count, 1, 0, 0);
	
}

void MeshRenderer::RenderShadow()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexBuffer()->GetVertexBufferView()); // Slot: (0~15)


	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->IASetIndexBuffer(&_mesh->GetIndexBuffer()->_indexBufferView);

	shared_ptr<Light> dirlight = CUR_SCENE->GetLight()->GetLight();

	XMVECTOR shadowPlane = ::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR toMainLight = -::XMLoadFloat3(&dirlight->GetLightDesc().direction);
	Matrix shadow = XMMatrixShadow(shadowPlane, toMainLight);
	Matrix shadowOffsetY = XMMatrixTranslation(0.f, 0.001f, 0.f);
	

	TransformData ctransformbuffer;
	ctransformbuffer.position = GetTransform()->GetPosition();
	ctransformbuffer.pad = 1.f;
	ctransformbuffer.world = GetTransform()->GetWorldMatrix() * shadow * shadowOffsetY;
	ctransformbuffer.matView = Camera::S_MatView;
	ctransformbuffer.matProjection = Camera::S_MatProjection;
	ctransformbuffer.worldnvTranspose = MathHelper::InverseTranspose(ctransformbuffer.world);
	ctransformbuffer.worldViewProj = ctransformbuffer.world * ctransformbuffer.matView * ctransformbuffer.matProjection;

	D3D12_CPU_DESCRIPTOR_HANDLE transformhandle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b1)->PushData(&ctransformbuffer, sizeof(ctransformbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(transformhandle, CBV_REGISTER::b1);


	MaterialDesc pushDesc;
	shared_ptr<Material> material = _mesh->GetShadowMaterial();

	material->UpdateShader();

	if (material)
	{
		pushDesc.ambient = material->GetAmbient();
		pushDesc.diffuse = material->GetDiffuse();
		pushDesc.specular = material->GetSpecular();
		pushDesc.emissive = Color(1.f, 1.f, 1.f, 1.f);
	}

	

	MaterialData cbuffer;
	cbuffer.mat = pushDesc;
	cbuffer.texTransform = Matrix::Identity;

	if (_texture)
	{
		cbuffer.texTransform *= 5;
		GRAPHICS->GetTableDescHeap()->SetShaderResourceView(_texture->GetCpuHandle(), SRV_REGISTER::t0);
	}




	D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b2)->PushData(&cbuffer, sizeof(cbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b2);


	GRAPHICS->GetTableDescHeap()->CommitTable();

	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->DrawIndexedInstanced(_mesh->GetIndexBuffer()->_count, 1, 0, 0, 0);
	else
		CMD_LIST->DrawInstanced(_mesh->GetVertexBuffer()->_count, 1, 0, 0);
}

void MeshRenderer::RenderReflect()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexBuffer()->GetVertexBufferView()); // Slot: (0~15)


	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->IASetIndexBuffer(&_mesh->GetIndexBuffer()->_indexBufferView);

	shared_ptr<Light> dirlight = CUR_SCENE->GetLight()->GetLight();

	XMVECTOR mirrorPlane = ::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
	XMMATRIX R = ::XMMatrixReflect(mirrorPlane);
	XMVECTOR lightDir = XMLoadFloat3(&dirlight->GetLightDesc().direction);

	TransformData ctransformbuffer;
	ctransformbuffer.position = GetTransform()->GetPosition();
	ctransformbuffer.pad = 1.f;
	ctransformbuffer.world = GetTransform()->GetWorldMatrix() * R;
	ctransformbuffer.matView = Camera::S_MatView;
	ctransformbuffer.matProjection = Camera::S_MatProjection;
	ctransformbuffer.worldnvTranspose = MathHelper::InverseTranspose(ctransformbuffer.world);
	ctransformbuffer.worldViewProj = ctransformbuffer.world * ctransformbuffer.matView * ctransformbuffer.matProjection;

	D3D12_CPU_DESCRIPTOR_HANDLE transformhandle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b1)->PushData(&ctransformbuffer, sizeof(ctransformbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(transformhandle, CBV_REGISTER::b1);


	MaterialDesc pushDesc;
	shared_ptr<Material> material = _mesh->GetReflectMaterial();

	material->UpdateShader();

	if (material)
	{
		pushDesc.ambient = material->GetAmbient();
		pushDesc.diffuse = material->GetDiffuse();
		pushDesc.specular = material->GetSpecular();
		pushDesc.emissive = Color(1.f, 1.f, 1.f, 1.f);
	}

	MaterialData cbuffer;
	cbuffer.mat = pushDesc;
	cbuffer.texTransform = Matrix::Identity;

	if (_texture)
	{
		cbuffer.texTransform *= 5;
		GRAPHICS->GetTableDescHeap()->SetShaderResourceView(_texture->GetCpuHandle(), SRV_REGISTER::t0);
	}




	D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b2)->PushData(&cbuffer, sizeof(cbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b2);

	GRAPHICS->GetTableDescHeap()->CommitTable();

	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->DrawIndexedInstanced(_mesh->GetIndexBuffer()->_count, 1, 0, 0, 0);
	else
		CMD_LIST->DrawInstanced(_mesh->GetVertexBuffer()->_count, 1, 0, 0);
}
