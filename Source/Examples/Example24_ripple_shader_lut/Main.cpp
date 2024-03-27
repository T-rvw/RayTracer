//////////////////////////////////////////////////////////////////////////////
// Generate LUT for nested for loop to optimize performance
// https://www.shadertoy.com/view/ldfyzl
//////////////////////////////////////////////////////////////////////////////

#include "ImageExporter.h"
#include "Math/XY.h"

#include <iostream>

namespace
{

double fract(double x)
{
    return x - floor(x);
}

XY fract(const XY& xy)
{
    return XY(fract(xy.x()), fract(xy.y()));
}

XYZ fract(const XYZ& xyz)
{
    return XYZ(fract(xyz.x()), fract(xyz.y()), fract(xyz.z()));
}

double hash12(const XY& p)
{
    constexpr double HASHSCALE1 = 0.1031;

    XYZ p3 = fract(XYZ(p.x(), p.y(), p.x()) * HASHSCALE1);
    p3 += XYZ::dot(p3, XYZ(p3.y() + 19.19, p3.z() + 19.19, p3.x() + 19.19));
    return fract((p3.x() + p3.y()) * p3.z());
}

XY hash22(const XY& p)
{
    static XYZ HASHSCALE3(0.1031, 0.1030, 0.0973);

    XYZ p3 = fract(XYZ(p.x(), p.y(), p.x()) * HASHSCALE3);
    p3 += XYZ::dot(p3, XYZ(p3.y() + 19.19, p3.z() + 19.19, p3.x() + 19.19));
    return fract((XY(p3.x()) + XY(p3.y(), p3.z())) * p3.z());
}

double clamp(double x, double minVal, double maxVal)
{
    return std::min(std::max(x, minVal), maxVal);
}

double smoothstep(double edge0, double edge1, double x)
{
    double t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

}

int main()
{
    // Maximum number of cells a ripple can cross.
    //constexpr int MAX_RADIUS = 2;
    constexpr int imageWidth = 400;
    constexpr int imageHeight = 400;
    constexpr int pixelNumber = imageHeight * imageWidth;
    FrameBuffer<PixelFormat::RGBA> frameBuffer(imageWidth, imageHeight);

    int curPixelCount = 0;
#pragma omp parallel for
    //for (double time = 0.0; time < 0.1; time += 0.1)
    {
        for (int w = 0; w < imageWidth; ++w)
        {
            for (int h = 0; h < imageHeight; ++h)
            {
                XY uv(static_cast<double>(w) / imageWidth, static_cast<double>(h) / imageHeight);
                XY p0(floor(uv.x()), floor(uv.y()));
                
                //XY circles(0.0);
                //for (int j = -MAX_RADIUS; j <= MAX_RADIUS; ++j)
                //{
                //    for (int i = -MAX_RADIUS; i <= MAX_RADIUS; ++i)
                //    {
                //        XY pi = p0 + XY(i, j);
                //        XY hsh(0.1);// hash22(pi);
                //        XY p = pi + 0.1;// hash22(hsh);
                //
                //        double t = 0.5;//fract(0.3 + hash12(hsh));
                //        XY v = p - uv;
                //        double d = v.length() - (MAX_RADIUS + 1.0) * t;
                //        double h0 = 0.001;
                //        double d1 = d - h0;
                //        double d2 = d + h0;
                //        double p1 = sin(31.0 * d1);
                //        double p2 = sin(31.0 * d2);
                //        circles += 0.5 * v.normalize() * ((p2 - p1) / (2. * h0) * (1. - t) * (1. - t));
                //    }
                //}
                //circles /= ((MAX_RADIUS * 2.0 + 1.0) * (MAX_RADIUS * 2.0 + 1.0));

                XYZ color(p0.x(), 1.0 - p0.y(), 0.0);
                int pixelIndex = frameBuffer.fill(w, h, color);
                printf("Fill color pixel placed at %d, progress = %d/%d\n", static_cast<int>(pixelIndex), ++curPixelCount, pixelNumber);
            }
        }
    }

    
    if (ImageExporter::generate(frameBuffer, "test.png"))
    {
        std::cout << "Succeed to generate image." << std::endl;
    }
    else
    {
        std::cout << "Failed to generate image." << std::endl;
    }

    return 0;
}