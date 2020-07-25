#include "classes.h"

/*
class ResourceMeter{
    string image_path;
    int width;
    int height;
    int icon_width;
    int icon_height;
    public:

    SDL_Renderer *gRender;
    SDL_Texture *resource_image;
    SDL_Texture *texture_meter;
    TextRenderer* textRenderer;
    int resources_level;
    ResourceMeter(string imagepath);
    void assignRenderer(SDL_Renderer* gRender);
    void update();
};
*/

ResourceMeter::ResourceMeter(string imagepath){
    this->imagepath = imagepath;
    cout<<"Loading font for RM"<<endl;
    textRenderer = new TextRenderer("../Assets/Fonts/Quicksand.ttf",12);
    cout<<"Loading font for RM"<<endl;
}
void ResourceMeter::setSize(int w,int h){
    width = w;
    height = h;
}
void ResourceMeter::assignRenderer(SDL_Renderer* gRender){
    this->gRender = gRender;
    textRenderer->renderer = gRender;
    SDL_Surface* surface = IMG_Load(imagepath.c_str());
    if(surface!=NULL){
        resource_image = SDL_CreateTextureFromSurface(gRender,surface);
        if(resource_image==NULL){
            cout<<"Error in Loading Resource Image"<<endl;
        }
        texture_meter = SDL_CreateTexture(gRender,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,width,height);       
    }
}
void ResourceMeter::update(int value){
    
    if(resources_level != value){
        cout<<"Drawing Rsesource meter";
        resources_level = value;
        SDL_SetRenderTarget(gRender,texture_meter);
        SDL_Rect fillRect = {0,0,width,height};
        SDL_SetRenderDrawColor(gRender,0xdc,0xdc,0xdc,0xff);
        SDL_RenderFillRect(gRender,&fillRect);
        SDL_Rect resource_rect = {5,5,icon_width,icon_height};
        SDL_RenderCopy(gRender,resource_image,NULL,&resource_rect);
        SDL_Texture* text = textRenderer->renderTexture(to_string(value));
        int tw,ty;
        SDL_QueryTexture(text,NULL,NULL,&tw,&ty);
        SDL_Rect textrect = {10 + icon_width+5,5,tw,ty};
        
        SDL_RenderCopy(gRender,text,NULL,&textrect);
        SDL_DestroyTexture(text);
        SDL_SetRenderTarget(gRender,NULL);

    }
}
void ResourceMeter::draw(int x,int y){
    SDL_Rect rect = {x,y,width,height};
    SDL_RenderCopy(gRender,texture_meter,NULL,&rect);
}