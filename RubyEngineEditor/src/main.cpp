#include <iostream>
#include <memory>
#include <imgui/imgui.h>
#include <imgui_internal.h>

#include <RubyEngineCore/Input.hpp>
#include <RubyEngineCore/Application.hpp>

class RubyEngineEditor :public RubyEngine::Application
{
	double m_initial_mouse_pos_x = 0.0;
	double m_initial_mouse_pos_y = 0.0;

	virtual void on_update() override
	{
		glm::vec3 movement_delta{ 0, 0, 0 };
		glm::vec3 rotation_delta{ 0, 0, 0 };

		//translation

		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_W))
		{
			movement_delta.x += 0.05f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_S))
		{
			movement_delta.x -= 0.05f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_D))
		{
			movement_delta.y += 0.05f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_A))
		{
			movement_delta.y -= 0.05f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_SPACE))
		{
			movement_delta.z += 0.05f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_LEFT_CONTROL))
		{
			movement_delta.z -= 0.05f;
		}
		//rotation
		
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_RIGHT))
		{
			rotation_delta.z += 0.5f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_LEFT))
		{
			rotation_delta.z -= 0.5f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_UP))
		{
			rotation_delta.y += 0.5f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_DOWN))
		{
			rotation_delta.y -= 0.5f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_Q))
		{
			rotation_delta.x -= 0.5f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_E))
		{
			rotation_delta.x += 0.5f;
		}

		if (RubyEngine::Input::is_mouse_button_pressed(RubyEngine::MouseButton::MOUSE_BUTTON_RIGHT))
		{
			glm::vec2 current_cursor_position = get_current_cursor_position();
			if (RubyEngine::Input::is_mouse_button_pressed(RubyEngine::MouseButton::MOUSE_BUTTON_LEFT))
			{
				camera.move_right(static_cast<float>(current_cursor_position.x - m_initial_mouse_pos_x) / 100.f);
				camera.move_up(static_cast<float>(m_initial_mouse_pos_y - current_cursor_position.y) / 100.f);
			}
			else
			{
				rotation_delta.z += static_cast<float>(m_initial_mouse_pos_x - current_cursor_position.x) / 5.f;
				rotation_delta.y -= static_cast<float>(m_initial_mouse_pos_y - current_cursor_position.y) / 5.f;
			}
			m_initial_mouse_pos_x = current_cursor_position.x;
			m_initial_mouse_pos_y = current_cursor_position.y;
		}

		camera.add_movement_and_rotation(movement_delta, rotation_delta);
	}

	virtual void on_mouse_button_event(const RubyEngine::MouseButton mouse_button, const double x_pos, const double y_pos, const bool pressed) override
	{
		m_initial_mouse_pos_x = x_pos;
		m_initial_mouse_pos_y = y_pos;
	}

	void setup_dockspase_menu()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene...", NULL))
				{ }
				if (ImGui::MenuItem("Open Scene...", NULL))
				{ }
				if (ImGui::MenuItem("Save Scene...", NULL))
				{ }

				ImGui::Separator();
				if (ImGui::MenuItem("Exit", NULL))
				{
					close_app();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	virtual void on_ui_draw() override
	{
		setup_dockspase_menu();
		camera_position[0] = camera.get_position().x;
		camera_position[1] = camera.get_position().y;
		camera_position[2] = camera.get_position().z;

		camera_rotation[0] = camera.get_rotation().x;
		camera_rotation[1] = camera.get_rotation().y;
		camera_rotation[2] = camera.get_rotation().z;

		camera_fov = camera.get_field_of_view();
		camera_far_plane = camera.get_far_clip_plane();
		camera_near_plane = camera.get_near_clip_plane();

		ImGui::Begin("Editor");
		if (ImGui::SliderFloat3("Camera position", camera_position, -10.f, 10.f))
		{
			camera.set_position(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
		}
		if (ImGui::SliderFloat3("Camera rotation", camera_rotation, 0.f, 360.f));
		{
			camera.set_rotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
		}
		if (ImGui::SliderFloat("Camera FOV", &camera_fov, 1.f, 120.f));
		{
			camera.set_field_of_view(camera_fov);
		}
		if (ImGui::SliderFloat("Camera far plane", &camera_far_plane, 1.f, 100.f));
		{
			camera.set_far_clip_plane(camera_far_plane);
		}
		if (ImGui::SliderFloat("Camera near plane", &camera_near_plane, 0.1f, 10.f ));
		{
			camera.set_near_clip_plane(camera_near_plane);
		}
		if (ImGui::Checkbox("Perspective", &perspective_camera))
		{
			camera.set_projection_mode(perspective_camera ? RubyEngine::Camera::ProjectionMode::Perspective : RubyEngine::Camera::ProjectionMode::Orthogonal);
		}
		ImGui::End();
	}
};

int main()
{
	auto pRubyEngineEditor = std::make_unique<RubyEngineEditor>();

	int returnCode = pRubyEngineEditor->start_app(1024, 1024, "RubyEngine Editior");
	//std::cin.get();
	return returnCode;
}