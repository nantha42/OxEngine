#ifndef CLASSES_H
#define CLASSES_H
//#include<bits/stdc++.h>

#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<string>
#include<ctime>
#include<map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
        bool k_up=false;
        bool k_down=false;
        bool k_left=false;
        bool k_right=false;
        bool mouse_clicked = false;
        bool mouse_moved = false;
        bool mouse_holded = false;
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
    bool visible=true;

    Button(int x,int y,string img_path){
        posx = x;
        posy = y;
        state = false;
        imgpath = img_path;
    }
    bool isVisible(){return visible;}

    void setPos(int x,int y){
        posx = x;
        posy = y;
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
    void stablize(){
        state = false;
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
    bool handleClicks(int mosx,int mosy){
        
        if(mosx>posx && mosx<posx+50 && mosy > posy && mosy<posy+50){
            state = !state;
            cout<<"state"<<state<<endl;
            return true;
            
        }
        return false;
    }
};

class TextRenderer{
    TTF_Font *font;
    string ttf_path;
    int size;

    public:
    SDL_Color textcolor;
    SDL_Renderer *renderer=NULL;
    TextRenderer(string ttf_path,int size,SDL_Color color):TextRenderer(ttf_path,size){
        cout<<"Colors:  "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
        
        textcolor = color;
    }
    TextRenderer(string ttf_path,int size){
        this->size = size;
        this->ttf_path = ttf_path;
        textcolor = {0xff,0xff,0xff,0xff};
        cout<<"Loading "<<ttf_path.c_str()<<endl;
        
        if(!TTF_WasInit()) {
            cerr << "TTF_Init failed " << TTF_GetError() << endl;
            exit(1);
        }else{
            cout<<"TTF initialized"<<endl;
        }
        
        font = TTF_OpenFont(ttf_path.c_str(),size);
        if ( font == NULL ) 
        {    
            fprintf(stderr, "Couldn't load 22 pt font from %s: %s\n", ttf_path.c_str(),SDL_GetError());    
        }else{
            cout<<"Font Loaded"<<endl;
        }
    }
    SDL_Surface* renderSurface(string s){
        if(font==NULL)
            cout<<"Font NULL"<<endl;
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, s.c_str(),textcolor);
        if(text_surface!=NULL){
            return text_surface;
        }else
            return NULL;
    }
    SDL_Texture* renderTexture(string s){
        
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, s.c_str(),textcolor);
        if(text_surface!=NULL){
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,text_surface);
            SDL_FreeSurface(text_surface);
            return texture;
        }else
            return NULL;
    }

};

class Text{
    SDL_Texture* texture=NULL;
    SDL_Surface* text_surface=NULL;
    
    public:
    SDL_Renderer *render=NULL;
    int x=0,y=0;
    int w=0,h =0;
    Text(SDL_Surface* text){
        if(text!=NULL){
            this->text_surface = text;   
            int a,b;
            w = text_surface->w;
            h = text_surface->h;
            cout<<"WH"<<w<<" "<<h<<endl;
        }
        else{
            cout<<"FFETexture is NULL"<<endl;
        }
    }
    SDL_Texture* getTexture(){

        if(texture!=NULL){
            
            return texture;
        }else{
            cout<<"FFTexture is NULL"<<endl;
            if(render!=NULL){
                cout<<"Texture Created"<<endl;
                texture = SDL_CreateTextureFromSurface(render,text_surface);
                return texture;
            }
            else{ cout<<"Renderer is NULL"<<endl;return NULL;}
        }
    }  
};
class TextBox{
    string text;
    int text_size = 0;
    
    int textBox_width=100;
    int textBox_height=150;
    int posx = 0;
    int posy = 0;
    
    int slider_width = 9;
    int slider_x = textBox_width-slider_width;
    int slider_y = 0;
    int slider_size = 1;
    int content_size = 0;
    int slider_drag_startpoint = 0;
    bool slider_dragging = false;
    
    TextRenderer* textRenderer;
    SDL_Surface* single_letter;
    SDL_Renderer* renderer;
    SDL_Texture* single_line;
    SDL_Texture* textbox_texture;
    
    void renderText();
    public:
    bool shown = true;
    TextBox(string font,int size);
    void setSize(int w,int h);
    void LoadText(string s);
    void showTextBox();
    void setPos(int x,int y);
    bool isTextBoxClear();
    void clearTexture();
    void hideTextBox();
    
