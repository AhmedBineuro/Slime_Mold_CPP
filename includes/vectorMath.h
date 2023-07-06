#ifndef VECTOR_MATH
#define VECTOR_MATH
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace sf;
namespace vecm
{
    float degrees(float radianAngle);
    float radians(float degreeAngle);
    float getMagnitude(Vector2f vec);

    float getAngleDeg(Vector2f vec);
    float getAngleRad(Vector2f vec);

    void rotateDeg(Vector2f &vec, float angle);
    void rotateRad(Vector2f &vec, float angle);
    Vector2f getRotatedDeg(Vector2f vec, float angle);
    Vector2f getRotatedRad(Vector2f vec, float angle);

    void normalize(Vector2f &vec);
    void normalizeAndScale(Vector2f &vec, float scalar);

    Vector2f getNormalized(Vector2f vec);
    Vector2f getNormalizedAndScaled(Vector2f vec, float scalar);
    void printVector(Vector2f vec);
}
#endif