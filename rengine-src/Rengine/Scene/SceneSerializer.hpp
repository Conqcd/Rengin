#pragma once

#include "Rengine/Core/core.hpp"
#include "Scene.hpp"

namespace Rengin
{
class SceneSerializer
{
private:
    Ref<Scene> m_scene;
public:
    SceneSerializer(const Ref<Scene>& scene);

    void Serializer(const std::string& filePath);
    void SerializerRuntime(const std::string& filePath);

    bool Deserializer(const std::string& filePath);
    bool DeserializerRuntime(const std::string& filePath);
};


    
} // namespace Rengin
