#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include <RubyEngineCore/Application.hpp>

class RubyEngineEditor :public RubyEngine::Application
{
	int frame = 0;
	virtual void on_update() override
	{
		//std::cout << "Update frame: " << frame++ << std::endl;
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