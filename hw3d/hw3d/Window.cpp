#include "Window.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return wndClass.hInst;
}

Window::Window(int width, int height, const char* name) noexcept
	:
	width(width),
	height(height)
{
	RECT wr;

	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string& title) {
	SetWindowText(hWnd, title.c_str());
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	if (msg == WM_NCCREATE) {

		//���� �����Ϳ��� ������ Ŭ������ PTR ����
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		//������ �ν��Ͻ��� ptr�� �����ϵ��� WinAPI ����� ������ ����
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// ������ �������� �޽��� proc�� �Ϲ� (non-setUP) ó���� ����
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// â �ν��Ͻ� �ڵ鷯�� �޽��� ��ô
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	//������ �ν��Ͻ��� ���� ptr �˻�
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	//������ �ν��Ͻ� �ڵ鷯�� �޽��� ����
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			//Ű ������Ʈ �ʱ�ȭ (while������ ���ư���, �ൿ�� �ϸ� clear�� ���� �ʿ䰡 ����)
		case WM_KILLFOCUS:
			kbd.ClearState();
			break;


		// Ű����
		case WM_KEYDOWN:
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			break;
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) {
				kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;

		case WM_CHAR:
			kbd.OnChar(static_cast<unsigned char>(wParam));
			break;


		//���콺
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lParam);
			if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
				mouse.OnMouseMove(pt.x, pt.y);
				if (!mouse.IsInWindow()) {
					SetCapture(hWnd);
					mouse.OnMouseEnter();
				}
			}
			else {
				if (wParam && (MK_LBUTTON | MK_RBUTTON)) {
					mouse.OnMouseMove(pt.x, pt.y);
				}
				else {
					ReleaseCapture();
					mouse.OnMouseLeave();
				}
			}
		}

		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftReleased(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightReleased(pt.x, pt.y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse.OnWheelDelta(pt.x, pt.y, delta);
			break;
		}		
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> Window::ProcessMessage() {
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}