#pragma once
#include <raylib-cpp.hpp>
#include "note.hpp"

class Track
{
	public:

	float height = 180;
	float thickness = 3;

	float songLength;
	float& tempo;
	float& offset;
	int& timeSig;
	int& subBeatSnap;
	float zoom; //pixels per second
	float pos;
	bool& panning;
	bool placing;
	bool& erasing;
	bool& playing;
	float snappedX;
	float snappedY;
	double& timePlayed;

	std::vector<Note>& notes;

	Track(bool& _panning, bool& _playing, bool& _erasing, float& _tempo, float& _offset, int& _timeSig, int& _subBeatSnap, double& _timePlayed, std::vector<Note>& _notes)
		: panning(_panning), erasing(_erasing), timeSig(_timeSig), tempo(_tempo), offset(_offset), subBeatSnap(_subBeatSnap), notes(_notes), playing(_playing), timePlayed(_timePlayed)
	{
		songLength = 30;
		tempo = 120;
		zoom = 40;
		pos = 0;
		snappedX = 400;
		snappedY = 400;
		placing = false;
	}

	void Update()
	{
		if (IsMouseButtonPressed(2) && 300 < GetMouseY() && GetMouseY() < 500 && !playing)
		{
			panning = true;
			SetMouseCursor(5);
		}
		if (IsMouseButtonReleased(2))
		{
			panning = false;
			SetMouseCursor(0);
		}
		if (panning)
		{
			pos -= GetMouseDelta().x;
		}

		float newZoom = Clamp(zoom + 2.5*GetMouseWheelMoveV().y, 5, 200);
		float fractionSongProgress = pos / (songLength * zoom);

		float newPos = fractionSongProgress * (songLength * newZoom);
		zoom = newZoom;
		pos = Clamp(newPos, 0, songLength * zoom);

		float snapLength = zoom / float(tempo / 60) / subBeatSnap;
		snappedX = floor((GetMousePosition().x - 400 + pos) / snapLength + 0.5) * snapLength;
		snappedX = Clamp(snappedX, 0, songLength * zoom);
		snappedY = GetMouseY() < 400 ? 358 : 442;

		Rectangle trackRect = { 400 - pos, float(400 - (height * 0.5)), songLength * zoom, height };
		if (IsMouseButtonPressed(0) && CheckCollisionPointRec(GetMousePosition(), trackRect) && !playing && !panning)
		{
			double newNotePos = snappedX / zoom;
			int newNoteType = snappedY < 400 ? 0 : 1;

			bool contains = false;
			for (int i = 0; i < notes.size(); i++)
			{
				if (abs(notes[i].start - newNotePos) <= 0.001 && notes[i].type == newNoteType)
				{
					contains = true;
					break;
				}
			}
			if (!contains)
			{
				placing = true;
				notes.push_back(Note(newNoteType, newNotePos, 0));
			}
		}
		if (placing)
		{
			notes[notes.size() - 1].end = Clamp(snappedX / zoom, notes[notes.size() - 1].start, songLength);
		}
		if (IsMouseButtonReleased(0))
		{
			placing = false;
		}

		if (IsMouseButtonPressed(1) && !playing && !panning)
		{
			erasing = true;
		}
		if (IsMouseButtonReleased(1))
		{
			erasing = false;
		}
		if (erasing)
		{
			for (int i = 0; i < notes.size(); i++)
			{
				double notePos = (GetMouseX() - 400 + pos) / zoom;
				int noteType = snappedY < 400 ? 0 : 1;
				if (abs(notes[i].start - notePos) * zoom <= 6 && notes[i].type == noteType)
				{
					notes.erase(notes.begin() + i);
					break;
				}
			}
		}
	}

	void Draw()
	{
		float pixelOffset = offset / 1000 * zoom;
		
		(pixelOffset);
		Vector2 bottom = { 400 - pos + pixelOffset, float(400 - (height * 0.5)) };
		DrawTriangle(bottom, { bottom.x + 7, bottom.y - 10 }, { bottom.x - 7, bottom.y - 10 }, YELLOW);

		float divisidorSupremeAuthorityOfHowBigTheBarsAre = zoom * ((float)1 / (tempo / float(60))); //zoom * seconds per beat
		int shped = 1;
		//while (divisidorSupremeAuthorityOfHowBigTheBarsAre * shped < 20)
		//{
		//	shped *= 2;
		//}
		float beats = floor(tempo * songLength / float(60)); //number of beats in song

		int fourth = shped - 1;
		for (int i = shped; i < beats; i += shped)
		{
			float x = i * divisidorSupremeAuthorityOfHowBigTheBarsAre;

			DrawLineEx({ 400 - pos + x, float(400 - (height * 0.5)) }, { 400 - pos + x, float(400 + (height * 0.5)) }, fourth == timeSig - 1 ? 6 : 2, { 255, 255, 255, 100 });
			fourth == timeSig - 1 ? fourth = shped - 1 : fourth += shped;
		}

		Color back = { 0, 0, 0, 100 };
		DrawRectangle(400 - pos, 400 - (height * 0.5), songLength * zoom, height, back);
		DrawRectOutline(raylib::Rectangle(400 - pos, 400 - (height * 0.5), songLength * zoom, height), 3, krisBlue);

		for (int i = 0; i < notes.size(); i++)
		{
			Rectangle dest = { 400 - pos + float(notes[i].start) * zoom, float(notes[i].type == 0 ? 358 : 442), 34 * 2, 10 * 2 };
			DrawTexturePro(note, { 0, 0, 34, 10 }, dest, { 17 * 2, 5 * 2 }, 90, krisBlue);
			float end = 400 - pos + float(notes[i].end) * zoom;
			if (notes[i].end != 0) DrawLineEx({dest.x, dest.y}, {end, dest.y}, 15, krisBlue);
		}

		if (!panning && !playing && !erasing && !placing) DrawTexturePro(note, {0, 0, 34, 10}, {400 + snappedX - pos, snappedY, 34 * 2, 10 * 2}, {17 * 2, 5 * 2}, 90, krisBlue);
	}
};