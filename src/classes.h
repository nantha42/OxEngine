#ifndef CLASSES_H
#define CLASSES_H
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
const int grid_size = 10;
const int tile_size = 64;
const int screen_width = 1030;
const int screen_height = 800;
using namespace std;

struct EventTriggered{
        bool k_a=false;
        bool k_w=false;
        bool k_s=false;
        bool k_d=false;
        bool k_p=false;
        bool k_space;
        bool k_shift;
        bool k_up;
        bool k_down;
        bool k_left;
        bool k_right;
        bool mouse_clicked = false;
        bool mouse_moved = false;
        bool k1=false,k2=false,k3=false,k4=false,k5=false,k6=false,k7=false,k8=false,k9=false;
        int mosx;
        int mosy;
        int movx;
        int movy;

};

class Vector{
    public:
    static vector<double> add(vector<double> a,vector<double> b);
    static vector<double> sub(vector<double> a,vector<double> b);
    static vector<int> add(vector<int> a,vector<int> b);
    static vector<int> sub(vector<int> a,vector<int> b);
    
};

    
class Button{
    int posx,posy;
    bool state;
    SDL_Texture *button_on;
    SDL_Texture *button_off;
    string imgpath;
    public:
    SDL_Renderer* renderer;
    
    Button(int x,int y,string img_path){
        posx = x;
        posy = y;
        state = false;
        imgpath = img_path;
    }
    void load_images(){
        cout<<"Button Imag path "<<imgpath<<endl;
        if(imgpath != ""){
            
            SDL_Surface* surf=NULL;
                string temp_off = imgpath+"0.png";
                string temp_on = imgpath+"1.png";
                surf = IMG_Load(temp_off.c_str());
                if(surf!=NULL){
                    button_off =  SDL_CreateTextureFromSurface(renderer,surf);
                    if(button_off==NULL)
                        cout<<"Is NULL"<<endl;
                    SDL_FreeSurface(surf);
                }
                surf = IMG_Load(temp_on.c_str());
                if(surf!=NULL){
                    button_on =  SDL_CreateTextureFromSurface(renderer,surf);
                    if(button_on==NULL)
                        cout<<"Is NULL"<<endl;
                    SDL_FreeSurface(surf);
                }
                cout<<"Button images Loaded"<<endl;    
        }
    }
    bool isPressed(){
        return state;
    }
    void drawButton(){
        SDL_Rect rect;
        rect.x = posx;
        rect.y = posy;
        rect.w = 40;
        rect.h = 40;
        if(state == true){
            SDL_RenderCopy(renderer,button_on,NULL,&rect);
        }else{
            SDL_RenderCopy(renderer,button_off,NULL,&rect);
        }
        
    }
    void handleClicks(int mosx,int mosy){
        if(mosx>posx && mosx<posx+60 && mosy > posy && mosy<posy+60){
            state = !state;
        }
    }
};

class Sprite{
    
    public:
    bool animated_sprite = false;
    bool isometric_sprite = false;
    int curframe = 0;
    SDL_Rect rect;
    bool alive = true;
    int size = 128;
    bool visible = true;
    string animation_images_path;
    int n_images = 0;
    string image_path;
    int isox;
    int isoy;
    SDL_Texture* image = NULL;
    vector<SDL_Texture*> images;
    SDL_Renderer* gRender;
    vector<int> pos;
    vector<int> vel;
    Uint32 prev_unit=0;
    int animation_fps = 20;

    Sprite(bool animated_sprite,bool isometric_sprite,SDL_Renderer* renderer);
    void kill();
    void make_visible();
    void make_invisible();
    void load_images();
    void set_image_path(string path);
    virtual void update();

};

class InventoryButton{
    string icon_path;
    bool show_state = false;
    public:
    int posx,posy;
    int id;
    bool state=false;
    string name;
    SDL_Texture* icon_on;
    SDL_Texture* icon_off;

