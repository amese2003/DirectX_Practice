#include <Windows.h>
#include "Window.h"
#include "App.h"
#include "CustomException.h"
#include <sstream>
#include <string>

using namespace std;

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) {
	
	try {
		return App().Go();
	}
	catch (const CustomException& e) {
		MessageBox(nullptr, e.what(), "커스텀 에러창", MB_OK);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "에러창", MB_OK);
	}
	catch (...) {
		MessageBox(nullptr, "데이터 없음", "확인 불가", MB_OK);
	}

	return -1;
}