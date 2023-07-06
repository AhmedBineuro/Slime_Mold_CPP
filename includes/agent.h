#ifndef SIM_AGENT
#define SIM_AGENT
#include "core.h"
#include "vectorMath.h"
#define AGENT_COUNT 8000
#define SENSOR_DIST 20
#define SENSOR_RADIUS 10
#define SENSOR_ANGLE 10
#define MIN_ANGLE -10
#define MAX_ANGLE 10
#define MAX_SPEED 4
using namespace vecm;
class agent
{
public:
    agent();
    agent(Vector2f position, char pheromoneChannel);
    void setPheromoneChannel(char pheromoneChannel);
    float getRadiusValueAt(Vector2f pixelPos, Image &frame);
    void decideVelocityDirection(Image &frame);
    void move(Image &frame);
    void draw(RenderTarget &RenderTarget);

private:
    CircleShape sprite;
    Vector2f position, velocity;
    char pheromoneChannel;
};
#endif