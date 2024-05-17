#pragma once
#include "Event.h"

namespace ynm
{

	//Key Events
	class YNM_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(Keyboard | Input)

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class YNM_API KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(int keycode, bool repeat)
			: KeyEvent(keycode), m_Repeat(repeat) {}

		inline bool GetRepeat() const { return m_Repeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyDownEvent: " << m_KeyCode << "Repeat: " << m_Repeat;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyDown);

	private:
		bool m_Repeat;
	};

	class YNM_API KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyUpEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyUp)
	};

	//Mouse Events
	class YNM_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(double x, double y)
			: m_MouseX(x), m_MouseY(y) {}

		inline double GetX() const { return m_MouseX; }
		inline double GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMove)
		EVENT_CLASS_CATEGORY(Mouse | Input)
	private:
		double m_MouseX, m_MouseY;
	};

	class YNM_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(double xOffset, double yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline double GetXOffset() const { return m_XOffset; }
		inline double GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScroll)
		EVENT_CLASS_CATEGORY(Mouse | Input)
	private:
		double m_XOffset, m_YOffset;
	};

	class YNM_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButtom() const { return m_Button; }

		EVENT_CLASS_CATEGORY(Mouse | Input)

	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class YNM_API MouseDownEvent : public MouseButtonEvent
	{
	public:
		MouseDownEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseDownEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseDown)
	};

	class YNM_API MouseUpEvent : public MouseButtonEvent
	{
	public:
		MouseUpEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseUpEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseUp)
	};
}