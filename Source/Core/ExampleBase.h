#pragma once

#include "Camera.h"
#include "FrameBuffer.h"
#include "HittableList.h"

class ExampleBase
{
public:
    ExampleBase() = delete;
    explicit ExampleBase(int imageWidth, int imageHeight);
    virtual ~ExampleBase() = default;

    ExampleBase(const ExampleBase&) = delete;
    ExampleBase(ExampleBase&&) = delete;
    ExampleBase& operator=(const ExampleBase&) = delete;
    ExampleBase& operator=(ExampleBase&&) = delete;

    void setSampleTimes(int sampleTimes) { m_sampleTimes = sampleTimes; }
    void setMaxRecursiveDepth(int depth) { m_maxRecursiveDepth = depth; }

    virtual Color getRayColor(const Ray& ray, const HittableList& world, int curDepth);
    void generate(const char* fileName);
    void process(const Camera& camera, const HittableList& world);

protected:
    FrameBuffer<PixelFormat::RGBA> m_frameBuffer;
    int m_maxRecursiveDepth;
    int m_sampleTimes;
};