#include "Application.h"
#include "FrameBuffer.h"
#include "Timer.h"

#include <iostream>

void drawLine(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int x1, int y1, int x2, int y2);
void drawCircle(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int ox, int oy, int r);
void drawOval(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int ox, int oy, int a, int b);
void drawHeart(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int ox, int oy, int r);

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

//画V字形  V形状的线的公式是： y = a*x
void drawMovingVLine(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    for (int x = 0; x < imageWidth; ++x)
    {
        int y = pixelPosY - abs(x - pixelPosX);
        frameBuffer.fill(x, y, pixelColor, pixelAlpha);
    }
}

//画抛物线  抛物线的公式是： y = a*x^2 + b*x + c
void drawMovingParabola(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    for (int x = 0; x < imageWidth; ++x)
    {
        int y = pixelPosY - (int)(pow(abs(x - pixelPosX), 2) / 200);
        frameBuffer.fill(x, y, pixelColor, pixelAlpha);
    }
}

//画正弦曲线   正弦曲线的公式是： y = sin(x)
void drawMovingSinusoidal(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    for (int x = 0; x < imageWidth; ++x)
    {
        int y = pixelPosY - (int)(sin(0.1 * abs(x - pixelPosX)) * 12);
        frameBuffer.fill(x, y, pixelColor, pixelAlpha);
    }
}

//画椭圆   椭圆的公式是：  x^2/a^2 + y^2/b^2 = 1  另外如果a==b，就是圆了
void drawMovingOval(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    drawOval(frameBuffer, pixelPosX, pixelPosY, 50, 80);
}

//画心形线  
void drawMovingHeart(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
{
    drawHeart(frameBuffer, pixelPosX, pixelPosY, 2000);
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
        //drawMovingPixel(frameBuffer);
        
        drawMovingVLine(frameBuffer);
        drawMovingParabola(frameBuffer);
        drawMovingSinusoidal(frameBuffer);
        drawMovingOval(frameBuffer);
        drawMovingHeart(frameBuffer);

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


//画椭圆，如果a==b，就是圆
void drawOval(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int ox, int oy, int a, int b)
{
    //椭圆公式： x^2/a^2 + y^2/b^2 = 1

    //沿x轴画一遍
    for (int x = ox - a; x < ox + a; ++x)
    {
        int y = (int)(sqrt(1 - pow(abs(x - pixelPosX), 2) / pow(a, 2)) * b);
        int y1 = oy + y;
        int y2 = oy - y;
        frameBuffer.fill(x, y1, pixelColor, pixelAlpha);
        frameBuffer.fill(x, y2, pixelColor, pixelAlpha);
    }

    //沿y轴方向再画一遍，可以弥补因强制类型转换的精度缺失而造成的像素缺失
    for (int y = oy - b; y < oy + b; ++y)
    {
        int x = (int)(sqrt(1 - pow(abs(y - pixelPosY), 2) / pow(b, 2)) * a);
        int x1 = ox + x;
        int x2 = ox - x;
        frameBuffer.fill(x1, y, pixelColor, pixelAlpha);
        frameBuffer.fill(x2, y, pixelColor, pixelAlpha);
    }
}

//画心形线
void drawHeart(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int ox, int oy, int r)
{
    //心形线直角坐标系下的心形线： x^2 -|x|y + y^2 = r

    //沿x轴画一遍
    for (int x = 0; x < imageWidth; ++x)
    {
        int y1 = (int)(abs(x - pixelPosX) / 2 + sqrt(r - pow(x - pixelPosX, 2) * 3 / 4));
        int y2 = (int)(abs(x - pixelPosX) / 2 - sqrt(r - pow(x - pixelPosX, 2) * 3 / 4));

        frameBuffer.fill(x, oy - y1, pixelColor, pixelAlpha);
        frameBuffer.fill(x, oy - y2, pixelColor, pixelAlpha);
    }

    //沿y轴方向再画一遍，可以弥补因强制类型转换的精度缺失而造成的像素缺失
    for (int y = 0; y < imageHeight; ++y)
    {
        int x = (int)(-(y - pixelPosY) / 2 + sqrt(r - pow(y - pixelPosY, 2) * 3 / 4));
        if (x >= 0)
        {
            frameBuffer.fill(ox + x, y, pixelColor, pixelAlpha);
            frameBuffer.fill(ox - x, y, pixelColor, pixelAlpha);
        }
    }
}