    void slider_clicked(int x,int y,bool mouse_holded);
    void assignRenderer(SDL_Renderer* grender);
    void handle_clicks(EventTriggered &et);
    void draw();
    
};
class LevelStatusBar{
    int bar_length;
    int current_value = 30000;
    int posx=0,posy=0;
    
    SDL_Texture* status_bg;
    SDL_Texture* bg;
    SDL_Renderer* renderer;
    SDL_Surface* img;
    TextRenderer *textRenderer;
    public:
    LevelStatusBar(int x,int y){
        posx = x;
        posy = y;

    }
    void setRenderer(SDL_Renderer* renderer){
        this->renderer = renderer;
        textRenderer = new TextRenderer("../Assets/Fonts/Quicksand-Bold.ttf",12);
        textRenderer->renderer = renderer;
        if(this->renderer!=NULL){
            img = IMG_Load("../Assets/Images/level_status_bar.png");
            if(img!=NULL){
                cout<<"Status Bar Loaded"<<endl;
                bg = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,img->w,img->h);
                status_bg = SDL_CreateTextureFromSurface(renderer,img);
            }else{
                cout<<"Status Bar Not Loaded"<<endl;
            }
        }
    }
    void draw(){
        SDL_Rect rect = {0,0,img->w,img->h};
        SDL_SetTextureBlendMode(bg,SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer,bg);
        SDL_RenderCopy(renderer,status_bg,NULL,&rect);
        int w,h;
        SDL_Texture* left_number = textRenderer->renderTexture(to_string(current_value/1000));
        SDL_QueryTexture(left_number,NULL,NULL,&w,&h);
        SDL_Rect text1 = {13-w/2,18,w,h};
        SDL_RenderCopy(renderer,left_number,NULL,&text1);
        SDL_DestroyTexture(left_number);
        SDL_Texture* right_number = textRenderer->renderTexture(to_string(current_value/1000+1));
        SDL_QueryTexture(right_number,NULL,NULL,&w,&h);
        SDL_Rect text2 = {173+13-w/2,18,w,h};
        SDL_RenderCopy(renderer,right_number,NULL,&text2);
        SDL_DestroyTexture(right_number);
        // SDL_RenderDrawRectF(renderer,)
        SDL_SetRenderTarget(renderer,NULL);
        rect = {posx,posy,img->w,img->h};
        SDL_RenderCopy(renderer,bg,NULL,&rect);   
    }
};
class Sprite{
    
    public:
    bool animated_sprite = false;
    bool isometric_sprite = false;
    int curframe = 0;
    SDL_Rect rect;
    bool alive = true;
    string name;
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
    string info="";
    int offset_x = 0;
    int offset_y = 0;
    int posx,posy;
    int id;
    bool state=false;
    string name;
    SDL_Texture* icon_on;
    SDL_Texture* icon_off;
    
    SDL_Rect rect;
    SDL_Renderer *renderer;
    InventoryButton(int id,string icon_path,string name,bool show_state){
        this->id = 0;
        this->icon_path = icon_path;
        this->show_state = show_state;
        this->name = name;
    }
    bool load_icon(SDL_Texture* text_name){
        string s;
        if(show_state)
            s = icon_path+"0.png";
        else
            s = icon_path+".png";
        cout<<"Loading "<<s<<endl;
        SDL_Surface* img = IMG_Load(s.c_str());
        if(img != NULL){
            int a=0,b=0;
            if(text_name!=NULL)
                SDL_QueryTexture(text_name,NULL,NULL,&a,&b);
            SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer,img);
            int iw=img->w,ih=img->h;
            if(iw>128){
                iw = 128;ih = 128;}

            // cout<<"IMWH"<<img->w<<" "<<img->h<<endl;
            icon_on = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, iw,ih+b);
            SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

