#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include <RubyEngineCore/Input.hpp>
#include <RubyEngineCore/Application.hpp>

class RubyEngineEditor :public RubyEngine::Application
{
	int frame = 0;
	virtual void on_update() override
	{
		//translation
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_W))
		{
			camera_position[2] -= 0.01f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_S))
		{
			camera_position[2] += 0.01f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_D))
		{
			camera_position[0] += 0.01f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_A))
		{
			camera_position[0] -= 0.01f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_SPACE))
		{
			camera_position[1] += 0.01f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_LEFT_CONTROL))
		{
			camera_position[1] -= 0.01f;
		}

		//rotation
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_RIGHT))
		{
			camera_rotation[1] -= 0.5f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_LEFT))
		{
			camera_rotation[1] += 0.5f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_UP))
		{
			camera_rotation[0] += 0.5f;
		}
		if (RubyEngine::Input::is_key_pressed(RubyEngine::KeyCode::KEY_DOWN))
		{
			camera_rotation[0] -= 0.5f;
		}
	}
	virtual void on_ui_draw() override
	{
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("Camera position", camera_position, -10.f, 10.f);
		ImGui::SliderFloat3("Camera rotation", camera_rotation, 0.f, 360.f);
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