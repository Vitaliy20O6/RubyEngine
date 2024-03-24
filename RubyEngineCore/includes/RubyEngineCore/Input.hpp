#pragma once

#include "Keys.hpp"

namespace RubyEngine
{
	class Input 
	{
	private:
		static bool m_keys_pressed[];
		static bool m_mouse_buttons_pressed[];

	public:
		static bool is_key_pressed(const KeyCode key_code);
		static void PressKey(const KeyCode key_code);
		static void ReleaseKey(const KeyCode key_code);

		static bool is_mouse_button_pressed(const MouseButton mouse_button);
		static void PressMouseButton(const MouseButton mouse_button);
		static void ReleaseMouseButton(const MouseButton mouse_button);
	};
}