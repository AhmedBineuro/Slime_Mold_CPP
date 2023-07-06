#pragma once
#ifndef DVD_DEMO
#define DVD_DEMO
#include "core.h"
#define DVD_COUNT 10000
#define DVD_RADIUS 1
static float speed = 4;
// Fun visualization of the diffusion
class dvd
{
public:
    float radius;
    Vector2f position, velocity;
    CircleShape sprite;
    bool pause;
    dvd();
    void move();
    void draw(RenderTarget &renderTarget);
    void setPosition(Vector2f position);
    void setPosition(Vector2i position);
    void fling(Vector2f oldPosition, Vector2f newPosition);
};
#endif