    SDL_Rect rect;
    SDL_Renderer *renderer;
    InventoryButton(int id,string icon_path,bool show_state){
        this->id = 0;
        this->icon_path = icon_path;
        this->show_state = show_state;
    }
    bool load_icon(){
        string s = icon_path+"0.png";
        cout<<"Loading "<<s<<endl;
        SDL_Surface* img = IMG_Load(s.c_str());
        if(img != NULL){
            icon_on = SDL_CreateTextureFromSurface(renderer,img);
            if(icon_on==NULL){
                cout<<"Error in loading inventory button"<<endl;
                SDL_FreeSurface(img);
                return false;
            }
            rect.w = 50;
            rect.h = 50;
        }else{
            cout<<"Loading Failed"<<endl;
        }
        if(!show_state)
            return true;
        s = icon_path+"1.png";
        img = IMG_Load(s.c_str());
        if(img != NULL){
            icon_off = SDL_CreateTextureFromSurface(renderer,img);
            if(icon_off==NULL){
                cout<<"Error in loading inventory button"<<endl;
                SDL_FreeSurface(img);
                return false;
            }
        }
        return true;
    }
    void draw(){
        if(!show_state){
            rect.x = posx;
            rect.y = posy;
            
            SDL_RenderCopy(renderer,icon_on,NULL,&rect);
        }else{
            if(state){
                rect.x = posx;
                rect.y = posy;
                cout<<icon_on<<endl;
                SDL_RenderCopy(renderer,icon_on,NULL,&rect);
            }else{
                rect.x = posx;
                rect.y = posy;
                SDL_RenderCopy(renderer,icon_off,NULL,&rect);
            }
        }
    }
    void handleClicks(int mx,int my){
        if(mx >posx && mx<posx+rect.w && my > posy && my < posy+rect.h){
            state = !state;
        }
    }
    
};

class Inventory{
    bool shown = false;
    public:
    vector<string> categories_names;
    vector<vector<string>>items_names;
    vector<SDL_Texture*> texture_categories;
    vector<vector<SDL_Texture*>> texture_items;
    vector<InventoryButton> buttons;
    vector<vector<InventoryButton>> sub_buttons;
    int posx,posy;
    SDL_Renderer * renderer;
    Inventory(string categoryfile);
    void place_inventory(int x,int y);
    void load_images();
    void assignRenderer(SDL_Renderer*gRender);
    void showInventory();
    void hideInventory();
    bool checkAnyItemClicked();
    void draw();
    
};

class Game{
    protected:
    bool isometric_game = false;
    Uint32 unit_time=0;
    
    public:
    
    bool selected_tile[grid_size][grid_size];
    int local_map[grid_size][grid_size];
    bool local_map_changed = false;
    vector<Sprite> sprites;
    vector<Button> buttons;
    Inventory* build_inventory;
    Game(bool isometric_game){
        this->isometric_game = isometric_game;
    }
    void loadSprites(vector<string> sprite_files){
        
    }
    virtual void update()=0;
    virtual void eventhandler(EventTriggered et)=0;
    
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
    Game *game = NULL;
    SDL_Texture* building =NULL;
    int SCREEN_WIDTH = 1030;
    int SCREEN_HEIGHT = 800;
    int camerax = 102;
    int cameray = -80;
    EventTriggered events_triggered;
    vector<vector<int>> tiles_positionx;
    vector<vector<int>> tiles_positiony;
    int tile_selected = 0;
    bool placing_buildings = false;

    void getPosition(int i,int j,int &x,int &y,int tile_size,int size);
    public:
    vector<Sprite> sprites;
    
    Engine(int screenwidth,int screenheight);
    Engine();
    void renderit(vector<vector<bool>> &rendered,int a,int b,int size);
    bool init(Game &game);
    bool loadMedia();
    void draw_selected_tiles();
    void close();
    void select_tilesOrder(int i,int j);
    void drawisoworld();
    void drawcontrols();
    void run();
    void update();
    void event_handler();
};

#endif