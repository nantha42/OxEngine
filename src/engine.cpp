#include<bits/stdc++.h>
#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include "classes.h"
using namespace std;


void Engine::getPosition(int i,int j,int &x,int &y,int tile_size)
{
    int originx = 300;
    int originy = 100;
    x = originx + (i-j)*tile_size;
    y = originy + (i+j)*tile_size/2.0;
}

bool Engine::init()
{
    //Initialization flag
    bool success = true;

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
        if(gTexture == NULL)
            cout<<"Texture is Null"<<endl;
        SDL_FreeSurface(gHelloWorld);
        gHelloWorld = NULL;
        gHelloWorld = IMG_Load("../Assets/Images/building.png");
        building = SDL_CreateTextureFromSurface(gRender,gHelloWorld);

        if(building == NULL)
            cout<<"Texture is Null"<<endl;
        SDL_FreeSurface(gHelloWorld);
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
void Engine::draw(int n,vector<vector<int> > mm){
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
                SDL_RenderCopy(gRender,gTexture,NULL,&rect);
                }
            }
        }
    
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
                if(mm[i][j]==2){
                    SDL_Rect rect;
                    rect.w  = 128;
                    rect.h = 128;
                    int x,y;
                getPosition(i,j,x,y,tilesize/2);
                rect.x = x;
                rect.y = y;
                SDL_RenderCopy(gRender,building,NULL,&rect);
                }
                }
            }
    SDL_RenderPresent(gRender);
}
void Engine::event_handler(){
    
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
        quit = true;
        }
        else if(e.type == SDL_KEYDOWN){
            switch(e.key.keysym.sym){
                case SDLK_a:
                    break;
                
                case SDLK_s:
                    break;
                
                case SDLK_d:
                    break;
                
                case SDLK_w:
                    break;
            }
        }
        else if(e.type == SDL_KEYUP){
            switch(e.key.keysym.sym){
                case SDLK_a:
                    break;
                
                case SDLK_s:
                    break;
                
                case SDLK_d:
                    break;
                
                case SDLK_w:
                    break;
            }
        }
    }
}
void Engine::run(){
    
    int n;
    cin>>n;
    vector<vector<int> > mm(n);
    int c;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>c;
            mm[i].push_back(c);
        }
    }
    //gCurrentSurface = gKeyPressSurface[surface_total];
    while(!quit){
        event_handler();
        draw(n,mm);
    }
}
