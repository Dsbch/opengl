#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "asset_manager.h"

const std::shared_ptr<texture> asset_manager::get_texture(const std::string& id)
{
	auto it = m_loaded_textures.find(id);
	if (it != m_loaded_textures.end())
	{
		return it->second;
	}
	else {
		throw std::runtime_error("tried to access not loaded texture.");
	}
}

const std::shared_ptr<shader_program> asset_manager::get_compiled_shader(const std::string& id)
{
	auto it = m_compiled_shaders.find(id);
	if (it != m_compiled_shaders.end())
	{
		return it->second;
	}
	else {
		throw std::runtime_error("tried to access not loaded shader.");
	}
}

const std::shared_ptr<texture> asset_manager::load_texture(const std::string& path)
{
	int width, height, nr_channels;
	uint8_t* data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
	if (!data)
	{
		throw std::invalid_argument("can't load texture");
	}

	auto t = std::make_shared<texture>(data, width, height, (image_channel)nr_channels);

	stbi_image_free(data);

	m_loaded_textures[path] = t;

	return m_loaded_textures[path];
}

const std::shared_ptr<shader_program> asset_manager::load_and_compile_shader(const std::string& v_sh_path, const std::string& f_sh_path)
{
	std::string vs = open_and_read(v_sh_path);
	std::string fs = open_and_read(f_sh_path);
	auto program = std::make_shared<shader_program>(fs, vs);
	program->compile();

	m_compiled_shaders[v_sh_path] = program;

	return m_compiled_shaders[v_sh_path];
}
