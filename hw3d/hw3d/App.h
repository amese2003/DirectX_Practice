#pragma once
#include "Window.h"
#include "CustomTimer.h"

class App {
public:
	App();
	~App();
	// 마스터 프레임, 메시지 루프
	int Go();

private:
	void DoFrame();
private:
	Window wnd;
	CustomTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
};