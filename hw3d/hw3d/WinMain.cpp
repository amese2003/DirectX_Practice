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
		MessageBox(nullptr, e.what(), "Ŀ���� ����â", MB_OK);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "����â", MB_OK);
	}
	catch (...) {
		MessageBox(nullptr, "������ ����", "Ȯ�� �Ұ�", MB_OK);
	}

	return -1;
}