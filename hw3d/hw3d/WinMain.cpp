#include <Windows.h>
#include "Window.h"
#include <sstream>
#include <string>

using namespace std;

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) {
	Window wnd(800, 300, "Custom!");

	MSG msg;
	BOOL gResult;

	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);


		if (wnd.kbd.KeyIsPressed(VK_MENU)) {
			MessageBox(nullptr, "눌렸다!", "스페이스 눌림.", MB_OK);
		}

	}

	if (gResult == -1)
		return -1;


	return msg.wParam;
}