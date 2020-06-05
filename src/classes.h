#ifndef CLASSES_H
#define CLASSES_H
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
using namespace std;
class Object{
    public:
    int x;
    int y;
    vector<vector<int> > shape;
    string file;
    Object(int a,int b,vector<vector<int> > structure);
    
};
class Engine{
    private:
    SDL_Event e;
    bool quit = false;
    SDL_Window* gWindow = NULL;
    SDL_Surface* gHelloWorld = NULL;
    SDL_Renderer* gRender = NULL;
    SDL_Texture* gTexture = NULL;

    SDL_Texture* building =NULL;
    const int SCREEN_WIDTH = 600;
    const int SCREEN_HEIGHT = 600;



    void getPosition(int i,int j,int &x,int &y,int tile_size);
    public:
    bool init();
    bool loadMedia();
    void close();
    void draw(int n,vector<vector<int> > mm);
    void run();
    void event_handler();
};
struct Camera{
    int x;
    int y;
};

#endif