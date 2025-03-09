#pragma once
#include "pch.h"
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct attribute_info {
	bool need_normalization;
	GLenum type;
	std::size_t count;
	std::size_t offset;
};

struct vertex_info {
	std::size_t stride;
	std::vector<attribute_info> attr_info;
};

class vertex_array_object
{
private:
	uint32_t m_id;
	std::size_t m_ibo_count;
	uint32_t m_index_count;
	uint32_t m_binding_index_count;
	int m_platform_max_attr_count;

	vertex_array_object(const vertex_array_object&) = delete;
public:
	vertex_array_object();
	virtual ~vertex_array_object();
	void bind() const;
	void set_element_count(const std::size_t);
	std::size_t get_element_count();
	void bind_vbo(const uint32_t m_vbo, const vertex_info& info);
	void bind_ibo(const uint32_t ibo, std::size_t count);
};

