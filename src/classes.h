#ifndef CLASSES_H
#define CLASSES_H
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
const int grid_size = 7;
const int tile_size = 128;

using namespace std;
class Object{
    public:
    int x;
    int y;
    vector<vector<int> > shape;
    string file;
    Object(int a,int b,vector<vector<int> > structure);
    
};
class Vector{
    public:
    static vector<double> add(vector<double> a,vector<double> b);
    static vector<double> sub(vector<double> a,vector<double> b);
    static vector<int> add(vector<int> a,vector<int> b);
    static vector<int> sub(vector<int> a,vector<int> b);
    
};
class Sprite{
    
    
    
    public:
    bool animated_sprite = false;
    bool isometric_sprite = false;
    int curframe = 0;
    SDL_Rect rect;
    bool alive = true;

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
    int animation_fps = 20;

    Sprite(bool animated_sprite,bool isometric_sprite,SDL_Renderer* renderer){
        this->animated_sprite = animated_sprite;
        this->isometric_sprite = isometric_sprite;
        gRender = renderer;
    }
    void kill(){
        alive = false;
    }
    void make_visible(){visible = true;}
    void make_invisible(){visible = false;}

    void load_images(){
        if(animation_images_path == "" && image_path==""){
            cout<<"Animation Path not defined";
            return;
        }
        if(animated_sprite){
            SDL_Surface* surf=NULL;
            
            bool loaded = true;
            for(int i=1;i<=n_images;i++){
                stringstream ss;
                ss<<i;
                string n_str = ss.str();
                cout<<animation_images_path<<endl;
                string temp = animation_images_path+n_str+".png";
                // cout<<"Loading "<<temp<<endl;
                surf = IMG_Load(temp.c_str());
                if(surf!=NULL){
                    SDL_Texture* textu =  SDL_CreateTextureFromSurface(gRender,surf);
                    if(textu==NULL)
                        cout<<"Is NULL"<<endl;
                    images.push_back(textu);
                    // cout<<"Loaded "<<temp<<" "<<images.size()<<endl;
                    SDL_FreeSurface(surf);
                }else{
                    loaded = false;
                    break;
                }
            }
            if(!loaded)
                cout<<"Loading Image failed "<<animation_images_path<<" at"<<n_images<<endl;
        }else{
            SDL_Surface* surf=NULL;
            string temp = image_path+".png";
            surf = IMG_Load(temp.c_str());
            if(surf!=NULL){
                SDL_Texture* textu =  SDL_CreateTextureFromSurface(gRender,surf);
                if(textu==NULL)
                    cout<<"Is NULL"<<endl;
                images.push_back(textu);
                // cout<<"Loaded "<<temp<<" "<<images.size()<<endl;
                SDL_FreeSurface(surf);
            }
            else{
                cout<<"Loading Image failed "<<image_path<<" at"<<n_images<<endl;
            }
        }
    }
    void set_image_path(string s){
        animated_sprite = false;
        image_path = s;

    }

};
class Game{
    protected:
    bool isometric_game = false;
    Uint32 unit_time=0;
    
    public:
    int world_map[grid_size][grid_size];
      vector<Sprite> sprites;

    Game(bool isometric_game){
        this->isometric_game = isometric_game;
    }
    void loadSprites(vector<string> sprite_files){
        
    }
    virtual void update()=0;    
    
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
    int SCREEN_WIDTH = 830;
    int SCREEN_HEIGHT = 600;
    int camerax = 102;
    int cameray = -80;
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
    vector<Sprite> sprites;

    Engine(int screenwidth,int screenheight);
    Engine();
    bool init(Game &game);
    bool loadMedia();
    void close();
    void isoworlddraw();
    void drawIsoSprites();
    void run();
    void update();
    void event_handler();
};


#endif