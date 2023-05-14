#include "pch.h"
#include "Waves.h"
#include <algorithm>
#include <vector>
#include <cassert>
#include "TimeManager.h"
#include "MathHelper.h"
#include "Light.h"

Waves::Waves() : Super(ComponentType::Waves)
{
}

Waves::~Waves()
{
}

void Waves::Update()
{
	_shader->Update();

	static float t_base = 0.0f;
	if ((TIME->TotalTime() - t_base) >= 0.25f)
	{
		t_base += 0.25f;

		DWORD i = 5 + rand() % 190;
		DWORD j = 5 + rand() % 190;

		float r = MathHelper::RandF(1.0f, 2.0f);

		Disturb(i, j, r);
	}


	Update(DT);
	Render();
}

void Waves::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	

	//
	// Draw the waves.
	//

	CMD_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexBuffer()->GetVertexBufferView()); // Slot: (0~15)
	CMD_LIST->IASetIndexBuffer(&_mesh->GetIndexBuffer()->GetIndexBufferView());

	TransformData cbuffer;
	cbuffer.offset = GetTransform()->GetWorldMatrix();
	cbuffer.matView = Camera::S_MatView;
	cbuffer.matProjection = Camera::S_MatProjection;

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b1)->PushData(&cbuffer, sizeof(cbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b1);

	if (_dirLight)
	{
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

		auto c3buffer = GetTransform()->CbPerObjectData;
		c3buffer.gMaterial.Ambient = _mesh->GetMaterial()->GetAmbient();
		c3buffer.gMaterial.Diffuse = _mesh->GetMaterial()->GetDiffuse();
		c3buffer.gMaterial.Specular = _mesh->GetMaterial()->GetSpecular();



		D3D12_CPU_DESCRIPTOR_HANDLE objecthandle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b3)->PushData(&c3buffer, sizeof(c3buffer));
		GRAPHICS->GetTableDescHeap()->SetConstantBuffer(objecthandle, CBV_REGISTER::b3);
	}

	

	

	GRAPHICS->GetTableDescHeap()->CommitTable();
	CMD_LIST->DrawIndexedInstanced(3 * TriangleCount(), 1, 0, 0, 0);
}

void Waves::Init(shared_ptr<Mesh> mesh, shared_ptr<Shader> shader, uint32 m, uint32 n, float dx, float dt, float speed, float damping)
{
	_shader = shader;
	_mesh = mesh;
	_numRows = m;
	_numCols = n;

	_vertexCount = m * n;
	_triangleCount = (m - 1) * (n - 1) * 2;

	_timeStep = dt;
	_spatialStep = dx;

	float d = damping * dt + 2.0f;
	float e = (speed * speed) * (dt * dt) / (dx * dx);
	_k1 = (damping * dt - 2.0f) / d;
	_k2 = (4.0f - 8.0f * e) / d;
	_k3 = (2.0f * e) / d;

	// In case Init() called again.
	_prevSolution.clear();
	_currSolution.clear();
	_normals.clear();
	_tangentX.clear();

	_prevSolution.resize(m * n);
	_currSolution.resize(m * n);
	_normals.resize(m * n);
	_tangentX.resize(m * n);

	// Generate grid vertices in system memory.

	float halfWidth = (n - 1) * dx * 0.5f;
	float halfDepth = (m - 1) * dx * 0.5f;

	vector<VertexTextureNormalTangentData> vtx;
	vtx.resize(m * n);

	for (uint32 i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dx;

		for (uint32 j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;

			_prevSolution[i * n + j] = XMFLOAT3(x, 0.0f, z);
			_currSolution[i * n + j] = XMFLOAT3(x, 0.0f, z);
			_normals[i * n + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);
			_tangentX[i * n + j] = XMFLOAT3(1.0f, 0.0f, 0.0f);


			VertexTextureNormalTangentData buffer;
			buffer.position = _currSolution[i * n + j];
			buffer.normal = _normals[i * n + j];
			buffer.tangent = _tangentX[i * n + j];
			
			vtx[i * n + j] = buffer;
		}
	}

	// Create the index buffer.  The index buffer is fixed, so we only 
	// need to create and set once.

	std::vector<uint32> indices(3 * TriangleCount()); // 3 indices per face

	// Iterate over each quad.
	m = RowCount();
	n = ColumnCount();
	int k = 0;
	for (uint32 i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1) * n + j;

			indices[k + 3] = (i + 1) * n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}

	_mesh->GetVertexBuffer()->CreateTexture(vtx);
	_mesh->GetIndexBuffer()->CreateTexture(indices);
}

