#pragma once
#include "pch.h"
#include <glad/glad.h>

class buffer_object
{
private:
	std::size_t m_max_size;
	std::size_t m_loaded_size;
	uint32_t m_id;

	buffer_object(const buffer_object&) = delete;
public:
	buffer_object();
	std::size_t get_size() const;
	std::size_t get_loaded_size() const;
	void reset_loaded_size();
	const uint32_t get_id() const;
	virtual ~buffer_object();
	virtual void load_data(const void* data, std::size_t m_size);
	virtual void load_sub_data(const void* data, std::size_t offset, std::size_t sub_size);
};
