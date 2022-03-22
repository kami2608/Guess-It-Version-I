#include <bits/stdc++.h>
#include <SDL2/SDL.h>
using namespace std;

const int SCREEN_HEIGHT = 517;
const int SCREEN_WIDTH = 861;
const int MAX_NUM = 100;

SDL_Window *window = NULL;
SDL_Renderer *ren = NULL;
SDL_Texture *bg, *host, *human, *start, *small, *big, *same;
SDL_Texture *num[MAX_NUM+1];

int generateRandomNumber();
int getPlayerGuess();
void printAnswer(int number, int secretNum);

bool init();
SDL_Texture *loadTexture(string path);
void waitUntilKeyPressed();
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void close();

void loadMedia();
void showGame();
void printHost();
void printHuman();
void printStart();
void printComputerComparision(SDL_Texture *tex);
void printNum(SDL_Texture *tex);

//*************** Main code *********
int main(int argc, char *argv[]) {
    loadMedia();
    srand(time(0));
    int secretNum = generateRandomNumber();
    int number;
    do {
        number = getPlayerGuess();
        printAnswer(number, secretNum);
    } while (number != secretNum);
    waitUntilKeyPressed();
    close();
    return 0;
}
//***********************************************
int generateRandomNumber() {
    showGame();
    SDL_Delay(1500);
    printStart();
    return rand() % MAX_NUM + 1;
}

int getPlayerGuess() {
    int number = 0;
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        SDL_Delay(10);
        if ( SDL_WaitEvent(&e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            close();
            exit(1);
        }
        // SDLK_KP_ENTER is unable
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
            quit = true;
        if(e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_0:
                    number = number * 10;
                    break;
                case SDLK_1:
                    number = number * 10 + 1;
                    break;
                case SDLK_2:
                    number = number * 10 + 2;
                    break;
                case SDLK_3:
                    number = number * 10 + 3;
                    break;
                case SDLK_4:
                    number = number * 10 + 4;
                    break;
                case SDLK_5:
                    number = number * 10 + 5;
                    break;
                case SDLK_6:
                    number = number * 10 + 6;
                    break;
                case SDLK_7:
                    number = number * 10 + 7;
                    break;
                case SDLK_8:
                    number = number * 10 + 8;
                    break;
                case SDLK_9:
                    number = number * 10 + 9;
                    break;
                default :
                    break;
            }
        }
    }
    SDL_Delay(100);
    showGame();
    printHuman();
    SDL_Delay(1500);
    showGame();
    printNum(num[number]);
    SDL_Delay(1500);
    return number;
}

void printAnswer(int number, int secretNum) {
    printHost();
    SDL_Delay(1500);
    SDL_Texture *tex;
    if(number > secretNum) tex = big;
    else if(number < secretNum) tex = small;
    else tex = same;
    printComputerComparision(tex);
}

//***********************************************
void showGame() {
    SDL_RenderClear(ren);
    renderTexture(bg, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void printStart() {
    renderTexture(start, ren, 555, 132);
    SDL_RenderPresent(ren);
}

void printHost() {
    renderTexture(host, ren, 552, 130);
    SDL_RenderPresent(ren);
}

void printHuman() {
    renderTexture(human, ren, 296, 107);
    SDL_RenderPresent(ren);
}

void printNum(SDL_Texture *tex) {
    renderTexture(tex, ren, 315, 102);
    SDL_RenderPresent(ren);
}

void printComputerComparision(SDL_Texture *tex) {
    renderTexture(tex, ren, 550, 147);
    SDL_RenderPresent(ren);
}

//***********************************************
bool init() {
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        success = false;
        cout<<"SDL could not be initialized   : "<<SDL_GetError()<<endl;
    }
    else {
        window = SDL_CreateWindow("Guess It", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL) {
            cout<<"Window could not be created  : "<<SDL_GetError()<<endl;
            success = false;
        }
        else 
            ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
    return success;
}

SDL_Texture *loadTexture(string path) {
    SDL_Texture *tex = NULL;
    SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
    if(loadedSurface == NULL) 
        cout<<"Could not load image surface  : "<<SDL_GetError()<<endl;
    else {
        tex = SDL_CreateTextureFromSurface(ren, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    return tex;
}

void loadMedia() {
    if(!init()) cout<<"Fail init"<<endl;
    else {
        bg = loadTexture("images/game.bmp");
        host = loadTexture("images/computer.bmp");
        human = loadTexture("images/human.bmp");
        start = loadTexture("images/start.bmp");
        small = loadTexture("images/small.bmp");
        big = loadTexture("images/big.bmp");
        same = loadTexture("images/same.bmp");
        bool check = true;
        for(int i=1; i<=MAX_NUM ; i++) {
            num[i] = loadTexture("images/"+to_string(i)+".bmp");
            if(num[i] == NULL) {
                check = false;
                break;
            }
        }
        if(!check || bg == NULL || host == NULL || human == NULL || start == NULL
        || small == NULL || big == NULL || same == NULL) {
            close();
        }
    }
}

void waitUntilKeyPressed() {
    SDL_Event e;
    while(true) {
        if(SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_QUIT)) 
            return;
        SDL_Delay(100);
    }
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void close() {
    SDL_DestroyTexture(bg);
    bg = NULL;
    SDL_DestroyTexture(host);
    host = NULL;
    SDL_DestroyTexture(human);
    human = NULL;
    SDL_DestroyTexture(start);
    start = NULL;
    SDL_DestroyTexture(small);
    small = NULL;
    SDL_DestroyTexture(big);
    big = NULL;
    SDL_DestroyTexture(same);
    same = NULL;
    for(int i=0; i<MAX_NUM ; i++) 
        SDL_DestroyTexture(num[i]);
    SDL_DestroyRenderer(ren);
    ren = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}