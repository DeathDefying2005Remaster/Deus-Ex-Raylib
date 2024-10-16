#pragma once
#include <raylib-cpp.hpp>
#include <cmath>
#include <time.h>
#include <string>
#include <vector>
#include "triangle.hpp"
using namespace std;

bool CheckTriangleInRectangle(Triangle triangle, Rectangle rectangle)
{
	Vector2 topLeft = { rectangle.x, rectangle.y };
	Vector2 topRight = { rectangle.x + rectangle.width, rectangle.y };
	Vector2 bottomLeft = { rectangle.x, rectangle.y + rectangle.height };
	Vector2 bottomRight = { rectangle.x + rectangle.width, rectangle.y + rectangle.height };
	
	Vector2 collisionPoint;

	if (
	CheckCollisionLines(triangle.pointLeft, triangle.pointRight, topLeft, topRight, &collisionPoint) ||
	CheckCollisionLines(triangle.pointLeft, triangle.pointRight, topLeft, bottomLeft, &collisionPoint) ||
	CheckCollisionLines(triangle.pointLeft, triangle.pointRight, topRight, bottomRight, &collisionPoint) ||
	CheckCollisionLines(triangle.pointLeft, triangle.pointRight, bottomLeft, bottomRight, &collisionPoint) ||

	CheckCollisionLines(triangle.pointTop, triangle.pointRight, topLeft, topRight, &collisionPoint) ||
	CheckCollisionLines(triangle.pointTop, triangle.pointRight, topLeft, bottomLeft, &collisionPoint) ||
	CheckCollisionLines(triangle.pointTop, triangle.pointRight, topRight, bottomRight, &collisionPoint) ||
	CheckCollisionLines(triangle.pointTop, triangle.pointRight, bottomLeft, bottomRight, &collisionPoint) ||

	CheckCollisionLines(triangle.pointTop, triangle.pointLeft, topLeft, topRight, &collisionPoint) ||
	CheckCollisionLines(triangle.pointTop, triangle.pointLeft, topLeft, bottomLeft, &collisionPoint) ||
	CheckCollisionLines(triangle.pointTop, triangle.pointLeft, topRight, bottomRight, &collisionPoint) ||
	CheckCollisionLines(triangle.pointTop, triangle.pointLeft, bottomLeft, bottomRight, &collisionPoint)
	) { return true; }

	return false;
}

Vector2 DownClampVector2(Vector2 vector, float magnitude)
{
	if (Vector2Length(vector) >= magnitude)
	{
		float factor = magnitude / Vector2Length(vector);
		return Vector2Scale(vector, factor);
	}
	return vector;
}