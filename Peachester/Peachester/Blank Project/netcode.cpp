#include "netcode.hpp"
#include <raylib-cpp.hpp>

std::string PackVar(Vector2 v)
{
	std::string x = std::to_string(v.x);
	std::string y = std::to_string(v.y);
	return "V " + x + " " + y + ":";
}
std::string PackVar(enet_uint32 v)
{
	std::string str = std::to_string(v);
	while (str.length() < 10)
	{
		str.insert(0, "0");
	}
	return "I " + str + ":";
}
std::string PackVar(int v)
{
	std::string str = std::to_string(v);
	while (str.length() < 10)
	{
		if (str[0] == '-')
		{
			str.insert(1, "0");
		}
		else
		{
			str.insert(0, "0");
		}
	}
	return "N " + str + ":";
}
std::string PackVar(bool v)
{
	return "B " + std::to_string((int)v) + ":";
}

void UnpackVar(std::string str, Vector2* v)
{
	std::string strX = "";
	std::string strY = "";
	size_t border = str.find(' ', 2);
	for (int j = 2; j < border; j++)
	{
		strX += str[j];
	}
	for (int j = int(border + 1); j < str.length(); j++)
	{
		strY += str[j];
	}
	*v = { stof(strX), stof(strY) };
}
void UnpackVar(std::string str, enet_uint32* v)
{
	std::string var = "";
	for (int i = 2; i < str.length(); i++)
	{
		var += str[i];
	}
	*v = stoul(var);
}
void UnpackVar(std::string str, enet_uint8* v)
{
	std::string var = "";
	for (int i = 2; i < str.length(); i++)
	{
		var += str[i];
	}
	*v = stoi(var);
}
void UnpackVar(std::string str, int* v)
{
	std::string var = "";
	for (int i = 2; i < str.length(); i++)
	{
		var += str[i];
	}
	*v = stoi(var);
}
void UnpackVar(std::string str, bool* v)
{
	*v = str[2] - '0';
}
