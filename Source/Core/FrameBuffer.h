#pragma once

#include "XYZ.h"

enum class PixelFormat
{
    RGBA,
    BGRA,
};

constexpr int component = 4;

template<PixelFormat pixelFormat>
class FrameBuffer
{
public:
    FrameBuffer() = delete;
    explicit FrameBuffer(int width, int height)
        : m_width(width),
        m_height(height),
        m_hasDataOwnership(true)
    {
        m_pData = new uint8_t[width * height * component];
    }

    explicit FrameBuffer(uint8_t* pData, int width, int height)
        : m_pData(pData),
        m_width(width),
        m_height(height),
        m_hasDataOwnership(false)
    {
    }

    virtual FrameBuffer::~FrameBuffer()
    {
        if (m_hasDataOwnership && m_pData != nullptr)
        {
            delete[] m_pData;
            m_pData = nullptr;
        }
    }

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer(FrameBuffer&&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
    FrameBuffer& operator=(FrameBuffer&&) = delete;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    const uint8_t* getData() const { return m_pData; }

    void clear(const XYZ& value, uint8_t alpha = 255)
    {
        for (int i = 0, num = m_width * m_height; i < num; ++i)
        {
            int rgbIndex = i * component;
            fillInternal(rgbIndex, static_cast<uint8_t>(value.x()), static_cast<uint8_t>(value.y()), static_cast<uint8_t>(value.z()), alpha);
        }
    }

    void fill(int index, const XYZ& value, uint8_t alpha = 255)
    {
        uint8_t r = static_cast<uint8_t>(255.999 * value.x());
        uint8_t g = static_cast<uint8_t>(255.999 * value.y());
        uint8_t b = static_cast<uint8_t>(255.999 * value.z());

        int rgbIndex = index * component;
        fillInternal(rgbIndex, r, g, b, alpha);
    }

    int fill(int x, int y, const XYZ& value, uint8_t alpha = 255)
    {
        uint8_t r = static_cast<uint8_t>(255.999 * value.x());
        uint8_t g = static_cast<uint8_t>(255.999 * value.y());
        uint8_t b = static_cast<uint8_t>(255.999 * value.z());

        int rgbIndex = (y * m_width + x) * component;
        fillInternal(rgbIndex, r, g, b, alpha);

        return rgbIndex;
    }

private:
    void fillInternal(int byteIndex, uint8_t a, uint8_t b, uint8_t c, uint8_t d)
    {
        // sanity checks to avoid crash in experiments
        if (byteIndex < 0 || byteIndex >= m_width * m_height * component) return;

        if constexpr (pixelFormat == PixelFormat::RGBA)
        {
            m_pData[byteIndex] = a;
            m_pData[byteIndex + 1] = b;
            m_pData[byteIndex + 2] = c;
            m_pData[byteIndex + 3] = d;
        }
        else if constexpr (pixelFormat == PixelFormat::BGRA)
        {
            m_pData[byteIndex] = c;
            m_pData[byteIndex + 1] = b;
            m_pData[byteIndex + 2] = a;
            m_pData[byteIndex + 3] = d;
        }
    }

protected:
    int         m_width;
    int         m_height;
    uint8_t*    m_pData;
    bool        m_hasDataOwnership;
};