#include <raylib-cpp.hpp>
#include <vector>
#include "global.hpp"
#include "shapeDefiner.hpp"

vector<Vector2> dirToIndex = { { 0, 0 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };
vector<vector<Vector2>> barrelPos = { { {19, 41}, {27, 23}, {49, 12}, {60, 24}, {66, 36} },
                                        { {5, 44}, {14, 25}, {34, 14}, {61, 25}, {68, 43} },
                                        { {11, 28}, {19, 10}, {46, -1}, {64, 9}, {75, 27} } };
Vector2 spriteSize = { 96, 64 };
vector<raylib::Rectangle> blocks = Box({ 0, 0 }, { 800, 500 }, 25);
Texture2D sheet = Texture2D();