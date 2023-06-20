#pragma once
#include "ConstantBuffer.h"

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

};

struct LightParams
{
	uint32			lightCount;
	Vec3			eyePosition;
	LightDesc		lights[50];

	float gFogStart;
	float gFogRange;
	Vec2 pad;
	Color gFogColor;
};


struct TransformData
{
	Vec3   position;
	float  pad;
	Matrix world;
	Matrix matView;
	Matrix matProjection;
	Matrix worldnvTranspose;
	Matrix worldViewProj;
	Matrix ViewProj;
};

struct MaterialData
{
	MaterialDesc mat;
	Matrix texTransform;
	Vec3 FresnelR0;
	float Roughness;
};

struct Fixeduv
{
	Vec2 gTexC[4];
};

struct BlurParam
{
	int BlurRadius = 5;
	float weight[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };
};
