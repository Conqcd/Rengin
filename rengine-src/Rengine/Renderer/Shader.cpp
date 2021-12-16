#include "repch.hpp"
#include "Shader.hpp"
#include "Rengine/log.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Rengin
{

Shader::Shader(const std::string& vertexSrc,const std::string& fragmentSrc)
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

Shader::~Shader()
{
	glDeleteProgram(m_render_id);
}

void Shader::Bind() const
{
	glUseProgram(m_render_id);
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

void Shader::UpLoadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLint location =  glGetUniformLocation(m_render_id,name.c_str());
	glUniformMatrix4fv(location,1,GL_FALSE,glm::value_ptr(matrix));
}

} // namespace Rengin
