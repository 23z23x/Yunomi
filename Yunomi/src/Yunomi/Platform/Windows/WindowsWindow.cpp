#include "pch.h"
#include "WindowsWindow.h"

namespace ynm
{
	static bool s_GLFWInit = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		YNM_CORE_INFO("GLFW: Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInit)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			YNM_CORE_ASSERT(success, "GLFW: Failed to initialize!");

			s_GLFWInit = true;
		}

		//Tells glfw not to start OpenGL
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		glfwSetFramebufferSizeCallback(m_Window, Window::ResizeCallback);
		SetVSync(true);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);

		glfwTerminate();

		YNM_CORE_INFO("GLFW: Window closed and instance terminated!");
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	bool WindowsWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::ResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		resizeFramebuffer = true;
		YNM_CORE_INFO("GLFW: ResizeCallback called");
	}
}
