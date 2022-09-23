/*
	File Name: RegisterMath.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "RegisterMath.h"
#include "glm/glm.hpp"

namespace q_engine
{
	void RegisterMath(sol::state& state)
	{
		state.new_usertype<glm::vec2>("Vector2",
			sol::constructors<glm::vec2(float, float)>(),
			"x", &glm::vec2::x,
			"y", &glm::vec2::y,
			sol::meta_function::addition, sol::overload(
				static_cast<glm::vec2(*)(glm::vec2 const&, glm::vec2 const&)>(&glm::operator+)
			),
			sol::meta_function::multiplication, sol::overload(
				static_cast<glm::vec2(*)(glm::vec2 const&, glm::vec2 const&)>(&glm::operator*)
			),
			sol::meta_function::subtraction, sol::overload(
				static_cast<glm::vec2(*)(glm::vec2 const&, glm::vec2 const&)>(&glm::operator-)
			),
			sol::meta_function::division, sol::overload(
				static_cast<glm::vec2(*)(glm::vec2 const&, glm::vec2 const&)>(&glm::operator/)
			),
			sol::meta_function::equal_to, static_cast<bool(*)(glm::vec2 const&, glm::vec2 const&)>(&glm::operator==)
			);
		
	}
}
