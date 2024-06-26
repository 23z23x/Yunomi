#pragma once
#include "pch.h"
#include "Yunomi/Render/Window.h"
#include <GLFW/glfw3.h>

namespace ynm {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline void* getWindow() const override { return m_Window; }

		inline void SetEventCallback(const EventCallbackFunc& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void ResetMousePosition() const override;
	private:

		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFunc EventCallback;
		};

		WindowData m_Data;
	};
}

