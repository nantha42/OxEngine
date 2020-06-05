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
    vector<SDL_Texture*> textures;
    
    SDL_Texture* building =NULL;
    int SCREEN_WIDTH = 600;
    int SCREEN_HEIGHT = 600;
    int camerax = 0;
    int cameray = 0;
    struct EventTriggered{
        bool k_a;
        bool k_w;
        bool k_s;
        bool k_d;
        bool k_space;
        bool k_shift;
        bool k_up;
        bool k_down;
        bool k_left;
        bool k_right;
    }events_triggered;


    void getPosition(int i,int j,int &x,int &y,int tile_size);
    public:
    Engine(int screenwidth,int screenheight);
    Engine();
    bool init();
    bool loadMedia();
    void close();
    void isoworlddraw(int n,vector<vector<int> > map);
    void run();
    void update();
    void event_handler();
};
struct Camera{
    int x;
    int y;
};

#endif