#pragma once

#include "Keys.hpp"

namespace RubyEngine
{
	class Input 
	{
	private:
		static bool m_keys_pressed[];

	public:
		static bool is_key_pressed(const KeyCode key_code);
		static void PressKey(const KeyCode key_code);
		static void ReleaseKey(const KeyCode key_code);
	};
}