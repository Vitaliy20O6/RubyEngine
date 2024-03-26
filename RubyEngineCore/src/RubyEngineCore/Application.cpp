#include "RubyEngineCore/Application.hpp"
#include "RubyEngineCore/Logs.hpp"
#include "RubyEngineCore/Window.hpp"
#include "RubyEngineCore/Camera.hpp"
#include "RubyEngineCore/Event.hpp"
#include "RubyEngineCore/Input.hpp"

#include "RubyEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "RubyEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "RubyEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "RubyEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "RubyEngineCore/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "RubyEngineCore/Rendering/OpenGL/Texture2D.hpp"
#include "RubyEngineCore/Modules/UIModule.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

namespace RubyEngine
{
	GLfloat positions_colors[] = {
		0.0f, -0.5f, -0.5f,    1.0f,  1.0f, 0.0f,	1.f,  0.f,
		0.0f,  0.5f, -0.5f,    0.0f,  1.0f, 1.0f,	0.f,  0.f,
		0.0f, -0.5f,  0.5f,    1.0f,  0.0f, 1.0f,	1.f,  1.f,
		0.0f,  0.5f,  0.5f,    1.0f,  0.0f, 0.0f,	0.f,  1.f
	};

	GLuint indices[] = {
		0,1,2,3,2,1
	};

