#include "repch.hpp"
#include "PrtCompute.hpp"
#include "Rengine/Math/Math.hpp"
#include <random>
#include <Eigen/Dense>

namespace Rengin
{


constexpr int GetIndex(int l, int m) {
  return l * (l + 1) + m;
}

const Eigen::Vector3f cubemapFaceDirections[6][3] = {
    {{0, 0, 1}, {0, -1, 0}, {-1, 0, 0}},  // negx
    {{0, 0, 1}, {0, -1, 0}, {1, 0, 0}},   // posx
    {{1, 0, 0}, {0, 0, -1}, {0, -1, 0}},  // negy
    {{1, 0, 0}, {0, 0, 1}, {0, 1, 0}},    // posy
    {{-1, 0, 0}, {0, -1, 0}, {0, 0, -1}}, // negz
    {{1, 0, 0}, {0, -1, 0}, {0, 0, 1}},   // posz
};

float CalcPreArea(const float &x, const float &y)
{
    return std::atan2(x * y, std::sqrt(x * x + y * y + 1.0));
}

float CalcArea(const float &u_, const float &v_, const int &width,
                   const int &height)
{
    float u = (2.0 * (u_ + 0.5) / width) - 1.0;
    float v = (2.0 * (v_ + 0.5) / height) - 1.0;

    float invResolutionW = 1.0 / width;
    float invResolutionH = 1.0 / height;

    float x0 = u - invResolutionW;
    float y0 = v - invResolutionH;
    float x1 = u + invResolutionW;
    float y1 = v + invResolutionH;
    float angle = CalcPreArea(x0, y0) - CalcPreArea(x0, y1) -
                    CalcPreArea(x1, y0) + CalcPreArea(x1, y1);

    return angle;
}

void ToSphericalCoords(const Eigen::Vector3d& dir, double& phi, double& theta) {
    theta = std::acos(Math::clamp(dir.z(), -1.0, 1.0));
    phi = std::atan2(dir.y(), dir.x());
}

double EvalLegendrePolynomial(int l, int m, double x) {
    double pmm = 1.0;
    if (m > 0) {
        double sign = (m % 2 == 0 ? 1 : -1);
        pmm = sign * Math::DoubleFactorial(2 * m - 1) * std::pow(1 - x * x, m / 2.0);
    }

    if (l == m) {
        return pmm;
    }

    double pmm1 = x * (2 * m + 1) * pmm;
    if (l == m + 1) {
        return pmm1;
    }

    for (int n = m + 2; n <= l; n++) {
        double pmn = (x * (2 * n - 1) * pmm1 - (n + m - 1) * pmm) / (n - m);
        pmm = pmm1;
        pmm1 = pmn;
    }
    return pmm1;
}

double EvalSHSlow(int l, int m, double phi, double theta) {
    double kml = std::sqrt((2.0 * l + 1) * Math::Factorial(l - std::abs(m)) /
                        (4.0 * M_PI * Math::Factorial(l + std::abs(m))));
    if (m > 0) {
        return std::sqrt(2.0) * kml * std::cos(m * phi) *
            EvalLegendrePolynomial(l, m, std::cos(theta));
    } else if (m < 0) {
        return std::sqrt(2.0) * kml * std::sin(-m * phi) *
            EvalLegendrePolynomial(l, -m, std::cos(theta));
    } else {
        return kml * EvalLegendrePolynomial(l, 0, std::cos(theta));
    }
}

double EvalSHSlow(int l, int m, const Eigen::Vector3d& dir) {
    double phi, theta;
    ToSphericalCoords(dir, phi, theta);
    return EvalSHSlow(l, m, phi, theta);
}

double EvalSH(int l, int m, const Eigen::Vector3d& dir) 
{
    RE_CORE_ASSERT(l >= 0, "l must be at least 0.");
    RE_CORE_ASSERT(-l <= m && m <= l, "m must be between -l and l.");
    RE_CORE_ASSERT(std::abs(dir.squaredNorm() - 1.0) > EPS, "dir is not unit.");

    return EvalSHSlow(l, m, dir);
}

std::vector<double> ProjectFunction(int order, const std::function<double(double, double)>& func, int sample_count) 
{
    RE_CORE_ASSERT(order >= 0, "Order must be at least zero.");
    RE_CORE_ASSERT(sample_count > 0, "Sample count must be at least one.");

    const int sample_side = static_cast<int>(floor(sqrt(sample_count)));
    std::vector<double> coeffs;
    coeffs.assign(GetCoefficientCount(order), 0.0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rng(0.0, 1.0);
    for (int t = 0; t < sample_side; t++) {
        for (int p = 0; p < sample_side; p++) {
        double alpha = (t + rng(gen)) / sample_side;
        double beta = (p + rng(gen)) / sample_side;
        double phi = 2.0 * M_PI * beta;
        double theta = acos(2.0 * alpha - 1.0);

        double func_value = func(phi, theta);

            for (int l = 0; l <= order; l++) {
                for (int m = -l; m <= l; m++) {
                    double sh = EvalSHSlow(l, m, phi, theta);
                    coeffs[GetIndex(l, m)] += func_value * sh;
                }
            }
        }
    }

    double weight = 4.0 * M_PI / (sample_side * sample_side);
    for (unsigned int i = 0; i < coeffs.size(); i++) {
        coeffs[i] *= weight;
    }

    return coeffs;
}


Eigen::Matrix3f ComputeSquareMatrix_3by3(const glm::mat4& rotation)
{
    Eigen::Matrix3f m33;
    // 1、pick ni - {ni}
	// Eigen n1 = [1, 0, 0, 0]; let n2 = [0, 0, 1, 0]; let n3 = [0, 1, 0, 0];

	// 2、{P(ni)} - A  A_inverse
	// let row1 = SHEval(n1[0],n1[1],n1[2],3);
	// let row2 = SHEval(n2[0],n2[1],n2[2],3);
	// let row3 = SHEval(n3[0],n3[1],n3[2],3);
	
	// let mathMatrix = [];
	// mathMatrix.push(row1.slice(1,4));
	// mathMatrix.push(row2.slice(1,4));
	// mathMatrix.push(row3.slice(1,4));
	// let A_inverse = math.matrix(mathMatrix);
	// A_inverse = math.inv(A_inverse);

	// 3、用 R 旋转 ni - {R(ni)}
	// n1 = math.multiply(rotationMatrix,n1);
	// n2 = math.multiply(rotationMatrix,n2);
	// n3 = math.multiply(rotationMatrix,n3);

	// 4、R(ni) SH投影 - S
	// let row11 = SHEval(n1._data[0],n1._data[1],n1._data[2],3);
	// let row22 = SHEval(n2._data[0],n2._data[1],n2._data[2],3);
	// let row33 = SHEval(n3._data[0],n3._data[1],n3._data[2],3);

	// let mathMatrix2 = [];
	// mathMatrix2.push(row11.slice(1,4));
	// mathMatrix2.push(row22.slice(1,4));
	// mathMatrix2.push(row33.slice(1,4));
	// let S = math.matrix(mathMatrix2);

	// 5、S*A_inverse
	// return math.multiply(S, A_inverse)
    return m33;
}

Eigen::Matrix<float,5,5> ComputeSquareMatrix_5by5(const glm::mat4& rotation)
{
    Eigen::Matrix<float,5,5> m55;

    return m55;
}

std::vector<glm::mat3> GetRotationPrecomputeL(const glm::mat4& rotation,const glm::mat3& PreComSHR
                        ,const glm::mat3& PreComSHG,const glm::mat3& PreComSHB)
{
    Eigen::Matrix<float,9,9>  rotMatrix;
    Eigen::Vector<float,9>  shR,shG,shB;
    std::vector<glm::mat3> PreCom(3);

    // 1 × 1
    rotMatrix(0,0) = 1.f;

    // 3 × 3
    auto M33 = ComputeSquareMatrix_3by3(rotation);

    // 5 × 5
    auto M55 = ComputeSquareMatrix_5by5(rotation);

    for (int i = 1; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            rotMatrix(i,j) = M33(i - 1,j - 1);
        }
    }
    for (int i = 4; i < 9; i++)
    {
        for (int j = 4; j < 9; j++)
        {
            rotMatrix(i,j) = M55(i - 4,j - 4);
        }
    }

    shR = rotMatrix * shR;
    shG = rotMatrix * shG;
    shB = rotMatrix * shB;
    int idx = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            PreCom[0][i][j] = shR(idx); 
            PreCom[1][i][j] = shG(idx); 
            PreCom[2][i][j] = shB(idx++); 
        }
    }
    return PreCom;
}
} // namespace Rengin
