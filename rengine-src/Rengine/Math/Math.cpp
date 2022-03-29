#include "repch.hpp"
#include "Math.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Rengin::Math
{
bool DecomposeTransform(const glm::mat4& transform,glm::vec3& outTranslation,glm::vec3 outRotation,glm::vec3 outScale)
{
    using namespace glm;
    using T = float;
    mat4 LocalMatrix(transform);

    if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
      return false;

    // First, isolate perspective.  This is the messiest.
    if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())) {
      // Clear the perspective partition
      LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] =
          static_cast<T>(0);
      LocalMatrix[3][3] = static_cast<T>(1);
    }
    // Next take care of translation (easy).
    outTranslation = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

    vec3 Row[3], Pdum3;

    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
      for (length_t j = 0; j < 3; ++j)
        Row[i][j] = LocalMatrix[i][j];

    // Compute X scale factor and normalize first row.
    outScale.x = length(Row[0]); // v3Length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));
    outScale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    outScale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
#if 0
    Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
    if (dot(Row[0], Pdum3) < 0) {
      for (length_t i = 0; i < 3; i++) {
        Scale[i] *= static_cast<T>(-1);
        Row[i] *= static_cast<T>(-1);
      }
    }
#endif

    outRotation.y = asin(-Row[0][2]);
    if (cos(outRotation.y) != 0) {
        outRotation.x = atan2(Row[1][2], Row[2][2]);
        outRotation.z = atan2(Row[0][1], Row[0][0]);
    } else {
        outRotation.x = atan2(-Row[2][0], Row[1][1]);
        outRotation.z = 0;
    }
    return true;
}

double clamp(double num,double l,double r)
{
    if(num < l) return l;
    if(num > r) return r;
    return num;
}

static const int kCacheSize = 16;

double Factorial(int x) {
    static const double factorial_cache[kCacheSize] = {1, 1, 2, 6, 24, 120, 720, 5040,
                                                40320, 362880, 3628800, 39916800,
                                                479001600, 6227020800,
                                                87178291200, 1307674368000};

    if (x < kCacheSize) {
        return factorial_cache[x];
    } else {
        double s = factorial_cache[kCacheSize - 1];
        for (int n = kCacheSize; n <= x; n++) {
            s *= n;
        }
        return s;
    }
}

double DoubleFactorial(int x) 
{
    static const double dbl_factorial_cache[kCacheSize] = {1, 1, 2, 3, 8, 15, 48, 105,
                                                    384, 945, 3840, 10395, 46080,
                                                    135135, 645120, 2027025};

    if (x < kCacheSize) {
        return dbl_factorial_cache[x];
    } else {
        double s = dbl_factorial_cache[kCacheSize - (x % 2 == 0 ? 2 : 1)];
        double n = x;
        while (n >= kCacheSize) {
            s *= n;
            n -= 2.0;
        }
        return s;
    }
}

Eigen::Vector3d ToVector(double phi, double theta) {
  double r = sin(theta);
  return Eigen::Vector3d(r * cos(phi), r * sin(phi), cos(theta));
}

} // namespace Rengin::Math
