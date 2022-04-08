#include "Application.h"
#include "FrameBuffer.h"
#include "Timer.h"

#include <iostream>

void drawLine(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int x1, int y1, int x2, int y2);
void drawCircle(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int ox, int oy, int r);

constexpr int imageWidth = 800;
constexpr int imageHeight = 800;
constexpr int pixelNumber = imageHeight * imageWidth;

static XYZ pixelColor(255, 0, 0);
static uint8_t pixelAlpha = 255;
static int pixelPosX = 0;
static int pixelPosY = 0;

void updateDrawVariables()
{
    ++pixelPosX;
    if (pixelPosX >= imageWidth) pixelPosX = 0;

    ++pixelPosY;
    if (pixelPosY >= imageHeight) pixelPosY = 0;
}

void drawMovingPixel(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    frameBuffer.fill(pixelPosX, pixelPosY, pixelColor, pixelAlpha);
}

void drawMovingStraightLine(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    for (int x = 0; x < imageWidth; ++x)
    {
        frameBuffer.fill(x, pixelPosY, pixelColor, pixelAlpha);
    }
    
    for (int y = 0; y < imageWidth; ++y)
    {
        frameBuffer.fill(pixelPosX, y, pixelColor, pixelAlpha);
    }
}

void drawMovingLine(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    drawLine(frameBuffer, pixelPosX, pixelPosY, 0, imageHeight);
    drawLine(frameBuffer, pixelPosX, pixelPosY, imageWidth, 0);
}

void drawMovingSphere(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    drawCircle(frameBuffer, pixelPosX, pixelPosY, 50);
}

int main()
{
    Color clearScreenColor(0, 0, 0);

    Application application(imageWidth, imageHeight, L"SoftRenderer");
    application.init();
    FrameBuffer<PixelFormat::BGRA> frameBuffer(application.getFrameBuffer(), application.getWidth(), application.getHeight());
    frameBuffer.clear(clearScreenColor);

    Timer timer;
    timer.init();

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

        // Clear screen
        frameBuffer.clear(clearScreenColor);

        // Draw
        drawMovingPixel(frameBuffer);
        updateDrawVariables();

        // Present the frame buffer
        application.present();

        // Message
        application.processMessages();
    }

    return 0;
}

void drawLine(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int x1, int y1, int x2, int y2)
{
    float k = 1.0f * (y2 - y1) / (x2 - x1);
    int flag = 0;
    if (k > 1 || k < -1)
    {
        flag = 1;
        x1 ^= y1 ^= x1 ^= y1;
        x2 ^= y2 ^= x2 ^= y2;
        k = 1.0f * (y2 - y1) / (x2 - x1);
    }
    float d = 0.5f - k;
    if (x1 > x2)
    {
        x1 ^= x2 ^= x1 ^= x2;
        y1 ^= y2 ^= y1 ^= y2;
    }
    while (x1 != x2)
    {
        if (k > 0.0f && d < 0.0f)
            ++y1, ++d;
        else if (k < 0.0f && d > 0.0f)
            --y1, --d;
        d -= k;
        ++x1;
        if (flag) frameBuffer.fill(y1, x1, pixelColor, pixelAlpha);
        else frameBuffer.fill(x1, y1, pixelColor, pixelAlpha);
    }
}

void drawCircle(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int ox, int oy, int r)
{
    float d = 1.25f - r;
    int x = 0, y = (int)r, fx = (int)(r / 1.4);
    while (x != fx)
    {
        if (d < 0)
            d += 2 * x + 3;
        else
        {
            d += 2 * (x - y) + 5;
            --y;
        }
        frameBuffer.fill(ox + x, oy + y, pixelColor, pixelAlpha);
        frameBuffer.fill(ox + x, oy - y, pixelColor, pixelAlpha);
        frameBuffer.fill(ox - x, oy + y, pixelColor, pixelAlpha);
        frameBuffer.fill(ox - x, oy - y, pixelColor, pixelAlpha);
        frameBuffer.fill(ox + y, oy - x, pixelColor, pixelAlpha);
        frameBuffer.fill(ox + y, oy + x, pixelColor, pixelAlpha);
        frameBuffer.fill(ox - y, oy + x, pixelColor, pixelAlpha);
        frameBuffer.fill(ox - y, oy - x, pixelColor, pixelAlpha);
        ++x;
    }
}