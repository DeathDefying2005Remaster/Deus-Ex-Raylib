#define _CRT_SECURE_NO_WARNINGS

#include "iostream"
#include "fstream"

#include <raylib-cpp.hpp>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "note.hpp"
#include "ui_things.hpp"
#include "track.hpp"
#include "input_field.hpp"
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Texture cursor;
Texture cursorWhite;
Texture cursorInside;
Texture note;

vector<Note> notes;

float tempo = 120;
float offset = 0;
int timeSig = 4;
int subBeatSnap = 1;
bool panning = false;
const char* path = "resources/psycho.ogg";
bool tempoEditMode = false;
bool playing = false;
bool erasing = false;
double playTime = 0;

//---- UI ----//
Grid grid = Grid(9, 1, -50, 0, bigPurple);
Grid grid2 = Grid(9, 1, 30, 50, smallPurple);
Track track = Track(panning, playing, erasing, tempo, offset, timeSig, subBeatSnap, playTime, notes);
InputField input = InputField(tempo, { 550, 70 }, { 30, 20 }, 30, 300);
InputField offsetInput = InputField(offset, { 630, 70 }, { 30, 20 }, -2000, 2000);

void Start()
{
	GuiLoadStyle("resources/greenie.rgs");
	cursor = LoadTexture("resources/cursor.png");
	cursorWhite = LoadTexture("resources/cursor_white.png");
	cursorInside = LoadTexture("resources/cursor_inside.png");
	note = LoadTexture("resources/note.png");
}

string Round(double value)
{
	double v2 = value * 100;
	double v3 = round(v2) / 100;
	string v4 = to_string(v3);
	for (int i = 0; i < v4.size(); i++)
	{
		if (v4[i] == '.')
		{
			v4.resize(i + 3);
			break;
		}
	}
	return v4;
}

void PrintE(string str, string* text)
{
	printf(str.c_str());
	text->append(str);
}

void Export()
{
	string text = "";

	PrintE("//--------\n", &text);
	PrintE("//BPM: ", &text);
	PrintE(to_string(tempo), &text);
	PrintE("\n", &text);
	PrintE("//OFFSET: ", &text);
	PrintE(to_string(offset), &text);
	PrintE("\n", &text);
	PrintE("//NOTES:\n", &text);
	PrintE("\n", &text);
	for (int i = 0; i < notes.size(); i++)
	{
		PrintE("scr_rhythmgame_addnote(", &text);
		float start = notes[i].start - (offset / 1000);
		PrintE(Round(start).c_str(), &text);
		PrintE(", ", &text);
		PrintE(to_string(notes[i].type).c_str(), &text);
		PrintE(", ", &text);
		float end = notes[i].end - (offset / 1000);
		PrintE(notes[i].end != 0 && notes[i].end != notes[i].start ? Round(end).c_str() : "0", &text);
		PrintE(");\n", &text);
	}
	PrintE("\n", &text);
	PrintE("//--------\n", &text);

	ofstream file("chart.txt");
	file << text;
	file.close();
}

void Load()
{
	notes = {};
	ifstream file("chart.txt");
	string text;
	string lineText;
	while (getline(file, lineText))
	{
		text += lineText;
		text += '\n';

		if (lineText[2] == 'B')
		{
			string bpmStr = "";
			int slop = 7;
			while (lineText[slop] != '\n')
			{
				bpmStr += lineText[slop];
				slop++;
			}
			tempo = stof(bpmStr);
			input.SetValue();
		}
		if (lineText[2] == 'O')
		{
			string offsetStr = "";
			int slop = 10;
			while (lineText[slop] != '\n')
			{
				offsetStr += lineText[slop];
				slop++;
			}
			offset = stof(offsetStr);
			offsetInput.SetValue();
		}

		if (lineText[0] == 's')
		{
			string startStr = "";
			string typeStr = "";
			string endStr = "";
			int shcib = lineText.find("(") + 1;
			while (lineText[shcib] != ',')
			{
				startStr += lineText[shcib] ;
				shcib++;
			}
			shcib += 2;
			while (lineText[shcib] != ',')
			{
				typeStr += lineText[shcib];
				shcib++;
			}
			shcib += 2;
			while (lineText[shcib] != ')')
			{
				endStr += lineText[shcib];
				shcib++;
			}
			double start = Clamp(stod(startStr) + (offset / 1000), 0, 9999);
			double end = stod(endStr);
			if (end != 0) end = Clamp(end + (offset / 1000), 0, 9999);
			notes.push_back(Note(stoi(typeStr), start, end));
		}
	}
}

