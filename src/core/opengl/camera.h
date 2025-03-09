#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class camera
{
private:
	float m_fov;
	float m_near;
	float m_far;
	float m_width;
	float m_height;

	glm::mat4 m_view_matrix;
	glm::mat4 m_projection_matrix;
	
	void update_transform_matrix();
public:
	camera(float fov, float near_plane, float far_plane, float width, float height);

	glm::vec3 m_up;
	glm::vec3 m_front;
	glm::vec3 m_position;
	float yaw;
	float pitch;

	inline void move(const glm::vec3 vec)
	{
		m_view_matrix = glm::translate(m_view_matrix, vec);
	}

	inline void apply(const glm::mat4 m)
	{
		m_view_matrix = m;
	}

	inline const glm::mat4& get_view_matrix() const
	{
		return m_view_matrix;
	}
	inline const glm::mat4& get_projection_matrix() const
	{
		return m_projection_matrix;
	}
	inline void set_fov(float fov)
	{
		m_fov = fov;
		update_transform_matrix();
	}
	inline void set_near(float near_plane)
	{
		m_near = near_plane;
		update_transform_matrix();
	}
	inline void set_far(float far_plane)
	{
		m_far = far_plane;
		update_transform_matrix();
	}
	inline void set_width(float width)
	{
		m_width = width;
		update_transform_matrix();
	}
	inline void set_height(float height)
	{
		m_height = height;
		update_transform_matrix();
	}
};
