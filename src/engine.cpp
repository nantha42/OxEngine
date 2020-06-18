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
    
    // sort(game->sprites.begin(),game->sprites.end(),isometric_position_comparator());
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "Powered By OxEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        
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
        this->textures.push_back(SDL_CreateTextureFromSurface(gRender,gHelloWorld));
        for(int i=0;i<game->sprites.size();i++){
            game->sprites[i].gRender = gRender;
            game->sprites[i].load_images();
            // cout<<"Size "<<game->sprites[i].images.size()<<endl;
        }
        // for(auto sprite:game->sprites){ 
        //     cout<<"Size "<<sprite.images.size()<<endl;
        // }
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
void Engine::getPosition(int i,int j,int &x,int &y,int tile_size,int size)
{
    // int originx = 300;
    // int originy = 100;
    
    x = (camerax-cameray)*2 + (i-j-(size-1) )*tile_size;
    y = (camerax+cameray)*1 + (i+j-(size-1) )*tile_size/2.0;
    if(size-1)
       y-= (size-1)*15;
    
}



void Engine::renderit(vector<vector<bool>> &rendered,int a,int b,int size){
    bool atleastonenotrendered = false;
    for(int j=b;j<b+size;j++){
        for(int i=0;i<a;i++){
            cout<<i<<"   "<<j<<endl;
            if(!atleastonenotrendered && game->local_map[i][j]==-1){
                continue;
            }
            if(!rendered[i][j] && game->local_map[i][j]!=-1){
                atleastonenotrendered = true;
                int x,y;
                int frame;
                SDL_Rect rect;
                int id = game->local_map[i][j];
                if(id==-1)
                    continue;
                int size = game->sprites[id].size;
                if(size==2){
                    rendered[i][j] = true;
                    renderit(rendered,i,b+1,size-1);
                    getPosition(i,j,x,y,tile_size/2,size);
                    game->sprites[id].rect.x = x;
                    game->sprites[id].rect.y = y;
                    rect = game->sprites[id].rect;
                    frame = game->sprites[id].curframe;
                    SDL_RenderCopy(gRender, game->sprites[id].images[frame],NULL,&rect);
                }
                if(size==1){
                    rendered[i][j] = true;
                    getPosition(i,j,x,y,tile_size/2,size);
                    game->sprites[id].rect.x = x;
                    game->sprites[id].rect.y = y;
                    rect = game->sprites[id].rect;
                    frame = game->sprites[id].curframe;
                    SDL_RenderCopy(gRender, game->sprites[id].images[frame],NULL,&rect);
                }
            }
        }
    }
}

void Engine::isoworlddraw(){

    SDL_RenderClear(gRender);
    int tilesize = tile_size;
    vector<vector<bool>> rendered;
    for(int i=0;i<grid_size;i++)
        rendered.push_back(vector<bool>(grid_size,false));

    for(int j=0;j<grid_size;j++){
        for(int i=0;i<grid_size;i++){
    
            if(!rendered[i][j]){
                
                int x,y;
                int frame;
                SDL_Rect rect;
                int id = game->local_map[i][j];
                if(id==-1){
                    rendered[i][j]= true;//doublt
                    continue;
                }
                // cout<<game->sprites[id].image_path<<"  "<<id<<endl;
                int size = game->sprites[id].size;
                if(size>1){
                    renderit(rendered,i,j+1,size-1);
                    rendered[i][j] = true;
                    getPosition(i,j,x,y,tilesize/2,size);
                    game->sprites[id].rect.x = x;
                    game->sprites[id].rect.y = y;
                    rect = game->sprites[id].rect;
                    frame = game->sprites[id].curframe;
                    SDL_RenderCopy(gRender, game->sprites[id].images[frame],NULL,&rect);
                }
                else{
                    rendered[i][j] = true;
                    getPosition(i,j,x,y,tilesize/2,size);
                    game->sprites[id].rect.x = x;
                    game->sprites[id].rect.y = y;
                    rect = game->sprites[id].rect;
                    // if(id==5)
                        // cout<<rect.w<<" ::"<<rect.h<<endl;
                    frame = game->sprites[id].curframe;
                    // cout<<id<<"  "<<game->sprites[id].images[frame]<<endl;
                    SDL_RenderCopy(gRender, game->sprites[id].images[frame],NULL,&rect);
                }   
            }
            
        }
    }

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
                
                case SDLK_UP:
                    events_triggered.k_up = true;
                    break;
                case SDLK_DOWN:
                    events_triggered.k_down = true;
                    break;

                case SDLK_LEFT:
                    events_triggered.k_left = true;
                    break;
                case SDLK_RIGHT:
                    events_triggered.k_right = true;
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
                
                case SDLK_UP:
                    events_triggered.k_up =false;
                    break;

                case SDLK_DOWN:
                    events_triggered.k_down = false;
                    break;

                case SDLK_LEFT:
                    events_triggered.k_left = false;
                    break;

                case SDLK_RIGHT:
                    events_triggered.k_right = false;
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
    
    game->eventhandler(events_triggered);

}
void Engine::run(){
    while(!quit){
        event_handler();
        game->update();
        isoworlddraw();
        update();
        
    }
}
