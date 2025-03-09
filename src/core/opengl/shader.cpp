#include "pch.h"
#include "shader.h"

const std::string shader_program::view_matrix_uni_name = "u_view";
const std::string shader_program::projection_matrix_uni_name = "u_projection";
const std::string shader_program::texture_samplers_uni_name = "u_textures[0]";

uint32_t shader_program::compile_shader(GLenum shader_type, const std::string& shader_src) const
{
	auto m_id = glCreateShader(shader_type);
	auto src_ptr = shader_src.c_str();
	glShaderSource(m_id, 1, &src_ptr, nullptr);
	glCompileShader(m_id);

	int result;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int len;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &len);
		char* message = (char*)alloca(len * sizeof(char));
		glGetShaderInfoLog(m_id, len, &len, message);
		LOGERROR("{}", message);

		return -1;
	}

	return m_id;
}

shader_program::shader_program(const std::string& m_fragment_src, const std::string& m_vertex_src)
	: m_active_uniforms(), m_active_vertex_attrs(), m_fragment_src(m_fragment_src), m_vertex_src(m_vertex_src), m_id(0)
{
	m_id = glCreateProgram();
}

shader_program::~shader_program()
{
	glDeleteProgram(m_id);
}

void shader_program::bind() const
{
	glUseProgram(m_id);
}

uint32_t shader_program::get_id() const
{
	return m_id;
}

void shader_program::set_uniform_int(const std::string& name, const int* data, uint32_t count = 1) const
{
	bind();
	auto elem = m_active_uniforms.find(name);
	if (elem == m_active_uniforms.end())
	{
		return;
	}

	GLuint location = glGetUniformLocation(m_id, name.c_str());
	glUniform1iv(location, count, data);
}

void shader_program::set_uniform_mat4(const std::string& name, const float* data, uint32_t count) const
{
	bind();
	auto elem = m_active_uniforms.find(name);
	if (elem == m_active_uniforms.end())
	{
		return;
	}

	GLuint location = glGetUniformLocation(m_id, name.c_str());
	glUniformMatrix4fv(location, count, GL_FALSE, data);
}

void shader_program::set_active_attrib_map()
{
	bind();
	GLint count;
	glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &count);
	
	shader_program::shader_variable_info var_info;
	const GLsizei bufSize = 16;
	GLchar name[bufSize];
	for (GLint i = 0; i < count; i++)
	{
		glGetActiveAttrib(m_id, (GLuint)i, bufSize, nullptr, (GLint*)&var_info.m_size, &var_info.type, name);
		m_active_vertex_attrs[name] = var_info;
	}
}

void shader_program::set_active_uniforms_map()
{
	bind();
	GLint count;
	glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count);

	shader_program::shader_variable_info var_info;
	const GLsizei bufSize = 16;
	GLchar name[bufSize];
	for (GLint i = 0; i < count; i++)
	{
		glGetActiveUniform(m_id, (GLuint)i, bufSize, nullptr, (GLint*)&var_info.m_size, &var_info.type, name);
		m_active_uniforms[name] = var_info;
	}
}

void shader_program::compile()
{
	auto vertex_id = compile_shader(GL_VERTEX_SHADER, m_vertex_src);
	glAttachShader(m_id, vertex_id);

	auto fragment_id = compile_shader(GL_FRAGMENT_SHADER, m_fragment_src);
	glAttachShader(m_id, fragment_id);
	
	glLinkProgram(m_id);
	glValidateProgram(m_id);
	
	set_active_uniforms_map();
	set_active_attrib_map();
}
