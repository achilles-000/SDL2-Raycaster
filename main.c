#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define FOV 60
#define WIDTH 800
#define HEIGHT 800




void player(SDL_Renderer *renderer, bool forward, bool backward, int *cx, int *cy, double *a, bool rotateRight, bool rotateLeft){

    int speed = 2;

     *a = fmod(*a, 2 * M_PI);
        if (*a < 0) *a += 2 * M_PI;

    if(forward){
        *cx += speed * cos(*a + 3.931);
        *cy += speed *sin(*a + 3.931);
    }
    if(backward){
        *cx -= speed * cos(*a + 3.931);
        *cy -= speed *sin(*a + 3.931);
    }

    if(rotateRight == true) *a += 0.015;
    if(rotateLeft == true) *a -= 0.015;

}

void makeWall3D(SDL_Renderer *renderer, int cx, int cy, double a, int array[8][8]){
    int xLocation, yLocation;
    int length;
    double xRayLocation, yRayLocation;
    double sections = 200;
    double angle = (FOV * (M_PI / 180)) / sections; 
    double rayAngle = 0;

    //For rectangle
    double rayLength;
    double rectWidth;
    double rectHeight;
    double placeHolder;

    for(int i = 0; i <= sections; i++){
        rayAngle = (a + 3.931) - (FOV * M_PI / 360) + (i * angle);
        length = 0;

        while(length < WIDTH && length < HEIGHT){  // Or length < 800

            xRayLocation = (cx + length * cos(rayAngle));
            yRayLocation = (cy + length * sin(rayAngle));

            //Ray coordinate
            xLocation = xRayLocation / 100;
            yLocation = yRayLocation / 100;

            if(array[yLocation][xLocation] == 1){
                //Find Length of the Ray
                rayLength = (sqrt(pow((cx - xRayLocation), 2) + pow((cy - yRayLocation), 2)));
            
                //Find width
                rectWidth = WIDTH / sections;

                //Find Height
                rectHeight = (120 * HEIGHT) / rayLength; 

                //Find (x,y) coordinates of where the rectage should be
                int topLeftX = (i * rectWidth);
                int topLeftY = (HEIGHT / 2) - (rectHeight / 2);

                SDL_Rect rect = { topLeftX, topLeftY, rectWidth, rectHeight }; 
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Rectange colour (white)
                SDL_RenderFillRect(renderer, &rect); 
                
                break;
            }
            length++;
        }
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { 
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    //Img
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Movable Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //Variables
    bool forward = false;
    bool backward = false;
    bool rotateRight = false;
    bool rotateLeft = false;
    int cx = 400;
    int cy = 200;
    double a = 0;


    int array[8][8] = {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,1,1},
        {1,1,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1}
    };

/*
    int array[16][16] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,1,1,0,0,0,0,0,1,0,1},
        {1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
        {1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
*/

    // Main loop
    SDL_bool running = SDL_TRUE;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        // Save the initial window position
                        int initialX, initialY;
                        SDL_GetWindowPosition(window, &initialX, &initialY);

                        // Move the window with the mouse
                        int windowX, windowY;
                        while (SDL_PollEvent(&event)) {
                            if (event.type == SDL_MOUSEMOTION) {
                                windowX = initialX + event.motion.x - event.button.x;
                                windowY = initialY + event.motion.y - event.button.y;
                                SDL_SetWindowPosition(window, windowX, windowY);
                            } else if (event.type == SDL_MOUSEBUTTONUP &&
                                       event.button.button == SDL_BUTTON_LEFT) {
                                break;
                            }
                        }
                    }
                    break;

                case SDL_KEYDOWN: {
                    if (event.key.keysym.sym == SDLK_w) {
                        forward = true;
                    }
                    if (event.key.keysym.sym == SDLK_s) {
                        backward = true;
                    }
                    if (event.key.keysym.sym == SDLK_d) {
                        rotateRight = true;
                    }
                    if (event.key.keysym.sym == SDLK_a) {
                        rotateLeft = true;
                    }
                    break;

                }
                case SDL_KEYUP: {
                    if (event.key.keysym.sym == SDLK_w) {
                        forward = false;
                    }
                    if (event.key.keysym.sym == SDLK_s) {
                        backward = false;
                    }
                    if (event.key.keysym.sym == SDLK_d) {
                        rotateRight = false;
                    }
                    if (event.key.keysym.sym == SDLK_a) {
                        rotateLeft = false;
                    }
                    break;
                }
                default:
                    break;
            }
        }

    SDL_SetRenderDrawColor(renderer, 0, 0 ,0, 255);
    SDL_RenderClear(renderer);

    player(renderer, forward, backward, &cx, &cy, &a, rotateRight, rotateLeft);

    makeWall3D(renderer, cx, cy, a, array);

    // Update the screen
    SDL_RenderPresent(renderer);

    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}