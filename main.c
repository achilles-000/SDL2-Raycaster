#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define FOV 90


void makeWall(int array[8][8], SDL_Renderer *renderer){

    for(int i = 0; i < 8; i++){

        int width = 100, height = 100;

        for(int j = 0; j < 8; j++){
            if (array[i][j] == 1 ){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect rect = {100 * j, 100 * i, width, height};
                SDL_RenderFillRect(renderer, &rect);               
            }
            /*
            else {
                SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
                SDL_Rect rect = {100 * j, 100 * i, width, height};
                SDL_RenderFillRect(renderer, &rect); 
            }       
            */       
        }
    }
}

void isWall(int array[8][8], int *row, int *column){
    for (int i = 0; i < 8; i++ ){
        for (int j = 0; j < 8; j++){
            if(array[i][j] == 1){
                *row = i;
                *column = j;
                return;
            }
        }
    }
}


void player(SDL_Renderer *renderer, bool forward, bool backward, int *cx, int *cy, double *a, bool rotateRight, bool rotateLeft){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int radius = 25;
    int speed = 2;
    double rx;
    double ry;

     *a = fmod(*a, 2 * M_PI);
        if (*a < 0) *a += 2 * M_PI;

    rx = radius*cos(*a);
    ry = radius*sin(*a);

    //printf("\ncos: %lf ", rx);
    //printf(" sin: %lf ", ry);
    //printf(" a: %lf ", *a);

    

    if(forward){
        *cx += speed * cos(*a + 3.931);
        *cy += speed *sin(*a + 3.931);
    }
    if(backward){
        *cx -= speed * cos(*a + 3.931);
        *cy -= speed *sin(*a + 3.931);
    }

    if(rotateRight == true) *a += 0.05;
    if(rotateLeft == true) *a -= 0.05;

    SDL_RenderDrawLine(renderer, *cx + rx, *cy + ry, *cx + ry, *cy - rx); //1-2
    SDL_RenderDrawLine(renderer, *cx + ry, *cy - rx, *cx - rx, *cy - ry); //2-3
    SDL_RenderDrawLine(renderer, *cx - rx, *cy - ry, *cx - ry, *cy + rx); //3-4
    SDL_RenderDrawLine(renderer, *cx + rx, *cy + ry, *cx - ry, *cy + rx); //1-4

}

void castRay(SDL_Renderer *renderer, int cx, int cy, double a, int array[8][8]){
    int row, column;

    //while (1);



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
    SDL_Window *window = SDL_CreateWindow("Movable Window",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800, 800,
                                          SDL_WINDOW_SHOWN);
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
    int speedY = 3;
    int cx = 400;
    int cy = 200;
    double a = 3.931;

        int row, column;

    int array[8][8] = {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1}
    };





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

     SDL_SetRenderDrawColor(renderer, 255, 0 ,0, 255);
     SDL_RenderClear(renderer);

    // Set the draw color to red and draw a filled rectangle
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_Rect rect = {100, 100, 200, 150};
    //SDL_RenderFillRect(renderer, &rect);

    
    makeWall(array, renderer);

    player(renderer, forward, backward, &cx, &cy, &a, rotateRight, rotateLeft);

    castRay(renderer, cx, cy, a, array);

    isWall(array, &row, &column);







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