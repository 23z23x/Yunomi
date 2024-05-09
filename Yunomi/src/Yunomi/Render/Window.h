#pragma once

#include "pch.h"

//Interface to be implemented per API

namespace ynm {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Yunomi",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};


	class YNM_API Window
	{
	public:
		using EventCallbackFunc = std::function<void(/*Event&*/)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void* getWindow() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual bool ShouldClose() const = 0;
		static void ResizeCallback(GLFWwindow* window, int width, int height);

		static Window* Create(const WindowProps& props = WindowProps());
	};

}