#include "core.h"
#include "dvd.h"
#include "agent.h"
#define THREAD_COUNT 8
Color pheromoes[4] = {Color::Blue, Color::Red, Color::Green, Color::Yellow};
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
RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
VertexArray agentCells;
agent agents[AGENT_COUNT];
void init();
void *moveThreaded(void *threadIndex);
int main()
{
    srand(time(NULL));
    agentCells = VertexArray(Points, AGENT_COUNT);
    // for (int i = 0; i < AGENT_COUNT / 3; i++)
    // {
    //     agents[i] = agent();
    //     agents[i].setPheromoneChannel(&pheromoes[0]);
    // }
    // for (int i = AGENT_COUNT / 3; i < AGENT_COUNT * (2.f / 3.f); i++)
    // {
    //     agents[i] = agent();
    //     agents[i].setPheromoneChannel(&pheromoes[1]);
    // }
    // for (int i = AGENT_COUNT * (2.f / 3.f); i < AGENT_COUNT; i++)
    // {
    //     agents[i] = agent();
    //     agents[i].setPheromoneChannel(&pheromoes[2]);
    // }
    for (int i = 0; i < AGENT_COUNT; i++)
    {
        agents[i] = agent();
        agents[i].setPheromoneChannel(&pheromoes[3]);
    }
    for (int i = 0; i < AGENT_COUNT; i++)
    {
        agents[i].linkVertex(&(agentCells[i]));
    }
    init();
    window.setPosition(Vector2(0, 0));
    bool hold = false;
    unsigned int frameCount = 0;
    /////////////////
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
        diffusion_shader.setUniform("resolution", Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        diffusion_shader.setUniform("texture", currentFrame.getTexture());
        diffusionLayer.draw(Sprite(currentFrame.getTexture()), &diffusion_shader);
        // Uncomment next for stable patterns
        // for (int i = 0; i < AGENT_COUNT; i++)
        // {
        //     agents[i].draw(diffusionLayer);
        // }
        //  window.clear(Color::White);
        window.clear();
        window.draw(Sprite(diffusionLayer.getTexture()));
        window.display();
        currentFrame.draw(Sprite(diffusionLayer.getTexture()));
        currentFrameImage = diffusionLayer.getTexture().copyToImage();
    }
    window.close();
    return 0;
}

void init()
{
    WINDOW_TITLE = "Slime Mold Workshop";
    run = true;
    diffusionLayer.create(WINDOW_WIDTH, WINDOW_HEIGHT);
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
}
void *moveThreaded(void *threadId)
{
    int id = *((int *)threadId);
    int lookupWindowWidth = (float)AGENT_COUNT / (float)THREAD_COUNT;
    int start = lookupWindowWidth * id;
    for (int i = start; i < (start + lookupWindowWidth) && i < AGENT_COUNT; i++)
        agents[i].move(currentFrameImage);
    pthread_exit(NULL);
}