#pragma once
#include "pch.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct win_deleter {
	void operator()(GLFWwindow* ptr);
};

class window {
private:
	std::unique_ptr<GLFWwindow, win_deleter> w;
	uint32_t m_width;
	uint32_t m_height;
	bool m_is_full_screen;
	const std::string m_name;
public:
	window(const std::string& name, uint32_t m_width, uint32_t m_height, bool use_full_screen);
	~window();

	void create();
	void make_opengl_context(int, int);
	void register_resize_callback(GLFWframebuffersizefun callback);
	void set_resolution(uint32_t m_width, uint32_t m_height);
	bool window_should_close();
	void swap_buffers();
	void poll_ivents();

	// bad function remv.
	inline glm::mat4 get_actions(glm::vec3& camera_pos, glm::vec3& up, glm::vec3& front, float& yaw, float& pitch)
	{
		if (glfwGetKey(w.get(), GLFW_KEY_W) == GLFW_PRESS)
		{
			camera_pos += 0.05f * front;
		}
		
		if (glfwGetKey(w.get(), GLFW_KEY_S) == GLFW_PRESS)
		{
			camera_pos -= 0.05f * front;
		}
		
		if (glfwGetKey(w.get(), GLFW_KEY_A) == GLFW_PRESS)
		{
			camera_pos -= 0.05f * glm::normalize(glm::cross(front, up));
		}
		
		if (glfwGetKey(w.get(), GLFW_KEY_D) == GLFW_PRESS)
		{
			camera_pos += 0.05f * glm::normalize(glm::cross(front, up));
		}

		if (glfwGetKey(w.get(), GLFW_KEY_Q) == GLFW_PRESS)
		{
			yaw -= 0.5f;
		}

		if (glfwGetKey(w.get(), GLFW_KEY_E) == GLFW_PRESS)
		{
			yaw += 0.5f;
		}

		if (glfwGetKey(w.get(), GLFW_KEY_Z) == GLFW_PRESS)
		{
			pitch -= 0.5f;
		
			if (pitch > 90.0f)
				pitch = 89.9f;
			
			if (pitch < -90.0f)
				pitch = -89.9f;
		}

		if (glfwGetKey(w.get(), GLFW_KEY_X) == GLFW_PRESS)
		{
			pitch += 0.5f;

			if (pitch > 90.0f)
				pitch = 89.9f;

			if (pitch < -90.0f)
				pitch = -89.9f;
		}

		front.y = sin(glm::radians(pitch));
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(front);

		auto target = camera_pos + front;
		glm::mat4 resultMatrix = glm::lookAt(camera_pos, target, up);

		return resultMatrix;
	}

private:
	window(const window&) = delete;
};