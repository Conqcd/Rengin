#include "repch.hpp"
#include "OpenGLShader.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Rengin
{
    
OpenGLShader::OpenGLShader(const std::string& name,const std::string& vertexSrc,const std::string& fragmentSrc)
	: m_name(name)
{

	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar *source = vertexSrc.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		
		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		// Use the infoLog as you see fit.
		
		// In this simple program, we'll just leave
		RE_CORE_ERROR("{0}",infoLog.data());
		RE_CORE_ASSERT(false,"Vertex Compilation failure!");
		return;
	}

	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = fragmentSrc.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
		
		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		// Use the infoLog as you see fit.
		
		// In this simple program, we'll just leave
		RE_CORE_ERROR("{0}",infoLog.data());
		RE_CORE_ASSERT(false,"Fragment Compilation failure!");
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_render_id = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(m_render_id, vertexShader);
	glAttachShader(m_render_id, fragmentShader);

	// Link our program
	glLinkProgram(m_render_id);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_render_id, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_render_id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_render_id, maxLength, &maxLength, &infoLog[0]);
		
		// We don't need the program anymore.
		glDeleteProgram(m_render_id);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Use the infoLog as you see fit.
		
		// In this simple program, we'll just leave
		RE_CORE_ERROR("{0}",infoLog.data());
		RE_CORE_ASSERT(false,"Shader link failure!");
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(m_render_id, vertexShader);
	glDetachShader(m_render_id, fragmentShader);
}

OpenGLShader::OpenGLShader(const std::string& filepath)
{
	auto src = ReadFile(filepath);

	auto lastSlash = filepath.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos? 0 : lastSlash + 1;
	auto lastDot = filepath.rfind('.');
	auto count = lastDot == std::string::npos? filepath.size() - lastSlash : lastDot - lastSlash;
	m_name = filepath.substr(lastSlash,count);
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_render_id);
}

void OpenGLShader::Bind() const
{
	glUseProgram(m_render_id);
}

void OpenGLShader::UnBind() const
{
	glUseProgram(0);
}

const std::string& OpenGLShader::getName() const
{
	return m_name;
}

void OpenGLShader::UpLoadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
	GLint location =  glGetUniformLocation(m_render_id,name.c_str());
	glUniformMatrix3fv(location,1,GL_FALSE,glm::value_ptr(matrix));
}

void OpenGLShader::UpLoadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLint location =  glGetUniformLocation(m_render_id,name.c_str());
	glUniformMatrix4fv(location,1,GL_FALSE,glm::value_ptr(matrix));
}

void OpenGLShader::UpLoadUniformInt(const std::string& name, int value)
{
	GLint location =  glGetUniformLocation(m_render_id,name.c_str());
	glUniform1i(location,value);
}

void OpenGLShader::UpLoadUniformFloat(const std::string& name, float value)
{
	GLint location =  glGetUniformLocation(m_render_id,name.c_str());
	glUniform1f(location,value);
}

void OpenGLShader::UpLoadUniformFloat2(const std::string& name, const glm::vec2& value)
{
	GLint location =  glGetUniformLocation(m_render_id,name.c_str());
	glUniform2f(location,value.x,value.y);
}

void OpenGLShader::UpLoadUniformFloat3(const std::string& name, const glm::vec3& value)
{
	GLint location =  glGetUniformLocation(m_render_id,name.c_str());
	glUniform3f(location,value.x,value.y,value.z);
}

void OpenGLShader::UpLoadUniformFloat4(const std::string& name, const glm::vec4& value)
{
	GLint location =  glGetUniformLocation(m_render_id,name.c_str());
	glUniform4f(location,value.x,value.y,value.z,value.w);
}

void OpenGLShader::Compile(const std::unordered_map<GLenum,std::string>& shaderSrc)
{
	m_render_id = glCreateProgram();

	RE_CORE_ASSERT(shaderSrc.size() <= 2 , "Only support 2 shader for now");
	std::vector<GLuint> shaderId;
	// std::array<GLuint,2> shaderId;
	shaderId.reserve(shaderSrc.size());
	for(auto& kv : shaderSrc)
	{
		auto type = kv.first;
		const std::string& sources = kv.second;
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
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(Shader, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the shader anymore.
			for(auto shId : shaderId)
			{
				glDeleteShader(shId);
			}

			// Use the infoLog as you see fit.
			
			// In this simple program, we'll just leave
			RE_CORE_ERROR("{0}",infoLog.data());
			RE_CORE_ASSERT(false,"Shader Compilation failure!");
			return;
		}
		glAttachShader(m_render_id, Shader);
	}
	
	// Link our program
	glLinkProgram(m_render_id);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_render_id, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_render_id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_render_id, maxLength, &maxLength, &infoLog[0]);
		
		// We don't need the program anymore.
		glDeleteProgram(m_render_id);
		// Don't leak shaders either.

		for(auto shId : shaderId)
		{
			glDeleteShader(shId);
		}

		// Use the infoLog as you see fit.
		
		// In this simple program, we'll just leave
		RE_CORE_ERROR("{0}",infoLog.data());
		RE_CORE_ASSERT(false,"Shader link failure!");
		return;
	}

	for(auto shId : shaderId)
	{
		glDetachShader(m_render_id, shId);
	}
}

const std::string OpenGLShader::ReadFile(const std::string& src)
{
	std::string result;
	std::ifstream in(src,std::ios::in,std::ios::binary);
	if(!in.is_open())
	{
		RE_CORE_ERROR("Could not open file '{0}'",src);
	}else
	{
		in.seekg(0,std::ios::end);
		result.resize(in.tellg());
		in.seekg(0,std::ios::beg);
		in.read(&result[0],result.size());
		in.close();
	}
	return result;
}

} // namespace Rengin
