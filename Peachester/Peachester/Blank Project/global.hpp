#pragma once
#include <raylib-cpp.hpp>
#include "font.hpp"

extern const int screenWidth;
extern const int screenHeight;

extern Color bigGreen;
extern Color smallGreen;
extern Color bigPurple;
extern Color smallPurple;
extern Color krisBlue;
extern Color susiePink;
extern Color ralseiGreen;
extern Color berdlyColor;

extern DeltaFont bigFont;
extern DeltaFont hpFont;

void Print(std::string str);
void Print(int n);
void Print(float n);
void Print(double n);
void Print(size_t n);

extern void DrawRectOutline(Rectangle rect, float thickness, Color color);

float Slerp(float start, float end, float seconds, float& timer);

bool Z();
bool X();