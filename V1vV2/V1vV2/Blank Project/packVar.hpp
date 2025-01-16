#pragma once
using namespace std;

string PackVar(Vector2 v)
{
	string x = to_string(v.x);
	while (x.length() < 10) { x.append("0"); }
	while (x.length() > 10) { x.erase(x.end()); }
	string y = to_string(v.y);
	while (y.length() < 10) { y.append("0"); }
	while (y.length() > 10) { y.erase(y.end()); }
	return "V " + x + " " + y + ":";
}
string PackVar(enet_uint32 v)
{
	string str = to_string(v);
	while (str.length() < 10)
	{
		str.insert(0, "0");
	}
	return "I " + str + ":";
}
string PackVar(bool v)
{
	return "B " + to_string(v) + ":";
}
void UnpackVar(string str, Vector2* v)
{
	string strX = "";
	string strY = "";
	for (int i = 2; i < 12; i++)
	{
		strX += str[i];
		strY += str[i + 11];
	}
	*v = { stof(strX), stof(strY) };
}
void UnpackVar(string str, enet_uint32* v)
{
	string var = "";
	for (int i = 2; i < str.length(); i++)
	{
		var += str[i];
	}
	*v = stoul(var);
}
void UnpackVar(string str, enet_uint8* v)
{
	string var = "";
	for (int i = 2; i < str.length(); i++)
	{
		var += str[i];
	}
	*v = stoi(var);
}
void UnpackVar(string str, bool* v)
{
	bool barf = stoi(to_string(str[2]));
	*v = barf;
}

void PutsVar(Vector2 v)
{
	puts(to_string(v.x).c_str());
	puts(to_string(v.y).c_str());
}