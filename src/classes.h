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
class Vector{
    public:
    static vector<double> add(vector<double> a,vector<double> b);
    static vector<double> sub(vector<double> a,vector<double> b);
    static vector<int> add(vector<int> a,vector<int> b);
    static vector<int> sub(vector<int> a,vector<int> b);
    
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
    Game game = NULL:
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
    vector<Sprite> sprites;

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

class Sprite{
    private:
    bool animated_sprite = false;
    bool isometric_sprite = false;
    string animation_images_path;
    string image_path;
    SDL_Texture* image = NULL;
    bool alive = false;
    bool visible = false;
    
    public:
    int isox;
    int isoy;
    
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

    void set_animation_images_path(string path,int n){
        animation_images_path = path;
        animated_sprite = true;
        SDL_Surface* surf=NULL;
        stringstream ss;
        bool loaded = true;
        for(int i=1;i<=n;i++){
            ss<<i;
            string n_str = ss.str();
            string temp = path+n_str;
            surf = IMG_Load(temp.c_str());
            if(surf!=NULL){
                SDL_Texture* textu =  SDL_CreateTextureFromSurface(gRender,surf);
                images.push_back(textu);
            }else{
                loaded = false;
                break;
            }
        }
        cout<<"Loading Image failed "<<path<<" at"<<n<<endl;
    }
    void set_image_path(string s){
        animated_sprite = false;
        image_path = s;

    }

};
class Game{
    vector<Sprite> sprites;
    bool isometric_game = false;

    public:
    Game(bool isometric_game){
        this->isometric_game = isometric_game;
    }
    void loadSprites(vector<string> sprite_files){
        for(string s:sprite_files){
            
        }
    }
    void update();
    
}
#endif