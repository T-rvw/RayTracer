#include "Application.h"
#include "FrameBuffer.h"

#include <iostream>

constexpr int imageWidth = 800;
constexpr int imageHeight = 800;
constexpr int pixelNumber = imageHeight * imageWidth;

static int pixelPosX = 0;
static int pixelPosY = 0;
void updateFrameBuffer(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    frameBuffer.fill(pixelPosX, pixelPosY, XYZ(255, 0, 0));

    ++pixelPosX;
    if (pixelPosX >= imageWidth) pixelPosX = 0;

    ++pixelPosY;
    if (pixelPosY >= imageHeight) pixelPosY = 0;
}

int main()
{
    Color clearScreenColor(0, 0, 0);

    Application application(imageWidth, imageHeight, L"SoftRenderer");
    application.init();
    FrameBuffer<PixelFormat::BGRA> frameBuffer(application.getFrameBuffer(), application.getWidth(), application.getHeight());

    while (!application.isClosed())
    {
        frameBuffer.clear(clearScreenColor);
        updateFrameBuffer(frameBuffer);
        application.present();

        application.processMessages();
    }

    return 0;
}