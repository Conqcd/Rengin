#include "OpenGLShader.hpp"
#include "Rengine/Core/log.hpp"
#include "Rengine/Debug/Instrumentor.hpp"
#include "repch.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Rengin {

OpenGLShader::OpenGLShader(const std::string &name,
                           const std::string &vertexPath,
                           const std::string &fragmentPath)
    : m_name(name) {
  RE_PROFILE_FUNCTION();
  auto vertexSrc = ReadFile(vertexPath);
  auto fragmentSrc = ReadFile(fragmentPath);

  std::unordered_map<GLenum, std::string> mp;

  mp[GL_VERTEX_SHADER] = vertexSrc;
  mp[GL_FRAGMENT_SHADER] = fragmentSrc;

  Compile(mp);
}

OpenGLShader::OpenGLShader(const std::string &vertexPath,
                           const std::string &fragmentPath) {
  RE_PROFILE_FUNCTION();
  auto vertexSrc = ReadFile(vertexPath);
  auto fragmentSrc = ReadFile(fragmentPath);

  auto lastSlash = vertexPath.find_last_of("/\\");
  lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
  auto lastDot = vertexPath.rfind('.');
  auto count = lastDot == std::string::npos ? vertexPath.size() - lastSlash
                                            : lastDot - lastSlash;
  m_name = vertexPath.substr(lastSlash, count);

  std::unordered_map<GLenum, std::string> mp;

  mp[GL_VERTEX_SHADER] = vertexSrc;
  mp[GL_FRAGMENT_SHADER] = fragmentSrc;

  Compile(mp);
}

OpenGLShader::~OpenGLShader() {
  RE_PROFILE_FUNCTION();
  glDeleteProgram(m_render_id);
}

void OpenGLShader::Bind() const {
  RE_PROFILE_FUNCTION();
  glUseProgram(m_render_id);
}

void OpenGLShader::UnBind() const {
  RE_PROFILE_FUNCTION();
  glUseProgram(0);
}

const std::string &OpenGLShader::getName() const {
  RE_PROFILE_FUNCTION();
  return m_name;
}

void OpenGLShader::SetUniformInt(const std::string &name, int value) {
  RE_PROFILE_FUNCTION();
  UpLoadUniformInt(name, value);
}

void OpenGLShader::SetUniformUint(const std::string &name, unsigned int value) {
  RE_PROFILE_FUNCTION();
  UpLoadUniformUint(name, value);
}

void OpenGLShader::SetUniformIntArray(const std::string &name, int *values,
                                      uint32_t count) {
  RE_PROFILE_FUNCTION();
  UpLoadUniformIntArray(name, values, count);
}

void OpenGLShader::SetUniformMat3(const std::string& name,const glm::mat3& mat)
{
    RE_PROFILE_FUNCTION();
    UpLoadUniformMat3(name, mat);
}

void OpenGLShader::SetUniformMat4(const std::string &name,
                                  const glm::mat4 &mat) {
  RE_PROFILE_FUNCTION();
  UpLoadUniformMat4(name, mat);
}

void OpenGLShader::SetUniformFloat(const std::string &name, float value) {
  RE_PROFILE_FUNCTION();
  UpLoadUniformFloat(name, value);
}

void OpenGLShader::SetUniformFloat2(const std::string &name,
                                    const glm::vec2 &vec) {
  RE_PROFILE_FUNCTION();
  UpLoadUniformFloat2(name, vec);
}

void OpenGLShader::SetUniformFloat3(const std::string &name,
                                    const glm::vec3 &vec) {
  RE_PROFILE_FUNCTION();
  UpLoadUniformFloat3(name, vec);
}

void OpenGLShader::SetUniformFloat4(const std::string &name,
                                    const glm::vec4 &vec) {
  RE_PROFILE_FUNCTION();
  UpLoadUniformFloat4(name, vec);
}

void OpenGLShader::SetSSBOBinding(const std::string& name,const glm::vec4& vec)
{

}

