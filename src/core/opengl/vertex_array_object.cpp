#include "pch.h"
#include "vertex_array_object.h"

vertex_array_object::vertex_array_object() : m_id(0), m_binding_index_count(0), m_index_count(0), m_ibo_count(0), m_platform_max_attr_count(0)
{
	glCreateVertexArrays(1, &m_id);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_platform_max_attr_count);
}

vertex_array_object::~vertex_array_object()
{
	glDeleteVertexArrays(1, &m_id);
}

void vertex_array_object::bind() const
{
	glBindVertexArray(m_id);
}

void vertex_array_object::set_element_count(const std::size_t count)
{
	m_ibo_count = count;
}

void vertex_array_object::bind_ibo(const uint32_t ibo_id, std::size_t count)
{
	m_ibo_count = count;
	glVertexArrayElementBuffer(m_id, ibo_id);
}

std::size_t vertex_array_object::get_element_count()
{
	return m_ibo_count;
}

void vertex_array_object::bind_vbo(const uint32_t vbo_id, const vertex_info& info)
{
	if (info.attr_info.size() > m_platform_max_attr_count)
	{
		LOGERROR("platform_max_attr_count: {:d} but got {:d}", m_platform_max_attr_count, info.attr_info.size());
		return;
	}

	for (const attribute_info& i : info.attr_info)
	{
		glEnableVertexArrayAttrib(m_id, m_index_count);
		glVertexArrayAttribBinding(m_id, m_index_count, m_binding_index_count);

		glVertexArrayVertexBuffer(m_id, m_binding_index_count, vbo_id, 0, info.stride);
		glVertexArrayAttribFormat(m_id, m_index_count, i.count, i.type, i.need_normalization, i.offset);
		
		m_index_count++;
		m_binding_index_count++;
	}
}
