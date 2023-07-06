#include "dvd.h"
// Fun visualization of the diffusion
dvd::dvd()
{
    radius = DVD_RADIUS;
    position = Vector2f(((float)rand() / RAND_MAX) * WINDOW_WIDTH, ((float)rand() / RAND_MAX) * WINDOW_HEIGHT);
    velocity = Vector2f(((float)rand() / RAND_MAX) * speed, ((float)rand() / RAND_MAX) * speed);
    sprite = CircleShape(radius);
    sprite.setPosition(position);
    sprite.setFillColor(Color::Red);
}
void dvd::move()
{
    if (position.x - radius < 0 || position.x + radius > WINDOW_WIDTH)
        velocity.x *= -1;
    if (position.y - radius < 0 || position.y + radius > WINDOW_HEIGHT)
        velocity.y *= -1;
    position += velocity;
    sprite.setPosition(Vector2f(position.x - radius, position.y - radius));
}
void dvd::draw(RenderTarget &renderTarget)
{
    float ratio = ((float)rand() / RAND_MAX);
    Color c = Color(0, 0, 255);
    sprite.setFillColor(c);
    renderTarget.draw(sprite);
}
void dvd::setPosition(Vector2f position)
{
    this->position = position;
    sprite.setPosition(Vector2f(position.x - radius, position.y - radius));
}
void dvd::setPosition(Vector2i position)
{
    this->position = Vector2f(position.x, position.y);
    sprite.setPosition(Vector2f(this->position.x - radius, this->position.y - radius));
}
void dvd::fling(Vector2f oldPosition, Vector2f newPosition)
{
    Vector2f difference = newPosition - oldPosition;
    this->velocity = difference;
}