#pragma once
#include "pch.h"
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include "vertex_array_object.h"

struct vertex {
	glm::vec3 position;
	glm::vec2 tex_coord;
	float text_index;

	inline static const vertex_info info()
	{
		vertex_info result;
		result.stride = sizeof(vertex);

		result.attr_info = {
			// Position.
			{
				false,
				GL_FLOAT,
				3,
				offsetof(vertex, vertex::position),
			},
			// Texture coords.
			{
				false,
				GL_FLOAT,
				2,
				offsetof(vertex, vertex::tex_coord),
			},
			// Texture index.
			{
				false,
				GL_FLOAT,
				1,
				offsetof(vertex, vertex::text_index),
			}
		};

		return result;
	}
};

static_assert(std::is_pod<vertex>()(), "vertex not a POD");
