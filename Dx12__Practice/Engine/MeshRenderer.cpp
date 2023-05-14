#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"

MeshRenderer::MeshRenderer() : Super(ComponentType::MeshRenderer)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{
	_shader->Update();
	Render();
}

void MeshRenderer::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexBuffer()->GetVertexBufferView()); // Slot: (0~15)
	CMD_LIST->IASetIndexBuffer(&_mesh->GetIndexBuffer()->_indexBufferView);

	if (_dirLight)
	{
		auto c3buffer = GetTransform()->CbPerObjectData;
		c3buffer.gMaterial.Ambient = _mesh->GetMaterial()->GetAmbient();
		c3buffer.gMaterial.Diffuse = _mesh->GetMaterial()->GetDiffuse();
		c3buffer.gMaterial.Specular = _mesh->GetMaterial()->GetSpecular();

		CbPerFrame cbLightBuffer;
		cbLightBuffer.gDirLight.Ambient = _dirLight->GetMaterial()->GetAmbient();
		cbLightBuffer.gDirLight.Diffuse = _dirLight->GetMaterial()->GetDiffuse();
		cbLightBuffer.gDirLight.Specular = _dirLight->GetMaterial()->GetSpecular();
		cbLightBuffer.gDirLight.Direction = _dirLight->GetDir();

		cbLightBuffer.gPointLight.Ambient = _point->GetMaterial()->GetAmbient();
		cbLightBuffer.gPointLight.Diffuse = _point->GetMaterial()->GetDiffuse();
		cbLightBuffer.gPointLight.Specular = _point->GetMaterial()->GetSpecular();
		cbLightBuffer.gPointLight.Att = _point->GetAtt();
		cbLightBuffer.gPointLight.Range = _point->GetRange();
		cbLightBuffer.gPointLight.Position = _point->GetTransform()->GetPosition();


		cbLightBuffer.gSpotLight.Ambient = _spotLight->GetMaterial()->GetAmbient();
		cbLightBuffer.gSpotLight.Diffuse = _spotLight->GetMaterial()->GetDiffuse();
		cbLightBuffer.gSpotLight.Specular = _spotLight->GetMaterial()->GetSpecular();
		cbLightBuffer.gSpotLight.Direction = _spotLight->GetDir();
		cbLightBuffer.gSpotLight.Position = _spotLight->GetTransform()->GetPosition();
		cbLightBuffer.gSpotLight.Range = _spotLight->GetRange();
		cbLightBuffer.gSpotLight.Att = _spotLight->GetAtt();
		cbLightBuffer.gSpotLight.Spot = _spotLight->GetSpot();

		cbLightBuffer.gEyePosW = Camera::S_Eyepos;


		D3D12_CPU_DESCRIPTOR_HANDLE lighthandle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b2)->PushData(&cbLightBuffer, sizeof(cbLightBuffer));
		GRAPHICS->GetTableDescHeap()->SetConstantBuffer(lighthandle, CBV_REGISTER::b2);




		D3D12_CPU_DESCRIPTOR_HANDLE objecthandle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b3)->PushData(&c3buffer, sizeof(c3buffer));
		GRAPHICS->GetTableDescHeap()->SetConstantBuffer(objecthandle, CBV_REGISTER::b3);
	}

	



	GRAPHICS->GetTableDescHeap()->CommitTable();
	CMD_LIST->DrawIndexedInstanced(_mesh->GetIndexBuffer()->_count, 1, 0, 0, 0);
	
}
