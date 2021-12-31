#include "repch.hpp"
#include "SceneSerializer.hpp"


namespace Rengin
{

SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
            : m_scene(scene)
{
}

void SceneSerializer::Serializer(const std::string& filePath)
{
    
}

void SceneSerializer::SerializerRuntime(const std::string& filePath)
{
    //Not Implement Now!
    RE_CORE_ASSERT(false,"x");
}

bool SceneSerializer::Deserializer(const std::string& filePath)
{
    return false;
}

bool SceneSerializer::DeserializerRuntime(const std::string& filePath)
{
    //Not Implement Now!
    RE_CORE_ASSERT(false, "x");
    return false;
}


} // namespace Rengin
