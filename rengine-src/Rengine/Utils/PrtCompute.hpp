#pragma once
#include "Rengine/Core/core.hpp"
#include <glm/glm.hpp>
#include <Eigen/Eigen>
#include <stb_image.h>

namespace Rengin
{

extern const Eigen::Vector3f cubemapFaceDirections[6][3];

float CalcArea(const float &u_, const float &v_, const int &width,const int &height);
double EvalSH(int l, int m, const Eigen::Vector3d& dir);

template <size_t SHOrder>
void PrecomputeCubemapSH(const std::string &path,glm::mat3& SHR,glm::mat3& SHG,glm::mat3& SHB)
{
    std::vector<Eigen::Vector3f> cubemapDirs;
    int height,width,channel;
    std::vector<stbi_uc*> images;
    char str[6][16] = {"negx.jpg","posx.jpg","negy.jpg","posy.jpg","negz.jpg","posz.jpg"};
    stbi_set_flip_vertically_on_load(0);
    for (int i = 0; i < 6; i++)
    {
        stbi_uc *data = nullptr;
        {
            RE_PROFILE_SCOPE(
                "stbi_load - OpenGLTextureCube::OpenGLTextureCube(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,const std::string&)");
            data = stbi_load((path + str[i]).c_str(), &width, &height, &channel, 0);
        }
        RE_CORE_ASSERT(data, "fail to load image!");
        images.push_back(data);
    }
    
    cubemapDirs.reserve(6 * width * height);
    for (int i = 0; i < 6; i++)
    {
        Eigen::Vector3f faceDirX = cubemapFaceDirections[i][0];
        Eigen::Vector3f faceDirY = cubemapFaceDirections[i][1];
        Eigen::Vector3f faceDirZ = cubemapFaceDirections[i][2];
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                float u = 2 * ((x + 0.5) / width) - 1;
                float v = 2 * ((y + 0.5) / height) - 1;
                Eigen::Vector3f dir = (faceDirX * u + faceDirY * v + faceDirZ).normalized();
                cubemapDirs.push_back(dir);
            }
        }
    }
    constexpr int SHNum = (SHOrder + 1) * (SHOrder + 1);
    std::vector<Eigen::Array3f> SHCoeffiecents(SHNum);
    for (int i = 0; i < SHNum; i++)
        SHCoeffiecents[i] = Eigen::Array3f(0);
    float sumWeight = 0;
    for (int i = 0; i < 6; i++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                Eigen::Vector3f dir = cubemapDirs[i * width * height + y * width + x];
                int index = (y * width + x) * channel;
                Eigen::Array3f Le(images[i][index + 0] / 256.f, images[i][index + 1] / 256.f,images[i][index + 2] / 256.f);
                
                int k = 0;
                for (int l = 0; l < SHOrder + 1; l++)
                {
                    for (int m = -l; m < l + 1; m++)
                    {
                        SHCoeffiecents[k++] += Le * EvalSH(l,m,dir.cast<double>()) * CalcArea(x,y,width,height);
                    }
                }
            }
        }
    }
    for (int i = 0; i < 6; i++)
    {
        stbi_image_free(images[i]);
    }
    int idx = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            SHR[i][j] = SHCoeffiecents[idx][0];
            SHG[i][j] = SHCoeffiecents[idx][1];
            SHB[i][j] = SHCoeffiecents[idx++][2];
        }
    }
}

std::vector<double> ProjectFunction(int order, const std::function<double(double, double)>& func, int sample_count);

} // namespace Rengin
