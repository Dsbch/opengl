#include "pch.h"
#include "buffer_object.h"

buffer_object::buffer_object() : m_id(0), m_max_size(0), m_loaded_size(0)
{
	glCreateBuffers(1, &m_id);
}

buffer_object::~buffer_object()
{
	glDeleteBuffers(1, &m_id);
}

std::size_t buffer_object::get_size() const
{
	return m_max_size;
}

std::size_t buffer_object::get_loaded_size() const
{
	return m_loaded_size;
}

void buffer_object::reset_loaded_size()
{
	m_loaded_size = 0;
}

const unsigned int buffer_object::get_id() const
{
	return m_id;
}

void buffer_object::load_data(const void* data, std::size_t m_size)
{
	this->m_max_size = m_size;
	glNamedBufferStorage(m_id, m_size, data, GL_DYNAMIC_STORAGE_BIT);
	
	if (data)
	{
		this->m_loaded_size = m_size;
	}
}

void buffer_object::load_sub_data(const void* data, std::size_t offset, std::size_t sub_size)
{
	if (m_loaded_size + sub_size > m_max_size)
	{
		LOGERROR("subbuffer size is greater that buffer size: buff size {} sub_size {}", m_max_size, sub_size);
		return;
	}

	m_loaded_size += sub_size;

	glNamedBufferSubData(m_id, offset, sub_size, data);
}
