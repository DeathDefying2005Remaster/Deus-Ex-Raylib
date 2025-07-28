//#define _CRT_SECURE_NO_WARNINGS
//#define RAYGUI_IMPLEMENTATION
//
//#include <raylib-cpp.hpp>
//#include <fstream>
//#include "json.hpp"
//#include "dropdown.hpp"
//#include "raygui.h"
//using namespace std;
//
//const int screenWidth = 800;
//const int screenHeight = 800;
//const int targetFps = 60;
//
//Camera2D camera;
//
//nlohmann::json chart;
//
//bool left = false;
//bool right = false;
//bool playing = false;
//
///*
//		scr_rhythmgame_addnote(0.65, 1, 0);
//		scr_rhythmgame_addnote(0.87, 1, 0);
//		scr_rhythmgame_addnote(1.07, 0, 0);
//		scr_rhythmgame_addnote(1.2, 1, 0);
//		scr_rhythmgame_addnote(1.5, 0, 0);
//		scr_rhythmgame_addnote(1.94, 0, 0);
//		scr_rhythmgame_addnote(2.07, 1, 2.36);
//		scr_rhythmgame_addnote(2.4, 1, 0);
//		scr_rhythmgame_addnote(2.56, 0, 0);
//		scr_rhythmgame_addnote(2.73, 0, 0);
//		scr_rhythmgame_addnote(3.18, 1, 0);
//		scr_rhythmgame_addnote(3.34, 1, 0);
//		scr_rhythmgame_addnote(3.54, 0, 0);
//		scr_rhythmgame_addnote(3.66, 1, 0);
//		scr_rhythmgame_addnote(3.79, 0, 0);
//		scr_rhythmgame_addnote(3.92, 1, 0);
//		scr_rhythmgame_addnote(4.32, 1, 0);
//		scr_rhythmgame_addnote(4.48, 0, 0);
//		scr_rhythmgame_addnote(4.67, 0, 0);
//		scr_rhythmgame_addnote(4.87, 1, 0);
//		scr_rhythmgame_addnote(4.98, 0, 0);
//		scr_rhythmgame_addnote(5.26, 1, 0);
//		scr_rhythmgame_addnote(5.41, 1, 0);
//		scr_rhythmgame_addnote(5.58, 0, 0);
//		scr_rhythmgame_addnote(5.72, 0, 0);
//		scr_rhythmgame_addnote(5.81, 0, 0);
//
//		scr_rhythmgame_addnote(21.7, 0, 22.18);
//        scr_rhythmgame_addnote(22.18, 1, 0);
//        scr_rhythmgame_addnote(22.54, 0, 0);
//        scr_rhythmgame_addnote(22.66, 1, 0);
//        scr_rhythmgame_addnote(23.02, 0, 0);
//        scr_rhythmgame_addnote(23.14, 1, 23.5);
//        scr_rhythmgame_addnote(23.5, 0, 23.98);
//        scr_rhythmgame_addnote(23.98, 0, 0);
//        scr_rhythmgame_addnote(24.1, 1, 0);
//        scr_rhythmgame_addnote(24.46, 0, 0);
//        scr_rhythmgame_addnote(24.58, 1, 25.06);
//        scr_rhythmgame_addnote(25.06, 1, 25.54);
//        scr_rhythmgame_addnote(25.54, 0, 26.02);
//        scr_rhythmgame_addnote(26.02, 1, 0);
//        scr_rhythmgame_addnote(26.38, 0, 0);
//        scr_rhythmgame_addnote(26.5, 1, 0);
//        scr_rhythmgame_addnote(26.86, 0, 0);
//        scr_rhythmgame_addnote(26.98, 1, 27.34);
//        scr_rhythmgame_addnote(27.34, 0, 27.82);
//        scr_rhythmgame_addnote(27.82, 0, 0);
//        scr_rhythmgame_addnote(27.94, 1, 0);
//        scr_rhythmgame_addnote(28.3, 0, 0);
//        scr_rhythmgame_addnote(28.42, 1, 28.9);
//        scr_rhythmgame_addnote(28.9, 1, 29.38);
//        scr_rhythmgame_addnote(29.38, 0, 29.86);
//        scr_rhythmgame_addnote(29.86, 1, 0);
//        scr_rhythmgame_addnote(30.22, 0, 0);
//        scr_rhythmgame_addnote(30.34, 1, 0);
//        scr_rhythmgame_addnote(30.7, 0, 0);
//        scr_rhythmgame_addnote(30.82, 1, 0);
//        scr_rhythmgame_addnote(31.18, 0, 31.66);
//        scr_rhythmgame_addnote(31.66, 0, 0);
//        scr_rhythmgame_addnote(31.78, 1, 0);
//        scr_rhythmgame_addnote(32.14, 0, 0);
//        scr_rhythmgame_addnote(32.26, 1, 32.74);
//        scr_rhythmgame_addnote(32.74, 1, 33.22);
//        scr_rhythmgame_addnote(33.22, 0, 33.7);
//        scr_rhythmgame_addnote(33.7, 1, 0);
//        scr_rhythmgame_addnote(34.06, 0, 0);
//        scr_rhythmgame_addnote(34.18, 1, 0);
//        scr_rhythmgame_addnote(34.54, 0, 0);
//        scr_rhythmgame_addnote(34.66, 1, 0);
//        scr_rhythmgame_addnote(35.02, 0, 35.5);
//        scr_rhythmgame_addnote(35.5, 0, 0);
//        scr_rhythmgame_addnote(35.62, 1, 0);
//        scr_rhythmgame_addnote(35.98, 0, 0);
//        scr_rhythmgame_addnote(36.1, 1, 36.58);
//        scr_rhythmgame_addnote(36.58, 1, 37.06);
//        scr_rhythmgame_addnote(37.06, 0, 37.54);
//        scr_rhythmgame_addnote(37.54, 1, 0);
//        scr_rhythmgame_addnote(37.9, 0, 0);
//        scr_rhythmgame_addnote(38.02, 1, 0);
//        scr_rhythmgame_addnote(38.38, 0, 0);
//        scr_rhythmgame_addnote(38.5, 1, 0);
//        scr_rhythmgame_addnote(38.86, 0, 39.34);
//        scr_rhythmgame_addnote(39.34, 0, 0);
//        scr_rhythmgame_addnote(39.46, 1, 0);
//        scr_rhythmgame_addnote(39.82, 0, 0);
//        scr_rhythmgame_addnote(39.94, 1, 40.42);
//        scr_rhythmgame_addnote(40.42, 1, 40.9);
//        scr_rhythmgame_addnote(40.9, 1, 0);
//*/
//
//class Note
//{
//	public:
//	int type;
//	double start;
//	double end;
//
//	Note(int _type, double _start, double _end)
//	{
//		type = _type;
//		start = _start;
//		end = _end;
//	}
//};
//
//vector<Note> notes;
//vector<Note> drumNotes;
//
//int currentLeft = 0;
//int currentRight = 0;
//
//Dropdown dropdown;
//Grid grid = Grid(9, -50, 0, bigPurple);
//Grid grid2 = Grid(9, 30, 50, smallPurple);
//
//void Start()
//{
//	camera.target = { 0, 0 };
//	camera.offset = Vector2();
//	camera.rotation = 0;
//	camera.zoom = 1;
//
//	notes = {};
//	drumNotes = {};
//
//	std::ifstream jsonChart("resources/chart.json");
//	chart = nlohmann::json::parse(jsonChart);
//}
//
//string Round(double value)
//{
//	double v2 = value * 100;
//	double v3 = round(v2) / 100;
//	string v4 = to_string(v3);
//	for (int i = 0; i < v4.size(); i++)
//	{
//		if (v4[i] == '.')
//		{
//			v4.resize(i + 3);
//			break;
//		}
//	}
//	return v4;
//}
//
//void Chart()
//{
//	int links = chart["charts"][0]["links"].size();
//	int nonLinks = chart["charts"][0]["notes"].size();
//
//	for (int i = 0; i < links; i++)
//	{
//		double start = chart["charts"][0]["links"][i]["startNote"]["songPos"];
//		start /= 1000;
//		start += 0.58;
//		double end = chart["charts"][0]["links"][i]["endNote"]["songPos"];
//		end /= 1000;
//		end += 0.58;
//		int lane = chart["charts"][0]["links"][i]["endNote"]["lane"];
//
//		if (lane == 0 || lane == 1)
//		{
//			Note note = Note(lane, start, end);
//			notes.push_back(note);
//
//			/*printf("scr_rhythmgame_addnote(");
//			printf(Round(start).c_str());
//			printf(", ");
//			printf(to_string(lane).c_str());
//			printf(", ");
//			printf(Round(end).c_str());
//			printf(");\n");*/
//		}
//		else
//		{
//			Note note = Note(lane - 2, start, end);
//			drumNotes.push_back(note);
//		}
//	}
//
//	for (int i = 0; i < nonLinks; i++)
//	{
//		double start = chart["charts"][0]["notes"][i]["songPos"];
//		start /= 1000;
//		start += 0.58;
//		int lane = chart["charts"][0]["notes"][i]["lane"];
//
//		if (lane == 0 || lane == 1)
//		{
//			bool unique = true;
//			for (int j = 0; j < notes.size(); j++)
//			{
//				if (start == notes[j].start)
//				{
//					unique = false;
//					break;
//				}
//			}
//
//			if (!unique) { continue; }
//
//			Note note = Note(lane, start, 0);
//			notes.push_back(note);
//
//			/*printf("scr_rhythmgame_addnote(");
//			printf(Round(start).c_str());
//			printf(", ");
//			printf(to_string(lane).c_str());
//			printf(", ");
//			printf("0");
//			printf(");\n");*/
//		}
//		else
//		{
//			Note note = Note(lane - 2, start, 0);
//			drumNotes.push_back(note);
//		}
//	}
//}
//
//vector<Note> Sort(vector<Note> slop)
//{
//	vector<Note> unslop = { slop[0] };
//	for (int i = 1; i < slop.size(); i++)
//	{
//		int j = 0;
//		while (unslop[j].start < slop[i].start)
//		{
//			j++;
//			if (j == unslop.size())
//			{
//				break;
//			}
//		}
//		unslop.insert(unslop.begin() + j, slop[i]);
//	}
//	return unslop;
//}
//
//int main()
//{
//	//---- Start ----//
//	Start();
//
//	InitWindow(screenWidth, screenHeight, "WAKE UP");
//	InitAudioDevice();
//	SetTargetFPS(targetFps);
//	GuiLoadStyle("resources/greenie.rgs");
//
//	Music song = LoadMusicStream("resources/psycho.ogg");
//
//	while (!WindowShouldClose())
//	{
//		UpdateMusicStream(song);
//
//		if (IsKeyPressed(KEY_P))
//		{
//			StopMusicStream(song);
//			notes = {};
//			printf("restarted\n");
//			PlayMusicStream(song);
//		}
//
//		/*if (IsKeyPressed(KEY_LEFT))
//		{
//			currentLeft = notes.size();
//			Note note = Note(0, GetMusicTimePlayed(song), 0);
//			notes.push_back(note);
//		}
//		if (IsKeyReleased(KEY_LEFT))
//		{
//			if (GetMusicTimePlayed(song) - notes[currentLeft].start > 0.2)
//			{
//				notes[currentLeft].end = GetMusicTimePlayed(song);
//			}
//		}
//
//		if (IsKeyPressed(KEY_RIGHT))
//		{
//			currentRight = notes.size();
//			Note note = Note(1, GetMusicTimePlayed(song), 0);
//			notes.push_back(note);
//		}
//		if (IsKeyReleased(KEY_RIGHT))
//		{
//			if (GetMusicTimePlayed(song) - notes[currentRight].start > 0.2)
//			{
//				notes[currentRight].end = GetMusicTimePlayed(song);
//			}
//		}*/
//
//		if (IsKeyPressed(KEY_ENTER))
//		{
//			/*for (int i = 0; i < notes.size(); i++)
//			{
//				printf("scr_rhythmgame_addnote(");
//				printf(Round(notes[i].start).c_str());
//				printf(", ");
//				printf(to_string(notes[i].type).c_str());
//				printf(", ");
//				printf(notes[i].end != 0 ? Round(notes[i].end).c_str() : "0");
//				printf(");\n");
//			}*/
//
//			Chart();
//			vector<Note> unslop = Sort(drumNotes);
//			for (int i = 0; i < unslop.size(); i++)
//			{
//				printf("scr_rhythmgame_addnote(");
//				printf(Round(unslop[i].start).c_str());
//				printf(", ");
//				printf(to_string(unslop[i].type).c_str());
//				printf(", ");
//				printf(unslop[i].end != 0 ? Round(unslop[i].end).c_str() : "0");
//				printf(");\n");
//			}
//		}
//
//		//---- Rendering ----//
//		BeginDrawing();
//		ClearBackground(BLACK);
//		//BeginMode2D(camera);
//
//		grid.Draw();
//		grid2.Draw();
//		/*DrawRectangle(0, 0, 800, 800, { 0, 0, 0, 0 });
//		dropdown.Draw();*/
//
//		EndMode2D();
//		EndDrawing();
//	}
//
//	UnloadMusicStream(song);
//
//	CloseAudioDevice();
//	CloseWindow();
//
//	return 0;
//}