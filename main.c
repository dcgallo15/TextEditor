#include <SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "vec.h"
#include "util.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// SDL Check Int
void scc(int a) {
    if (a != 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(1);
    }
}

// SDL Check pointer
void *scp(void* p) {
    if (p == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(1);
    }
    return p;
}

#define FONT_IMAGE_WIDTH 128
#define FONT_IMAGE_HEIGHT 64
#define FONT_COLS 18
#define FONT_ROWS 7
#define FONT_CHAR_WIDTH floorf(FONT_IMAGE_WIDTH / FONT_COLS)
#define FONT_CHAR_HEIGHT floorf(FONT_IMAGE_HEIGHT / FONT_ROWS)

// Text Buffer
#define MAX_COLS 1024
#define MAX_LINES 16
char buffer[MAX_LINES][MAX_COLS]; // 16 lines, 1024 characters long
size_t bufferSize[MAX_LINES] = {0};
size_t lineNum = 0;

int textScaleFactor = 2;

Vec2 cursorPos;

SDL_Texture* initCursor(SDL_Renderer* renderer) {
    cursorPos.x = 0;
    cursorPos.y = 0;
    SDL_Surface* cursorSurface = scp(SDL_CreateRGBSurface(0, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT, 4, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF));
    SDL_Texture* cursorTexture = scp(SDL_CreateTextureFromSurface(renderer, cursorSurface));
    return cursorTexture;
}

void renderCursor(SDL_Renderer* renderer, SDL_Texture* tex) {
    SDL_Rect src = {
        .x = 0,
        .y = 0,
        .w = FONT_CHAR_WIDTH,
        .h = FONT_CHAR_HEIGHT
    };

    SDL_Rect dst = {
        .x = cursorPos.x * FONT_CHAR_WIDTH * textScaleFactor,
        .y = cursorPos.y * FONT_CHAR_HEIGHT * textScaleFactor,
        .w = FONT_CHAR_WIDTH * textScaleFactor,
        .h = FONT_CHAR_HEIGHT * textScaleFactor
    };
    scc(SDL_RenderCopy(renderer, tex, &src, &dst));
}

// TODO: improve efficency by storing a list of charcters at the sttart of the program
// TODO: ensure unknown text can be rendered as '?' or something equivalent
void renderChar(SDL_Renderer* renderer, SDL_Texture* tex, char c, int x, int y) {
    const size_t index = c - 32;
    const size_t col = index % FONT_COLS;
    const size_t row = index / FONT_COLS;
    SDL_Rect src = {
        .x = col * FONT_CHAR_WIDTH,
        .y = row * FONT_CHAR_HEIGHT,
        .w = FONT_CHAR_WIDTH,
        .h = FONT_CHAR_HEIGHT
    };

    SDL_Rect dst = {
        .x = x * FONT_CHAR_WIDTH * textScaleFactor,
        .y = y * FONT_CHAR_HEIGHT * textScaleFactor,
        .w = FONT_CHAR_WIDTH * textScaleFactor,
        .h = FONT_CHAR_HEIGHT * textScaleFactor
    };
    scc(SDL_RenderCopy(renderer, tex, &src, &dst));
}

void renderBuffer(SDL_Renderer* renderer, SDL_Texture* tex) {
    for(size_t j = 0; j <= lineNum; j++) {
        for (size_t i = 0; i < bufferSize[j]; i++) {
            renderChar(renderer, tex, buffer[j][i], i, j);
        }
    }
}

int main(void) {
    scc(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window* window = scp(SDL_CreateWindow(
			    "Text Editor",
			    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    SCREEN_WIDTH, SCREEN_HEIGHT,
			    SDL_WINDOW_SHOWN
			    ));
    
    SDL_Renderer* renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    SDL_Surface* fontSurface = scp(IMG_Load("charmap-oldschool_white.png"));
    SDL_Texture* fontTexture = scp(SDL_CreateTextureFromSurface(renderer, fontSurface));

    SDL_Texture* cursorTexture = initCursor(renderer);

    bool ctrlPressed = false;

    bool runnning = true;
    while (runnning == true) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT: {
                runnning = false;
                break;
            }
            case SDL_TEXTINPUT: {
                if (ctrlPressed == false) {
                    size_t textSize = strlen(event.text.text);
                    const size_t remainingSpace = MAX_COLS - bufferSize[cursorPos.y];
                    if (remainingSpace < textSize) {
                        textSize = remainingSpace;
                    }
                    insertIntoStaticArray(buffer[cursorPos.y], bufferSize[cursorPos.y] + textSize, cursorPos.x, event.text.text, textSize);
                    bufferSize[cursorPos.y] += textSize;
                    cursorPos.x += textSize;
                }
                break;
            }
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym)
                {
                case SDLK_BACKSPACE: {
                    if (bufferSize[cursorPos.y] > 0) {
                        // Need to remove the element behind the cursor's position
                        removeElemFromStaticArray(buffer[cursorPos.y], bufferSize[cursorPos.y], (cursorPos.x - 1));
                        bufferSize[cursorPos.y] -= 1;
                        cursorPos.x -= 1;
                    } else {
                        if (cursorPos.y > 0) {
                            cursorPos.y -= 1;
                            cursorPos.x = bufferSize[cursorPos.y];
                        }
                    }
                    break;
                }
                case SDLK_RETURN: {
                    if (MAX_LINES > lineNum) {
                        lineNum += 1;
                        cursorPos.y += 1;
                        if (cursorPos.x > bufferSize[cursorPos.y]) {
                            cursorPos.x = bufferSize[cursorPos.y];
                        }
                    }
                    break;
                }
                case SDLK_s : {
                    if (ctrlPressed == true) {
                        printf("SAVE FILE\n");
                    }
                    break;
                }
                case SDLK_MINUS : {
                    if (ctrlPressed == true && textScaleFactor > 1) {
                        textScaleFactor -= 1;
                    }
                    break;
                }
                case SDLK_EQUALS : {
                    if (ctrlPressed == true && textScaleFactor < 10) {
                        textScaleFactor += 1;
                    }
                    break;
                }
                case SDLK_LCTRL: {
                    ctrlPressed = true;
                    break;
                }
                case SDLK_UP: {
                    if (cursorPos.y > 0 && lineNum > 0) {
                        cursorPos.y -= 1;
                        if (cursorPos.x > bufferSize[cursorPos.y]) {
                            cursorPos.x = bufferSize[cursorPos.y];
                        }
                    }
                    break;
                }
                case SDLK_DOWN: {
                    if (cursorPos.y < lineNum) {
                        cursorPos.y += 1;
                        if (cursorPos.x > bufferSize[lineNum]) {
                            cursorPos.x = bufferSize[lineNum];
                        }
                    }
                    break;
                }
                case SDLK_LEFT: {
                    if (cursorPos.x > 0) {
                        cursorPos.x -= 1;
                    }
                    break;
                }
                case SDLK_RIGHT: {
                    if (cursorPos.x < bufferSize[lineNum]) {
                        cursorPos.x += 1;
                    }
                    break;
                }
                default:
                    break;
                }
                break;
            }
            case SDL_KEYUP: {
                switch (event.key.keysym.sym)
                {
                case SDLK_LCTRL: {
                    ctrlPressed = false;
                    break;
                }
                
                default:
                    break;
                }
                break;
            } 
            default:
                break;
            }
        }

        // Render:
        renderBuffer(renderer, fontTexture);
        renderCursor(renderer, cursorTexture);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    SDL_FreeSurface(fontSurface);
    SDL_DestroyTexture(fontTexture);
    SDL_DestroyTexture(cursorTexture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}