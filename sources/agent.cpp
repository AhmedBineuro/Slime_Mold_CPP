#include "agent.h"
agent::agent()
{
    sprite = CircleShape();
    sprite.setFillColor(Color::Blue);
    sprite.setRadius(1);
    velocity = Vector2f(((float)rand() / (float)RAND_MAX) * ((float)MAX_SPEED * 2) - (float)MAX_SPEED, ((float)rand() / (float)RAND_MAX) * ((float)MAX_SPEED * 2) - (float)MAX_SPEED);
    normalizeAndScale(velocity, MAX_SPEED);
    position = Vector2f(((float)rand() / RAND_MAX) * WINDOW_WIDTH, ((float)rand() / RAND_MAX) * WINDOW_HEIGHT);
    sprite.setPosition(Vector2f(position.x - 1, position.y - 1));
    pheromoneColor = NULL;
    linked_vertex = NULL;
}
agent::agent(Vector2f position, Color *pheromoneChannel)
{
    sprite = CircleShape();
    sprite.setRadius(1);
    sprite.setPosition(Vector2f(position.x - 1, position.y - 1));
    velocity = Vector2f(((float)rand() / (float)RAND_MAX) * ((float)MAX_SPEED * 2) - (float)MAX_SPEED, ((float)rand() / (float)RAND_MAX) * ((float)MAX_SPEED * 2) - (float)MAX_SPEED);
    normalizeAndScale(velocity, MAX_SPEED);
    this->pheromoneColor = pheromoneColor;
    this->sprite.setFillColor(*(this->pheromoneColor));
    linked_vertex = NULL;
}
void agent::setPheromoneChannel(Color *pheromoneChannel)
{
    this->pheromoneColor = pheromoneChannel;
    this->sprite.setFillColor(*(this->pheromoneColor));
}
void agent::decideVelocityDirection(Image &frame)
{
    Vector2f forward = velocity;
    Vector2f left = getRotatedDeg(velocity, -SENSOR_ANGLE);
    Vector2f right = getRotatedDeg(velocity, SENSOR_ANGLE);
    normalizeAndScale(forward, SENSOR_DIST);
    normalizeAndScale(left, SENSOR_DIST);
    normalizeAndScale(right, SENSOR_DIST);
    float forwardPher = getRadiusValueAt(position + forward, frame);
    float leftPher = getRadiusValueAt(position + left, frame);
    float rightPher = getRadiusValueAt(position + right, frame);
    if (forwardPher >= leftPher && forwardPher >= rightPher)
        velocity = getNormalizedAndScaled(forward, MAX_SPEED);
    else if (leftPher >= forwardPher && leftPher >= rightPher)
        velocity = getNormalizedAndScaled(left, MAX_SPEED);
    else if (rightPher >= forwardPher && rightPher >= leftPher)
        velocity = getNormalizedAndScaled(right, MAX_SPEED);
    float turnChance = ((float)rand() / (float)RAND_MAX);
    if (turnChance <= RAND_TURN_CHANCE)
    {
        float angle = (float)(MAX_ANGLE - MIN_ANGLE) * ((float)rand() / (float)RAND_MAX) + ((float)MIN_ANGLE) + 1;
        rotateDeg(velocity, angle);
    }
}

void agent::linkVertex(Vertex *vert)
{
    linked_vertex = vert;
    linked_vertex->position = position;
    linked_vertex->color = *pheromoneColor;
}
// Function To get the average value of a radius at the given pixel
float agent::getRadiusValueAt(Vector2f pixelPos, Image &frame)
{
    float output = 0;
    int pixelCount = 0;
    for (int i = pixelPos.y - SENSOR_RADIUS; i < pixelPos.y + SENSOR_RADIUS; i++)
    {
        for (int j = pixelPos.x - SENSOR_RADIUS; j < pixelPos.x + SENSOR_RADIUS; j++)
        {
            float mag = sqrt(((i - pixelPos.y) * (i - pixelPos.y)) + ((j - pixelPos.x) * (j - pixelPos.x)));
            if (mag <= SENSOR_RADIUS)
            {
                int pixelX = j;
                int pixelY = i;
                if (pixelX < 0)
                    pixelX = WINDOW_WIDTH - 1;
                if (pixelX > WINDOW_WIDTH)
                    pixelX = 1;
                if (pixelY < 0)
                    pixelY = WINDOW_HEIGHT - 1;
                if (pixelY > WINDOW_HEIGHT)
                    pixelY = 1;
                Color c = frame.getPixel(pixelX, pixelY);
                Color pher;
                if (pheromoneColor != NULL)
                    pher = *(this->pheromoneColor);
                else
                    pher = Color::Blue;
                float totalSumPher = pher.r + pher.g + pher.b;
                float rWeight = (float)pher.r / totalSumPher;
                float gWeight = (float)pher.g / totalSumPher;
                float bWeight = (float)pher.b / totalSumPher;
                float r = c.r, g = c.g, b = c.b;
                output += ((r * rWeight) + (b * bWeight) + (g * gWeight)) / totalSumPher;
                pixelCount++;
            }
        }
    }
    if (pixelCount != 0)
        output = output / ((float)pixelCount);
    else
        output = 0;
    return output;
}
void agent::draw(RenderTarget &renderTarget)
{
    renderTarget.draw(sprite);
}
void agent::move(Image &frame)
{
    decideVelocityDirection(frame);
    position += velocity;
    if (position.x < 0)
    {
        position.x = WINDOW_WIDTH - 1;
    }
    else if (position.x > WINDOW_WIDTH)
    {
        position.x = 1;
    }
    if (position.y < 0)
    {
        position.y = WINDOW_HEIGHT - 1;
    }
    else if (position.y > WINDOW_HEIGHT)
    {
        position.y = 1;
    }
    if (linked_vertex != NULL)
        linked_vertex->position = position;
    else
        sprite.setPosition(Vector2f(position.x - 1, position.y - 1));
}