#include "pch.h"
#include "window.h"

void win_deleter::operator()(GLFWwindow* ptr)
{
	if (ptr)
	{
		glfwDestroyWindow(ptr);
	}
}

window::window(const std::string& name, uint32_t m_width, uint32_t m_height, bool use_full_screen)
	: m_name(name), m_width(m_width), m_height(m_height), m_is_full_screen(use_full_screen)
{ 
	if (!glfwInit())
	{
		throw std::runtime_error("glfw init err");
	}
}

window::~window()
{
	glfwTerminate();
}

void window::create()
{
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	w = std::unique_ptr<GLFWwindow, win_deleter>(glfwCreateWindow(m_width, m_height, m_name.c_str(), m_is_full_screen ? glfwGetPrimaryMonitor() : nullptr, nullptr));
	if (!w)
	{
		throw std::runtime_error("failed to create window");
	}
	else {
		LOGINFO("window {} created", m_name);
	}
}

void window::register_resize_callback(GLFWframebuffersizefun callback)
{
	glfwSetFramebufferSizeCallback(w.get(), callback);
}

void window::make_opengl_context(int major_version, int minor_version)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwMakeContextCurrent(w.get());
}

void window::set_resolution(uint32_t m_width, uint32_t m_height)
{
	glfwSetWindowSize(w.get(), m_width, m_height);
	this->m_width = m_width;
	this->m_height = m_height;
}

bool window::window_should_close()
{
	return glfwWindowShouldClose(w.get());
}

void window::swap_buffers()
{
	glfwSwapBuffers(w.get());
}

void window::poll_ivents()
{
	glfwPollEvents();
}

