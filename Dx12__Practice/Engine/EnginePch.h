#pragma once

#define WIN32_LEAN_AND_MEAN

#include "Types.h"
#include "Define.h"

// STL
#include <memory>
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;

// WIN
#include <windows.h>
#include <assert.h>
#include <optional>

// DX
#include "d3dx12.h"
#include "SimpleMath.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

// 각종 lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

// Managers
#include "Game.h"
#include "Graphics.h"
#include "Geometry.h"
//#include "InputManager.h"
//#include "TimeManager.h"
//#include "ResourceManager.h"

#define CHECK(p)		assert(SUCCEEDED(p))
#define GAME			GET_SINGLE(Game)		
#define GRAPHICS		GET_SINGLE(Graphics)
#define DEVICE			GRAPHICS->GetDevice()->GetDevice()
#define CMD_LIST		GRAPHICS->GetCommandQueue()->GetCmdList()
#define ROOT_SIGNATURE	GRAPHICS->GetRootsignature()->GetSignature()
#define INPUT			GET_SINGLE(InputManager)
#define TIME			GET_SINGLE(TimeManager)
#define DT				TIME->GetDeltaTime()
#define RESOURCES		GET_SINGLE(ResourceManager)

// Engine
#include "Device.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "CommandQueue.h"


#include "ResourceBase.h"
#include "VertexData.h"
#include "VertexBuffer.h"
//#include "IndexBuffer.h"
//#include "ConstantBuffer.h"
#include "Shader.h"
//#include "IExecute.h"
//
//#include "GameObject.h"
//#include "Transform.h"
//#include "Texture.h"
//#include "Mesh.h"



struct WindowInfo
{
	HWND	hwnd; // 출력 윈도우
	int32	width; // 너비
	int32	height; // 높이
	bool	windowed; // 창모드 or 전체화면
};