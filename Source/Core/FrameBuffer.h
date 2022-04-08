#pragma once

#include "XYZ.h"

class FrameBuffer
{
public:
    FrameBuffer() = delete;
    explicit FrameBuffer(int width, int height);
    virtual ~FrameBuffer();

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer(FrameBuffer&&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
    FrameBuffer& operator=(FrameBuffer&&) = delete;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    const uint8_t* getData() const { return m_pData; }

    void fill(int index, const XYZ& value);
    int fill(int x, int y, const XYZ& value);

protected:
    int         m_width;
    int         m_height;
    uint8_t*    m_pData;
};