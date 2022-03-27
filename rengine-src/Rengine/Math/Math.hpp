#pragma once

#include <glm/glm.hpp>
#define M_PI 3.141592653589793f
#define EPS 1e-6

namespace Rengin
{
namespace Math
{
bool DecomposeTransform(const glm::mat4 &transform, glm::vec3 &outTranslation,
                        glm::vec3 outRotation, glm::vec3 outScale);

double clamp(double num,double l,double r);

double Factorial(int x);

double DoubleFactorial(int x);
} // namespace Math
    
} // namespace Rengin