int main()
{
	//---- Start ----//
	InitWindow(screenWidth, screenHeight, "WAKE UP");
	InitAudioDevice();
	SetTargetFPS(targetFps);
	HideCursor();
	Start();

	Music song = LoadMusicStream(path);
	track.songLength = GetMusicTimeLength(song);
	PlayMusicStream(song);
	PauseMusicStream(song);

	while (!WindowShouldClose())
	{
		UpdateMusicStream(song);

		if (IsFileDropped())
		{
			FilePathList droppedFiles = LoadDroppedFiles();
			char* newPath = droppedFiles.paths[0];
			string str = newPath;
			string end = str.substr(str.size() - 4, 4);
			if (end == ".ogg" || end == ".wav" || end == ".mp3")
			{
				path = newPath;
				StopMusicStream(song);
				playing = false;
				track.pos = 0;
				playTime = 0;
				song = LoadMusicStream(path);
				track.songLength = GetMusicTimeLength(song);
			}
			UnloadDroppedFiles(droppedFiles);
		}

		if (IsKeyPressed(KEY_SPACE) && !panning && !input.active && playTime < track.songLength)
		{
			playing = !playing;
			if (playing && playTime >= offset / 1000)
			{
				SeekMusicStream(song, playTime - (offset / 1000));
				ResumeMusicStream(song);
			}
			else { PauseMusicStream(song); }
		}
		if (IsKeyPressed(KEY_BACKSPACE) && !input.active && !offsetInput.active)
		{
			PauseMusicStream(song);
			track.pos = 0;
			playTime = 0;
		}

		//destroy the spineless, just break the silence
		if (playing)
		{
			playTime += GetFrameTime();
			if (!IsMusicStreamPlaying(song) && playTime >= (offset / 1000))
			{
				SeekMusicStream(song, playTime - (offset / 1000));
				ResumeMusicStream(song);
			}
			if (playTime > track.songLength) { PauseMusicStream(song); playing = false; }
			track.pos = playTime * track.zoom;
		}
		track.Update();
		if (panning)
		{
			playTime = track.pos / track.zoom;
		}

		float lastTempo = tempo;
		if (IsMouseButtonPressed(0)) { input.Click(); offsetInput.Click(); }
		input.TypeInput();
		offsetInput.TypeInput();
		if (lastTempo != tempo)
		{
			for (int i = 0; i < notes.size(); i++)
			{
				float changeInterruptsEveryAndAll = lastTempo / tempo;
				notes[i].start *= changeInterruptsEveryAndAll;
				notes[i].end *= changeInterruptsEveryAndAll;
			}
		}

		//---- Rendering ----//
		BeginDrawing();
		ClearBackground(BLACK);

		grid.Draw();
		grid2.Draw();
		DrawLineEx({ 400, 200 }, { 400, 600 }, 4, { 0, 255, 0, 100 });
		track.Draw();

		/*GuiSpinner({ 50, 50, 90, 40 }, NULL, &timeSig, 1, 16, false);
		GuiSpinner({ 200, 50, 90, 40 }, NULL, &subBeatSnap, 1, 16, false);
		int lastTempo = tempo;f
		if (GuiValueBox({ 350, 50, 40, 40 }, "bpm", &lastTempo, 50, 300, tempoEditMode)) tempoEditMode = !tempoEditMode;*/
		/*if (tempo != lastTempo)
		{
			for (int i = 0; i < notes.size(); i++)
			{
				int changeInterruptsEveryAndAll = tempo / lastTempo;
				notes[i].start *= changeInterruptsEveryAndAll;
				notes[i].end *= changeInterruptsEveryAndAll;
			}
		}*/

		input.Draw();
		offsetInput.Draw();
		GuiSpinner({ 400, 50, 100, 40 }, "", &subBeatSnap, 1, 16, false);
		if (GuiButton({ 50, 50, 70, 40 }, "save")) Export();
		if (GuiButton({ 140, 50, 70, 40 }, "load")) Load();

		DrawText("bpm", 537, 25, 15, smallGreen);
		DrawText("offset (ms)", 595, 25, 15, smallGreen);
		DrawText("snap", 430, 25, 15, smallGreen);

		Rectangle source = { float(panning ? 8 : erasing ? 16 : 0), 0, 8, 8 };
		Rectangle destination = { GetMousePosition().x, GetMousePosition().y, 24, 24 };
		if (erasing) { destination.x -= 12; destination.y -= 12; }
		DrawTexturePro(cursorWhite, source, destination, {}, 0, krisBlue);
		DrawTexturePro(cursorInside, source, destination, {}, 0, BLACK);

		EndDrawing();
	}

	UnloadMusicStream(song);

	CloseAudioDevice();
	CloseWindow();

	return 0;
}