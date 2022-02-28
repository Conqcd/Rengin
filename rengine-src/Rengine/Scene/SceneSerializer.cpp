#include "repch.hpp"
#include "SceneSerializer.hpp"
#include "Component.hpp"
#include <fstream>
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>


namespace YAML
{
template<>
struct convert<glm::vec3>
{
    static Node encode(const glm::vec3& rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        return node;
    }
    
    static bool decode(const Node& node,glm::vec3& rhs)
    {
        if (!node.IsSequence() || node.size() != 3)
        {
            return false;
        }
        
        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};
template<>
struct convert<glm::vec4>
{
    static Node encode(const glm::vec4& rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.push_back(rhs.w);
        return node;
    }
    
    static bool decode(const Node& node,glm::vec4& rhs)
    {
        if (!node.IsSequence() || node.size() != 4)
        {
            return false;
        }
        
        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        rhs.w = node[3].as<float>();
        return true;
    }
};
}
namespace Rengin
{



YAML::Emitter& operator<<(YAML::Emitter& out,const glm::vec4& v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out,const glm::vec3& v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
            : m_scene(scene)
{
}

static void SerializeEntity(YAML::Emitter& out,Entity entity)
{
    RE_ASSERT(entity.HasComponent<IDComponent>());

    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<IDComponent>().ID;

    if (entity.HasComponent<TagComponent>())
    {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap;

        auto& tag = entity.GetComponent<TagComponent>().Tag;
        out <<  YAML::Key << "Tag" << YAML::Value << tag;
        out << YAML::EndMap;
    }

    if (entity.HasComponent<TransformComponent>())
    {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap;

        auto& tc = entity.GetComponent<TransformComponent>();
        out <<  YAML::Key << "Translation" << YAML::Value << tc.Translation;
        out <<  YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
        out <<  YAML::Key << "Scale" << YAML::Value << tc.Scale;
        out << YAML::EndMap;
    }
    
    if (entity.HasComponent<CameraComponent>())
    {
        out << YAML::Key << "CameraComponent";
        out << YAML::BeginMap;

        auto& cameraComponent = entity.GetComponent<CameraComponent>();
        auto camera = cameraComponent.Camera;
        
        out <<  YAML::Key << "Camera" << YAML::Value;
        out <<  YAML::BeginMap;
        out <<  YAML::Key << "ProjectionType" << YAML::Value << static_cast<int>(camera.GetProjectionType());
        out <<  YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
        out <<  YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
        out <<  YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
        out <<  YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicsSize();
        out <<  YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicsNearClip();
        out <<  YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicsFarClip();
        out << YAML::EndMap;
        out <<  YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
        out <<  YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
        out << YAML::EndMap;
    }
    
    if (entity.HasComponent<SpriteRendererComponent>())
    {
        out << YAML::Key << "SpriteRendererComponent";
        out << YAML::BeginMap;

        auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
        out <<  YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
        out << YAML::EndMap;
    }

    if (entity.HasComponent<CircleRendererComponent>())
    {
        out << YAML::Key << "CircleRendererComponent";
        out << YAML::BeginMap;

        auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
        out <<  YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
        out <<  YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
        out <<  YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;
        out << YAML::EndMap;
    }

    if (entity.HasComponent<Texture3DComponent>())
    {
        out << YAML::Key << "Texture3DComponent";
        out << YAML::BeginMap;

        auto& texture3DComponent = entity.GetComponent<Texture3DComponent>();
        out <<  YAML::Key << "Path" << YAML::Value << texture3DComponent.Path;
        out << YAML::EndMap;
    }
    
    if (entity.HasComponent<ColorTransferFunctionComponent>())
    {
        out << YAML::Key << "ColorTransferFunctionComponent";
        out << YAML::BeginMap;

        auto& colorTransfer = entity.GetComponent<ColorTransferFunctionComponent>().Color;
        out <<  YAML::Key << "ColorTransfer" << YAML::Value << YAML::BeginSeq;
        for (auto it = colorTransfer.begin(); it != colorTransfer.end(); it++)
        {
            // out << YAML::Key << "first" << YAML::Value << it->first;
            // out << YAML::Key << "second" << YAML::Value << it->second;
            out << it->first << it->second;
        }
        
        out << YAML::EndSeq << YAML::EndMap;
    }

    if (entity.HasComponent<OpacityTransferFunctionComponent>())
    {
        out << YAML::Key << "OpacityTransferFunctionComponent";
        out << YAML::BeginMap;

        auto& opacityTransfer = entity.GetComponent<OpacityTransferFunctionComponent>().Opacity;
        out <<  YAML::Key << "OpacityTransfer" << YAML::Value << YAML::BeginSeq;
        for (auto it = opacityTransfer.begin(); it != opacityTransfer.end(); it++)
        {
            // out << YAML::Key << "first" << YAML::Value << it->first;
            // out << YAML::Key << "second" << YAML::Value << it->second;
            out << it->first << it->second;
        }
        
        out << YAML::EndSeq << YAML::EndMap;
    }

    out << YAML::EndMap;
}

void SceneSerializer::Serializer(const std::string& filePath)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene";
    out << YAML::Value << "Untitled";
    out << YAML::Key << "Entities";
    out << YAML::Value << YAML::BeginSeq;
    
    m_scene->m_registry.each([&](auto entityID){
        Entity entity{entityID,m_scene.get()};
        if(!entity)
            return;
        SerializeEntity(out,entity);
    });

    out << YAML::EndSeq;
    out << YAML::EndMap;
    std::string Path = filePath;
    if(filePath.find_last_of(".yaml") == std::string::npos)
        Path += ".yaml";
    std::ofstream fout(Path);
    fout << out.c_str();
}

void SceneSerializer::SerializerRuntime(const std::string& filePath)
{
    //Not Implement Now!
    RE_CORE_ASSERT(false,"x");
}

bool SceneSerializer::Deserializer(const std::string& filePath)
{
    std::ifstream stream(filePath);
    std::stringstream strStream;
    strStream << stream.rdbuf();

    YAML::Node data = YAML::Load(strStream.str());
    if(!data["Scene"])
        return false;
    
    std::string sceneName = data["Scene"].as<std::string>();

    RE_CORE_TRACE("Deserializing Scene '{0}'",sceneName);

    auto entities = data["Entities"];
    if (entities)
    {
        for(auto entity: entities)
        {
            uint64_t uuid = entity["Entity"].as<uint64_t>();

            std::string name;
            auto tagComponent = entity["TagComponent"];
            if(tagComponent)
                name = tagComponent["Tag"].as<std::string>();
            RE_CORE_TRACE("Deserializing Entity with ID = {0} , name = {1}",uuid,name);

            Entity deserializerEntity = m_scene->CreateEntity(name);

            auto transformComponent = entity["TransformComponent"];

            if (transformComponent)
            {
                auto& tc = deserializerEntity.GetComponent<TransformComponent>();
                tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                tc.Scale = transformComponent["Scale"].as<glm::vec3>();
            }

            auto cameraComponent = entity["CameraComponent"];

            if (cameraComponent)
            {
                auto& cc = deserializerEntity.AddComponent<CameraComponent>();

                auto&& cameraProps = cameraComponent["Camera"];
                cc.Camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(cameraProps["ProjectionType"].as<int>()));

                cc.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
                cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
                cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

                cc.Camera.SetOrthographicsSize(cameraProps["OrthographicSize"].as<float>());
                cc.Camera.SetOrthographicsNearClip(cameraProps["OrthographicNear"].as<float>());
                cc.Camera.SetOrthographicsFarClip(cameraProps["OrthographicFar"].as<float>());

                cc.Primary = cameraComponent["Primary"].as<bool>();
                cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
            }

            auto spriteRendererComponent = entity["SpriteRendererComponent"];

            if (spriteRendererComponent)
            {
                auto& src = deserializerEntity.AddComponent<SpriteRendererComponent>();
                src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
            }

            auto circleRendererComponent = entity["CircleRendererComponent"];

            if (circleRendererComponent)
            {
                auto& src = deserializerEntity.AddComponent<CircleRendererComponent>();
                src.Color = circleRendererComponent["Color"].as<glm::vec4>();
                src.Thickness = circleRendererComponent["Thickness"].as<float>();
                src.Fade = circleRendererComponent["Fade"].as<float>();
            }

            auto texture3DComponent = entity["Texture3DComponent"];
            if (texture3DComponent) {
                auto filepath = texture3DComponent["Path"].as<std::string>();
                auto &t3c = deserializerEntity.AddComponent<Texture3DComponent>();
                t3c.Path = filepath;
                t3c.Texture = Texture3D::Create(filepath);
            }

            auto colorTransferFunctionComponent = entity["ColorTransferFunctionComponent"];
            if (colorTransferFunctionComponent) {

                auto ctcs = colorTransferFunctionComponent["ColorTransfer"];
                std::vector<float> l1;
                std::vector<glm::vec3> l2;
                int odd = 1;
                for (auto &&ct : ctcs)
                {
                    if(odd % 2)
                    {
                      auto fr = ct.as<float>();
                      l1.push_back(fr);
                    }else
                    {
                      auto cr = ct.as<glm::vec3>();
                      l2.push_back(cr);
                    }
                    odd++;
                }
                
                auto &ctc = deserializerEntity.AddComponent<ColorTransferFunctionComponent>(TransferFunction<float,glm::vec3>{l1,l2});
            }

            auto opacityTransferFunctionComponent = entity["OpacityTransferFunctionComponent"];
            if (opacityTransferFunctionComponent) {
                auto otcs = opacityTransferFunctionComponent["OpacityTransfer"];
                std::vector<float> l1, l2;
                int odd = 1;
                for (auto &&ot : otcs)
                {
                    if(odd % 2)
                    {
                      auto fr = ot.as<float>();
                      l1.push_back(fr);
                    }else
                    {
                      auto cr = ot.as<float>();
                      l2.push_back(cr);
                    }
                    odd++;
                }

                auto &otc = deserializerEntity
                                .AddComponent<OpacityTransferFunctionComponent>(
                                    TransferFunction<float, float>{l1, l2});
            }
        }
    }
    return true;
}

bool SceneSerializer::DeserializerRuntime(const std::string& filePath)
{
    //Not Implement Now!
    RE_CORE_ASSERT(false, "x");
    return false;
}


} // namespace Rengin
