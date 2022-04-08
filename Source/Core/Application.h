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

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

private:
    int                     m_width;
    int                     m_height;
    const wchar_t*          m_pTitle;

    // Win32
    HWND                    m_windowHandle;
    HDC                     m_deviceContextHandle;
    HBITMAP                 m_deviceIndependentBitmap;
    HBITMAP                 m_oldBitmap;
    uint8_t*                m_pFrameBufferData;
};