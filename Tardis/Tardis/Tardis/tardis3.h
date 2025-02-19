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
	vector<double> reference;
	int interval;
	int currentIndex;

	TardisValue(T _startValue, double _startTime, int _interval)
	{
		value = { _startValue };
		reference = { _startTime };
		interval = _interval;
		currentIndex = 0;
	}
	TardisValue()
	{
		value = {};
		reference = {};
		interval = 1;
		currentIndex = 0;
	}

	void Clear()
	{
		value.clear();
		reference.clear();
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
	
	TardisValue<Vector2> list;
	
	int currentIndex;
	
	PlayerTardis(Vector2 startPos)
	{
		list = TardisValue<Vector2>(startPos, GetTime(), 3);
		currentIndex = 0;
	}
	PlayerTardis()
	{
		list = TardisValue<Vector2>({0, 0}, GetTime(), 3);
		currentIndex = 0;
	}
	
	void UpdateIndex(double time)
	{
		if (list.reference.size() > 1)
		{
			while (list.reference[currentIndex + 1] < time && currentIndex + 1 < list.reference.size())
			{
				currentIndex++;
			}
			while (list.reference[currentIndex] > time && currentIndex > 0)
			{
				currentIndex--;
			}
		}
	}
	
	float GetInterpolation(double time)
	{
		//PROBLEM HERE!!!!!!!!!!
		printf(to_string(currentIndex).c_str());
		printf(" : ");
		if (list.reference.size() > 1) return (time - list.reference[currentIndex]) / (list.reference[currentIndex + 1] - list.reference[currentIndex]);
		else return 0;
	}
	
	Vector2 GetPlayerPosition(double time)
	{
		double timmy = Clamp(time, list.reference[0], list.reference[list.reference.size() - 1]);
		UpdateIndex(timmy);
		if (list.reference.size() > 1 && currentIndex + 1 < list.reference.size()) return Vector2Lerp(list.value[currentIndex], list.value[currentIndex + 1], GetInterpolation(timmy));
		else return list.value[currentIndex];
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