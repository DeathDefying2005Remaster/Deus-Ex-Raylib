#pragma once
#include <raylib-cpp.hpp>
#include <cmath>
#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "event.h"
using namespace std;

class Tardis
{
	public:

	vector<double> timeHistory;
	vector<Vector2> playerPosition;

	int currentIndex;

	Tardis()
	{
		timeHistory = {};
		playerPosition = {};
		currentIndex = 0;
	}

	void UpdateIndex(double time)
	{
		if (timeHistory.size() > 1)
		{
			while (timeHistory[currentIndex + 1] < time && currentIndex + 1 < timeHistory.size())
			{
				currentIndex++;
			}
			while (timeHistory[currentIndex] > time && currentIndex > 0)
			{
				currentIndex--;
			}
		}
	}

	float GetInterpolation(double time)
	{
		return (time - timeHistory[currentIndex]) / (timeHistory[currentIndex + 1] - timeHistory[currentIndex]);
	}

	Vector2 GetPlayerPosition()
	{
		double timmy = Clamp(GetTime(), 0, timeHistory[timeHistory.size() - 1]);
		UpdateIndex(timmy);
		return Vector2Lerp(playerPosition[currentIndex], playerPosition[currentIndex + 1], GetInterpolation(timmy));
	}
	Vector2 GetPlayerPosition(double time)
	{
		double timmy = Clamp(time, 0, timeHistory[timeHistory.size() - 1]);
		UpdateIndex(timmy);
		return Vector2Lerp(playerPosition[currentIndex], playerPosition[currentIndex + 1], GetInterpolation(timmy));
	}
};