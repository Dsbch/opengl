#include "pch.h"
#include "camera.h"

void camera::update_transform_matrix()
{
	m_projection_matrix = glm::perspective(glm::radians(m_fov), m_width / m_height, m_near, m_far);
}

camera::camera(float fov, float near_plane, float far_plane, float width, float height) :
	m_fov(fov), m_near(near_plane), m_far(far_plane), m_width(width), m_height(height), m_front(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f), m_position(0.0f, 0.0f, 1.0f), yaw(-90.0f), pitch(0.0f)
{
	m_projection_matrix = glm::perspective(glm::radians(m_fov), m_width / m_height, m_near, m_far);
	
	m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
}
