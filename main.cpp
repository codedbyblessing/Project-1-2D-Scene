/**
* Author: Blessing Yeboah
* Assignment: Simple 2D Scene
* Date due: 2025-09-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "raylib.h"
#include "CS3113/cs3113.h"
#include <math.h>
#include <vector>

// Enums
//enum AppStatus { TERMINATED, RUNNING };

// Global Constants
constexpr int SCREEN_WIDTH  = 1200;
constexpr int SCREEN_HEIGHT = 675;
constexpr int FPS           = 60;

// Asset paths
constexpr char BG_FP[]       = "assets/background.png";
constexpr char FISH_FP[]     = "assets/fish.png";
constexpr char NOTE_FP[]     = "assets/music note.png";
constexpr char ELEPHANT_FP[] = "assets/elephant.png";

// Elephant constants
constexpr float RADIUS      = 100.0f;
constexpr float ORBIT_SPEED = 0.05f;

// Fish constants
constexpr float BASE_SIZE     = 100.0f;
constexpr float MAX_AMPLITUDE = 20.0f;

// Globals
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;
float gTime          = 0.0f;
float gNoteY         = 0.0f;

// Textures
Texture2D gBackground;
Texture2D gFish;
Texture2D gNote;
Texture2D gElephant;

// Animation vars
float gFishX         = SCREEN_WIDTH - 250.0f;
float gFishY         = 100.0f;
float gFishVX        = 150.0f;  
float gFishVY        = 100.0f;  
float gFishScale     = BASE_SIZE;
float gNoteAngle     = 0.0f;
float gElephantAngle = 0.0f;

// Background colors (pink shades)
std::vector<Color> BG_COLOURS = {
    {255, 182, 193, 255}, // LightPink
    {255, 105, 180, 255}, // HotPink
    {255, 192, 203, 255}, // Pink
    {219, 112, 147, 255}, // Violet Red-ish
    {255, 20, 147, 255}   // DeepPink
};
int gBGIndex = 0;
float gBGTimer = 0.0f; // timer for changing background every 4 seconds

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blessings Simple 2D Scene");

    gBackground = LoadTexture(BG_FP);
    gFish       = LoadTexture(FISH_FP);
    gNote       = LoadTexture(NOTE_FP);
    gElephant   = LoadTexture(ELEPHANT_FP);

    SetTargetFPS(FPS);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    gTime += deltaTime;

    // Background color timer
    gBGTimer += deltaTime;
    if (gBGTimer >= 1.0f) {
        gBGTimer = 0.0f;
        gBGIndex = (gBGIndex + 1) % BG_COLOURS.size();
    }

    // Fish movement (diagonal bouncing)
    gFishX += gFishVX * deltaTime;
    gFishY += gFishVY * deltaTime;

    // Bounce within right rectangle
    float fishMinX = SCREEN_WIDTH - 300.0f;
    float fishMaxX = SCREEN_WIDTH - 50.0f;
    float fishMinY = 50.0f;
    float fishMaxY = 250.0f;

    if (gFishX < fishMinX) { gFishX = fishMinX; gFishVX *= -1; }
    if (gFishX > fishMaxX) { gFishX = fishMaxX; gFishVX *= -1; }
    if (gFishY < fishMinY) { gFishY = fishMinY; gFishVY *= -1; }
    if (gFishY > fishMaxY) { gFishY = fishMaxY; gFishVY *= -1; }

    gFishScale = BASE_SIZE - MAX_AMPLITUDE * sin(gTime * 3.0f);

    // Music note vertical oscillation on left
    float noteAmplitude = 50.0f;
    float noteSpeed     = 3.0f;
    float sineValue     = sin(gTime * noteSpeed);
    gNoteY = SCREEN_HEIGHT / 2.0f + noteAmplitude * sineValue;
    gNoteAngle = 20.0f * sineValue;

    // Elephant orbit
    gElephantAngle += ORBIT_SPEED;
}

void render() {
    BeginDrawing();

    // Background color (solid pink shade)
    ClearBackground(BG_COLOURS[gBGIndex]);

    // Draw background image 
    Rectangle bgSrc = { 0.0f, 0.0f, (float)gBackground.width, (float)gBackground.height };
    Rectangle bgDest = { 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    Vector2 bgOrigin = { 0.0f, 0.0f };
    DrawTexturePro(gBackground, bgSrc, bgDest, bgOrigin, 0.0f, WHITE);

    // Draw fish
    Rectangle fishSrc  = { 0.0f, 0.0f, (float)gFish.width, (float)gFish.height };
    Rectangle fishDest = { gFishX, gFishY, gFishScale, gFishScale };
    Vector2 fishOrigin = { gFishScale / 2, gFishScale / 2 };
    DrawTexturePro(gFish, fishSrc, fishDest, fishOrigin, 0.0f, WHITE);

    // Draw music note
    Rectangle noteSrc  = { 0.0f, 0.0f, (float)gNote.width, (float)gNote.height };
    Rectangle noteDest = { 100.0f, gNoteY, 100, 100 }; 
    Vector2 noteOrigin = { 50, 50 };
    DrawTexturePro(gNote, noteSrc, noteDest, noteOrigin, gNoteAngle, WHITE);

    // Draw elephant
    float centerX = SCREEN_WIDTH / 2.0f;
    float centerY = SCREEN_HEIGHT / 2.0f + 150;
    float elephantX = centerX + RADIUS * cos(gElephantAngle);
    float elephantY = centerY + RADIUS * sin(gElephantAngle);

    Rectangle elephantSrc = { 0.0f, 0.0f, (float)gElephant.width, (float)gElephant.height };
    Rectangle elephantDest = { elephantX, elephantY, 180, 180 }; 
    Vector2 elephantOrigin = { 90, 90 };
    DrawTexturePro(gElephant, elephantSrc, elephantDest, elephantOrigin, gElephantAngle * 30, WHITE);

    EndDrawing();
}

void shutdown() {
    UnloadTexture(gBackground);
    UnloadTexture(gFish);
    UnloadTexture(gNote);
    UnloadTexture(gElephant);
    CloseWindow();
}

int main() {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}
