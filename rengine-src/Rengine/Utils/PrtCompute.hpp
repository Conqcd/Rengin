#pragma once
#include "Rengine/Core/core.hpp"
#include <glm/glm.hpp>
#include <Eigen/Eigen>

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
    std::vector<float*> images;
    for (int i = 0; i < 6; i++)
    {

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
                Eigen::Array3f Le(images[i][index + 0], images[i][index + 1],
                                    images[i][index + 2]);
                
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
}

} // namespace Rengin
