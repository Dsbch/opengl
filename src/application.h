#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include "core/opengl/renderer.h"
#include "core/opengl/camera.h"
#include "core/opengl/shader.h"
#include "core/opengl/texture.h"
#include "core/window/window.h"
#include "core/asset_manager/asset_manager.h"

static void opengl_log(GLenum source,
	GLenum attribute_type,
	GLuint m_id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

class application
{
private:
	static void resize_callback(GLFWwindow* window, int m_width, int height);
public:
	static std::unique_ptr<camera> m_camera;
	static std::unique_ptr<window> m_window;
	static std::unique_ptr<asset_manager> m_asset_manager;

	static void init();
	static void run();
	
private:
	static void render();
	// Просто вынужденная мера.
	static void update_state(std::shared_ptr<shader_program>);

	application() = delete;
	application(const application&) = delete;
};

