#include "repch.hpp"
#include "Math.hpp"

namespace Rengin::Math
{
bool DecomposeTransform(const glm::mat4& transform,glm::vec3& outTranslation,glm::vec3 outRotation,glm::vec3 outScale)
{
    using namespace glm;
    using T = float;
    mat4 LocalMatrix(transform);

    if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
      return false;

    for (length_t i = 0; i < 4; ++i)
      for (length_t j = 0; j < 4; ++j)
        LocalMatrix[i][j] /= LocalMatrix[3][3];

    mat4 PerspectiveMatrix(LocalMatrix);

    for (length_t i = 0; i < 3; i++)
      PerspectiveMatrix[i][3] = static_cast<T>(0);
    PerspectiveMatrix[3][3] = static_cast<T>(1);

    /// TODO: Fixme!
    if (epsilonEqual(determinant(PerspectiveMatrix), static_cast<T>(0),
                     epsilon<T>()))
      return false;

    // First, isolate perspective.  This is the messiest.
    if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())) {
      // rightHandSide is the right hand side of the equation.
      vec4 RightHandSide;
      RightHandSide[0] = LocalMatrix[0][3];
      RightHandSide[1] = LocalMatrix[1][3];
      RightHandSide[2] = LocalMatrix[2][3];
      RightHandSide[3] = LocalMatrix[3][3];

      // Solve the equation by inverting PerspectiveMatrix and multiplying
      // rightHandSide by the inverse.  (This is the easiest way, not
      // necessarily the best.)
      mat4 InversePerspectiveMatrix =
          glm::inverse(PerspectiveMatrix); //   inverse(PerspectiveMatrix,
                                           //   inversePerspectiveMatrix);
      mat4 TransposedInversePerspectiveMatrix = glm::transpose(
          InversePerspectiveMatrix); //   transposeMatrix4(inversePerspectiveMatrix,
                                     //   transposedInversePerspectiveMatrix);

      Perspective = TransposedInversePerspectiveMatrix * RightHandSide;
      //  v4MulPointByMatrix(rightHandSide, transposedInversePerspectiveMatrix,
      //  perspectivePoint);

      // Clear the perspective partition
      LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] =
          static_cast<T>(0);
      LocalMatrix[3][3] = static_cast<T>(1);
    } else {
      // No perspective.
      Perspective = vec4(0, 0, 0, 1);
    }

    // Next take care of translation (easy).
    Translation = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

    vec3 Row[3], Pdum3;

    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
      for (length_t j = 0; j < 3; ++j)
        Row[i][j] = LocalMatrix[i][j];

    // Compute X scale factor and normalize first row.
    Scale.x = length(Row[0]); // v3Length(Row[0]);

    Row[0] = detail::scale(Row[0], static_cast<T>(1));

    // Compute XY shear factor and make 2nd row orthogonal to 1st.
    Skew.z = dot(Row[0], Row[1]);
    Row[1] = detail::combine(Row[1], Row[0], static_cast<T>(1), -Skew.z);

    // Now, compute Y scale and normalize 2nd row.
    Scale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    Skew.z /= Scale.y;

    // Compute XZ and YZ shears, orthogonalize 3rd row.
    Skew.y = dot(Row[0], Row[2]);
    Row[2] = detail::combine(Row[2], Row[0], static_cast<T>(1), -Skew.y);
    Skew.x = dot(Row[1], Row[2]);
    Row[2] = detail::combine(Row[2], Row[1], static_cast<T>(1), -Skew.x);

    // Next, get Z scale and normalize 3rd row.
    Scale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));
    Skew.y /= Scale.z;
    Skew.x /= Scale.z;

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
    Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
    if (dot(Row[0], Pdum3) < 0) {
      for (length_t i = 0; i < 3; i++) {
        Scale[i] *= static_cast<T>(-1);
        Row[i] *= static_cast<T>(-1);
      }
    }

    // Now, get the rotations out, as described in the gem.

    // FIXME - Add the ability to return either quaternions (which are
    // easier to recompose with) or Euler angles (rx, ry, rz), which
    // are easier for authors to deal with. The latter will only be useful
    // when we fix https://bugs.webkit.org/show_bug.cgi?id=23799, so I
    // will leave the Euler angle code here for now.

    // ret.rotateY = asin(-Row[0][2]);
    // if (cos(ret.rotateY) != 0) {
    //     ret.rotateX = atan2(Row[1][2], Row[2][2]);
    //     ret.rotateZ = atan2(Row[0][1], Row[0][0]);
    // } else {
    //     ret.rotateX = atan2(-Row[2][0], Row[1][1]);
    //     ret.rotateZ = 0;
    // }


    return true;
}

} // namespace Rengin::Math
