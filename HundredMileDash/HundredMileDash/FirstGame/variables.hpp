#pragma once
#include <raylib-cpp.hpp>
#include <cmath>
#include <time.h>
#include <string>
#include <vector>
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Camera2D camera;
bool gameover;
float globalTimeScale;
bool timeflow;

//---- Binds ----//
KeyboardKey forward1 = KEY_UP;
KeyboardKey forward2 = KEY_W;
KeyboardKey left1 = KEY_LEFT;
KeyboardKey left2 = KEY_A;
KeyboardKey right1 = KEY_RIGHT;
KeyboardKey right2 = KEY_D;
KeyboardKey restart = KEY_R;
KeyboardKey dash = KEY_SPACE;
int dashMouse = 0;

//---- Input ----//
bool holdingDash;

//---- Player ----//
float startSize = 40;
float minWidthAngle = 35;
float maxWidthAngle = 45;
float speed = 3000;
float drag = 0.05;
float maxVelocity = 800;
float minRotSpeed = 3;
float maxRotSpeed = 6;
float dashTime = 0.4;
float minDashVelocity = maxVelocity * 0.6;
float ghostFadeSpeed = 3;
float pieceShrinkSpeed = 0.005;
float pieceSpinSpeed = 0.2;
float pieceMoveSpeed = 1;

//---- Juice ----//
float juice;
float juiceRecharge = 0.1;
float juicePlanDecay = 0.3;
float juiceRequiredDash = 0;
float juiceCostDash = (float)1 / 3;

//---- Screen Shake ----//
float screenShake;
float screenShakeTime = 0.5;
float screenShakeMagnitude = 10;

//---- Screen Shrink ----//
float screenShrinkInterval = 0.5;
float screenShrinkAmount = 2;