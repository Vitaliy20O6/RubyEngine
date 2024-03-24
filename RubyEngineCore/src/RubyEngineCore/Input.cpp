#include "RubyEngineCore/Input.hpp"

namespace RubyEngine
{
	bool Input::m_key_pressed[static_cast<size_t>(KeyCode::KEY_LAST)] = {};

	bool Input::is_key_pressed(const KeyCode key_code)
	{
		return m_key_pressed [static_cast<size_t>(key_code)];
	}
	void Input::PressKey(const KeyCode key_code)
	{
		m_key_pressed[static_cast<size_t>(key_code)] = true;
	}
	void Input::ReleaseKey(const KeyCode key_code)
	{
		m_key_pressed[static_cast<size_t>(key_code)] = false;
	}
}
