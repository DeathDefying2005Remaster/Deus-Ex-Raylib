#pragma once
#include <raylib-cpp.hpp>
#include "global.hpp"

class GridHalf
{
	public:

	int lines = 10;
	float speed = 100;
	float thickness = 1;
	float xOffset = 0;
	float yOffset = 0;
	Color color = bigGreen;

	GridHalf(int _lines, float _thickness, float _speed, float _offset, Color _color)
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
		float sWidth = float(screenWidth);
		float sHeight = float(screenHeight);
		int longLines = ceil(sWidth / sHeight * lines);

		for (int i = 0; i < longLines; i++)
		{
			float x = xOffset + (i * (sWidth / longLines));
			if (x > sWidth) { x -= sWidth; }
			if (x < 0) { x += sWidth; }

			DrawLineEx({ x, 0 }, { x, sHeight }, thickness, color);

			if (i < lines)
			{
				float y = yOffset + (i * (sHeight / lines));
				if (y > sHeight) { y -= sHeight; }
				if (y < 0) { y += sHeight; }

				DrawLineEx({ 0, y }, { sWidth, y }, thickness, color);
			}
		}

		xOffset += speed * GetFrameTime();
		if (xOffset > sWidth) { xOffset -= sWidth; }
		if (xOffset < 0) { xOffset += sWidth; }
		yOffset += speed * GetFrameTime();
		if (yOffset > 800) { yOffset -= 800; }
		if (yOffset < 0) { yOffset += 800; }
	}
};

void FightBox(Vector2 centrePos, Vector2 size, float thickness, Color color)
{
	Rectangle rect = { centrePos.x - size.x / 2, centrePos.y - size.y / 2, size.x, size.y };
	DrawRectOutline(rect, thickness, color);
}

class Grid
{
	public:

	GridHalf front = GridHalf(9, 1, -50, 0, bigPurple);
	GridHalf back = GridHalf(9, 1, 30, 20, smallPurple);

	void Draw()
	{
		back.Draw();
		front.Draw();
	}
};