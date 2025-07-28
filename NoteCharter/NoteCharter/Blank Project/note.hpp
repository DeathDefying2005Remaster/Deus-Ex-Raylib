#pragma once
#include <raylib-cpp.hpp>

class Note
{
	public:
	int type;
	double start;
	double end;

	Note(int _type, double _start, double _end)
	{
		type = _type;
		start = _start;
		end = _end;
	}
};