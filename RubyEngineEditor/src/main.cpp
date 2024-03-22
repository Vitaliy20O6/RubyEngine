#include <iostream>
#include <memory>
#include <RubyEngineCore/Application.hpp>

class TestApp :public RubyEngine::Application
{
	int frame = 0;
	/*virtual void on_update() override
	{
		std::cout << "Update frame: " << frame++ << std::endl;
	}*/

};

int main()
{
	auto test_app = std::make_unique<TestApp>();

	int returnCode = test_app->startApp(1024,768, "RubyEngine Editior");
	std::cin.get();
	return returnCode;
}