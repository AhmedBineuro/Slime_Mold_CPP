#include "core.h"
#include "vectorMath.h"
#define AGENT_COUNT 100000
#define SENSOR_DIST 10
#define SENSOR_RADIUS 4
#define SENSOR_ANGLE 15
#define MIN_ANGLE -90
#define MAX_ANGLE 90
#define MAX_SPEED 2
#define RAND_TURN_CHANCE 0.4f
#define DEFAULT_PHER Color::Blue
#define THREAD_COUNT 12
#define PHEROMONE_COUNT 3
using namespace vecm;
Color pheromones[PHEROMONE_COUNT] = {Color::Red, Color::Green, Color::Blue};
string WINDOW_TITLE;
Image currentFrameImage;
bool run;
RenderTexture diffusionLayer, currentFrame;
Vector2f oldmousePos;
Clock fps_clock;
Time deltaTime;
Time fpsUpdateTime = seconds(1.0f);
pthread_t threadPool[THREAD_COUNT];
Time elapsedUpdateTime = Time::Zero;
RectangleShape evaporation_filter;
Shader diffusion_shader;
Texture currentTexture;
Sprite nextFrameSprite;
RenderWindow window;
Vector2f velocities[AGENT_COUNT];
VertexArray agentCells;
void init();
void *moveThreaded(void *threadId);
void decideDirection(int &agentIndex);
void move(int &agentIndex);
float getRadiusValueAt(Vector2f position, int &agentIndex);
int main()
{
    srand(time(NULL));
    // window.create(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, Style::Fullscreen);
    window.create(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    agentCells = VertexArray(Points, AGENT_COUNT);
    init();
    window.setPosition(Vector2(0, 0));
    bool hold = false;
    unsigned int frameCount = 0;
    /////////////////
    diffusion_shader.setUniform("resolution", Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    while (run)
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                run = false;
                break;
            case Event::KeyPressed:
                switch (event.key.code)
                {
                case Keyboard::Escape:
                    run = false;
                    break;
                }
                break;
            case Event::MouseButtonPressed:
            {
                hold = true;
                break;
            }
            case Event::MouseButtonReleased:
            {
                Vector2i pos = Mouse::getPosition(window);
                hold = false;
                // d.fling(oldmousePos, Vector2f(pos.x, pos.y));
            }
            break;
            default:
                break;
            }
        }
        deltaTime = fps_clock.restart();
        elapsedUpdateTime += deltaTime;
        frameCount++;

        if (elapsedUpdateTime >= fpsUpdateTime)
        {
            float fps = static_cast<float>(frameCount) / elapsedUpdateTime.asSeconds();
            std::string fpsString = WINDOW_TITLE + "|FPS: " + to_string(fps);

            window.setTitle(fpsString);
            frameCount = 0;
            elapsedUpdateTime -= fpsUpdateTime;
        }
        if (!hold)
        {
            for (int i = 0; i < THREAD_COUNT; i++)
            {
                int failed = pthread_create(&threadPool[i], NULL, moveThreaded, &i);
                if (failed)
                {
                    printf("Error: unnable to create thread:%d\n", i);
                    exit(-1);
                }
            }
            for (int i = 0; i < THREAD_COUNT; i++)
            {
                int failed = pthread_join(threadPool[i], NULL);
                if (failed)
                {
                    printf("Error: unnable to join thread:%d\n", i);
                    exit(-1);
                }
            }
        }
        currentFrame.draw(evaporation_filter);
        currentFrame.draw(agentCells);
        currentTexture = currentFrame.getTexture();
        diffusion_shader.setUniform("texture", currentTexture);
        diffusionLayer.draw(Sprite(currentFrame.getTexture()), &diffusion_shader);
        nextFrameSprite = Sprite(diffusionLayer.getTexture());
        window.clear();
        window.draw(nextFrameSprite);
        window.display();
        currentFrame.draw(nextFrameSprite);
        currentFrameImage = nextFrameSprite.getTexture()->copyToImage();
    }
    window.close();
    return 0;
}

