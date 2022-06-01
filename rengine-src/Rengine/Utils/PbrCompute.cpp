#include "repch.hpp"
#include "PbrCompute.hpp"
#include "Rengine/Math/Math.hpp"
#include <random>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

namespace Rengin
{

const unsigned int Resolution = 256;

glm::vec2 Hammersley(uint32_t i, uint32_t N) { // 0-1
    uint32_t bits = (i << 16u) | (i >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    float rdi = float(bits) * 2.3283064365386963e-10;
    return {float(i) / float(N), rdi};
}

glm::vec3 ImportanceSampleGGX(glm::vec2 Xi, glm::vec3 N, float roughness) {
    float a = roughness * roughness;

    //TODO: in spherical space - Bonus 1


    //TODO: from spherical space to cartesian space - Bonus 1
 

    //TODO: tangent coordinates - Bonus 1


    //TODO: transform H to tangent space - Bonus 1
    
    return glm::vec3(1.0f);
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float k = (roughness + 1) * (roughness + 1) / 8;
    return NdotV / (NdotV * (1 - k) + k);
}

float GeometrySmith(float roughness, float NoV, float NoL) {
    float ggx2 = GeometrySchlickGGX(NoV, roughness);
    float ggx1 = GeometrySchlickGGX(NoL, roughness);

    return ggx1 * ggx2;
}

glm::vec3 IntegrateBRDF(glm::vec3 V, float roughness) {

    const int sample_count = 1024;
    glm::vec3 N(0.0, 0.0, 1.0);
    for (int i = 0; i < sample_count; i++) {
        glm::vec2 Xi = Hammersley(i, sample_count);
        glm::vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        glm::vec3 L = normalize(H * 2.0f * dot(V, H) - V);

        float NoL = std::max(L.z, 0.0f);
        float NoH = std::max(H.z, 0.0f);
        float VoH = std::max(dot(V, H), 0.0f);
        float NoV = std::max(dot(N, V), 0.0f);
        
        // TODO: To calculate (fr * ni) / p_o here - Bonus 1


        // Split Sum - Bonus 2
        
    }

    return glm::vec3(1.0f);
}
void PreComputeEmu_IS(const std::string& path)
{
    unsigned char data[Resolution * Resolution * 3];
    float step = 1.0 / Resolution;
    for (int i = 0; i < Resolution; i++) {
        for (int j = 0; j < Resolution; j++) {
            float roughness = step * (static_cast<float>(i) + 0.5f);
            float NdotV = step * (static_cast<float>(j) + 0.5f);
            glm::vec3 V(std::sqrt(1.f - NdotV * NdotV), 0.f, NdotV);

            glm::vec3 irr = IntegrateBRDF(V, roughness);

            data[(i * Resolution + j) * 3 + 0] = uint8_t(irr.x * 255.0);
            data[(i * Resolution + j) * 3 + 1] = uint8_t(irr.y * 255.0);
            data[(i * Resolution + j) * 3 + 2] = uint8_t(irr.z * 255.0);
        }
    }
    stbi_flip_vertically_on_write(true);
    stbi_write_png(path.c_str(), Resolution, Resolution, 3, data, Resolution * 3);
    
    RE_CORE_INFO("Finished Emu Importance Sampling precomputed!");
}

glm::vec3 IntegrateEmu(glm::vec3 V, float roughness, float NdotV, glm::vec3 Ei) {
    glm::vec3 Eavg(0.0f);
    const int sample_count = 1024;
    glm::vec3 N(0.0, 0.0, 1.0);

    for (int i = 0; i < sample_count; i++) 
    {
        glm::vec2 Xi = Hammersley(i, sample_count);
        glm::vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        glm::vec3 L = normalize(H * 2.0f * dot(V, H) - V);

        float NoL = std::max(L.z, 0.0f);
        float NoH = std::max(H.z, 0.0f);
        float VoH = std::max(dot(V, H), 0.0f);
        float NoV = std::max(dot(N, V), 0.0f);

        // TODO: To calculate Eavg here - Bonus 1
        
    }

    return glm::vec3(1.0);
}

void setRGB(int x, int y, float alpha, unsigned char *data,int resolution) {
	data[3 * (resolution * x + y) + 0] = uint8_t(alpha);
    data[3 * (resolution * x + y) + 1] = uint8_t(alpha);
    data[3 * (resolution * x + y) + 2] = uint8_t(alpha);
}

void setRGB(int x, int y, glm::vec3 alpha, unsigned char *data,int resolution) {
	data[3 * (resolution * x + y) + 0] = uint8_t(alpha.x);
    data[3 * (resolution * x + y) + 1] = uint8_t(alpha.y);
    data[3 * (resolution * x + y) + 2] = uint8_t(alpha.z);
}

glm::vec3 getEmu(int x, int y, int alpha, unsigned char *data, float NdotV, float roughness,int resolution) {
    return {data[3 * (resolution * x + y) + 0],data[3 * (resolution * x + y) + 1],data[3 * (resolution * x + y) + 2]};
}

void PreComputeEavg_IS(const std::string& path,const std::string& EuPath)
{
    int resolution;
    int channel;
    unsigned char *Edata = stbi_load(EuPath.c_str(), &resolution, &resolution, &channel, 3);
    RE_CORE_ASSERT(Edata,"ERROE_FILE_NOT_LOAD");
    unsigned char* data = new unsigned char[resolution * resolution * 3];
    float step = 1.0 / resolution;
    glm::vec3 Eavg(0.0);
    for (int i = 0; i < resolution; i++) 
    {
        float roughness = step * (static_cast<float>(i) + 0.5f);
        for (int j = 0; j < resolution; j++) 
        {
            float NdotV = step * (static_cast<float>(j) + 0.5f);
            glm::vec3 V(std::sqrt(1.f - NdotV * NdotV), 0.f, NdotV);

            glm::vec3 Ei = getEmu((resolution - 1 - i), j, 0, Edata, NdotV, roughness,resolution);
            Eavg += IntegrateEmu(V, roughness, NdotV, Ei) * step;
            setRGB(i, j, 0.0, data,resolution);
        }

        for(int k = 0; k < resolution; k++)
        {
            setRGB(i, k, Eavg, data,resolution);
        }

        Eavg = glm::vec3(0.0);
    }
    stbi_flip_vertically_on_write(true);
    stbi_write_png(path.c_str(), resolution, resolution, channel, data, 0);
	stbi_image_free(Edata);
    RE_CORE_INFO("Finished Eavg Importance Sampling precomputed!");
}

} // namespace Rengin
