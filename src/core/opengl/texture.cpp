#include "pch.h"
#include "texture.h"

std::atomic_int texture::occupied_slots = 0;

void texture::bind()
{
	if (m_slot_id != 0)
	{
		return;
	}

	m_slot_id = next_texture_slot();
	if (m_slot_id == 0)
	{
		throw std::overflow_error("Reached max texture slots");
	}

	glBindTextureUnit(m_slot_id, m_id);
}

texture::texture(uint8_t* image_data, int width, int height, image_channel channel)
	: m_width(width), m_height(height), m_channel(channel), m_id(0), m_slot_id(0)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTextureStorage2D(m_id, 1, GL_RGB8, m_width, m_height);
	glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glGenerateTextureMipmap(m_id);
}

texture::~texture()
{
	if (m_slot_id != 0)
	{
		occupied_slots--;
	}

	glDeleteTextures(1, &m_id);
}

const uint32_t texture::get_id() const
{
	return m_id;
}

const uint32_t texture::get_slot_id()
{
	return m_slot_id;
}

uint32_t texture::next_texture_slot()
{
	int max_slots;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_slots);

	if (occupied_slots >= max_slots)
	{
		return 0;
	}

	return ++occupied_slots;
}
