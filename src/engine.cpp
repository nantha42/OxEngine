#include<bits/stdc++.h>
#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include "classes.h"
using namespace std;


struct isometric_position_comparator
{
    inline bool operator() (const Sprite& sprite1, const Sprite& sprite2)
    {
        if(sprite1.isox < sprite2.isox){
            return (sprite1.isoy < sprite2.isoy);
        }
        return false;
    }
};

Engine::Engine(int screenwidth,int screenheight){
    this->SCREEN_WIDTH = screenwidth;
    this->SCREEN_HEIGHT = screenheight;
}
Engine::Engine(){
}

bool Engine::init(Game &ggame)
{
    //Initialization flag
    bool success = true;
    this->game = &ggame;
    
    // sort(this->game->sprites.begin(),this->game->sprites.end(),isometric_position_comparator);
    
    sort(game->sprites.begin(),game->sprites.end(),isometric_position_comparator());
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }

        else
        {
            if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG)){
                cout<<"Error Img INit"<<endl;
            }
            gRender = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
        }
    }

    return success;
}
bool Engine::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load splash image
    
    gHelloWorld = IMG_Load("../Assets/Images/isometric_tile.png");
    if( gHelloWorld == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "images/hello_world.bmp", SDL_GetError() );
        success = false;
    }else{
        SDL_SetRenderDrawColor(gRender,0x00,0x00,0x00,0x00);
        gTexture = SDL_CreateTextureFromSurface(gRender,gHelloWorld);
        this->textures.push_back(gTexture);
        if(gTexture == NULL)
            cout<<"Texture is Null"<<endl;
        SDL_FreeSurface(gHelloWorld);

        gHelloWorld = NULL;
        gHelloWorld = IMG_Load("../Assets/Images/building1.png");
        building = SDL_CreateTextureFromSurface(gRender,gHelloWorld);
        this->textures.push_back(building);
        if(building == NULL)
            cout<<"Texture is Null"<<endl;
        SDL_FreeSurface(gHelloWorld);
        gHelloWorld = NULL;

        gHelloWorld = IMG_Load("../Assets/Images/isometric_water_tile.png");
        // building = 
        this->textures.push_back(SDL_CreateTextureFromSurface(gRender,gHelloWorld));
        // if(building == NULL)
        //     cout<<"Texture is Null"<<endl;

    }   
    return success;
}
void Engine::close()
{
    //Deallocate surface
    SDL_DestroyRenderer(gRender);

    //Destroy window
    SDL_DestroyWindow( gWindow );
    
    gWindow = NULL;
    
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
void Engine::getPosition(int i,int j,int &x,int &y,int tile_size)
{
    // int originx = 300;
    // int originy = 100;
    x = camerax + (i-j)*tile_size;
    y = cameray + (i+j)*tile_size/2.0;
}


void Engine::isoworlddraw(int n,vector<vector<int> > mm){

    
    //cout<<"Drawing"<<endl;
    
    SDL_RenderClear(gRender);
    int tilesize = 128;

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(mm[i][j]!=0){
                SDL_Rect rect;
                    rect.w = 128;
                    rect.h = 128;
                int x,y;
                getPosition(i,j,x,y,tilesize/2);
                rect.x = x;
                rect.y = y;
                SDL_RenderCopy(gRender,textures[0],NULL,&rect);
                }
            else if(mm[i][j]==0){
                SDL_Rect rect;
                    rect.w = 128;
                    rect.h = 128;
                int x,y;
                getPosition(i,j,x,y,tilesize/2);
                rect.x = x;
                rect.y = y;
                SDL_RenderCopy(gRender,textures[2],NULL,&rect);
            }
            }
        }
    
    // for(int i=0;i<n;i++){
    //     for(int j=0;j<n;j++){
    //             if(mm[i][j]==2){
    //                 SDL_Rect rect;
    //                 rect.w  = 128;
    //                 rect.h = 128;
    //                 int x,y;
    //             getPosition(i,j,x,y,tilesize/2);
    //             rect.x = x;
    //             rect.y = y;
    //             SDL_RenderCopy(gRender,textures[1],NULL,&rect);
    //             }
    //         }
    //     }
    SDL_RenderPresent(gRender);
}

void Engine::drawIsoSprites(){
    SDL_RenderClear(gRender);
    int tilesize = 128;
    
}

void Engine::event_handler(){
    
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
        quit = true;
        }
        else if(e.type == SDL_KEYDOWN){
            switch(e.key.keysym.sym){
                case SDLK_a:
                    this->events_triggered.k_a = true;
                    break;
                
                case SDLK_s:
                    this->events_triggered.k_s = true;
                    break;
                
                case SDLK_d:
                    this->events_triggered.k_d = true;
                    break;
                
                case SDLK_w:
                    this->events_triggered.k_w = true;
                    break;
            }
        }
        else if(e.type == SDL_KEYUP){
            switch(e.key.keysym.sym){
                case SDLK_a:
                    this->events_triggered.k_a = false;
                    break;
                
                case SDLK_s:
                    this->events_triggered.k_s = false;
                    break;
                
                case SDLK_d:
                    this->events_triggered.k_d = false;
                    break;
                
                case SDLK_w:
                    this->events_triggered.k_w = false;
                    break;
            }
        }
    }
}
void Engine::update(){
    if(events_triggered.k_a)
        camerax+=2;
    if(events_triggered.k_d)
        camerax-=2;
    if(events_triggered.k_w)
        cameray+=2;
    if(events_triggered.k_s)
        cameray-=2;
}
void Engine::run(){
    
    int n;
    cin>>n;
    vector<vector<int> > mm(n);
    int c;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            mm[i].push_back(rand()%3);
        }
    }
    //gCurrentSurface = gKeyPressSurface[surface_total];
    while(!quit){
        event_handler();
        isoworlddraw(n,mm);
        update();
    }
}
