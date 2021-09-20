#pragma once

#include "Texture.h"
#include "XYZ.h"

#include <memory>

class ImageTexture : public Texture
{
public:
    ImageTexture(const char* fileName);

    virtual ~ImageTexture() = default;

    ImageTexture(const ImageTexture&) = delete;
    ImageTexture(ImageTexture&&) = delete;
    ImageTexture& operator=(const ImageTexture&) = delete;
    ImageTexture& operator=(ImageTexture&&) = delete;

    virtual Color value(double u, double v, const XYZ& p) const;

private:
    int m_imageWidth, m_imageHeight;
    unsigned char* m_imageData;
};