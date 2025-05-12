#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
int *pixelBuffer;

constexpr int EXIT = -1;
constexpr int SCENE_0 = 0;
int scene = SCENE_0;

inline void setPixel(int *buffer, int width, int x, int y, int r, int g, int b, int a = 255)
{
    buffer[y * width + x] = (a << 24) | (b << 16) | (g << 8) | r;
}

inline void renderTest()
{
    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            setPixel(pixelBuffer, WINDOW_WIDTH, x, y, rand() % 256, rand() % 256, rand() % 256);
        }
    }

    char *pix;
    int pitch;
    SDL_LockTexture(texture, NULL, (void **)&pix, &pitch);
    for (int i = 0, sp = 0, dp = 0; i < WINDOW_HEIGHT; i++, dp += WINDOW_WIDTH, sp += pitch)
        memcpy(pix + sp, pixelBuffer + dp, WINDOW_WIDTH * 4);

    SDL_UnlockTexture(texture);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

inline void handleInput()
{
    SDL_Event e;
    if (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
            scene = EXIT;
    }
}

void scene0()
{
    // initialize resources for this scene

    while (scene == SCENE_0)
    {
        handleInput();

        // game logic

        renderTest();
    }

    // cleanup resources for this scene
}

int main(int argc, char **argv)
{
    std::cout << "Hello\n";

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        return -1;

    window = SDL_CreateWindow("SDL3 Window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, NULL);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    pixelBuffer = new int[WINDOW_WIDTH * WINDOW_HEIGHT];

    if (!pixelBuffer || !window || !renderer || !texture)
        return -1;

    while (true)
    {
        switch (scene)
        {
        case SCENE_0:
            scene0();
            break;

        default:
            scene = EXIT;
            break;
        }
        if (scene == EXIT)
            break;
    }

    delete[] pixelBuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}