void init()
{
    WINDOW_TITLE = "Slime Mold Workshop";
    run = true;
    diffusionLayer.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    diffusionLayer.setRepeated(true);
    currentFrame.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    currentFrameImage = diffusionLayer.getTexture().copyToImage();
    evaporation_filter = RectangleShape();
    evaporation_filter.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    evaporation_filter.setFillColor(Color(0, 0, 0, 20));
    evaporation_filter.setPosition(Vector2f(0, 0));
    if (!diffusion_shader.loadFromFile("diffusion.frag", Shader::Fragment))
    {
        printf("Error Loading shader\n");
    }
    for (int i = 0; i < AGENT_COUNT; i++)
    {
        velocities[i] = Vector2f(((float)rand() / (float)RAND_MAX) * ((float)MAX_SPEED * 2) - (float)MAX_SPEED, ((float)rand() / (float)RAND_MAX) * ((float)MAX_SPEED * 2) - (float)MAX_SPEED);
        agentCells[i] = Vertex(Vector2f(((float)rand() / RAND_MAX) * WINDOW_WIDTH, ((float)rand() / RAND_MAX) * WINDOW_HEIGHT));
        agentCells[i].color = pheromones[(i % 3)];
    }
    currentTexture = currentFrame.getTexture();
    nextFrameSprite = Sprite(currentTexture);
}
void *moveThreaded(void *threadId)
{
    int id = *((int *)threadId);
    int lookupWindowWidth = (float)AGENT_COUNT / (float)THREAD_COUNT;
    int start = lookupWindowWidth * id;
    for (int i = start; i < (start + lookupWindowWidth) && i < AGENT_COUNT; i++)
        move(i);
    pthread_exit(NULL);
}
// Function To get the average value of a radius at the given pixel
float getRadiusValueAt(Vector2f pixelPos, int &agentIndex)
{
    float output = 0;
    int pixelCount = 0;
    for (int i = pixelPos.y - SENSOR_RADIUS; i < pixelPos.y + SENSOR_RADIUS; i++)
    {
        for (int j = pixelPos.x - SENSOR_RADIUS; j < pixelPos.x + SENSOR_RADIUS; j++)
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
            Color c = currentFrameImage.getPixel(pixelX, pixelY);
            Color pher = agentCells[agentIndex].color;
            float totalSumPher = pher.r + pher.g + pher.b;
            float rWeight = (float)pher.r / totalSumPher;
            float gWeight = (float)pher.g / totalSumPher;
            float bWeight = (float)pher.b / totalSumPher;
            float r = c.r, g = c.g, b = c.b;
            output += ((r * rWeight) + (b * bWeight) + (g * gWeight)) / totalSumPher;
            pixelCount++;
        }
    }
    return output;
}
void decideDirection(int &agentIndex)
{
    Vector2f position = agentCells[agentIndex].position;
    Vector2f forward = velocities[agentIndex];
    Vector2f left = getRotatedDeg(forward, -SENSOR_ANGLE);
    Vector2f right = getRotatedDeg(forward, SENSOR_ANGLE);
    normalizeAndScale(forward, SENSOR_DIST);
    normalizeAndScale(left, SENSOR_DIST);
    normalizeAndScale(right, SENSOR_DIST);
    float forwardPher = getRadiusValueAt(position + forward, agentIndex);
    float leftPher = getRadiusValueAt(position + left, agentIndex);
    float rightPher = getRadiusValueAt(position + right, agentIndex);
    if (forwardPher >= leftPher && forwardPher >= rightPher)
        velocities[agentIndex] = getNormalizedAndScaled(forward, MAX_SPEED);
    else if (leftPher >= forwardPher && leftPher >= rightPher)
        velocities[agentIndex] = getNormalizedAndScaled(left, MAX_SPEED);
    else if (rightPher >= forwardPher && rightPher >= leftPher)
        velocities[agentIndex] = getNormalizedAndScaled(right, MAX_SPEED);
    float turnChance = ((float)rand() / (float)RAND_MAX);
    if (turnChance <= RAND_TURN_CHANCE)
    {
        float angle = (float)(MAX_ANGLE - MIN_ANGLE) * ((float)rand() / (float)RAND_MAX) + ((float)MIN_ANGLE) + 1;
        rotateDeg(velocities[agentIndex], angle);
    }
    return;
}
void move(int &agentIndex)
{
    decideDirection(agentIndex);
    agentCells[agentIndex].position += velocities[agentIndex];
    if (agentCells[agentIndex].position.x < 0)
    {
        agentCells[agentIndex].position.x = WINDOW_WIDTH - 1;
    }
    else if (agentCells[agentIndex].position.x > WINDOW_WIDTH)
    {
        agentCells[agentIndex].position.x = 1;
    }
    if (agentCells[agentIndex].position.y < 0)
    {
        agentCells[agentIndex].position.y = WINDOW_HEIGHT - 1;
    }
    else if (agentCells[agentIndex].position.y > WINDOW_HEIGHT)
    {
        agentCells[agentIndex].position.y = 1;
    }
}