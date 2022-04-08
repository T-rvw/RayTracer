#pragma once

#include "Camera.h"
#include "FrameBuffer.h"
#include "HittableList.h"

class ExampleWindows
{
public:
    ExampleWindows() = delete;
    explicit ExampleWindows(FrameBuffer<PixelFormat::BGRA>* pFrameBuffer);
    virtual ~ExampleWindows() = default;

    ExampleWindows(const ExampleWindows&) = delete;
    ExampleWindows(ExampleWindows&&) = delete;
    ExampleWindows& operator=(const ExampleWindows&) = delete;
    ExampleWindows& operator=(ExampleWindows&&) = delete;

    void setSampleTimes(int sampleTimes) { m_sampleTimes = sampleTimes; }
    void setMaxRecursiveDepth(int depth) { m_maxRecursiveDepth = depth; }

    virtual Color getRayColor(const Ray& ray, const HittableList& world, int curDepth);
    bool process(const Camera& camera, const HittableList& world);

protected:
    FrameBuffer<PixelFormat::BGRA>* m_pFrameBuffer;
    int m_maxRecursiveDepth;
    int m_sampleTimes;

private:
    uint64_t    m_beginTimeStamp = 0ULL;
    int         m_curPixelCount = 0;
    int         m_currentProgressI = 0;
    int         m_currentProgressJ = 0;
    int         m_currentProgressSamplerTimes = 1;
    bool        m_finish = false;
};