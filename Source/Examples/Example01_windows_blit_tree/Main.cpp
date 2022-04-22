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

static XYZ truckColor(165, 42, 0); //������֦����ɫ
static XYZ leafColor(0, 255, 0); //��Ҷ����ɫ
constexpr int truckWidth = 28; //���ɵĿ��
constexpr int truckHeight = 200; //���ɵĸ߶�
constexpr int truckPosX = imageWidth/2;  //����������Ĵ�������X(��1����辿��洦)
constexpr int truckPosY = imageHeight-truckHeight;  //����������Ĵ�������Y(��1����辿��洦)
constexpr float branchWidthRatio = 0.55f;  //n+1�����֦��n�����֦�ֶȵı�ֵ����֦Խ����Խϸ
constexpr float branchHeightRatio = 0.75f; //n+1�����֦��n�����֦���ȵı�ֵ����֦Խ����Խ��
constexpr int branchDegree = 19; //n+1�����֦��n�����֦�ļн�
constexpr int HighestLevel = 7; //������ܳɳ����ٲ�

bool isAnimationPlay = true; //�Ƿ񲥷����Ķ�����true�����Ŷ�����false��ֱ�����ɽ��
constexpr float aTimeInterval = 0.5f; //���Ŷ���ʱ��ÿ����һ����֦��ʱ����
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

//�ݹ������
void drawTree()
{
    //��������
    for (int x = -truckWidth/2; x < truckWidth/2; ++x)
    {
        for (int y = 0; y < truckHeight; ++y)
        {
            frameBuffer.fill(truckPosX+x, truckPosY+y, truckColor, pixelAlpha);
        }
    }
    
    //������辺���Ҷ
    int l_treeLevel = 0; //���Ĳ�����������0�㣬ÿ��һ�β��������һ��
    int l_rootX = truckPosX; //ÿһ����辵ĸ��ڵ�
    int l_rootY = truckPosY; //ÿһ����辵ĸ��ڵ�
    int l_branchWidth = (int)(truckWidth * branchWidthRatio); //��辵Ĵֶȣ�Խ���ϵ����Խϸ
    int l_branchHeight = (int)(truckHeight * branchHeightRatio); //��辵ĳ��ȣ�Խ���ϵ����Խ��
    int l_branchDegree = branchDegree; //ÿһ����辷ֲ�ĽǶ�

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
        /*����������֦�ĳ����ε������ߣ�screen�е����ص�yֵ������������
        ֱ��֮������أ����ڳ������ڣ�������ڻ�����֦*/
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
            //�Ѵﵽ��߲㣬��˻���Ҷ����һ������������Ҷ
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
            //��δ�ﵽ��߲㣬��˻���֦
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
    int l_branchWidth = (int)(branchWidth * branchWidthRatio); //��辵Ĵֶȣ�Խ���ϵ����Խϸ
    int l_branchHeight = (int)(branchHeight * branchHeightRatio); //��辵ĳ��ȣ�Խ���ϵ����Խ��
    int l_branchDegree = branchDegree* (treeLevel+1);
    
    drawBranch(treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, l_branchDegree);
    drawBranch(treeLevel, l_rootX, l_rootY, l_branchWidth, l_branchHeight, -l_branchDegree);
}