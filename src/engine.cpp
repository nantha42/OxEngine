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
    for(int i=0;i<grid_size;i++){
        tiles_positionx.push_back(vector<int>(grid_size,0));
        tiles_positiony.push_back(vector<int>(grid_size,0));   
    }
}
Engine::Engine(){
    for(int i=0;i<grid_size;i++){
        tiles_positionx.push_back(vector<int>(grid_size,0));
        tiles_positiony.push_back(vector<int>(grid_size,0));
        
    }
    
}

bool Engine::init()
{
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
        gWindow = SDL_CreateWindow( "Powered By OxEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            cout<<"Created Window"<<endl;
            if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG)){
                cout<<"Error Img INit"<<endl;
            }
            gRender = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED| SDL_RENDERER_TARGETTEXTURE);
            if( TTF_Init() == -1 )
            {
                printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                success = false;
            }
        }
    }

    return success;
}
void Engine::assignGame(Game &game){
    this->game = &game;
}
bool Engine::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load splash image    
    gHelloWorld = IMG_Load("../Assets/Images/isometric_tile.png");
        cout<<"Assigning Render"<<endl;
        for(int i=0;i<game->texts.size();i++)
            game->texts[i]->render = gRender;
        cout<<"Assigning Render"<<endl;
        game->textRenderer->renderer = gRender;
        
        cout<<"Assigning Render for "<<endl;
        for(int i=0;i<game->sprites.size();i++){
            game->sprites[i].gRender = gRender;
            game->sprites[i].load_images();
        }
        cout<<"Assigning Render"<<endl;
        cout<<"Button size in engine:  "<<game->buttons.size()<<endl;
        for(int i=0;i<game->buttons.size();i++){
            game->buttons[i].renderer = gRender;
            cout<<"Loading button images"<<endl;
            game->buttons[i].load_images();
            
        }
        
        cout<<"Inventory Loading"<<endl;
        game->build_inventory->renderer = gRender;
        game->build_inventory->assignRenderer(gRender);
        game->build_inventory->load_images();
        
        game->level_status_bar->setRenderer(gRender);
    // }   
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
    TTF_Quit();
}
void Engine::getPosition(int i,int j,int &x,int &y,int tile_size,int size)
{
    
    x = (camerax-cameray)*2 + (i-j-(size-1) )*tile_size;
    y = (camerax+cameray)*1 + (i+j-(size-1) )*tile_size/2.0;
    if(size-1)
       y-= (size-1)*17;
    //problem might be here
    //tiles_positionx[i][j] = x+(64.0*tile_size/2/128.0);
    //tiles_positiony[i][j] = y+(52.0*tile_size/2/128.0);
    tiles_positionx[i][j] = x;
    tiles_positiony[i][j] = y+((52.0/128.0)*tile_size*2);

}
void Engine::renderit(vector<vector<bool>> &rendered,int a,int b,int size){
    bool atleastonenotrendered = false;
    for(int j=b;j<b+size;j++){
        for(int i=0;i<a;i++){
            if(!atleastonenotrendered && game->local_map[i][j]==-1){
                int x,y;
                getPosition(i,j,x,y,tile_size/2,tile_size);
                continue;
            }
            if(!rendered[i][j] && game->local_map[i][j]!=-1){
                atleastonenotrendered = true;
                int x,y;
                int frame;
                SDL_Rect rect;
                int id = game->local_map[i][j];
                if(id==-1){
                    getPosition(i,j,x,y,tile_size/2,tile_size);
                    continue;
                }
                int size = game->sprites[id].size;
                if(size>1){
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
void Engine::draw_selected_tiles(){
    int tilesize = tile_size;
    for(int j=0;j<grid_size;j++){
        for(int i=0;i<grid_size;i++){
                int x,y;
                int frame;
                SDL_Rect rect;
                int id = game->local_map[i][j];
                if(!game->selected_tile[i][j]){
                    continue;
                }
                // cout<<game->sprites[id].image_path<<"  "<<id<<endl;
                int size = game->sprites[id].size;
                
                getPosition(i,j,x,y,tilesize/2,size);
                
                rect.x = x;
                rect.y = y;
                rect.w = game->sprites[id].rect.w;
                rect.h = game->sprites[id].rect.h;
                frame = game->sprites[6].curframe;
                SDL_RenderCopy(gRender, game->sprites[6].images[frame],NULL,&rect);  
        }
    }
}
void Engine::drawcontrols(){
    for(int i=0;i<game->buttons.size();i++){
        if(game->buttons[i].isVisible())
            game->buttons[i].drawButton();
    }
}
void Engine::drawisoworld(){

    SDL_SetRenderDrawColor(gRender,0,0,0,0);
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
                if(id==10)
                    cout<<"Oil refinery: "<<game->sprites[id].size<<endl;
                if(id==-1){
                    rendered[i][j]= true;//doublt
                    getPosition(i,j,x,y,tilesize/2,1);
                    continue;
                }
                // cout<<game->sprites[id].image_path<<"  "<<id<<endl;
                int size = game->sprites[id].size;
                
                if(size>1){
                    //for(int q=1;q<size;q++)
                    if(j+(size-1) < grid_size)
                        renderit(rendered,i,j+1,size-1);
                    //if(j+1 < grid_size)
                    //    renderit(rendered,i,j+1,size-1);
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
    if(game->buttons[0].isPressed()){
        draw_selected_tiles();
        // game->buttons[0].stablize();
        game->build_inventory->shown = true;
    }else{
        game->build_inventory->shown = false;
        game->build_inventory->stable_buttons();
    }
    game->build_inventory->draw();
    
}


void Engine::event_handler(){
    if(events_triggered.mouse_clicked)
        events_triggered.mouse_clicked = false;
            
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
                
                case SDLK_p:
                    this->events_triggered.k_p = !events_triggered.k_p;
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
                case SDLK_1:
                    events_triggered.k1 = true;
                    break;
                case SDLK_2:
                    events_triggered.k2 = true;
                    break;
                case SDLK_3:
                    events_triggered.k3 = true;
                    break;
                case SDLK_4:
                    events_triggered.k4 = true;
                    break;
                case SDLK_5:
                    events_triggered.k5 = true;
                    break;
                case SDLK_6:
                    events_triggered.k6 = true;
                    break;
                case SDLK_7:
                    events_triggered.k7 = true;
                    break;
                case SDLK_8:
                    events_triggered.k8 = true;
                    break;
                case SDLK_9:
                    events_triggered.k9 = true;
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
        else if(e.type == SDL_MOUSEBUTTONDOWN){
            
            if(!events_triggered.mouse_holded)
                events_triggered.mouse_clicked = true;
        
            events_triggered.mouse_holded = true;
            // cout<<events_triggered.mouse_holded<<"  "<<events_triggered.mouse_clicked<<endl; 
            SDL_GetMouseState(&events_triggered.mosx,&events_triggered.mosy);
            // cout<<"Mouse :"<<events_triggered.mosx<<"   "<<events_triggered.mosy<<endl;
        }
        else if(e.type == SDL_MOUSEBUTTONUP){
            events_triggered.mouse_holded = false;
            if(events_triggered.mouse_clicked)
                events_triggered.mouse_clicked = false;
        }
        else if(e.type == SDL_MOUSEMOTION){
            events_triggered.mouse_moved = true;
            SDL_GetMouseState(&events_triggered.movx,&events_triggered.movy);
        }
    
    }
    if(game->buttons[0].isPressed())
        placing_buildings = true;

    
    if(game->buttons[2].isPressed()){
        placing_buildings = false; 
        game->buttons[2].stablize();
        for(int j=0;j<grid_size;j++)
            for(int i=0;i<grid_size;i++){
                if(game->selected_tile[i][j]){
                    
                    // game->local_map[i][j] = tile_selected;
                    if(game->sprites[tile_selected].size==1){
                        game->local_map_changed = true;
                        cout<<"Previous Size: "<<game->sprites[game->local_map[i][j]].size<<endl;
                        if(game->sprites[game->local_map[i][j]].size > 1 ){
                            int size = game->sprites[game->local_map[i][j]].size;
                            for(int k=j;k<j+size;k++){
                                for(int q=i;q<i+size;q++){
                                    game->local_map[q][k] = 1;
                                }
                            }
                        }
                        game->local_map[i][j]= tile_selected;

                    }
                    
                    
                    else if(game->sprites[tile_selected].size>1){
                        //assigning the other indices of the structure area with -1
                        game->local_map_changed = true;
                        game->local_map[i][j] = tile_selected;
                        
                        int size = game->sprites[tile_selected].size;
                        
                        for(int k=j;k<j+size;k++){
                            for(int q=i;q<i+size;q++){
                                game->local_map[q][k] = -1;
                            }
                        }
                        //assigning the left corner indice with the structure index
                        game->local_map[i][j] = tile_selected;
                        
                        //setting false for the -1 index in the selected_tile 
                        //so we won't place the structure multiple times 
                        for(int k=j;k<j+size;k++){
                            for(int q=i;q<i+size;q++){
                                game->selected_tile[q][k] = false;
                            }}}}
                game->selected_tile[i][j]= false;
            }
    }
    
}

float area(int x1,int y1,int x2, int y2,int x3,int y3){
    return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}
bool IsOutside(int x1,int y1,int x2, int y2,int x3,int y3,int x,int y){
    float a = area(x1,y1,x2,y2,x3,y3);
    float a1 = area(x,y,x2,y2,x3,y3);
    float a2 = area(x1,y1,x,y,x3,y3);
    float a3 = area(x1,y1,x2,y2,x,y);
    return a!=(a1+a2+a3);
}
void Engine::update(){
    SDL_RenderPresent(gRender);
    if(events_triggered.k_right && camerax < 400)//a
        camerax+=4;
    if(events_triggered.k_left && camerax > -100)//d
        camerax-=4;
    if(events_triggered.k_down && cameray<200)//w
        cameray+=4;
    if(events_triggered.k_up && cameray > -100)//s
        cameray-=4;
    int x,y;
    x = events_triggered.mosx;
    y = events_triggered.mosy;
    game->eventhandler(events_triggered);
    
    if(events_triggered.mouse_clicked){
        
        int i=0;
        if(x>tiles_positionx[0][grid_size-1]&& x<tiles_positionx[grid_size-1][0]+tile_size && y>tiles_positiony[0][0] && y<tiles_positiony[grid_size-1][grid_size-1]+tile_size/2){
            bool found = false;
            int j=0;
            for(i=0;i<grid_size ;i++){
                for(j=0;j<grid_size;j++){
                    //cout<<"Traversing :"<<i<<"  "<<j<<endl;
                    if(x>tiles_positionx[i][j] && x < tiles_positionx[i][j]+tile_size){
                        if(y>tiles_positiony[i][j] && y< tiles_positiony[i][j]+tile_size/2){
                            events_triggered.mouse_clicked = false;
                            found = true;
                            break;
                        }
                    }
                }
                if(found)
                    break;
            }
            if(found){
                int ax = tiles_positionx[i][j];
                int ay = tiles_positiony[i][j];
                int xp = x-ax;
                int yp = y-ay;

                if(xp<tile_size/2 && yp<tile_size/4){
                    int x1 = tile_size/2,x2 =0 ,x3 =tile_size/2 ;
                    int y1 = 0,y2 =tile_size/4 ,y3 =tile_size/4 ;
                    // cout<<"1"<<endl;
                    if(IsOutside(x1,y1,x2,y2,x3,y3,xp,yp)){
                        i--;}
                        
                }else if(xp>tile_size/2 && yp<tile_size/4){
                    int x1 = 0+tile_size/2,x2 =tile_size/2+tile_size/2 ,x3 = tile_size/2+tile_size/2;
                    int y1 = 0,y2 = tile_size/2-tile_size/4,y3 =tile_size/4-tile_size/4 ;
                    // cout<<"2"<<endl;
                    if(!IsOutside(x1,y1,x2,y2,x3,y3,xp,yp)){
                    j--;}
                }else if(xp<tile_size/2 && yp>tile_size/4){
                    int x1 = 0,x2 =tile_size/2 ,x3 = tile_size/2;
                    int y1 = tile_size/4,y2 = tile_size/2,y3 =tile_size/4 ;
                    // cout<<"3"<<endl;
                    if(IsOutside(x1,y1,x2,y2,x3,y3,xp,yp)){
                    j++;
                    }
                }else{
                    int x1 = tile_size/2+tile_size/2,x2 =0+tile_size/2 ,x3 =tile_size/2+tile_size/2 ;
                    int y1 = 0+tile_size/4,y2 =tile_size/4+tile_size/4 ,y3 =tile_size/4+tile_size/4 ;
                    // cout<<"4"<<endl;
                    if(!IsOutside(x1,y1,x2,y2,x3,y3,xp,yp)){
                        i++;
                    }
                }
            }
            
            if(i>=0 && i<grid_size && j>=0 && j<grid_size){
                if(game->buttons[0].isPressed()){
                    cout<<"Click once Placing"<<endl;
                    select_tilesOrder(i,j);
                    selected_tile_i = i;
                    selected_tile_j = j;
                }
            }
        }
        //Code to handle buttons like build_inventory button and all the main buttons
        cout<<game->buttons[i].isPressed()<<endl;
        for(int i=0;i<game->buttons.size();i++){
            if(game->buttons[i].handleClicks(x,y)){
                events_triggered.mouse_clicked = false;
            }
        }
     }
    else if(events_triggered.mouse_holded){
        int i=0;
        x = events_triggered.movx;
        y = events_triggered.movy;
        if(x>tiles_positionx[0][grid_size-1]&& x<tiles_positionx[grid_size-1][0]+tile_size && y>tiles_positiony[0][0] && y<tiles_positiony[grid_size-1][grid_size-1]+tile_size/2){
            bool found = false;
            int j=0;
            for(i=0;i<grid_size ;i++){
                for(j=0;j<grid_size;j++){
                    //cout<<"Traversing :"<<i<<"  "<<j<<endl;
                    if(x>tiles_positionx[i][j] && x < tiles_positionx[i][j]+tile_size){
                        if(y>tiles_positiony[i][j] && y< tiles_positiony[i][j]+tile_size/2){
                            found = true;
                            break;
                        }
                    }
                }
                if(found)
                    break;
            }
            if(found){
                int ax = tiles_positionx[i][j];
                int ay = tiles_positiony[i][j];
                int xp = x-ax;
                int yp = y-ay;

                if(xp<tile_size/2 && yp<tile_size/4){
                    int x1 = tile_size/2,x2 =0 ,x3 =tile_size/2 ;
                    int y1 = 0,y2 =tile_size/4 ,y3 =tile_size/4 ;
                    // cout<<"1"<<endl;
                    if(IsOutside(x1,y1,x2,y2,x3,y3,xp,yp)){
                        i--;}
                        
                }else if(xp>tile_size/2 && yp<tile_size/4){
                    int x1 = 0+tile_size/2,x2 =tile_size/2+tile_size/2 ,x3 = tile_size/2+tile_size/2;
                    int y1 = 0,y2 = tile_size/2-tile_size/4,y3 =tile_size/4-tile_size/4 ;
                    // cout<<"2"<<endl;
                    if(!IsOutside(x1,y1,x2,y2,x3,y3,xp,yp)){
                    j--;}
                }else if(xp<tile_size/2 && yp>tile_size/4){
                    int x1 = 0,x2 =tile_size/2 ,x3 = tile_size/2;
                    int y1 = tile_size/4,y2 = tile_size/2,y3 =tile_size/4 ;
                    // cout<<"3"<<endl;
                    if(IsOutside(x1,y1,x2,y2,x3,y3,xp,yp)){
                    j++;
                    }
                }else{
                    int x1 = tile_size/2+tile_size/2,x2 =0+tile_size/2 ,x3 =tile_size/2+tile_size/2 ;
                    int y1 = 0+tile_size/4,y2 =tile_size/4+tile_size/4 ,y3 =tile_size/4+tile_size/4 ;
                    // cout<<"4"<<endl;
                    if(!IsOutside(x1,y1,x2,y2,x3,y3,xp,yp)){
                        i++;
                    }
                }
            }
            
            if(i>=0 && i<grid_size && j>=0 && j<grid_size){
                cout<<"After found"<<"  "<<game->buttons[0].isPressed() <<"  "<<i<<selected_tile_i <<"  "<<j<<selected_tile_j<<endl;    
                if(game->buttons[0].isPressed() && (selected_tile_i!= i || selected_tile_j != j)){
                    cout<<"Calling seelcte"<<i<<"  "<<j<<endl;
                    select_tilesOrder(i,j);
                    selected_tile_i = i;
                    selected_tile_j = j;
                }
            }
        }
        
    }
    
    SDL_Delay(50);
}
void Engine::select_tilesOrder(int i,int j){
    

    tile_selected=0;
    tile_selected = game->build_inventory->getClickedItem();
    cout<<"Tile seelcted:  "<<tile_selected<<endl;
    
    if(tile_selected==-1)return;
    int size = game->sprites[tile_selected].size;
    
    if(size==1){
        // if(game->sprites[->local_map[i-1][j-1]]
        if(i-1 >= 0 && j-1 >= 0){
            if(game->sprites[game->local_map[i-1][j-1]].size==2 ||
                game->sprites[game->local_map[i-1][j]].size==2 ||
                game->sprites[game->local_map[i][j-1]].size==2)
                return;
        }
        else if(game->local_map[i][j]== -1)
            return;
            
        game->selected_tile[i][j] = !game->selected_tile[i][j];
    }
    else{
        
        if(i+size<=grid_size && j+size<=grid_size){
            if(game->selected_tile[i][j]==1){
                //This code checks if we selecting already selected tile 
                //so deselection happens
                int selected_tops = 0;
                int selected_sides = 0;
                for(int a=i-1; a>0 && game->selected_tile[a][j]==1;a--)
                    selected_tops++;
                int start_i = i-selected_tops;
                for(int b=j-1; b>0 && game->selected_tile[i][b]==1;b--)
                    selected_sides++;
                int start_j = j-selected_sides;
                if( (i-start_i)%size!=0 || (j-start_j)%size!=0)
                    return;   
            }else{
                //Checks if the selection of tiles are done rightly
                //without overlapping 
                for(int x=0;x<size;x++)
                    for(int y=0;y<size;y++)
                        if((x!=0 || y!=0) && game->selected_tile[i+x][j+y]==1)
                            return;
            }

            for(int a=i;a<i+size;a++)
                for(int b=j;b<j+size;b++){
                    
                    game->selected_tile[a][b] = !game->selected_tile[a][b];
                }
        }
    }    
}
void Engine::free(){

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}
void Engine::drawTexts(){
    for(int i=0;i<game->texts.size();i++){

        SDL_Rect rect;
        rect.x = game->texts[i]->x;
        rect.y = game->texts[i]->y;
        rect.w = game->texts[i]->w;
        rect.h = game->texts[i]->h;
        SDL_RenderCopy(gRender,game->texts[i]->getTexture(),NULL,&rect);
    }
}
void Engine::drawStatusBars(){
    game->level_status_bar->draw();
}
void Engine::run(){
    while(!quit){
        
        event_handler();
        game->update();
        drawisoworld();
        drawcontrols();
        drawTexts();
        drawStatusBars();
        update();
        
    }
    free();
    
}
