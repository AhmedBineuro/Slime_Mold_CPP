#include "core.h"
#include "dvd.h"
#include "agent.h"
#define THREAD_COUNT 6
string WINDOW_TITLE;
Image currentFrameImage;
bool run;
RenderTexture diffusionLayer, currentFrame;
Vector2f oldmousePos;
Clock fps_clock;
Time deltaTime;
Time fpsUpdateTime = seconds(1.0f);
Time elapsedUpdateTime = Time::Zero;
RectangleShape evaporation_filter;
Shader diffusion_shader;
RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
void init();
void *updateThreaded(void *threadIndex);
int main()
{
    srand(time(0));
    agent agents[AGENT_COUNT];
    // for (int i = 0; i < AGENT_COUNT / 3; i++)
    // {
    //     agents[i] = agent();
    //     agents[i].setPheromoneChannel('R');
    // }
    // for (int i = AGENT_COUNT / 3; i < (2 * AGENT_COUNT / 3); i++)
    // {
    //     agents[i] = agent();
    //     agents[i].setPheromoneChannel('B');
    // }
    // for (int i = (2 * AGENT_COUNT / 3); i < AGENT_COUNT; i++)
    // {
    //     agents[i] = agent();
    //     agents[i].setPheromoneChannel('G');
    // }
    for (int i = 0; i < AGENT_COUNT; i++)
    {
        agents[i] = agent();
        agents[i].setPheromoneChannel('W');
    }
    init();
    window.setPosition(Vector2(0, 0));
    // DVD demo code
    bool hold = false;
    unsigned int frameCount = 0;
    // dvd d = dvd();
    // dvd ds[DVD_COUNT];
    // for (int i = 0; i < DVD_COUNT; i++)
    //     ds[i] = dvd();
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
        // if (!hold)
        // {
        //     // d.move();
        //     for (int i = 0; i < DVD_COUNT; i++)
        //         ds[i].move();
        // }
        // else
        // {
        //     oldmousePos = Vector2f(Mouse::getPosition().x, Mouse::getPosition().y);
        //     // d.setPosition(Mouse::getPosition());
        // }
        // d.draw(currentFrame);
        if (!hold)
        {
            for (int i = 0; i < AGENT_COUNT; i++)
            {
                agents[i].move(currentFrameImage);
            }
        }
        currentFrame.draw(evaporation_filter);
        for (int i = 0; i < AGENT_COUNT; i++)
        {
            agents[i].draw(currentFrame);
        }
        // for (int i = 0; i < DVD_COUNT; i++)
        //     ds[i].draw(currentFrame);
        diffusion_shader.setUniform("resolution", Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        diffusion_shader.setUniform("texture", currentFrame.getTexture());
        diffusionLayer.draw(Sprite(currentFrame.getTexture()), &diffusion_shader);
        // window.clear(Color::White);
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
    evaporation_filter.setFillColor(Color(0, 0, 0, 40));
    evaporation_filter.setPosition(Vector2f(0, 0));
    if (!diffusion_shader.loadFromFile("diffusion.frag", Shader::Fragment))
    {
        printf("Error Loading shader\n");
    }
}
void *updateThreaded(void *threadId)
{
}