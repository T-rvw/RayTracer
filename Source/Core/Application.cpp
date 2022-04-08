#include "Application.h"

static Application* s_appliction = nullptr;

static LRESULT CALLBACK handleWin32Messages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Application::Application(int width, int height, const wchar_t* pTitle)
	: m_width(width),
	m_height(height),
	m_pTitle(pTitle)
{
	s_appliction = this;
}

Application::~Application()
{
	if (m_deviceContextHandle)
	{
		if (m_oldBitmap)
		{
			SelectObject(m_deviceContextHandle, m_oldBitmap);
			m_oldBitmap = NULL;
		}
		DeleteDC(m_deviceContextHandle);
		m_deviceContextHandle = NULL;
	}

	if (m_deviceIndependentBitmap)
	{
		DeleteObject(m_deviceIndependentBitmap);
		m_deviceIndependentBitmap = NULL;
	}

	if (m_windowHandle)
	{
		CloseWindow(m_windowHandle);
		m_windowHandle = NULL;
	}
}

void Application::init()
{
	// Register window class
	{
		WNDCLASS wc;
		wc.style = CS_BYTEALIGNCLIENT;
		wc.lpfnWndProc = static_cast<WNDPROC>(handleWin32Messages);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"RendererWindow";
		RegisterClass(&wc);
	}

	// Create window
	{
		m_windowHandle = CreateWindow(L"RendererWindow", m_pTitle,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
	}

	// Create device context
	{
		HDC tempDC = GetDC(m_windowHandle);
		m_deviceContextHandle = CreateCompatibleDC(tempDC);
		ReleaseDC(m_windowHandle, tempDC);
	}

	// Create bitmap
	{
		BITMAPINFOHEADER bi;
		memset(&bi, 0, sizeof(BITMAPINFOHEADER));
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = m_width;
		bi.biHeight = -m_height; // From top to bottom
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = m_width * m_height * 4;

		LPVOID pBitmapData;
		m_deviceIndependentBitmap = CreateDIBSection(m_deviceContextHandle, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &pBitmapData, 0, 0);
		m_oldBitmap = (HBITMAP)SelectObject(m_deviceContextHandle, m_deviceIndependentBitmap);
		m_pFrameBufferData = static_cast<uint8_t*>(pBitmapData);
	}

	// Show window in screen center position
	{
		RECT rect = { 0, 0, m_width, m_height };
		AdjustWindowRect(&rect, GetWindowLong(m_windowHandle, GWL_STYLE), 0);

		int windowPosX = rect.right - rect.left;
		int windowPosY = rect.bottom - rect.top;
		int screenCenterX = (GetSystemMetrics(SM_CXSCREEN) - windowPosX) >> 1;
		int screenCenterY = (GetSystemMetrics(SM_CYSCREEN) - windowPosY) >> 1;
		if (screenCenterY < 0) screenCenterY = 0;

		SetWindowPos(m_windowHandle, NULL, screenCenterX, screenCenterY, windowPosX, windowPosY, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
		SetForegroundWindow(m_windowHandle);
		ShowWindow(m_windowHandle, SW_NORMAL);
	}

	processMessages();
}

void Application::present() const
{
	//LOGFONT logfont;
	//ZeroMemory(&logfont, sizeof(LOGFONT));
	//logfont.lfCharSet = ANSI_CHARSET;
	//logfont.lfHeight = 20;
	//HFONT hFont = CreateFontIndirect(&logfont);

	HDC tempDC = GetDC(m_windowHandle);
	//SelectObject(m_deviceContextHandle, hFont);
	//SetTextColor(m_deviceContextHandle, RGB(190, 190, 190));
	//SetBkColor(m_deviceContextHandle, RGB(80, 80, 80));

	//TextOut(m_deviceContextHandle, 20, 20, L"Debug Output", sizeof("Debug Output"));

	BitBlt(tempDC, 0, 0, m_width, m_height, m_deviceContextHandle, 0, 0, SRCCOPY);
	ReleaseDC(m_windowHandle, tempDC);
}

void Application::processMessages()
{
	MSG msg;
	while (true)
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}