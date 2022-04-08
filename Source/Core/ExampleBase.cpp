#include "ExampleBase.h"
#include "ImageExporter.h"

#include <iostream>

ExampleBase::ExampleBase(int imageWidth, int imageHeight) :
    m_frameBuffer(imageWidth, imageHeight)
{
}

void ExampleBase::generate(const char* fileName)
{
    if (ImageExporter::generate(m_frameBuffer, fileName))
    {
        std::cout << "Succeed to generate image." << std::endl;
    }
    else
    {
        std::cout << "Failed to generate image." << std::endl;
    }
}

void ExampleBase::process(const Camera& camera, const HittableList& world)
{
    uint64_t beginTimeStamp = ::time(nullptr);
    std::atomic<int> curPixelCount = 0;
    const int frameBufferWidth = m_frameBuffer.getWidth();
    const int frameBufferHeight = m_frameBuffer.getHeight();
    const int frameBufferPixelNumber = frameBufferWidth * frameBufferHeight;

#pragma omp parallel for
    for (int jj = frameBufferHeight - 1; jj >= 0; --jj)
    {
        for (int ii = 0; ii < frameBufferWidth; ++ii)
        {
            Color pixelColor(0.0, 0.0, 0.0);

            for (int sampleTimes = 0; sampleTimes < m_sampleTimes; ++sampleTimes)
            {
                // Generate random rays in a cluster
                double u = static_cast<double>(ii + MathUtils::randomDouble()) / (frameBufferWidth - 1);
                double v = static_cast<double>(jj + MathUtils::randomDouble()) / (frameBufferHeight - 1);
                Ray ray = camera.getRay(u, v);
                pixelColor += getRayColor(ray, world, m_maxRecursiveDepth);
            }

            // sample && gamma-correct(1/2)
            double sampleScale = 1.0 / m_sampleTimes;
            pixelColor[0] = std::clamp(pow(pixelColor.x() * sampleScale, 0.5), 0.0, 1.0);
            pixelColor[1] = std::clamp(pow(pixelColor.y() * sampleScale, 0.5), 0.0, 1.0);
            pixelColor[2] = std::clamp(pow(pixelColor.z() * sampleScale, 0.5), 0.0, 1.0);

            // size_t pixelIndex = (imageHeight - 1 - jj) * imageWidth + ii;
            int pixelIndex = frameBufferPixelNumber - (jj + 1) * frameBufferWidth + ii;
            m_frameBuffer.fill(pixelIndex, pixelColor);

            printf("Fill color pixel placed at %d, progress = %d/%d.\n", static_cast<int>(pixelIndex), ++curPixelCount, frameBufferPixelNumber);
        }
    }

    uint64_t endTimeStamp = ::time(nullptr);
    printf("Finish processing, costs %d seconds.\n", static_cast<int>(endTimeStamp - beginTimeStamp));
}

Color ExampleBase::getRayColor(const Ray& ray, const HittableList& world, int curDepth)
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (curDepth <= 0)
    {
        return Color(1.0, 1.0, 1.0);
    }

    std::optional<HitRecord> optHitRecord = world.hit(ray, DOUBLE_EPS, DOUBLE_INFINITY);
    if (optHitRecord.has_value())
    {
        const HitRecord& hitRecord = optHitRecord.value();
        if (const GeometryBase* pHitObject = hitRecord.hitObject())
        {
            Ray scattered;
            Color attenuation;
            if (std::shared_ptr<Material> pMaterial = pHitObject->material())
            {
                if (pMaterial->scatter(ray, hitRecord, attenuation, scattered))
                {
                    return attenuation * getRayColor(scattered, world, curDepth - 1);
                }

                return Color(0.0, 0.0, 0.0);
            }
        }
    }

    // background
    XYZ unitDir = ray.direction();
    unitDir.normalize();
    double factor = 0.5 * (unitDir.y() + 1.0);
    return (1.0 - factor) * Color(1.0, 1.0, 1.0) + factor * Color(0.5, 0.7, 1.0);
}