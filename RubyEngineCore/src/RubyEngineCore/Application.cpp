#include <iostream>
#include "RubyEngineCore/Application.hpp"
#include "RubyEngineCore/Logs.hpp"
#include "RubyEngineCore/Window.hpp"


namespace RubyEngine
{
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
				LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				LOG_INFO("[Resized] Changet size to {0}x{1}", event.width, event.height);
			}
		);
		
		m_pWindow->set_event_callback(
			[&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			}
		);

		while (!m_bCloseWindow)
		{
			m_pWindow->on_update();
			on_update();
		}
        return 0;
	}
}