void OpenGLShader::UpLoadUniformMat3(const std::string &name,
                                     const glm::mat3 &matrix) {
  GLint location = glGetUniformLocation(m_render_id, name.c_str());
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UpLoadUniformIntArray(const std::string &name, int *values,
                                         uint32_t count) {
  GLint location = glGetUniformLocation(m_render_id, name.c_str());
  glUniform1iv(location, count, values);
}

void OpenGLShader::UpLoadUniformMat4(const std::string &name,
                                     const glm::mat4 &matrix) {
  GLint location = glGetUniformLocation(m_render_id, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UpLoadUniformInt(const std::string &name, int value) {
  GLint location = glGetUniformLocation(m_render_id, name.c_str());
  glUniform1i(location, value);
}

void OpenGLShader::UpLoadUniformUint(const std::string &name, unsigned int value)
{
  GLint location = glGetUniformLocation(m_render_id, name.c_str());
  glUniform1ui(location, value);
}

void OpenGLShader::UpLoadUniformFloat(const std::string &name, float value) {
  GLint location = glGetUniformLocation(m_render_id, name.c_str());
  glUniform1f(location, value);
}

void OpenGLShader::UpLoadUniformFloat2(const std::string &name,
                                       const glm::vec2 &value) {
  GLint location = glGetUniformLocation(m_render_id, name.c_str());
  glUniform2f(location, value.x, value.y);
}

void OpenGLShader::UpLoadUniformFloat3(const std::string &name,
                                       const glm::vec3 &value) {
  GLint location = glGetUniformLocation(m_render_id, name.c_str());
  glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::UpLoadUniformFloat4(const std::string &name,
                                       const glm::vec4 &value) {
  GLint location = glGetUniformLocation(m_render_id, name.c_str());
  glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::Compile(
    const std::unordered_map<GLenum, std::string> &shaderSrc) {
  RE_PROFILE_FUNCTION();
  m_render_id = glCreateProgram();

  RE_CORE_ASSERT((shaderSrc.size() <= 2), "Only support 2 shader for now");
  std::vector<GLuint> shaderId;
  // std::array<GLuint,2> shaderId2;
  shaderId.reserve(shaderSrc.size());
  for (auto &kv : shaderSrc) {
    auto type = kv.first;
    const std::string &sources = kv.second;
    GLuint Shader = glCreateShader(type);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar *source = sources.c_str();
    glShaderSource(Shader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(Shader);

    GLint isCompiled = 0;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &isCompiled);
    shaderId.push_back(Shader);
    if (isCompiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(Shader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      for (auto shId : shaderId) {
        glDeleteShader(shId);
      }

      // Use the infoLog as you see fit.

      // In this simple program, we'll just leave
      RE_CORE_ERROR("{0}", infoLog.data());
      RE_CORE_ASSERT(false, "Shader Compilation failure!");
      return;
    }
    glAttachShader(m_render_id, Shader);
  }

  // Link our program
  glLinkProgram(m_render_id);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(m_render_id, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(m_render_id, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(m_render_id, maxLength, &maxLength, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(m_render_id);
    // Don't leak shaders either.

    for (auto shId : shaderId) {
      glDeleteShader(shId);
    }

    // Use the infoLog as you see fit.

    // In this simple program, we'll just leave
    RE_CORE_ERROR("{0}", infoLog.data());
    RE_CORE_ASSERT(false, "Shader link failure!");
    return;
  }

  for (auto shId : shaderId) {
    glDetachShader(m_render_id, shId);
  }
}

const std::string OpenGLShader::ReadFile(const std::string &src) {
  RE_PROFILE_FUNCTION();
  std::string result;
  std::ifstream in(src, std::ios::in | std::ios::binary);
  if (!in.is_open()) {
    RE_CORE_ERROR("Could not open file '{0}'", src);
  } else {
    in.seekg(0, std::ios::end);
    result.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&result[0], result.size());
    in.close();
  }
  return result;
}

} // namespace Rengin
