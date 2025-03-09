#include "pch.h"
#include "application.h"
#include <chrono>

struct config {
	const std::string app_name = "OPENGL_EXPLORING";
	const std::string enviroment = "DEBUG";
	const spdlog::level::level_enum log_level = spdlog::level::debug;
	const std::string log_pattern = "[%l] %T %v%$";
	const uint32_t width = 640;
	const uint32_t height = 480;
	const bool use_full_screen = false;
	const float near_plane = 0.1f;
	const float far_plane = 1000.0f;
	const float fov = 90.0f;
};

std::unique_ptr<camera> application::m_camera;
std::unique_ptr<window> application::m_window;
std::unique_ptr<asset_manager> application::m_asset_manager;

void application::resize_callback(GLFWwindow* window, int width, int height)
{
	LOGINFO("Window was resized: w: {:d} h: {:d}", width, height);
	
	opengl_renderer::set_view_port(0, 0, width, height);

	if (width)
	{
		m_camera->set_width(width);
	}

	if (height)
	{
		m_camera->set_height(height);
	}
}

void application::init()
{
	config cfg;

	logger::init_logger(cfg.app_name, cfg.log_level, cfg.log_pattern);

	m_camera = std::make_unique<camera>(cfg.fov, cfg.near_plane, cfg.far_plane, cfg.width, cfg.height);
	m_asset_manager = std::make_unique<asset_manager>();
	m_window = std::make_unique<window>(cfg.app_name, cfg.width, cfg.height, cfg.use_full_screen);

	m_window->create();
	m_window->register_resize_callback(resize_callback);

	m_window->make_opengl_context(4, 4);

	opengl_renderer::init(glm::vec4(0.2f, 0.24f, 0.4f, 1.0f), cfg.width, cfg.height, false);

	// компиляция шейдеров.
	m_asset_manager->load_and_compile_shader("../assets/shaders/vertex.glsl", "../assets/shaders/fragment.glsl");
}

static void processMesh(aiMesh* mesh, std::vector<vertex>& vertexes, std::vector<uint32_t>& indixes, aiMatrix4x4 transform, size_t index_shift)
{
	// load vertex data, vbo.
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		vertex v;
		// set posistion.
		v.position.x = mesh->mVertices[i].x;
		v.position.y = mesh->mVertices[i].y;
		v.position.z = mesh->mVertices[i].z;
		v.text_index = 0;
		
		aiVector3D p(v.position.x, v.position.y, v.position.z);
		p *= transform;

		v.position.x = p.x;
		v.position.y = p.y;
		v.position.z = p.z;

		// set text coords.
		if (mesh->mTextureCoords[0])
		{
			v.tex_coord.x = mesh->mTextureCoords[0][i].x;
			v.tex_coord.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			v.tex_coord = glm::vec2(0.0f);
		}

		vertexes.push_back(v);
	}

	// load indixes.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indixes.push_back(face.mIndices[j] + index_shift);
		}
	}

	return;
}

static void process_node_test(aiNode* node, const aiScene* scene, std::vector<vertex>& vertexes, std::vector<uint32_t>& indixes, aiMatrix4x4 transform)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, vertexes, indixes, transform, vertexes.size());
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_node_test(node->mChildren[i], scene, vertexes, indixes, transform*node->mChildren[i]->mTransformation);
	}
}

static void import_model(const std::string& path, std::vector<vertex>& vertexes, std::vector<uint32_t>& indices)
{
	static Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_RemoveRedundantMaterials
	);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOGERROR("loading assimp model {s}", importer.GetErrorString());
		return;
	}

	process_node_test(scene->mRootNode, scene, vertexes, indices, scene->mRootNode->mTransformation);
}

void application::run()
{
	// Все что ниже нужно будет вынести в классы material, model и asset_manager.
	std::vector<vertex> vertexes;
	std::vector<uint32_t> indexes;
	import_model("../assets/models/Survival_BackPack_2.fbx", vertexes, indexes);

	//std::vector<vertex> vertexes1;
	//std::vector<uint32_t> indexes1;
	//import_model("../assets/models/shifted_chuchillo.glb", vertexes1, indexes1);

	auto t = m_asset_manager->load_texture("../assets/textures/1001_albedo.jpg");
	try
	{
		t->bind();
	}
	catch (const std::overflow_error& e)
	{
		LOGERROR(e.what());
	}

	int sample_arr[] = { t->get_slot_id() };
	auto program = m_asset_manager->get_compiled_shader("../assets/shaders/vertex.glsl");

	program->set_uniform_int(shader_program::texture_samplers_uni_name, sample_arr, 1);
	program->set_uniform_mat4(shader_program::projection_matrix_uni_name, glm::value_ptr(application::m_camera->get_projection_matrix()), 1);
	program->set_uniform_mat4(shader_program::view_matrix_uni_name, glm::value_ptr(application::m_camera->get_view_matrix()), 1);

	program->bind();

	///////////////////////////////////////////////// Все что выше, ну ты понял.

#ifndef now
#define now std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
#endif // !now
	
	uint32_t frames_per_seconds = 120;
	std::chrono::milliseconds update_shift = std::chrono::milliseconds(1000 / frames_per_seconds);
	std::chrono::milliseconds next_update = now;
	const int frames_to_skip = 24;
	const int per_loop = 1000/(1000/ frames_per_seconds * frames_to_skip);

	while (!m_window->window_should_close())
	{
		for (int i = 0; now >= next_update && i < per_loop; i++)
		{
			update_state(program);
			next_update += update_shift;
		}

		opengl_renderer::add_to_batch(vertexes, indexes);
		render();
	}
}

void application::render()
{
	//opengl_renderer::add_to_batch(vertexes1, indexes1);
	opengl_renderer::render();
}

void application::update_state(std::shared_ptr<shader_program> program)
{
	program->set_uniform_mat4(shader_program::projection_matrix_uni_name, glm::value_ptr(application::m_camera->get_projection_matrix()), 1);
	program->set_uniform_mat4(shader_program::view_matrix_uni_name, glm::value_ptr(application::m_camera->get_view_matrix()), 1);
	
	m_window->swap_buffers();
	m_window->poll_ivents();
	m_camera->apply(m_window->get_actions(m_camera->m_position, m_camera->m_up, m_camera->m_front, m_camera->yaw, m_camera->pitch));
}
