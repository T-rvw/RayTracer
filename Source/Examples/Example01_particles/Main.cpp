#include "Application.h"
#include "FrameBuffer.h"
#include "Timer.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

void drawLine(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int x1, int y1, int x2, int y2, const XYZ& pixelColor, uint8_t pixelAlpha);
void drawRect(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int x1, int y1, int x2, int y2, const XYZ& pixelColor, uint8_t pixelAlpha);
void drawCircle(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int ox, int oy, int r, const XYZ& pixelColor, uint8_t pixelAlpha);

constexpr int imageWidth = 800;
constexpr int imageHeight = 800;

class Particle
{
public:
    Particle() { reset(); }

    void setPos(float x, float y) { m_posX = x; m_posY = y; }
    void setSpeed(float sx, float sy) { m_speedX = sx; m_speedY = sy; }
    void setAcceleration(float ax, float ay) { m_accelerationX = ax; m_accelerationY = ay; }
    void setColor(const XYZ& color) { m_color = color; }
    void setAlpha(uint8_t alpha) { m_alpha = alpha; }

    void setLifeTime(float lifeTime) { m_lifeTime = lifeTime; }
    void active() { m_isActive = true; }
    bool isActive() const { return m_isActive; }

    void reset()
    {
        m_posX = 0.0f;
        m_posY = 0.0f;
        m_speedX = 0.0f;
        m_speedY = 0.0f;
        m_accelerationX = 0.0f;
        m_accelerationY = 0.0f;

        m_isActive = false;
        m_currentTime = 0.0f;
        m_lifeTime = 6.0f;

        m_color = XYZ{ 1.0f, 0.0f, 0.0f };
        m_alpha = 255;
    }

    void draw(FrameBuffer<PixelFormat::BGRA>& frameBuffer)
    {
        int leftTopX = static_cast<int>(m_posX - 1);
        int leftTopY = static_cast<int>(m_posY - 1);
        int rightBottomX = static_cast<int>(m_posX + 1);
        int rightBottomY = static_cast<int>(m_posY + 1);
        drawRect(frameBuffer, leftTopX, leftTopY, rightBottomX, rightBottomY, m_color, m_alpha);
    }

    void update(float deltaTime)
    {
        if(m_currentTime >= m_lifeTime)
        {
            m_isActive = false;
            return;
        }

        m_posX = m_posX + m_speedX * deltaTime + 0.5f * m_accelerationX * deltaTime * deltaTime;
        m_posY = m_posY + m_speedY * deltaTime + 0.5f * m_accelerationY * deltaTime * deltaTime;

        m_speedX += m_accelerationX * deltaTime;
        m_speedY += m_accelerationY * deltaTime;

        m_currentTime += deltaTime;
    }

private:
    float   m_posX;
    float   m_posY;
    float   m_speedX;
    float   m_speedY;
    float   m_accelerationX;
    float   m_accelerationY;

    bool    m_isActive;
    float   m_currentTime;
    float   m_lifeTime;

    XYZ     m_color;
    uint8_t m_alpha;
};

// Particle Memory Pool
constexpr int MaxParticleCount = 300;
class ParticlePool
{
public:
    ParticlePool() = delete;
    ParticlePool(FrameBuffer<PixelFormat::BGRA>* pFrameBuffer)
        : m_pRenderTarget(pFrameBuffer)
    {
        m_vecFreeParticleIndexes.clear();
        m_vecFreeParticleIndexes.reserve(MaxParticleCount);
    }
    ~ParticlePool() = default;

    int allocateParticleIndex()
    {
        int particelIndex = -1;
        if(!m_vecFreeParticleIndexes.empty())
        {
            int index = m_vecFreeParticleIndexes.back();
            m_vecFreeParticleIndexes.pop_back();
            particelIndex = index;
        }
        else
        {
            ++m_currentParticleCount;
            if (m_currentParticleCount >= MaxParticleCount)
            {
                m_currentParticleCount = 0;
            }

            if(m_particles[m_currentParticleCount].isActive())
            {
                // full
                particelIndex = -1;
            }
            else
            {
                particelIndex = m_currentParticleCount;
            }
        }

        if(particelIndex != -1)
        {
            m_particles[particelIndex].reset();
            m_particles[particelIndex].active();
        }

        return particelIndex;
    }

    Particle& getParticle(int index)
    {
        return m_particles[index];
    }

    int getParticleCount()
    {
        return m_currentActiveCount;
    }

    void draw()
    {
        for(int i = 0; i < MaxParticleCount; ++i)
        {
            if(m_particles[i].isActive())
            {
                m_particles[i].draw(*m_pRenderTarget);
            }
        }
    }

    void update(float deltaTime)
    {
        m_currentActiveCount = 0;
        for (int i = 0; i < MaxParticleCount; ++i)
        {
            if(!m_particles[i].isActive())
            {
                continue;
            }

            m_particles[i].update(deltaTime);
            if(!m_particles[i].isActive())
            {
                m_vecFreeParticleIndexes.push_back(i);
            }
            else
            {
                ++m_currentActiveCount;
            }
        }
    }

private:
    int                                 m_currentActiveCount = 0;
    FrameBuffer<PixelFormat::BGRA>*     m_pRenderTarget = nullptr;
    int                                 m_currentParticleCount = 0;
    Particle                            m_particles[MaxParticleCount];
    std::vector<int>                    m_vecFreeParticleIndexes;
};

int main()
{
    Color clearScreenColor(0, 0, 0);

    Application application(imageWidth, imageHeight, L"SoftRenderer");
    application.init();
    FrameBuffer<PixelFormat::BGRA> frameBuffer(application.getFrameBuffer(), application.getWidth(), application.getHeight());
    frameBuffer.clear(clearScreenColor);

    srand((unsigned)time(NULL));
    ParticlePool particlePool(&frameBuffer);

    Timer timer;
    timer.init();

    wchar_t pDebugFont[256];
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
        int particleIndex = particlePool.allocateParticleIndex();
        if(particleIndex != -1)
        {
            Particle& particle = particlePool.getParticle(particleIndex);
            particle.setPos(static_cast<float>(application.getMouseX()), static_cast<float>(application.getMouseY()));
            particle.setSpeed(-20.0f + rand() % 41, -20.0f + rand() % 10);
            particle.setAcceleration(0.0f, 20.0f);
            particle.setColor(XYZ(0.01 * (rand() % 101), 0.01 * (rand() % 101), 0.01 * (rand() % 101)));
            particle.setLifeTime(1.0f + rand() % 7);
        }

        particlePool.update(frameTime);
        particlePool.draw();

        // Present the frame buffer
        wsprintf(pDebugFont, L"Count = %d", particlePool.getParticleCount());
        application.setDebugOutput(pDebugFont);
        application.present();

        // Message
        application.processMessages();
    }

    return 0;
}

void drawLine(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int x1, int y1, int x2, int y2, const XYZ& pixelColor, uint8_t pixelAlpha)
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

void drawRect(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int x1, int y1, int x2, int y2, const XYZ& pixelColor, uint8_t pixelAlpha)
{
    for(int y = y1; y <= y2; ++y)
    {
        drawLine(frameBuffer, x1, y, x2, y, pixelColor, pixelAlpha);
    }
}

void drawCircle(FrameBuffer<PixelFormat::BGRA>& frameBuffer, int ox, int oy, int r, const XYZ& pixelColor, uint8_t pixelAlpha)
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