            SDL_SetRenderTarget(renderer,icon_on);
            SDL_SetTextureBlendMode(icon_on,SDL_BLENDMODE_BLEND);
            // SDL_SetRenderDrawColor(renderer,0x80,0x80,0x80,0xff);
            // SDL_Rect fillrect = {0,0,img->w,img->h};
            // SDL_RenderFillRect(renderer,&fillrect);
            SDL_Rect temp_rect = {0,0,ih,ih};
            SDL_RenderCopy(renderer,temp,NULL,&temp_rect);
            SDL_Rect text_rect = {0,ih,a,b};
            SDL_RenderCopy(renderer,text_name,NULL,&text_rect);
            SDL_SetRenderTarget(renderer,NULL);
            SDL_DestroyTexture(temp);
            if(icon_on==NULL){
                cout<<"Error in loading inventory button"<<endl;
                SDL_FreeSurface(img);
                return false;
            }
            // SDL_QueryTexture(icon_on,NULL,NULL,&rect.w,&rect.h);
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
                // cout<<icon_on<<endl;
                SDL_RenderCopy(renderer,icon_on,NULL,&rect);
            }else{
                rect.x = posx;
                rect.y = posy;
                SDL_RenderCopy(renderer,icon_off,NULL,&rect);
            }
        }
    }
    bool handleClicks(int mx,int my,EventTriggered &et){   
        if(mx >posx+offset_x && mx<posx+offset_x+rect.w && my > posy+offset_y && my < posy+offset_y+rect.h && et.mouse_clicked){
            et.mouse_clicked = false;
            cout<<"Set mouseclicked"<<et.mouse_clicked<<endl;
            state = !state;
            return true;
        }
        return false;
    }
};
struct point{
    int x,y;
};


class Inventory{
    
    int category_slider_x = 60;
    int category_slider_y = 0;

    int item_slider_x = 50+50+5+10+5;
    int item_slider_y = 0;

    int category_slider_size = 0;
    int item_slider_size = 0;

    bool category_dragging = false;
    bool item_dragging = false;
    
    int category_drag_startpoint;
    int items_drag_startpoint;
    
    bool drag_inventory = false;
    int inv_drag_offset = 0;
    
    int inventory_width = 200;
    int inventory_height = 150;
    void category_slider_clicked(int x,int y,bool mouse_holded);
    void item_slider_clicked(int x,int y,bool mouse_holded);
    vector<Button* >attached_buttons; 
    TextBox *textBox;
    vector<string> infos;
    public:
    
    bool shown = false;
    TextRenderer *textRenderer;
    vector<string> categories_names;
    vector<vector<string>>items_names;
    vector<SDL_Texture*> texture_categories;
    vector<vector<SDL_Texture*>> texture_items;
    vector<InventoryButton> buttons;
    vector<vector<InventoryButton>> sub_buttons;
    int posx=500,posy=500;
    SDL_Renderer * renderer;
    Inventory(string categoryfile);
    void place_inventory(int x,int y);
    void handle_clicks(EventTriggered &et);
    void load_images();
    void stable_buttons();
    SDL_Texture* render_categoryButtons();
    SDL_Texture* render_itemButtons();
    void assignRenderer(SDL_Renderer*gRender);
    void showInventory();
    void hideInventory();
    
    int getClickedItem();
    void draw();
    void add_attached_button(Button *button);
    void update_attached_buttons();
};

class Game{
    protected:
    bool isometric_game = false;
    Uint32 unit_time=0;
    
    public:
    TextRenderer *textRenderer;
    vector<Text*> texts;
    bool selected_tile[grid_size][grid_size];
    int local_map[grid_size][grid_size];
    bool local_map_changed = false;
    vector<pair<int,int>> local_map_changed_pos;
    vector<Sprite> structural_sprites;
    vector<Button> buttons;
    Inventory* build_inventory;
    LevelStatusBar *level_status_bar;
    Game(bool isometric_game){
        this->isometric_game = isometric_game;
    }
    void loadSprites(vector<string> sprite_files){
        
    }
    virtual void update()=0;
    virtual void eventhandler(EventTriggered &et)=0;
    
};

class Engine{
    private:
    SDL_Event e;
    bool quit = false;
    SDL_Window* gWindow = NULL;
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
    int selected_tile_i=-1;
    int selected_tile_j=-1;
    void getPosition(int i,int j,int &x,int &y,int tile_size,int size);
    public:
    vector<Sprite> sprites;
    
    Engine(int screenwidth,int screenheight);
    Engine();
    void renderit(vector<vector<bool>> &rendered,int a,int b,int size);
    bool init();
    bool loadMedia();
    void draw_selected_tiles();
    void close();
    void assignGame(Game &game);
    void select_tilesOrder(int i,int j);
    void drawisoworld();
    void drawcontrols();
    void drawTexts();
    void drawStatusBars();
    void run();
    void update();
    void event_handler();
    void free();
};

#endif