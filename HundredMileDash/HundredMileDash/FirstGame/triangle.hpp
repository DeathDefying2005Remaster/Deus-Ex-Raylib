#pragma once
#include <raylib-cpp.hpp>
#include "variables.hpp"
#include <cmath>
#include <time.h>
#include <string>
#include <vector>
using namespace std;

class Triangle
{
	public:
	Vector2 pointTop;
	Vector2 pointLeft;
	Vector2 pointRight;
	float tRotation;

	Triangle(Vector2 top, Vector2 left, Vector2 right)
	{
		pointTop = top;
		pointLeft = left;
		pointRight = right;
		tRotation = 0;
	}
	Triangle()
	{
		pointTop = Vector2();
		pointLeft = Vector2();
		pointRight = Vector2();
		tRotation = 0;
	}

	void Move(Vector2 add)
	{
		pointTop = Vector2Add(pointTop, add);
		pointLeft = Vector2Add(pointLeft, add);
		pointRight = Vector2Add(pointRight, add);
	}

	void Rotate(float rotDegrees)
	{
		tRotation += rotDegrees * DEG2RAD;

		pointTop.x = (((pointTop.x - FindCentroid().x) * cos(tRotation)) - ((pointTop.y - FindCentroid().y) * sin(tRotation))) + FindCentroid().x;
		pointTop.y = (((pointTop.y - FindCentroid().y) * cos(tRotation)) + ((pointTop.x - FindCentroid().x) * sin(tRotation))) + FindCentroid().y;

		pointLeft.x = (((pointLeft.x - FindCentroid().x) * cos(tRotation)) - ((pointLeft.y - FindCentroid().y) * sin(tRotation))) + FindCentroid().x;
		pointLeft.y = (((pointLeft.y - FindCentroid().y) * cos(tRotation)) + ((pointLeft.x - FindCentroid().x) * sin(tRotation))) + FindCentroid().y;

		pointRight.x = (((pointRight.x - FindCentroid().x) * cos(tRotation)) - ((pointRight.y - FindCentroid().y) * sin(tRotation))) + FindCentroid().x;
		pointRight.y = (((pointRight.y - FindCentroid().y) * cos(tRotation)) + ((pointRight.x - FindCentroid().x) * sin(tRotation))) + FindCentroid().y;
	}

	Vector2 FindCentroid()
	{
		Vector2 middle = Vector2Lerp(pointLeft, pointRight, 0.5);
		Vector2 centroid = Vector2Lerp(middle, pointTop, (float)1/3);
		return centroid;
	}
};

vector<Triangle> SplitTriangleRandom(Triangle triangle)
{
	vector<Triangle> triangles = { Triangle(), Triangle() };

	int rand = GetRandomValue(0, 2);
	if (rand == 0)
	{
		triangles[0].pointTop.x = Lerp(triangle.pointTop.x, triangle.pointLeft.x, 0.5);
		triangles[0].pointTop.y = Lerp(triangle.pointTop.y, triangle.pointLeft.y, 0.5);
		triangles[0].pointLeft = triangle.pointLeft;
		triangles[0].pointRight = triangle.pointRight;

		triangles[1].pointTop = triangle.pointTop;
		triangles[1].pointLeft.x = Lerp(triangle.pointTop.x, triangle.pointLeft.x, 0.5);
		triangles[1].pointLeft.y = Lerp(triangle.pointTop.y, triangle.pointLeft.y, 0.5);
		triangles[1].pointRight = triangle.pointRight;
	}
	else if (rand == 1)
	{
		triangles[0].pointTop.x = Lerp(triangle.pointTop.x, triangle.pointRight.x, 0.5);
		triangles[0].pointTop.y = Lerp(triangle.pointTop.y, triangle.pointRight.y, 0.5);
		triangles[0].pointLeft = triangle.pointLeft;
		triangles[0].pointRight = triangle.pointRight;

		triangles[1].pointTop = triangle.pointTop;
		triangles[1].pointLeft = triangle.pointLeft;
		triangles[1].pointRight.x = Lerp(triangle.pointTop.x, triangle.pointRight.x, 0.5);
		triangles[1].pointRight.y = Lerp(triangle.pointTop.y, triangle.pointRight.y, 0.5);
	}
	else if (rand == 2)
	{
		triangles[0].pointTop = triangle.pointTop;
		triangles[0].pointLeft = triangle.pointLeft;
		triangles[0].pointRight.x = Lerp(triangle.pointLeft.x, triangle.pointRight.x, 0.5);
		triangles[0].pointRight.y = Lerp(triangle.pointLeft.y, triangle.pointRight.y, 0.5);

		triangles[1].pointTop = triangle.pointTop;
		triangles[1].pointLeft.x = Lerp(triangle.pointLeft.x, triangle.pointRight.x, 0.5);
		triangles[1].pointLeft.y = Lerp(triangle.pointLeft.y, triangle.pointRight.y, 0.5);
		triangles[1].pointRight = triangle.pointRight;
	}

	return triangles;
}

vector<Triangle> BreakTriangle(Triangle triangle, int slices)
{
	vector<Triangle> triangles = { triangle };

	for (int i = 0; i < slices; i++)
	{
		vector<Triangle> newTriangles = {};
		for (Triangle tri : triangles)
		{
			vector<Triangle> newTris = SplitTriangleRandom(tri);
			newTriangles.push_back(newTris[0]);
			newTriangles.push_back(newTris[1]);
		}
		triangles = newTriangles;
	}

	return triangles;
}