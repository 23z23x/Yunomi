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
		MouseButton = 1 << 3,
		App = 1 << 4
	};

	//Enum to define inputs
	enum InputCode
	{
		INVALID = 0,
		//Mouse
		M1,
		M2,
		M3,
		M4,
		M5,
		M6,
		M7,
		M8,
		//Keyboard
		SPACE,
		APOST,
		COMMA,
		MINUS,
		PERIOD,
		SLASH,
		ZERO,
		KP_ZERO,
		ONE,
		KP_ONE,
		TWO,
		KP_TWO,
		THREE,
		KP_THREE,
		FOUR,
		KP_FOUR,
		FIVE,
		KP_FIVE,
		SIX,
		KP_SIX,
		SEVEN,
		KP_SEVEN,
		EIGHT,
		KP_EIGHT,
		NINE,
		KP_NINE,
		SEMICOL,
		EQUAL,
		KP_EQUAL,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LFT_BRCKT,
		RGT_BRCKT,
		BCKLSH,
		ESCAPE,
		ENTER,
		KP_ENTER,
		TAB,
		BCKSPCE,
		INSERT,
		DLT,
		RIGHT,
		LEFT,
		UP,
		DOWN,
		PG_UP,
		PG_DN,
		HOME,
		END,
		CAP_LCK,
		SCRL_LCK,
		NM_LCK,
		PRNT_SCRN,
		PAUSE,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		DECI,
		MULT,
		DIV,
		SUB,
		ADD,
		LFT_SHFT,
		LFT_CTRL,
		LFT_SUP,
		LFT_ALT,
		RGT_SHFT,
		RGT_CTRL,
		RGT_SUP,
		RGT_ALT,
		MENU
	};

	//Macro to implement these without having to retype them every time
#define EVENT_CLASS_TYPE(type) \
    static EventType GetStaticType() { return EventType::type; } \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; }


#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlag() const override { return category; }

	//Event class
	class YNM_API Event
	{
		//Allows EventDispatcher to see private and protected members of the class
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
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
			if (m_Event.GetEventType() == T::GetStaticType())
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