	void generate_circle(unsigned char* data,
						 const unsigned int width,
						 const unsigned int height,
						 const unsigned int center_x,
						 const unsigned int center_y,
						 const unsigned int radius,
						 const unsigned char color_r,
						 const unsigned char color_g,
						 const unsigned char color_b)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius)
				{
					data[3 * (x + width * y) + 0] = color_r;
					data[3 * (x + width * y) + 1] = color_g;
					data[3 * (x + width * y) + 2] = color_b;
				}
			}
		}
	}

	void generate_smile_texture(unsigned char* data,
							    const unsigned int width,
							    const unsigned int height)
	{
		// background
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				data[3 * (x + width * y) + 0] = 200;
				data[3 * (x + width * y) + 1] = 191;
				data[3 * (x + width * y) + 2] = 231;
			}
		}

		// face
		generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);

		// smile
		generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
		generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);

		// eyes
		generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
		generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);
	}

	void generate_quads_texture(unsigned char* data,
								const unsigned int width,
								const unsigned int height)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				if ((x < width / 2 && y < height / 2) || x >= width / 2 && y >= height / 2)
				{
					data[3 * (x + width * y) + 0] = 0;
					data[3 * (x + width * y) + 1] = 0;
					data[3 * (x + width * y) + 2] = 0;
				}
				else
				{
					data[3 * (x + width * y) + 0] = 255;
					data[3 * (x + width * y) + 1] = 255;
					data[3 * (x + width * y) + 2] = 255;
				}
			}
		}
	}


	const char* vertex_shader =
		R"(#version 460
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec3 vertex_color;
        layout(location = 2) in vec2 texture_coord;


        uniform mat4 model_matrix;
        uniform mat4 view_projection_matrix;
		uniform int current_frame; 

        out vec3 color;
		out vec2 tex_coord;
        out vec2 tex_coord_smile;
        out vec2 tex_coord_quads;

        void main() {
            color = vertex_color;
			tex_coord_smile = texture_coord;
            tex_coord_quads = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);
            gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
        }
        )";

	const char* fragment_shader =
		R"(#version 460
        in vec3 color;
		in vec2 tex_coord_smile;
        in vec2 tex_coord_quads;

        layout (binding = 0) uniform sampler2D InTexture_Smile;
        layout (binding = 1) uniform sampler2D InTexture_Quads;

        out vec4 frag_color;

        void main() {
			//frag_color = vec4(color, 1.0);
			frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);
			//frag_color = texture(InTexture_Quads, tex_coord_quads);
			//frag_color = texture(InTexture_Smile, tex_coord_smile);
        }
        )";

	std::unique_ptr<ShaderProgram> p_shader_program;
	std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
	std::unique_ptr<IndexBuffer> p_index_buffer;
	std::unique_ptr<Texture2D> p_texture_smile;
	std::unique_ptr<Texture2D> p_texture_quads;
	std::unique_ptr<VertexArray> p_vao;
	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };

	float m_background_color[4] = { 0.3125f, 0.078125f, 0.46875f, 0.f };

	Application::Application()
	{
        LOG_INFO("Starting application");
	}

	Application::~Application()
	{
		LOG_INFO("Closing application");
	}

	int Application::startApp(unsigned int window_width, unsigned int window_height, const char* title)
	{
		m_pWindow = std::make_unique<Window>(title, window_width, window_height);
		
		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{
				//LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				LOG_INFO("[Resized] Changet size to {0}x{1}", event.width, event.height);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event)
			{
				LOG_INFO("[WindowClose]");
				m_bCloseWindow = true;
			}
		);

		m_event_dispatcher.add_event_listener<EventKeyPressed>(
			[&](EventKeyPressed& event)
			{
				if (event.key_code <= KeyCode::KEY_Z)
				{
					if (event.repeated)
					{
						LOG_INFO("[KeyPressed] {0}, Repeated", static_cast<char>(event.key_code));
					}
					else
					{
						LOG_INFO("[KeyPressed] {0}", static_cast<char>(event.key_code));
					}
				}				
				Input::PressKey(event.key_code);
			}
		);

		m_event_dispatcher.add_event_listener<EventKeyReleased>(
			[&](EventKeyReleased& event)
			{
				if (event.key_code <= KeyCode::KEY_Z)
				{
					LOG_INFO("[KeyReleased] {0}", static_cast<char>(event.key_code));
				}
				Input::ReleaseKey(event.key_code);
			}
		);

		m_event_dispatcher.add_event_listener<EventMouseButtonPressed>(
			[&](EventMouseButtonPressed& event)
			{
				LOG_INFO("[MouseButtonPressed] {0} at {1}x{2}", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
				Input::PressMouseButton(event.mouse_button);
				on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, true);
			}
		);

		m_event_dispatcher.add_event_listener<EventMouseButtonReleased>(
			[&](EventMouseButtonReleased& event)
			{
				LOG_INFO("[MouseButtonReleased] {0} at {1}x{2}", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
				Input::ReleaseMouseButton(event.mouse_button);
				on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, false);
			}
		);
		
		m_pWindow->set_event_callback(
			[&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			}
		);

		const unsigned int width = 1000;
		const unsigned int height = 1000;
		const unsigned int channels = 3;

		auto* data = new unsigned char[width * height * channels];

		generate_smile_texture(data, width, height);
		p_texture_smile = std::make_unique<Texture2D>(data,width,height);
		p_texture_smile->bind(0);

		generate_quads_texture(data, width, height);
		p_texture_quads = std::make_unique<Texture2D>(data, width, height);
		p_texture_quads->bind(1);

		delete[] data;

		//--------------------------------------------------------------//
		p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
		if (!p_shader_program->is_compiled())
		{
			return false;
		}

		BufferLayout buffer_layout_vec3_vec3_vec2
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3,
			ShaderDataType::Float2

		};

		p_vao = std::make_unique<VertexArray>();
		p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), buffer_layout_vec3_vec3_vec2);
		p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		p_vao->add_vertex_buffer(*p_positions_colors_vbo);
		p_vao->set_index_buffer(*p_index_buffer);
		//--------------------------------------------------------------//

		static int current_frame = 0;	

		while (!m_bCloseWindow)
		{
			Renderer_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
			Renderer_OpenGL::clear();

			p_shader_program->bind();
			// SCALE
			glm::mat4 scale_matrix(
				scale[0], 0, 0, 0,
				0, scale[1], 0, 0,
				0, 0, scale[2], 0,
				0, 0, 0, 1
			);
			//

			// ROTATE
			float rotate_radians = glm::radians(rotate);

			glm::mat4 rotate_matrix(
				cos(rotate_radians), sin(rotate_radians), 0, 0,
				-sin(rotate_radians), cos(rotate_radians), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			//

			// TRANSLATE
			glm::mat4 translate_matrix(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate[0], translate[1], translate[2], 1
			);
			//
			glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
			p_shader_program->set_matrix4("model_matrix", model_matrix);
			p_shader_program->set_int("current_frame", current_frame++);

			camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthogonal);

			p_shader_program->set_matrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());

			Renderer_OpenGL::draw(*p_vao);

			//--------------------------------------------------------------//
			UIModule::on_ui_draw_begin();
			bool show = true;
			UIModule::ShowExampleAppDockSpace(&show);
			ImGui::ShowDemoWindow();
			ImGui::Begin("Background Color");
			ImGui::ColorEdit4("Change color", m_background_color);
			ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);
			ImGui::SliderFloat("Rotation", &rotate, 0.f, 360.f);
			ImGui::SliderFloat3("Translation", translate, -1.f, 1.f);
			ImGui::SliderFloat3("Camera position", camera_position, -10.f, 10.f);
			ImGui::SliderFloat3("Camera rotation", camera_rotation, 0.f, 360.f);
			ImGui::Checkbox("Perspective", &perspective_camera);
			ImGui::End();
			//--------------------------------------------------------------//

			on_ui_draw();
			UIModule::on_ui_draw_end();

			m_pWindow->on_update();
			on_update();
		}

		m_pWindow = nullptr;
        return 0;
	}

	glm::vec2 Application::get_current_cursor_position() const
	{
		return m_pWindow->get_current_cursor_pos();
	}
}