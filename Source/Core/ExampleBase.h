#pragma once

#include "Camera.h"
#include "HittableList.h"
#include "ImageExporter.h"
#include "Material.h"
#include "Ray.h"

class Ray;
class HittableList;

class ExampleBase
{
public:
    ExampleBase() = delete;
    ExampleBase(int imageWidth, int imageHeight);
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
    int m_imageWidth;
    int m_imageHeight;
    int m_pixelNumber;
    int m_maxRecursiveDepth;
    int m_sampleTimes;

private:
    ImageExporter m_imageExporter;
};