#pragma once
#include <raylib-cpp.hpp>
using namespace std;

inline vector<raylib::Rectangle> Box(Vector2 center, Vector2 innerSize, float thickness)
{
	raylib::Rectangle top = raylib::Rectangle(
		center.x - (innerSize.x/2 + thickness),
		center.y - (innerSize.y/2 + thickness),
		innerSize.x + (2 * thickness),
		thickness
	);
	raylib::Rectangle left = raylib::Rectangle(
		center.x - (innerSize.x/2 + thickness),
		center.y - (innerSize.y/2 + thickness),
		thickness,
		innerSize.y + (2 * thickness)
	);
	raylib::Rectangle bottom = raylib::Rectangle(
		center.x - (innerSize.x/2 + thickness),
		center.y + innerSize.y/2,
		innerSize.x + (2 * thickness),
		thickness
	);
	raylib::Rectangle right = raylib::Rectangle(
		center.x + innerSize.x/2,
		center.y - (innerSize.y/2 + thickness),
		thickness,
		innerSize.y + (2 * thickness)
	);
	vector<raylib::Rectangle> list = { top, left, bottom, right };
	return list;
}