/*
	File Name: Rect.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Rect.h"


 glm::vec2 Rect::Size() const
{
	return { Right() - Left(), Top() - Bottom() };
}

 float Rect::Right() const
{
	return std::max(bottomLeft.x, topRight.x);
}

 float Rect::Left() const
{
	return std::min(bottomLeft.x, topRight.x);
}

 float Rect::Top() const
{
	return topRight.y;
}

 float Rect::Bottom() const
{
	return bottomLeft.y;
}

 float Rect::CenterX() const
{
	return (bottomLeft.x + topRight.x) / 2;
}

 float Rect::CenterY() const
{
	return (bottomLeft.y + topRight.y) / 2;
}

 glm::vec2 Rect::Center() const
{
	return { CenterX(), CenterY() };
}

 bool Rect::contains(const Rect& rect) const noexcept
{
	return Left() <= rect.Right() && Right() >= rect.Left() && Bottom() <= rect.Top() && Top() >= rect.Bottom();

}

 bool Rect::intersects(const Rect& rect) const noexcept
{
	 if (Left() > rect.Right() || rect.Left() > Right()) return false;

	 // has vertical gap
	 if (Bottom() > rect.Top() || rect.Bottom() > Top()) return false;

	 return true;

}