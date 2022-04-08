#include "ExampleWindows.h"
#include "ImageExporter.h"

#include <iostream>

ExampleWindows::ExampleWindows(FrameBuffer<PixelFormat::BGRA>* pFrameBuffer) :
    m_pFrameBuffer(pFrameBuffer)
{
    m_beginTimeStamp = ::time(nullptr);
}

bool ExampleWindows::process(const Camera& camera, const HittableList& world)
{
    const int frameBufferWidth = m_pFrameBuffer->getWidth();
    const int frameBufferHeight = m_pFrameBuffer->getHeight();
    const int frameBufferPixelNumber = frameBufferWidth * frameBufferHeight;

    ++m_currentProgressJ;
    if (m_currentProgressJ >= frameBufferHeight)
    {
        m_currentProgressJ = 0;
        ++m_currentProgressI;
        if (m_currentProgressI >= frameBufferWidth)
        {
            // End the whole process
            if (m_currentProgressSamplerTimes >= m_sampleTimes)
            {
                m_finish = true;
                uint64_t endTimeStamp = ::time(nullptr);
                printf("Finish processing, costs %d seconds.\n", static_cast<int>(endTimeStamp - m_beginTimeStamp));
                return true;
            }
            else
            {
                // End one step sampler
                m_currentProgressSamplerTimes += 10;
                if (m_currentProgressSamplerTimes >= m_sampleTimes)
                {
                    m_currentProgressSamplerTimes = m_sampleTimes;
                }

                m_currentProgressI = 0;
                m_currentProgressJ = 0;
                m_curPixelCount = 0;
                return false;
            }
        }
        else
        {
            // End one column
            return false;
        }
    }

    Color pixelColor(0.0, 0.0, 0.0);
    for (int sampleTimes = 0; sampleTimes < m_currentProgressSamplerTimes; ++sampleTimes)
    {
        // Generate random rays in a cluster
        double u = static_cast<double>(m_currentProgressI + MathUtils::randomDouble()) / (frameBufferWidth - 1);
        double v = static_cast<double>(m_currentProgressJ + MathUtils::randomDouble()) / (frameBufferHeight - 1);
        Ray ray = camera.getRay(u, v);
        pixelColor += getRayColor(ray, world, m_maxRecursiveDepth);
    }

    // sample && gamma-correct(1/2)
    double sampleScale = 1.0 / m_sampleTimes;
    pixelColor[0] = std::clamp(pow(pixelColor.x() * sampleScale, 0.5), 0.0, 1.0);
    pixelColor[1] = std::clamp(pow(pixelColor.y() * sampleScale, 0.5), 0.0, 1.0);
    pixelColor[2] = std::clamp(pow(pixelColor.z() * sampleScale, 0.5), 0.0, 1.0);

    // size_t pixelIndex = (imageHeight - 1 - jj) * imageWidth + ii;
    int pixelIndex = frameBufferPixelNumber - (m_currentProgressJ + 1) * frameBufferWidth + m_currentProgressI;
    m_pFrameBuffer->fill(pixelIndex, pixelColor);

    printf("Fill color pixel placed at %d, progress = %d/%d.\n", static_cast<int>(pixelIndex), ++m_curPixelCount, frameBufferPixelNumber);

    return false;
}

Color ExampleWindows::getRayColor(const Ray& ray, const HittableList& world, int curDepth)
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