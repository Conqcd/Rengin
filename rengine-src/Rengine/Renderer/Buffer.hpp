#pragma once
#include "Rengine/core.hpp"

namespace Rengin
{
enum class ShadeDataType
{
    None = 0,Float,Float2,Float3,Float4,Mat3,Mat4,Int,Int2,Int3,Int4,Bool
};

static uint32_t ShaderDataTypeSize(ShadeDataType type)
{
    switch (type)
    {
        case ShadeDataType::Float: return 4;
        case ShadeDataType::Float2: return 8;
        case ShadeDataType::Float3: return 12;
        case ShadeDataType::Float4: return 16;
        case ShadeDataType::Mat3: return 36;
        case ShadeDataType::Mat4: return 64;
        case ShadeDataType::Int: return 4;
        case ShadeDataType::Int2: return 8;
        case ShadeDataType::Int3: return 12;
        case ShadeDataType::Int4: return 16;
        case ShadeDataType::Bool: return 1;
    }
    RE_CORE_ASSERT(false,"Unknown Shader Type");
    return 0;
}
struct RE_API BufferElement
{
    std::string Name;
    ShadeDataType Type;
    uint32_t Size;
    uint32_t Offset;
    bool m_normalized;
    BufferElement(ShadeDataType type,const std::string& name,bool normalized = false)
        :Name(name) , Type(type) , Size(ShaderDataTypeSize(type)) , Offset(0) ,m_normalized(normalized)
    {}
    BufferElement(){}

    uint32_t getComponentCount()const
    {
        switch (Type)
        {
            case ShadeDataType::Float: return 1;
            case ShadeDataType::Float2: return 2;
            case ShadeDataType::Float3: return 3;
            case ShadeDataType::Float4: return 4;
            case ShadeDataType::Mat3: return 9;
            case ShadeDataType::Mat4: return 16;
            case ShadeDataType::Int: return 1;
            case ShadeDataType::Int2: return 2;
            case ShadeDataType::Int3: return 3;
            case ShadeDataType::Int4: return 4;
            case ShadeDataType::Bool: return 1;
        }
        RE_CORE_ASSERT(false,"Unknown Shader Type");
        return 0;
    }
};

class RE_API BufferLayout
{
private:
    std::vector<BufferElement> m_elements;
    uint32_t m_stride;
    
    void CalculateOffsetAndStride()
    {
        uint32_t offset = 0;
        m_stride = 0;
        for(auto& element : m_elements)
        {
             element.Offset = offset;
             offset += element.Size;
             m_stride += element.Size;
        }
    }
public:
    BufferLayout() {}
    BufferLayout(const std::initializer_list<BufferElement>& elements)
    : m_elements(elements) {
        CalculateOffsetAndStride();
    }
    ~BufferLayout(){}
    inline uint32_t getStride()const {return m_stride;}
    inline const std::vector<BufferElement>& getElements()const {return m_elements;}
    std::vector<BufferElement>::iterator begin(){return m_elements.begin();}
    std::vector<BufferElement>::iterator end(){return m_elements.end();}
    std::vector<BufferElement>::const_iterator begin()const{return m_elements.begin();}
    std::vector<BufferElement>::const_iterator end() const{return m_elements.end();}
};

class RE_API VertexBuffer
{
private:
    /* data */
public:
    virtual ~VertexBuffer() {}

    virtual void Bind()const = 0;
    virtual void Unbind()const = 0;

    virtual void SetLayout(const BufferLayout& layout) = 0;
    virtual const BufferLayout& getLayout()const = 0;

    static VertexBuffer* Create(float *vertices,uint32_t size);
};

class RE_API IndexBuffer
{
private:
    /* data */
public:
    virtual ~IndexBuffer() {}
    virtual void Bind()const = 0;
    virtual void Unbind()const = 0;
    
    virtual uint32_t GetCount() const = 0;
    static IndexBuffer* Create(uint32_t *indices,uint32_t size);
};
    

    
} // namespace Rengin
