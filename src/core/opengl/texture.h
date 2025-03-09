#pragma once
#include "pch.h"
#include <glad/glad.h>

enum image_channel {
	grayscale = 1,
	rgb = 3,
	rgba = 4,
};

class texture
{
public:
	texture(const texture&) = delete;
	texture(uint8_t* data, int m_width, int heigth, image_channel m_channel);
	~texture();
	void bind();
	const uint32_t get_id() const;
	const uint32_t get_slot_id();
private:
	uint32_t m_id;
	uint32_t m_slot_id;
	uint32_t m_width;
	uint32_t m_height;
	image_channel m_channel;

	static std::atomic_int occupied_slots;
	static uint32_t next_texture_slot();
};

