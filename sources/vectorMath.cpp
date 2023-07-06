#include "vectorMath.h"
namespace vecm
{
    float degrees(float radianAngle)
    {
        return radianAngle * (180.f / M_PI);
    }
    float radians(float degreeAngle)
    {
        return degreeAngle * (M_PI / 180.f);
    }
    float getMagnitude(Vector2f vec)
    {
        return sqrt((vec.x * vec.x) + (vec.y * vec.y));
    }

    float getAngleDeg(Vector2f vec)
    {
        return degrees(atan2(vec.y, vec.x));
    }
    float getAngleRad(Vector2f vec)
    {
        return atan2(vec.y, vec.x);
    }

    void rotateRad(Vector2f &vec, float angle)
    {
        Vector2f oldVec = vec;
        vec.x = (oldVec.x * cos(angle)) - (oldVec.y * sin(angle));
        vec.y = (oldVec.y * cos(angle)) + (oldVec.x * sin(angle));
        return;
    }
    void rotateDeg(Vector2f &vec, float angle)
    {
        float angleRad = radians(angle);
        Vector2f oldVec = vec;
        vec.x = (oldVec.x * cos(angleRad)) - (oldVec.y * sin(angleRad));
        vec.y = (oldVec.y * cos(angleRad)) + (oldVec.x * sin(angleRad));
        return;
    }
    Vector2f getRotatedDeg(Vector2f vec, float angle)
    {
        Vector2f output = vec;
        rotateDeg(output, angle);
        return output;
    }
    Vector2f getRotatedRad(Vector2f vec, float angle)
    {
        Vector2f output = vec;
        rotateRad(output, angle);
        return output;
    }

    void normalize(Vector2f &vec)
    {
        float magnitude = getMagnitude(vec);
        if (magnitude != 0)
        {
            vec.x /= magnitude;
            vec.y /= magnitude;
        }
    }
    void normalizeAndScale(Vector2f &vec, float scalar)
    {
        normalize(vec);
        vec *= scalar;
    }

    Vector2f getNormalized(Vector2f vec)
    {
        Vector2f output = vec;
        normalize(output);
        return output;
    }
    Vector2f getNormalizedAndScaled(Vector2f vec, float scalar)
    {
        return getNormalized(vec) * scalar;
    }
    void printVector(Vector2f vec)
    {
        printf("(%f,%f)\n", vec.x, vec.y);
    }
}