#pragma once
#include "pch.h"
#include <stb_image.h>
#include "../opengl/texture.h"
#include "../opengl/shader.h"
#include "../file/file.h"

class asset_manager
{
private:
	std::map<std::string, std::shared_ptr<texture>> m_loaded_textures;
	std::map<std::string, std::shared_ptr<shader_program>> m_compiled_shaders;
public:
	asset_manager() = default;
	const std::shared_ptr<texture> get_texture(const std::string& id);
	const std::shared_ptr<texture>  load_texture(const std::string& path);
	const std::shared_ptr<shader_program> get_compiled_shader(const std::string& id);
	const std::shared_ptr<shader_program> load_and_compile_shader(const std::string& v_sh_path, const std::string& f_sh_path);
};
