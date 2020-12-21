#pragma once
#include "Window.h"
#include "CustomTimer.h"

class App {
public:
	App();
	~App();
	// ������ ������, �޽��� ����
	int Go();

private:
	void DoFrame();
private:
	Window wnd;
	CustomTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
};