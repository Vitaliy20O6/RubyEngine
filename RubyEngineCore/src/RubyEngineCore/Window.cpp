#include "RubyEngineCore/Window.hpp"
#include "RubyEngineCore/Logs.hpp"

#include "RubyEngineCore/Modules/UIModule.hpp"
#include "RubyEngineCore/Rendering/OpenGL/Renderer_OpenGL.hpp"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace RubyEngine
{
	Window::Window(std::string title, const unsigned int width, const unsigned int height)
        :m_data({ std::move(title), width, height })
	{
		int resultCode = init();
	}

	Window::~Window()
	{
		shutdown();
	}

	int Window::init()
	{
        LOG_INFO("Creating window \"{0}\" window size: {1}x{2}", m_data.title, m_data.width, m_data.height);

        glfwSetErrorCallback([](int error_code, const char* description)
        {
                LOG_CRIT("GLFW error: {0}", description);
        });

        if (!glfwInit())
        {
            LOG_CRIT("Can't initialize GLFW!");
            return -1;
        }

        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRIT("Can't create window {0}", m_data.title);
            return -2;
        }

        if (!Renderer_OpenGL::init(m_pWindow))
        {
            LOG_CRIT("Failed to init OpenGL renderer");
            return -3;
        }

        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;
                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(m_pWindow,
            [](GLFWwindow* pWindow, double x, double y)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(m_pWindow,
            [](GLFWwindow* pWindow)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventWindowClose event;
                data.eventCallbackFn(event);
            }
        );

        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                Renderer_OpenGL::set_viewport(width, height);
            }
        );

        UIModule::on_window_create(m_pWindow);

        return 0;
	}

    void Window::on_update()
    {
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }

    void Window::shutdown()
    {
        UIModule::on_window_close();
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}
}
