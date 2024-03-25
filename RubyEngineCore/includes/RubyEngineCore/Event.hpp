#pragma once

#include "Keys.hpp"

#include <functional>
#include <array>

namespace RubyEngine
{
	enum class EventType
	{
		WindowResize = 0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonKeyPressed,
		MouseButtonKeyReleased,
		MouseMoved,

		EventsCount
	};

	struct BaseEvent
	{
		virtual ~BaseEvent() = default;
		virtual EventType get_type() const = 0;
	};

	class EventDispatcher
	{
	public:
		template<typename EventType>
		void add_event_listener(std::function<void(EventType&)> callback)
		{
			auto baseCallback = [func = std::move(callback)](BaseEvent& e)
				{
					if (e.get_type() == EventType::type)
					{
						func(static_cast<EventType&>(e));
					}
				};
			m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
		}

		void dispatch(BaseEvent& event)
		{
			auto callback = m_eventCallbacks[static_cast<size_t>(event.get_type())];
				{
					if (callback)
					{
						callback(event);
					}
				};
		}

	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;
	};

	struct EventMouseMoved: public BaseEvent
	{
		double x_pos;
		double y_pos;
		static const EventType type = EventType::MouseMoved;

		EventMouseMoved(const double new_x, const double new_y)
			: x_pos(new_x)
			, y_pos(new_y)
		{}

		virtual EventType get_type() const override
		{
			return type;
		}
	};

	struct EventWindowResize : public BaseEvent
	{
		unsigned int width;
		unsigned int height;
		static const EventType type = EventType::WindowResize;

		EventWindowResize(const unsigned int new_width, const unsigned int new_height)
			: width(new_width)
			, height(new_height)
		{}

		virtual EventType get_type() const override
		{
			return type;
		}
	};

	struct EventWindowClose : public BaseEvent
	{
		static const EventType type = EventType::WindowClose;

		virtual EventType get_type() const override
		{
			return type;
		}
	};

	struct EventKeyPressed : public BaseEvent
	{
		KeyCode key_code;
		bool repeated;
		static const EventType type = EventType::KeyPressed;

		EventKeyPressed(const KeyCode key_code, const bool repeated)
			: key_code(key_code)
			, repeated(repeated)
		{}

		virtual EventType get_type() const override
		{
			return type;
		}
	};

	struct EventKeyReleased : public BaseEvent
	{
		KeyCode key_code;
		static const EventType type = EventType::KeyReleased;

		EventKeyReleased(const KeyCode key_code)
			: key_code(key_code)
		{}

		virtual EventType get_type() const override
		{
			return type;
		}
	};
}