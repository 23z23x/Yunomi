#pragma once

#include "pch.h"

namespace ynm
{
	//Enum to determine type of event
	enum class EventType
	{
		None = 0,
		WinClose, WinResize, WinFocus, WinLoseFocus, WinMove,
		KeyDown, KeyUp, MouseDown, MouseUp, MouseMove, MouseScroll
	};

	//Enum to determine category of event
	enum EventCategory
	{
		None = 0,
		Input = 1 << 0,
		Keyboard = 1 << 1,
		Mouse = 1 << 2,
		MouseButton = 1 << 3
	};

	//Macro to implement these without having to retype them every time
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	//Event class
	class YNM_API Event
	{
		//Allows EventDispatcher to see private and protected members of the class
		friend class EventDispatcher;
	public:
		virtual EventType getEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlag() const = 0;
		virtual std::string ToString() const { return GetName(); }

		//Tests if this event is in a certain category
		inline bool IsInCategory(EventCategory cat)
		{
			return GetCategoryFlag() & cat;
		}
	protected:
		//Whether or not the event has been handled
		bool m_Handled = false;
	};

	//Dispatcher class
	class EventDispatcher
	{
		//Function that returns bool and takes a type address T
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		//Takes an event reference
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		//Takes in a Function to dispatch an event with
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			//If the event is the same type as the function, handle it
			if (m_Event.getEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}