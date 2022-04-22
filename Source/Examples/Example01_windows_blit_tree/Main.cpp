#include "Application.h"
#include "FrameBuffer.h"
#include "Timer.h"
#include <iostream> 
#include <math.h>
#include <thread>
#include <chrono>

void drawTree();
void drawBranch(int treeLevel, int rootX, int rootY, int branchWidth, int branchHeight, int degree);

constexpr double pi = 3.14159265f;
constexpr int imageWidth = 800;
constexpr int imageHeight = 800;
constexpr int pixelNumber = imageHeight * imageWidth;
FrameBuffer<PixelFormat::BGRA> frameBuffer(nullptr, imageWidth, imageHeight);

static XYZ pixelColor(255, 0, 0);
static uint8_t pixelAlpha = 255;
static int pixelPosX = 400;
static int pixelPosY = 400;

static XYZ truckColor(165, 42, 0); //the color of the truck and branches
static XYZ leafColor(0, 255, 0); //the color of the leaves
constexpr int truckWidth = 28; //the width of the truck
constexpr int truckHeight = 200; //the height of the truck
constexpr int truckPosX = imageWidth/2;  //the X of the top middle of the truck
constexpr int truckPosY = imageHeight-truckHeight;  //the Y of the top middle of the truck
constexpr float branchWidthRatio = 0.55f;  //the ratio of the thickness of the brances in n+1 layer to n layer
constexpr float branchHeightRatio = 0.75f; //the ratio of the height of the brances in n+1 layer to n layer
constexpr int branchDegree = 19; //the angle between the branch at n+1 layer and n layer.
constexpr int HighestLevel = 7; //the highest layer of the tree
constexpr bool isAnimationPlay = true; //whether to play the animation of the tree.

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


void drawTree()
{
    //draw the truck
    for (int x = -truckWidth/2; x < truckWidth/2; ++x)
    {
        for (int y = 0; y < truckHeight; ++y)
        {
            frameBuffer.fill(truckPosX+x, truckPosY+y, truckColor, pixelAlpha);
        }
    }
    
    //draw the branches and leaves
    int l_treeLevel = 0; //the level of the tree
    int l_rootX = truckPosX; //the X of the root of the current branch
    int l_rootY = truckPosY; //the Y of the root of the current branch
    int l_branchWidth = (int)(truckWidth * branchWidthRatio); 
    int l_branchHeight = (int)(truckHeight * branchHeightRatio); 
    int l_branchDegree = branchDegree; 

    drawBranch(l_treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, l_branchDegree);
    drawBranch(l_treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, -l_branchDegree);
}

void drawBranch(int treeLevel, int rootX, int rootY, int branchWidth, int branchHeight, int degree)
{
    treeLevel++;
    if (treeLevel > HighestLevel) return;
    if constexpr(isAnimationPlay) std::this_thread::sleep_for(std::chrono::milliseconds(500));

    double l_180 = 1/180.0;
    double l_sin = sin(degree * pi * l_180);
    double l_cos = cos(degree * pi * l_180);
    double l_tan_1 = tan((90 + degree) * pi * l_180);
    double l_tan_2 = tan((degree ) * pi * l_180);
    double l_r_sin = 1 / l_sin;

    for (int x = 0; x < imageWidth; ++x)
    {
        /*the four sides of the rectangle of the branch*/
        int y_1 = (int)(l_tan_1 * (x - rootX - branchWidth * 0.5) + rootY);
        int y_2 = (int)(l_tan_1 * (x - rootX + branchWidth * 0.5) + rootY);
        int y_3 = (int)(l_tan_2 * (x - rootX) + rootY);
        int y_4 = (int)(l_tan_2 * (x - rootX - branchHeight * l_r_sin) + rootY);

        XYZ l_color = XYZ(0, 0, 0);
        if (treeLevel >= HighestLevel)
        {
            //draw leaves
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
            //draw branches
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
    int l_branchWidth = (int)(branchWidth * branchWidthRatio);
    int l_branchHeight = (int)(branchHeight * branchHeightRatio);
    int l_branchDegree = branchDegree* (treeLevel+1);
    
    drawBranch(treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, l_branchDegree);
    drawBranch(treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, -l_branchDegree);
}