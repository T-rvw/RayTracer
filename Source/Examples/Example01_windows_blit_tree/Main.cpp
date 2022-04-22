#include "Application.h"
#include "FrameBuffer.h"
#include "Timer.h"
#include <iostream> 
#include <math.h>
#include <thread>

#define PI 3.14159265

void drawTree();
void drawBranch(int treeLevel,int rootX, int rootY, int branchWidth, int branchHeight, int degree);

void renderMain();


constexpr int imageWidth = 800;
constexpr int imageHeight = 800;
constexpr int pixelNumber = imageHeight * imageWidth;
FrameBuffer<PixelFormat::BGRA> frameBuffer(nullptr, imageWidth, imageHeight);

static XYZ pixelColor(255, 0, 0);
static uint8_t pixelAlpha = 255;
static int pixelPosX = 400;
static int pixelPosY = 400;

static XYZ truckColor(165, 42, 0); //树干树枝的颜色
static XYZ leafColor(0, 255, 0); //树叶的颜色
constexpr int truckWidth = 28; //树干的宽度
constexpr int truckHeight = 200; //树干的高度
constexpr int truckPosX = imageWidth/2;  //树干最顶端中心处的坐标X(第1层树杈开叉处)
constexpr int truckPosY = imageHeight-truckHeight;  //树干最顶端中心处的坐标Y(第1层树杈开叉处)
constexpr float branchWidthRatio = 0.55f;  //n+1层的树枝和n层的树枝粗度的比值，树枝越往上越细
constexpr float branchHeightRatio = 0.75f; //n+1层的树枝和n层的树枝长度的比值，树枝越往上越短
constexpr int branchDegree = 19; //n+1层的树枝和n层的树枝的夹角
constexpr int HighestLevel = 7; //树最高能成长多少层

bool isAnimationPlay = true; //是否播放树的动画。true：播放动画，false：直接生成结果
constexpr float aTimeInterval = 0.5f; //播放动画时，每生成一个树枝的时间间隔
Timer aTimer;

int main()
{
    Color clearScreenColor(0, 0, 0);

    Application application(imageWidth, imageHeight, L"SoftRenderer");
    application.init();
    frameBuffer.setData(application.getFrameBuffer());
    frameBuffer.clear(clearScreenColor);

    Timer timer;
    timer.init();

    std::thread renderThread = std::thread(drawTree);
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

//递归分形树
void drawTree()
{
    //绘制树干
    for (int x = -truckWidth/2; x < truckWidth/2; ++x)
    {
        for (int y = 0; y < truckHeight; ++y)
        {
            frameBuffer.fill(truckPosX+x, truckPosY+y, truckColor, pixelAlpha);
        }
    }
    
    //绘制树杈和树叶
    int l_treeLevel = 0; //树的层数，树干是0层，每分一次叉层数增加一层
    int l_rootX = truckPosX; //每一层树杈的根节点
    int l_rootY = truckPosY; //每一层树杈的根节点
    int l_branchWidth = (int)(truckWidth * branchWidthRatio); //树杈的粗度，越往上的树杈越细
    int l_branchHeight = (int)(truckHeight * branchHeightRatio); //树杈的长度，越往上的树杈越短
    int l_branchDegree = branchDegree; //每一层树杈分叉的角度

    aTimer.init();

    drawBranch(l_treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, l_branchDegree);
    drawBranch(l_treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, -l_branchDegree);
}

void drawBranch(int treeLevel, int rootX, int rootY, int branchWidth, int branchHeight, int degree)
{
    treeLevel++;
    if (treeLevel > HighestLevel) return;

    if (isAnimationPlay==true)
    {
        float aTime = 0.0f;
        float deltaTime = aTimer.tick();

        bool isFinished = false;
        while (!isFinished)
        {
            deltaTime = aTimer.tick();
            if (aTime < aTimeInterval)
            {
                aTime += deltaTime;
                isFinished = false;
            }
            else
            {
                aTime = 0.0f;
                isFinished = true;
            }
        }
    }

    double l_180 = 1/180.0;
    double l_sin = sin(degree * PI * l_180);
    double l_cos = cos(degree * PI * l_180);
    double l_tan_1 = tan((90 + degree) * PI * l_180);
    double l_tan_2 = tan((degree)*PI * l_180);
    double l_r_sin = 1 / l_sin;

    for (int x = 0; x < imageWidth; ++x)
    {
        /*这四条是树枝的长方形的四条边，screen中的像素的y值，介于这两组
        直线之间的像素，皆在长方形内，因此用于绘制树枝*/
        int y_1 = (int)(l_tan_1 * (x - rootX - branchWidth*0.5) + rootY);
        int y_2 = (int)(l_tan_1 * (x - rootX + branchWidth*0.5) + rootY);
        int y_3 = (int)(l_tan_2 * (x - rootX) + rootY);
        int y_4 = (int)(l_tan_2 * (x - rootX - branchHeight*l_r_sin) + rootY);

        /*
        frameBuffer.fill(x, y_1, pixelColor, pixelAlpha);
        frameBuffer.fill(x, y_2, pixelColor, pixelAlpha);
        frameBuffer.fill(x, y_3, pixelColor, pixelAlpha);
        frameBuffer.fill(x, y_4, pixelColor, pixelAlpha);
        */

        XYZ l_color = XYZ(0, 0, 0);
        if (treeLevel >= HighestLevel)
        {
            //已达到最高层，因此画树叶，用一个菱形来画树叶
            l_color = XYZ(leafColor.x(), leafColor.y(), leafColor.z());
         
            y_1 = (int)(-1.73f * (x - rootX ) + rootY);
            y_2 = (int)(-1.73f * (x - rootX -10) + rootY + 5);
            y_3 = (int)(1.73f * (x - rootX) + rootY);
            y_4 = (int)(1.73f * (x - rootX +10) + rootY + 5);

            for (int y = 0; y < imageHeight; ++y)
            {
                if ((y >= y_1 && y <= y_2) || (y >= y_2 && y <= y_1))
                {
                    if ((y >= y_3 && y <= y_4) || (y >= y_4 && y <= y_3))
                    {
                        frameBuffer.fill(x, y, l_color, pixelAlpha);
                    }
                }
            }
        }
        else if (treeLevel < HighestLevel)
        {
            //尚未达到最高层，因此画树枝
            l_color = XYZ(truckColor.x(), truckColor.y(), truckColor.z());

            for (int y = 0; y < imageHeight; ++y)
            {
                if ((y >= y_1 && y <= y_2) || (y >= y_2 && y <= y_1))
                {
                    if ((y >= y_3 && y <= y_4) || (y >= y_4 && y <= y_3))
                    {
                        frameBuffer.fill(x, y, l_color, pixelAlpha);
                    }
                }
            }
        }
    }
     
    int l_rootX = (int)(rootX + l_sin * branchHeight);
    int l_rootY = (int)(rootY - l_cos * branchHeight); 
    int l_branchWidth = (int)(branchWidth * branchWidthRatio); //树杈的粗度，越往上的树杈越细
    int l_branchHeight = (int)(branchHeight * branchHeightRatio); //树杈的长度，越往上的树杈越短
    int l_branchDegree = branchDegree* (treeLevel+1);
    
    drawBranch(treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, l_branchDegree);
    drawBranch(treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, -l_branchDegree);
}