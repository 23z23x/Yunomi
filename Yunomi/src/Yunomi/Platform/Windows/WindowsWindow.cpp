#include "pch.h"
#include "WindowsWindow.h"

#include "Yunomi/Event/AppEvent.h"
#include "Yunomi/Event/InputEvent.h"

namespace ynm
{

	InputCode GLFWToYNMInput(int code)
	{
		switch (code)
		{
		case GLFW_MOUSE_BUTTON_1:
			return InputCode::M1;
		case GLFW_MOUSE_BUTTON_2:
			return InputCode::M2;
		case GLFW_MOUSE_BUTTON_3:
			return InputCode::M3;
		case GLFW_MOUSE_BUTTON_4:
			return InputCode::M4;
		case GLFW_MOUSE_BUTTON_5:
			return InputCode::M5;
		case GLFW_MOUSE_BUTTON_6:
			return InputCode::M6;
		case GLFW_MOUSE_BUTTON_7:
			return InputCode::M7;
		case GLFW_MOUSE_BUTTON_8:
			return InputCode::M8;
		case GLFW_KEY_SPACE:
			return InputCode::SPACE;
		case GLFW_KEY_APOSTROPHE:
			return InputCode::APOST;
		case GLFW_KEY_COMMA:
			return InputCode::COMMA;
		case GLFW_KEY_MINUS:
			return InputCode::MINUS;
		case GLFW_KEY_PERIOD:
			return InputCode::PERIOD;
		case GLFW_KEY_SLASH:
			return InputCode::SLASH;
		case GLFW_KEY_0:
			return InputCode::ZERO;
		case GLFW_KEY_1:
			return InputCode::ONE;
		case GLFW_KEY_2:
			return InputCode::TWO;
		case GLFW_KEY_3:
			return InputCode::THREE;
		case GLFW_KEY_4:
			return InputCode::FOUR;
		case GLFW_KEY_5:
			return InputCode::FIVE;
		case GLFW_KEY_6:
			return InputCode::SIX;
		case GLFW_KEY_7:
			return InputCode::SEVEN;
		case GLFW_KEY_8:
			return InputCode::EIGHT;
		case GLFW_KEY_9:
			return InputCode::NINE;
		case GLFW_KEY_SEMICOLON:
			return InputCode::SEMICOL;
		case GLFW_KEY_EQUAL:
			return InputCode::EQUAL;
		case GLFW_KEY_A:
			return InputCode::A;
		case GLFW_KEY_B:
			return InputCode::B;
		case GLFW_KEY_C:
			return InputCode::C;
		case GLFW_KEY_D:
			return InputCode::D;
		case GLFW_KEY_E:
			return InputCode::E;
		case GLFW_KEY_F:
			return InputCode::F;
		case GLFW_KEY_G:
			return InputCode::G;
		case GLFW_KEY_H:
			return InputCode::H;
		case GLFW_KEY_I:
			return InputCode::I;
		case GLFW_KEY_J:
			return InputCode::J;
		case GLFW_KEY_K:
			return InputCode::K;
		case GLFW_KEY_L:
			return InputCode::L;
		case GLFW_KEY_M:
			return InputCode::M;
		case GLFW_KEY_N:
			return InputCode::N;
		case GLFW_KEY_O:
			return InputCode::O;
		case GLFW_KEY_P:
			return InputCode::P;
		case GLFW_KEY_Q:
			return InputCode::Q;
		case GLFW_KEY_R:
			return InputCode::R;
		case GLFW_KEY_S:
			return InputCode::S;
		case GLFW_KEY_T:
			return InputCode::T;
		case GLFW_KEY_U:
			return InputCode::U;
		case GLFW_KEY_V:
			return InputCode::V;
		case GLFW_KEY_W:
			return InputCode::W;
		case GLFW_KEY_X:
			return InputCode::X;
		case GLFW_KEY_Y:
			return InputCode::Y;
		case GLFW_KEY_Z:
			return InputCode::Z;
		case GLFW_KEY_LEFT_BRACKET:
			return InputCode::LFT_BRCKT;
		case GLFW_KEY_BACKSLASH:
			return InputCode::BCKLSH;
		case GLFW_KEY_RIGHT_BRACKET:
			return InputCode::RGT_BRCKT;
		case GLFW_KEY_ESCAPE:
			return InputCode::ESCAPE;
		case GLFW_KEY_ENTER:
			return InputCode::ENTER;
		case GLFW_KEY_TAB:
			return InputCode::TAB;
		case GLFW_KEY_BACKSPACE:
			return InputCode::BCKSPCE;
		case GLFW_KEY_INSERT:
			return InputCode::INSERT;
		case GLFW_KEY_DELETE:
			return InputCode::DLT;
		case GLFW_KEY_RIGHT:
			return InputCode::RIGHT;
		case GLFW_KEY_LEFT:
			return InputCode::LEFT;
		case GLFW_KEY_UP:
			return InputCode::UP;
		case GLFW_KEY_DOWN:
			return InputCode::DOWN;
		case GLFW_KEY_PAGE_UP:
			return InputCode::PG_UP;
		case GLFW_KEY_PAGE_DOWN:
			return InputCode::PG_DN;
		case GLFW_KEY_HOME:
			return InputCode::HOME;
		case GLFW_KEY_END:
			return InputCode::END;
		case GLFW_KEY_CAPS_LOCK:
			return InputCode::CAP_LCK;
		case GLFW_KEY_SCROLL_LOCK:
			return InputCode::SCRL_LCK;
		case GLFW_KEY_NUM_LOCK:
			return InputCode::NM_LCK;
		case GLFW_KEY_PRINT_SCREEN:
			return InputCode::PRNT_SCRN;
		case GLFW_KEY_PAUSE:
			return InputCode::PAUSE;
		case GLFW_KEY_F1:
			return InputCode::F1;
		case GLFW_KEY_F2:
			return InputCode::F2;
		case GLFW_KEY_F3:
			return InputCode::F3;
		case GLFW_KEY_F4:
			return InputCode::F4;
		case GLFW_KEY_F5:
			return InputCode::F5;
		case GLFW_KEY_F6:
			return InputCode::F6;
		case GLFW_KEY_F7:
			return InputCode::F7;
		case GLFW_KEY_F8:
			return InputCode::F8;
		case GLFW_KEY_F9:
			return InputCode::F9;
		case GLFW_KEY_F10:
			return InputCode::F10;
		case GLFW_KEY_F11:
			return InputCode::F11;
		case GLFW_KEY_F12:
			return InputCode::F12;
		case GLFW_KEY_KP_0:
			return InputCode::KP_ZERO;
		case GLFW_KEY_KP_1:
			return InputCode::KP_ONE;
		case GLFW_KEY_KP_2:
			return InputCode::KP_TWO;
		case GLFW_KEY_KP_3:
			return InputCode::KP_THREE;
		case GLFW_KEY_KP_4:
			return InputCode::KP_FOUR;
		case GLFW_KEY_KP_5:
			return InputCode::KP_FIVE;
		case GLFW_KEY_KP_6:
			return InputCode::KP_SIX;
		case GLFW_KEY_KP_7:
			return InputCode::KP_SEVEN;
		case GLFW_KEY_KP_8:
			return InputCode::KP_EIGHT;
		case GLFW_KEY_KP_9:
			return InputCode::KP_NINE;
		case GLFW_KEY_KP_DECIMAL:
			return InputCode::DECI;
		case GLFW_KEY_KP_DIVIDE:
			return InputCode::DIV;
		case GLFW_KEY_KP_MULTIPLY:
			return InputCode::MULT;
		case GLFW_KEY_KP_SUBTRACT:
			return InputCode::SUB;
		case GLFW_KEY_KP_ADD:
			return InputCode::ADD;
		case GLFW_KEY_KP_ENTER:
			return InputCode::KP_ENTER;
		case GLFW_KEY_KP_EQUAL:
			return InputCode::KP_EQUAL;
		case GLFW_KEY_LEFT_SHIFT:
			return InputCode::LFT_SHFT;
		case GLFW_KEY_LEFT_CONTROL:
			return InputCode::LFT_CTRL;
		case GLFW_KEY_LEFT_ALT:
			return InputCode::LFT_ALT;
		case GLFW_KEY_LEFT_SUPER:
			return InputCode::LFT_SUP;
		case GLFW_KEY_RIGHT_SHIFT:
			return InputCode::RGT_SHFT;
		case GLFW_KEY_RIGHT_CONTROL:
			return InputCode::RGT_CTRL;
		case GLFW_KEY_RIGHT_ALT:
			return InputCode::RGT_ALT;
		case GLFW_KEY_RIGHT_SUPER:
			return InputCode::RGT_SUP;
		case GLFW_KEY_MENU:
			return InputCode::MENU;
		default:
			return InputCode::INVALID;
		}
	}

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
		//Tells glfw not to allow the window to resize
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		//Setting callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyDownEvent event(GLFWToYNMInput(key), false);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyUpEvent event(GLFWToYNMInput(key));
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyDownEvent event(GLFWToYNMInput(key), true);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseDownEvent event(GLFWToYNMInput(button));
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
					MouseUpEvent event(GLFWToYNMInput(button));
					data.EventCallback(event);
					break;
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOff, double yOff)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event(xOff, yOff);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event(xPos, yPos);
				data.EventCallback(event);
			});


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

	void WindowsWindow::ResetMousePosition() const
	{
		glfwSetCursorPos(m_Window, this->GetWidth() / 2, this->GetHeight() / 2);
	}

}
