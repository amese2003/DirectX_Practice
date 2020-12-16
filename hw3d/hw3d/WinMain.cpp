#include "App.h"


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
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "기본 에러", MB_OK);
	}
	catch (...) {
		MessageBox(nullptr, "데이터 없음", "확인 불가", MB_OK);
	}

	return -1;
}