#include "pch.h"
#include "Main.h"
#include "03. CameraDemo.h"
#include "01. RectangleDemo.h"
#include "04. TextureDemo.h"
#include "05. HillsDemo.h"
#include "06. ShapesDemo.h"
#include "07. WavesDemo.h"
#include "08. LightingDemo.h"
#include "09. SkullDemo.h"
#include "10. CrateDemo.h"
#include "11. TexturedHillsAndWavesDemo.h"
#include "12. BlendDemo.h"
#include "13. MirrorDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GameDesc desc;
	desc.appName = L"GameCoding";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 800;
	desc.height = 600;
	desc.clearColor = Color(0.5f, 0.5f, 1.f, 1.f);
	desc.app = make_shared<MirrorDemo>();

	GAME->Run(desc);

	return 0;
}