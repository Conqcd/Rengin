#pragma once
#include "Texture.hpp"


namespace Rengin
{
class SubTexture2D : public Texture2D
{
private:
    /* data */
    void inmt();
public:
    SubTexture2D(/* args */);
    ~SubTexture2D();
};

SubTexture2D::SubTexture2D(/* args */)
{
}

SubTexture2D::~SubTexture2D()
{
}

} // namespace Rengin
