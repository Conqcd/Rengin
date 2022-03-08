#include "repch.hpp"
#include "ObjManager.hpp"
#include "tiny_obj_loader.hpp"
#include "Rengine/Renderer/VertexArray.hpp"

namespace Rengin
{
    
ObjManager::ObjManager(const std::string& path,const std::string& material_path)
{
    std::string* warn = new std::string(),*err = new std::string();
    auto attrib = new tinyobj::attrib_t();
    auto shapes = new std::vector<tinyobj::shape_t>();
    auto material = new std::vector<tinyobj::material_t>();

    tinyobj::LoadObj(attrib, shapes, material, warn, err, path.c_str(), material_path.c_str());
    
    delete warn;
    delete err;
    delete attrib;
    delete shapes;
    delete material;
}

ObjManager::~ObjManager()
{
}


} // namespace Rengin
