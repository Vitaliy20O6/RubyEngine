#pragma once

#include "Keys.hpp"

namespace RubyEngine
{
	class Input 
	{
	private:
		static bool m_key_pressed[static_cast<size_t>(KeyCode::KEY_LAST)];

	public:
		static bool is_key_pressed(const KeyCode key_code);
		static void PressKey(const KeyCode key_code);
		static void ReleaseKey(const KeyCode key_code);
	};
}