void Waves::Update(float dt)
{
	static float t = 0;

	// Accumulate time.
	t += dt;

	// Only update the simulation at the specified time step.
	if (t >= _timeStep)
	{
		// Only update interior points; we use zero boundary conditions.
		for (DWORD i = 1; i < _numRows - 1; ++i)
		{
			for (DWORD j = 1; j < _numCols - 1; ++j)
			{
				// After this update we will be discarding the old previous
				// buffer, so overwrite that buffer with the new update.
				// Note how we can do this inplace (read/write to same element) 
				// because we won't need prev_ij again and the assignment happens last.

				// Note j indexes x and i indexes z: h(x_j, z_i, t_k)
				// Moreover, our +z axis goes "down"; this is just to 
				// keep consistent with our row indices going down.

				_prevSolution[i * _numCols + j].y =
					_k1 * _prevSolution[i * _numCols + j].y +
					_k2 * _currSolution[i * _numCols + j].y +
					_k3 * (_currSolution[(i + 1) * _numCols + j].y +
						_currSolution[(i - 1) * _numCols + j].y +
						_currSolution[i * _numCols + j + 1].y +
						_currSolution[i * _numCols + j - 1].y);
			}
		}

		// We just overwrote the previous buffer with the new data, so
		// this data needs to become the current solution and the old
		// current solution becomes the new previous solution.
		_prevSolution.swap(_currSolution);

		t = 0.0f; // reset time

		vector<VertexTextureNormalTangentData> vtx;
		vtx.resize(_currSolution.size());

		//
		// Compute normals using finite difference scheme.
		//
		for (uint32 i = 1; i < _numRows - 1; ++i)
		{
			for (uint32 j = 1; j < _numCols - 1; ++j)
			{
				float l = _currSolution[i * _numCols + j - 1].y;
				float r = _currSolution[i * _numCols + j + 1].y;
				float t = _currSolution[(i - 1) * _numCols + j].y;
				float b = _currSolution[(i + 1) * _numCols + j].y;
				_normals[i * _numCols + j].x = -r + l;
				_normals[i * _numCols + j].y = 2.0f * _spatialStep;
				_normals[i * _numCols + j].z = b - t;

				XMVECTOR n = ::XMVector3Normalize(::XMLoadFloat3(&_normals[i * _numCols + j]));
				::XMStoreFloat3(&_normals[i * _numCols + j], n);

				_tangentX[i * _numCols + j] = XMFLOAT3(2.0f * _spatialStep, r - l, 0.0f);
				XMVECTOR T = ::XMVector3Normalize(::XMLoadFloat3(&_tangentX[i * _numCols + j]));
				::XMStoreFloat3(&_tangentX[i * _numCols + j], T);

				VertexTextureNormalTangentData buffer;
				buffer.position = _currSolution[i * _numCols + j];
				buffer.normal = _normals[i * _numCols + j];
				buffer.tangent = _tangentX[i * _numCols + j];

				vtx[i * _numCols + j] = buffer;
			}
		}

		_mesh->GetVertexBuffer()->PushData(vtx);
	}
}

void Waves::Disturb(uint32 i, uint32 j, float magnitude)
{
	// Don't disturb boundaries.
	assert(i > 1 && i < _numRows - 2);
	assert(j > 1 && j < _numCols - 2);

	float halfMag = 0.5f * magnitude;

	// Disturb the ijth vertex height and its neighbors.
	_currSolution[i * _numCols + j].y += magnitude;
	_currSolution[i * _numCols + j + 1].y += halfMag;
	_currSolution[i * _numCols + j - 1].y += halfMag;
	_currSolution[(i + 1) * _numCols + j].y += halfMag;
	_currSolution[(i - 1) * _numCols + j].y += halfMag;
}
