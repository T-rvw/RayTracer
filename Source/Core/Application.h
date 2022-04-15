#pragma once

#include "FrameBuffer.h"

#include <inttypes.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

class Application
{
public:
    Application() = delete;
    Application(int width, int height, const wchar_t* pTitle);
    virtual ~Application();

    void init();
    void processMessages();
    void present() const;
    bool isClosed() const { return false; }
    uint8_t* getFrameBuffer() { return m_pFrameBufferData; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    void setDebugOutput(const wchar_t* pOutputText) { m_pOutputDebugText = pOutputText; }
    void setMousePos(int x, int y) { m_mouseX = x; m_mouseY = y; }
    int getMouseX() const { return m_mouseX; }
    int getMouseY() const { return m_mouseY; }

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

private:
    int                     m_width;
    int                     m_height;
    const wchar_t*          m_pTitle;

    int                     m_mouseX = 0;
    int                     m_mouseY = 0;
    const wchar_t*          m_pOutputDebugText = L"Debug Output:";

    // Win32
    HWND                    m_windowHandle;
    HDC                     m_deviceContextHandle;
    HBITMAP                 m_deviceIndependentBitmap;
    HBITMAP                 m_oldBitmap;
    uint8_t*                m_pFrameBufferData;
};