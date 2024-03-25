	#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include <RubyEngineCore/Input.hpp>
#include <RubyEngineCore/Application.hpp>

class RubyEngineEditor :public RubyEngine::Application
{
	int frame = 0;

	double m_initial_mouse_pos_x = 0.0;
	double m_initial_mouse_pos_y = 0.0;

	virtual void on_update() override
	{
		glm::vec3 movement_delta{ 0, 0, 0 };
		glm::vec3 rotation_delta{ 0, 0, 0 };

		//translation

		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_W))
		{
			movement_delta.x += 0.025f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_S))
		{
			movement_delta.x -= 0.025f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_D))
		{
			movement_delta.y += 0.025f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_A))
		{
			movement_delta.y -= 0.025f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_SPACE))
		{
			movement_delta.z += 0.025f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_LEFT_CONTROL))
		{
			movement_delta.z -= 0.025f;
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
			glm::vec2 current_cursor_position = get_current_cursor_pos();
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

	virtual void on_mouse_button_event(const RubyEngine::MouseButton mouse_button,
									   const double x_pos, 
									   const double y_pos,
									   const bool pressed) override
	{
		m_initial_mouse_pos_x = x_pos;
		m_initial_mouse_pos_y = y_pos;
	}

	virtual void on_ui_draw() override
	{
		camera_position[0] = camera.get_camera_position().x;
		camera_position[1] = camera.get_camera_position().y;
		camera_position[2] = camera.get_camera_position().z;

		camera_rotation[0] = camera.get_camera_rotation().x;
		camera_rotation[1] = camera.get_camera_rotation().y;
		camera_rotation[2] = camera.get_camera_rotation().z;

		ImGui::Begin("Editor");
		if (ImGui::SliderFloat3("Camera position", camera_position, -10.f, 10.f))
		{
			camera.set_position(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
		}
		if (ImGui::SliderFloat3("Camera rotation", camera_rotation, 0.f, 360.f));
		{
			camera.set_rotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
		}
		ImGui::Checkbox("Perspective", &perspective_camera);
		ImGui::End();
	}
};

int main()
{
	auto pRubyEngineEditor = std::make_unique<RubyEngineEditor>();

	int returnCode = pRubyEngineEditor->startApp(1024,768, "RubyEngine Editior");
	std::cin.get();
	return returnCode;
}