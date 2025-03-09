#include "pch.h"
#include "renderer.h"

std::mutex opengl_renderer::render_lock;
std::unique_ptr<vertex_array_object> opengl_renderer::m_vao;
std::unique_ptr<buffer_object> opengl_renderer::m_vbo;
std::unique_ptr<buffer_object> opengl_renderer::m_ibo;
glm::vec4 opengl_renderer::m_color;

static void opengl_log(GLenum source, GLenum type, GLuint m_id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
		}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
		}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
		}();

	LOGINFO("{}, {}, {}, {:d}, {}", src_str, type_str, severity_str, m_id, message);
}

void opengl_renderer::init(glm::vec4 color, uint32_t width, uint32_t height, bool is_poly_mode)
{
	const std::lock_guard<std::mutex> lock(render_lock);
	
	int err = gladLoadGL();
	if (0 == err)
	{
		LOGERROR("Error glagladLoadGL");
		return;
	}
	else {
		LOGINFO("OpenGL version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	m_color = color;
	
	set_view_port(0, 0, width, height);

	if (is_poly_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	// face culling, didn't test that works.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	create_new_batch();
}

void opengl_renderer::add_to_batch(std::vector<vertex>& vertexes, std::vector<uint32_t> indices)
{
	if (m_vbo->get_loaded_size() + vertexes.size() * sizeof(vertex) >= m_vbo->get_size() || m_ibo->get_loaded_size() + indices.size() * sizeof(uint32_t) >= m_ibo->get_size())
	{
		render();
	}
	
	const std::lock_guard<std::mutex> lock(render_lock);

	for (uint32_t& index : indices)
	{
		index += m_vbo->get_loaded_size()/sizeof(vertex);
	}

	m_vbo->load_sub_data(vertexes.data(), m_vbo->get_loaded_size(), vertexes.size() * sizeof(vertex));
	m_ibo->load_sub_data(indices.data(), m_ibo->get_loaded_size(), indices.size()*sizeof(uint32_t));
}

void opengl_renderer::render()
{
	const std::lock_guard<std::mutex> lock(render_lock);

	glClearColor(m_color.x, m_color.y, m_color.z, m_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_vao->bind_vbo(m_vbo->get_id(), vertex::info());
	m_vao->bind_ibo(m_ibo->get_id(), m_ibo->get_loaded_size()/sizeof(uint32_t));
	m_vao->bind();
	
	glDrawElements(GL_TRIANGLES, m_vao->get_element_count(), GL_UNSIGNED_INT, nullptr);

	prepare_for_next_render();
}

void opengl_renderer::set_view_port(int x, int y, std::size_t width, std::size_t height)
{
	glViewport(x, y, width, height);
}

void opengl_renderer::create_new_batch()
{
	m_vao.reset();
	m_vbo.reset();
	m_ibo.reset();

	m_vao = std::make_unique<vertex_array_object>();
	m_vbo = std::make_unique<buffer_object>();
	m_ibo = std::make_unique<buffer_object>();

	m_vbo->load_data(nullptr, max_vbo_byte_count);
	m_ibo->load_data(nullptr, max_vbo_byte_count);
}

void opengl_renderer::prepare_for_next_render()
{
	m_vao->set_element_count(0);
	m_vbo->reset_loaded_size();
	m_ibo->reset_loaded_size();
}
