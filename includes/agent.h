#ifndef SIM_AGENT
#define SIM_AGENT
#include "core.h"
#include "vectorMath.h"
// Stable pattern param count=30000 senDist=10 senRad=8 senAng=30 max-min=40
#define AGENT_COUNT 1000000
#define SENSOR_DIST 12
#define SENSOR_RADIUS 4
#define SENSOR_ANGLE 15
#define MIN_ANGLE -90
#define MAX_ANGLE 90
#define MAX_SPEED 2
#define RAND_TURN_CHANCE 0.3f
#define DEFAULT_PHER Color::Blue
using namespace vecm;
class agent
{
public:
    agent();
    agent(Vector2f position, Color *pheromoneChannel);
    void setPheromoneChannel(Color *pheromoneChannel);
    float getRadiusValueAt(Vector2f pixelPos, Image &frame);
    void decideVelocityDirection(Image &frame);
    void linkVertex(Vertex *vert);
    void move(Image &frame);
    void draw(RenderTarget &RenderTarget);

private:
    CircleShape sprite;
    Vertex *linked_vertex;
    Vector2f *linked_position, linked_velocity;
    Color *pheromoneColor;
    Vector2f position, velocity;
};
#endif