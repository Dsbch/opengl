#pragma once
#include "pch.h"
#include <glad/glad.h>

class shader_program
{
public:
	static const std::string view_matrix_uni_name;
	static const std::string projection_matrix_uni_name;
	static const std::string texture_samplers_uni_name;

	struct shader_variable_info {
		GLenum type;
		size_t m_size;
	};

	shader_program(const shader_program&) = delete;
	shader_program(const std::string& m_fragment_src, const std::string& m_vertex_src);
	~shader_program();
	void compile();
	void bind() const;
	uint32_t get_id() const;

	void set_uniform_int(const std::string& name, const int* data, uint32_t count) const;
	void set_uniform_mat4(const std::string& name, const float* data, uint32_t count) const;
private:
	void set_active_attrib_map();
	void set_active_uniforms_map();
	uint32_t compile_shader(GLenum shader_type, const std::string& shader_src) const;
	
	std::map<std::string, shader_variable_info> m_active_uniforms;
	std::map<std::string, shader_variable_info> m_active_vertex_attrs;
	const std::string m_fragment_src;
	const std::string m_vertex_src;
	uint32_t m_id;
};