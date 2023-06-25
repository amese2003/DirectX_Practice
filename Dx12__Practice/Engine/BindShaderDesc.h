#pragma once
#include "ConstantBuffer.h"
#include "MathHelper.h"

#define MaxLights 16

class Shader;

struct ShaderArg
{
	string vs = "VS_Main";
	string hs;
	string ds;
	string gs;
	string ps = "PS_Main";
	string cs = "CS_Main";
};


struct MaterialDesc
{
	Color ambient = Color(0.f, 0.f, 0.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(0.f, 0.f, 0.f, 1.f);
	Color emissive = Color(0.f, 0.f, 0.f, 1.f);

	//Color diffuseAlbedo = Color(1.f, 1.f, 1.f, 1.f);
	//Vec3 FresmeIRO = Vec3::Zero;
	//float shininess = 0;
};

struct LightDesc
{
	Color ambient = Color(1.f, 1.f, 1.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(1.f, 1.f, 1.f, 1.f);
	Color emissive = Color(1.f, 1.f, 1.f, 1.f);

	Vec3 direction = Vec3(0.f, 0.f, 0.f);;
	float range = 26.f;

	Vec3 Att = Vec3(0.f, 0.f, 0.f);
	float _spot = 90.f;

	Vec3 position = Vec3(0.f, 0.f, 0.f);
	float pad = 0.f;

	Vec3 Strength = { 0.5f, 0.5f, 0.5f };
	float falloffStart = 1.0f;

};

struct GlobalParams
{
	Matrix	matView = Matrix::Identity;
	Matrix	invView = Matrix::Identity;
	Matrix	matProj = Matrix::Identity;
	Matrix	invProj = Matrix::Identity;
	Matrix	viewProj = Matrix::Identity;;
	Matrix	invViewProj = Matrix::Identity;;
	Vec3	eyePosition;
	int     lightCount = 0;
	Vec2	renderTargetSize;
	Vec2	invRenderTargetSize;

	float nearZ = 0.f;
	float farZ = 0.f;
	float totalTime = 0.f;
	float deltaTime = 0.f;
	Vec4 ambientLight = { 0.f, 0.f, 0.f, 1.f };



	/*float gFogStart;
	float gFogRange;
	Vec2 pad;
	Color gFogColor;*/


	LightDesc		lights[16];
};


struct TransformData
{
	Matrix world;
};

//struct MaterialData
//{
//	MaterialDesc mat;
//	Matrix texTransform;
//	Vec3 FresnelR0;
//	float Roughness;
//};

struct Fixeduv
{
	Vec2 gTexC[4];
};

struct BlurParam
{
	int BlurRadius = 5;
	float weight[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };
};





struct LightData
{
	DirectX::XMFLOAT3 Strength = { 0.5f, 0.5f, 0.5f };
	float FalloffStart = 1.0f;                          // point/spot light only
	DirectX::XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };// directional/spot light only
	float FalloffEnd = 10.0f;                           // point/spot light only
	DirectX::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };  // point/spot light only
	float SpotPower = 64.0f;                            // spot light only
};


struct InstanceData
{
	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();
	UINT MaterialIndex;
	UINT InstancePad0;
	UINT InstancePad1;
	UINT InstancePad2;
};


struct PassConstants
{
	DirectX::XMFLOAT4X4 View = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 Proj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
	float cbPerObjectPad1 = 0.0f;
	DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;

	DirectX::XMFLOAT4 AmbientLight = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Indices [0, NUM_DIR_LIGHTS) are directional lights;
	// indices [NUM_DIR_LIGHTS, NUM_DIR_LIGHTS+NUM_POINT_LIGHTS) are point lights;
	// indices [NUM_DIR_LIGHTS+NUM_POINT_LIGHTS, NUM_DIR_LIGHTS+NUM_POINT_LIGHT+NUM_SPOT_LIGHTS)
	// are spot lights for a maximum of MaxLights per object.
	LightData Lights[MaxLights];
};

struct MaterialData
{
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 64.0f;

	// Used in texture mapping.
	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();

	UINT DiffuseMapIndex = 0;
	UINT MaterialPad0;
	UINT MaterialPad1;
	UINT MaterialPad2;
};