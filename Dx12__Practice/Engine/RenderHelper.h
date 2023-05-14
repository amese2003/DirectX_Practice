#pragma once


struct CameraData
{
	Matrix matView;
	Matrix matProjection;
};

struct TransformData
{
	Matrix offset;
	Matrix matView;
	Matrix matProjection;
};

struct DirectionalLight
{
	Vec4 Ambient;
	Vec4 Diffuse;
	Vec4 Specular;
	Vec3 Direction;
	float pad;
};

struct PointLight
{
	Vec4 Ambient;
	Vec4 Diffuse;
	Vec4 Specular;

	Vec3 Position;
	float Range;

	Vec3 Att;
	float pad;
};

struct SpotLight
{
	Vec4 Ambient;
	Vec4 Diffuse;
	Vec4 Specular;

	Vec3 Position;
	float Range;

	Vec3 Direction;
	float Spot;

	Vec3 Att;
	float pad;
};

struct CbMaterial
{
	Vec4 Ambient;
	Vec4 Diffuse;
	Vec4 Specular; // w = SpecPower
	Vec4 Reflect;
};

struct CbPerFrame
{
	DirectionalLight gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	Vec3 gEyePosW;
};

struct CbPerObject
{
	Matrix gWorld;
	Matrix gWorldInvTranspose;
	Matrix gWorldViewProj;
	CbMaterial gMaterial;
};

