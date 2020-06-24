#include "classes.h"

Inventory::Inventory(string categoryfile){
        ifstream file(categoryfile);
        int ncategories;
        file>>ncategories;
        int id=0;
        while(ncategories--){
            string categoryname;
            file>>categoryname;
            //categories_names.push_back(categoryname);
            InventoryButton button(id++,categoryname,true);
            buttons.push_back(button);
            int nitems;
            file>>nitems;
            vector<string>items_group;
            int id2=0;
            vector<InventoryButton> temp;
            while(nitems--){
                string itemname;
                file>>itemname;
                // items_group.push_back(itemname);
                InventoryButton subbutton(id2++,itemname,false);
                temp.push_back(subbutton);
            }
            sub_buttons.push_back(temp);
            // items_names.push_back(items_group);
        }
    }
void Inventory::place_inventory(int x,int y){
    posx = x;
    posy = y;
}

void Inventory::load_images(){
        for(int i=0;i<buttons.size();i++){
            if(buttons[i].load_icon()==false){
                cout<<"Button Image loading failed"<<endl;
                continue;
            }
            else{
            /*
            //SDL_Surface*temp;
            //string name = categories_names[i] +".png";
            //cout<<name<<endl;
            temp =  IMG_Load(name.c_str());
            cout<<"Herer"<<temp<<endl;
            if(temp!=NULL){
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,temp);
                if(texture !=NULL){
                    cout<<"Loaded "<<name<<endl;
                    texture_categories.push_back(texture);
                    SDL_FreeSurface(temp);
                }else{
                    cout<<"Error in Loading Category  :"<<name<<endl;
                    continue;
                }*/
                cout<<"Camer here 1"<<endl;
                // vector<SDL_Texture*> texture_category_items;
                for(int j=0;j<sub_buttons[i].size();j++){
                    sub_buttons[i][j].load_icon();
                }
                /*
                for(int j=0;j<items_names[i].size();j++){
                    cout<<"Camer here 2"<<endl;
                    string s = items_names[i][j]+".png";
                    temp = IMG_Load(s.c_str());
                    if(temp!=NULL){
                        SDL_Texture* temp_texture = SDL_CreateTextureFromSurface(renderer,temp);
                        if(temp_texture!=NULL){
                            cout<<"Loaded "<<s<<endl;
                            cout<<"Camer here 3"<<endl;
                            texture_category_items.push_back(temp_texture);
                            SDL_FreeSurface(temp);
                        }
                        else{
                            cout<<"Error Loading item for Invetory:  "<<s<<endl;
                        }
                    }
                }
                */
                //texture_items.push_back(texture_category_items);
        }
    }
}
void Inventory::assignRenderer(SDL_Renderer* gRender){
    for(int i=0;i<buttons.size();i++){
        buttons[i].renderer = gRender;
        for(int j=0;j<sub_buttons[i].size();j++){
            sub_buttons[i][j].renderer = gRender;
        }
    }

}
void Inventory::showInventory(){
        shown = true;
}
void Inventory::hideInventory(){
    shown = false;
}
bool Inventory::checkAnyItemClicked(){
    return true;
}
/*void Inventory::handleClicks(int x,int y,bool holded){
}*/
void Inventory::draw(){
    int inv_w = 150,inv_h = 150;
    SDL_Texture* texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,inv_w,inv_h);
    if(texture == NULL)
        cout<<"Texture is Null :"<<SDL_GetError()<<endl;
    else{
        SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer,texture);
        SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
        
        SDL_Rect box = {0,0,inv_w,inv_h};
        int x = 10;
        int y = 10;
        for(int i=0;i<buttons.size()-1;i++){
            buttons[i].posx = x;
            buttons[i].posy = y;
            buttons[i].draw();
            y+= 60;
            
        }
        SDL_RenderDrawRect(renderer,&box);
        SDL_SetRenderTarget(renderer,NULL);
        SDL_Rect tar_rect = {500,500,inv_w,inv_h};
        SDL_RenderCopy(renderer,texture,NULL,&tar_rect);
        SDL_DestroyTexture(texture);
    }
}