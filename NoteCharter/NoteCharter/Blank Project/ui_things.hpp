#pragma once
#include <raylib-cpp.hpp>
#include "global.hpp"

class Grid
{
	public:

	int lines = 10;
	float speed = 100;
	float thickness = 1;
	float xOffset = 0;
	float yOffset = 0;
	Color color = bigGreen;

	Grid(int _lines, float _thickness, float _speed, float _offset, Color _color)
	{
		yOffset = 0;
		xOffset = _offset;
		lines = _lines;
		speed = _speed;
		thickness = _thickness;
		color = _color;
	}

	void Draw()
	{
		for (int i = 0; i < lines; i++)
		{
			float x = xOffset + (i * (800 / lines));
			if (x > 800) { x -= 800; }
			if (x < 0) { x += 800; }

			float y = yOffset + (i * (800 / lines));
			if (y > 800) { y -= 800; }
			if (y < 0) { y += 800; }

			DrawLineEx({ x, 0 }, { x, 800 }, thickness, color);
			DrawLineEx({ 0, y }, { 800, y }, thickness, color);
		}

		xOffset += speed * GetFrameTime();
		if (xOffset > 800) { xOffset -= 800; }
		if (xOffset < 0) { xOffset += 800; }
		yOffset += speed * GetFrameTime();
		if (yOffset > 800) { yOffset -= 800; }
		if (yOffset < 0) { yOffset += 800; }
	}
};

class Button
{
	public:

	Vector2 position;
	Vector2 size;
	std::string name;
	float thickness = 5;

	Button()
	{
		position = {};
		size = {};
		name = "";
	}
	Button(Vector2 _position, Vector2 _size, std::string _name)
	{
		position = _position;
		size = _size;
		name = _name;
	}

	bool Pressed()
	{
		Rectangle button = raylib::Rectangle(position.x, position.y, size.x, size.y);
		return (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(0));
	}

	void Draw()
	{
		Color color = smallGreen;
		Rectangle button = raylib::Rectangle(position.x, position.y, size.x, size.y);
		if (CheckCollisionPointRec(GetMousePosition(), button))
		{
			color = bigGreen;
		}

		/*DrawRectangle(position.x, position.y, size.x, buffer, color);
		DrawRectangle(position.x, position.y + size.y - buffer, size.x, buffer, color);
		DrawRectangle(position.x, position.y + buffer, buffer, size.y - buffer - buffer, color);
		DrawRectangle(position.x + size.x - buffer, position.y + buffer, buffer, size.y - buffer - buffer, color);*/

		DrawRectangleV(position, size, color);
		DrawRectangle(position.x + thickness, position.y + thickness, size.x - 2*thickness, size.y - 2*thickness, BLACK);
	}
};