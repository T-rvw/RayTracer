#include "Application.h"
#include "Camera.h"
#include "ExampleWindows.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Sphere.h"
#include "Timer.h"

#include <thread>

constexpr int imageWidth = 400;
constexpr int imageHeight = 225;
constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);

FrameBuffer<PixelFormat::BGRA> frameBuffer(nullptr, imageWidth, imageHeight);
Color clearScreenColor(0, 0, 0);

void renderMain()
{
    // World
    auto groudMaterial = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto centerMaterial = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto leftMaterial = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
    auto rightMaterial = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    HittableList hittableList;
    hittableList.reserve(4);
    hittableList.add(std::make_shared<Sphere>(XYZ(+0.0, -100.5, -1.0), 100.0)).setMaterial(groudMaterial);
    hittableList.add(std::make_shared<Sphere>(XYZ(+0.0, 0.0, -1.0), 0.5)).setMaterial(centerMaterial);
    hittableList.add(std::make_shared<Sphere>(XYZ(-1.2, 0.0, -1.0), 0.5)).setMaterial(leftMaterial);
    hittableList.add(std::make_shared<Sphere>(XYZ(+1.2, 0.0, -1.0), 0.5)).setMaterial(rightMaterial);

    // Camera
    Camera camera(90.0, aspectRatio);

    // Init example and run
    ExampleWindows example(&frameBuffer);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);

    bool isFinished = false;
    while (!isFinished)
    {
        isFinished = example.process(camera, hittableList);
    }
}

int main()
{
    // Main
    Application application(imageWidth, imageHeight, L"SoftRenderer");
    application.init();
    frameBuffer.setData(application.getFrameBuffer());
    frameBuffer.clear(clearScreenColor);
    
    // Init start time point
    Timer timer;
    timer.init();
    
    // RayTracer
    std::thread renderThread = std::thread(renderMain);
    renderThread.detach();

    constexpr float frameTime = 1.0f / 60.0f;
    float totalTime = 0.0f;
    while (true)
    {
        // Timer
        float deltaTime = timer.tick();
        if (totalTime < frameTime)
        {
            totalTime += deltaTime;
            continue;
        }
        else
        {
            totalTime = 0.0f;
        }
    
        // Present the frame buffer
        application.present();
    
        // Message
        application.processMessages();
    }

    return 0;
}