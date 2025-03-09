#pragma once
#include "pch.h"
#include <glm/glm.hpp>
#include "buffer_object.h"
#include "vertex_array_object.h"
#include "vertex.h"
#include "camera.h"

// Max vbo size, 500 mb. TODO: query opengl in runtime.
static const std::size_t max_vbo_byte_count = 60e6;

class opengl_renderer
{
public:
	opengl_renderer() = delete;
	static void init(glm::vec4 color, uint32_t width, uint32_t height, bool is_poly_mode);
	static void add_to_batch(std::vector<vertex>& vertexes, std::vector<uint32_t> indices);
	static void render();
	static void set_view_port(int x, int y, std::size_t width, std::size_t height);
private:
	static void create_new_batch();
	static void prepare_for_next_render();

	static std::mutex render_lock;
	static std::unique_ptr<vertex_array_object> m_vao;
	static std::unique_ptr<buffer_object> m_vbo;
	static std::unique_ptr<buffer_object> m_ibo;
	static glm::vec4 m_color;
};