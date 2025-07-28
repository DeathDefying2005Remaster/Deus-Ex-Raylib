#pragma once
#include <raylib-cpp.hpp>

extern Color bigGreen;
extern Color smallGreen;
extern Color bigPurple;
extern Color smallPurple;
extern Color krisBlue;
extern Color susiePink;
extern Color ralseiGreen;

extern Texture cursor;
extern Texture cursorWhite;
extern Texture cursorInside;
extern Texture note;

void Print(std::string str);
void Print(int n);
void Print(float n);
void Print(double n);
void Print(size_t n);

extern void DrawRectOutline(Rectangle rect, float thickness, Color color);