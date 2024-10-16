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

template <typename T> class TardisValue
{
	public:

	vector<T> value;
	double startTime;
	int interval;
	int currentIndex;

	TardisValue(double _startTime, int _interval)
	{
		value = {};
		startTime = _startTime;
		interval = _interval;
		currentIndex = 0;
	}
};

class Tardis
{
	public:

	vector<double> timeHistory;

	Tardis()
	{
		timeHistory = {};
	}
};

class PlayerTardis
{
	public:
	
	TardisValue<Vector2> list = TardisValue<Vector2>(GetTime(), 3);
	
	int currentIndex;
	
	PlayerTardis(vector<Vector2> stardList)
	{
		list.value = stardList;
		currentIndex = 0;
	}
	
	void UpdateIndex(Tardis tardis, double time)
	{
		if (tardis.timeHistory.size() > 1)
		{
			while (tardis.timeHistory[currentIndex + 1] < time && currentIndex + 1 < tardis.timeHistory.size())
			{
				currentIndex++;
			}
			while (tardis.timeHistory[currentIndex] > time && currentIndex > 0)
			{
				currentIndex--;
			}
		}
	}
	
	float GetInterpolation(Tardis tardis, double time)
	{
		return (time - tardis.timeHistory[currentIndex]) / (tardis.timeHistory[currentIndex + 1] - tardis.timeHistory[currentIndex]);
	}
	
	Vector2 GetPlayerPosition(Tardis tardis, double time)
	{
		double timmy = Clamp(time, list.startTime, tardis.timeHistory[tardis.timeHistory.size() - 1]);
		UpdateIndex(tardis, timmy);
		return Vector2Lerp(list.value[currentIndex], list.value[currentIndex + 1], GetInterpolation(tardis, timmy));
	}
};

//class PlayerTardis
//{
//	public:
//
//	vector<Vector2> positionList;
//
//	int currentIndex;
//
//	PlayerTardis(vector<Vector2> _list)
//	{
//		positionList = _list;
//		currentIndex = 0;
//	}
//
//	void UpdateIndex(Tardis tardis, double time)
//	{
//		if (tardis.timeHistory.size() > 1)
//		{
//			while (tardis.timeHistory[currentIndex + 1] < time && currentIndex + 1 < tardis.timeHistory.size())
//			{
//				currentIndex++;
//			}
//			while (tardis.timeHistory[currentIndex] > time && currentIndex > 0)
//			{
//				currentIndex--;
//			}
//		}
//	}
//
//	float GetInterpolation(Tardis tardis, double time)
//	{
//		return (time - tardis.timeHistory[currentIndex]) / (tardis.timeHistory[currentIndex + 1] - tardis.timeHistory[currentIndex]);
//	}
//
//	Vector2 GetPlayerPosition(Tardis tardis, double time)
//	{
//		double timmy = Clamp(time, 0, tardis.timeHistory[tardis.timeHistory.size() - 1]);
//		UpdateIndex(tardis, timmy);
//		return Vector2Lerp(positionList[currentIndex], positionList[currentIndex + 1], GetInterpolation(tardis, timmy));
//	}
//};