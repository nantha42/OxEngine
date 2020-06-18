#include<bits/stdc++.h>
#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include "classes.h"

Sprite::Sprite(bool animated_sprite,bool isometric_sprite,SDL_Renderer* renderer){
        this->animated_sprite = animated_sprite;
        this->isometric_sprite = isometric_sprite;
        gRender = renderer;
    }
void Sprite::kill(){
    alive = false;
}

void Sprite::make_visible(){visible = true;}

void Sprite::make_invisible(){visible = false;}

void Sprite::load_images(){
    // cout<<"In nsprite:  "<<animation_images_path<<endl;
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
            string temp = animation_images_path+n_str+".png";
            surf = IMG_Load(temp.c_str());
            if(surf!=NULL){
                SDL_Texture* textu =  SDL_CreateTextureFromSurface(gRender,surf);
                if(textu==NULL)
                    cout<<"Is NULL"<<endl;
                images.push_back(textu);
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
            SDL_FreeSurface(surf);
        }
        else{
            cout<<"Loading Image failed "<<image_path<<" at"<<n_images<<endl;
        }
    }
}
void Sprite::set_image_path(string s){
    animated_sprite = false;
    image_path = s;

}
void Sprite::update(){
    Uint32 curtime = SDL_GetTicks();
    if(curtime-prev_unit>= 1000/animation_fps){
            prev_unit = curtime;
        
    if(animated_sprite){
        curframe = (curframe+1)%n_images;
    }       
            
    }
}