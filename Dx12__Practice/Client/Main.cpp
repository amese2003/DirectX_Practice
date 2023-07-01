#include "pch.h"
#include "Main.h"

#include "20. InstancingAndCullingDemo.h"
#include "21. PickingDemo.h"
#include "22. CubeMapDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//GameDesc desc;
	//desc.appName = L"GameCoding";
	//desc.hInstance = hInstance;
	//desc.vsync = false;
	//desc.hWnd = NULL;
	//desc.width = 800;
	//desc.height = 600;
	//desc.clearColor = Color(0.5f, 0.5f, 1.f, 1.f);
	//desc.app = make_shared<InstancingAndCullingDemo>();

	//GAME->Run(desc);


	//InstancingAndCullingDemo app(hInstance);
	PickingDemo app(hInstance);
	//CubeMapDemo app(hInstance);

	if (!app.Init())
		return 0;

	app.Run();

	return 0;
}