#pragma once
#include <rengine.hpp>
#include <imgui.h>

class SandBox2D : public Rengin::Layer
{
private:
    Rengin::ShaderLibrary m_shader_lib;
    Rengin::Ref<Rengin::Shader> m_shader;
    Rengin::Ref<Rengin::VertexBuffer> m_verbuf;
    Rengin::Ref<Rengin::IndexBuffer> m_indbuf;
    Rengin::Ref<Rengin::VertexArray> m_verarr;
    Rengin::Ref<Rengin::Texture> m_texture;
    Rengin::OrthoGraphicsCameraController m_camera_controller;
public:
    SandBox2D(/* args */);
    ~SandBox2D();
    void OnUpdate(Rengin::TimeStep timestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnAttach()override;
    virtual void OnDetach()override;
    void OnEvent(Rengin::Event& ev) override;
};

