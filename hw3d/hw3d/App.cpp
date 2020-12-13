#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	:
	wnd(800, 600, "The Test DirectX!")
{}

int App::Go() {
	MSG msg;
	BOOL gResult;

	while ((gResult = GetMessage(&msg, nullptr, 0, 0) > 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		DoFrame();
	}

	if (gResult == -1)
		return -1;

	return msg.wParam;
}

void App::DoFrame() {
	const float t = timer.Peek();
	std::stringstream oss;
	oss << "Time Elapsed: " << std::setprecision(1) << std::fixed << t;
	wnd.SetTitle(oss.str());
}