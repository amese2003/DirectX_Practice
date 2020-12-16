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
		MessageBox(nullptr, e.what(), "�⺻ ����", MB_OK);
	}
	catch (...) {
		MessageBox(nullptr, "������ ����", "Ȯ�� �Ұ�", MB_OK);
	}

	return -1;
}