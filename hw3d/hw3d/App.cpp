#include "App.h"
//#include <sstream>
//#include <iomanip>

App::App()
	:
	wnd(800, 600, "The Test DirectX!")
{}

int App::Go() {
	/*MSG msg;
	BOOL gResult;*/

	while (1) {
		if (const auto ecode = Window::ProcessMessage()) {
			return *ecode;
		}

		DoFrame();
	}

	/*if (gResult == -1)
		return -1;

	return msg.wParam;*/
}

void App::DoFrame() {
	/*const float t = timer.Peek();
	std::stringstream oss;
	oss << "Time Elapsed: " << std::setprecision(1) << std::fixed << t;
	wnd.SetTitle(oss.str());*/

	wnd.Gfx().EndFrame();
}