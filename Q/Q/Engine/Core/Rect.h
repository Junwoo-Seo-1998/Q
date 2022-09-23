#pragma once
/*
	File Name: Rect.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include <glm/mat3x3.hpp>
#include <algorithm>

struct Rect {

	Rect() : bottomLeft{ 0.f,0.f }, topRight{0.f,0.f} {}
	Rect(glm::vec2 BLeft, glm::vec2 TRight) :bottomLeft(BLeft), topRight(TRight) {}
	Rect(glm::vec2 BLeft, glm::vec2 size, bool constructor_has_size) : bottomLeft(BLeft), constructor_type(constructor_has_size)
	{
		topRight = glm::vec2{ bottomLeft.x + size.x ,bottomLeft.y + size.y };
	}
	 glm::vec2 bottomLeft;
	 glm::vec2 topRight;
	bool constructor_type = false;
	 glm::vec2 Size() const;
	 float Right() const;
	 float Left() const;
	 float Top() const;
	 float Bottom() const;
	 float CenterX() const;
	 float CenterY() const;

	 glm::vec2 Center() const;

	 bool contains(const Rect& rect) const noexcept;

	 bool intersects(const Rect& rect) const noexcept;
};


