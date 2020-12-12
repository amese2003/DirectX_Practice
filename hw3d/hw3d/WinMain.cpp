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

		/*while (!wnd.mouse.IsEmpty()) {
			const auto e = wnd.mouse.Read();

			if (e.GetType() == Mouse::Event::Type::Move) {
				std::ostringstream oss;
				oss << "Mouse Position : (" << e.GetPosX() << "," << e.GetPosY() << ")";
				wnd.SetTitle(oss.str());
			}
		}*/


		/*if (wnd.kbd.KeyIsPressed(VK_MENU)) {
			MessageBox(nullptr, "눌렸다!", "스페이스 눌림.", MB_OK);
		}*/

	}

	if (gResult == -1)
		return -1;


	return msg.wParam;
}