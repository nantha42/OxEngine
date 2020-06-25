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
void Inventory::category_slider_clicked(int x,int y,bool mouse_holded){
    cout<<x<<" "<<y<<" "<<mouse_holded<<endl;
    if(mouse_holded){
        if(x>= category_slider_x && x <category_slider_x+9 && y>=category_slider_y && y< category_slider_y + category_slider_size){
            if(!category_dragging){
                category_dragging = true;
                category_drag_startpoint = y;
            }
            
        }
    }else{
        if(category_dragging)
            category_dragging = false;
    }
}
void Inventory::item_slider_clicked(int x,int y,bool mouse_holded){
    
}
void Inventory::handle_clicks(int x,int y,bool mouse_holded){
    x -= posx;y-=posy;
    category_slider_clicked(x,y,mouse_holded);
    item_slider_clicked(x,y,mouse_holded);
}
bool Inventory::checkAnyItemClicked(){
    return true;
}
/*void Inventory::handleClicks(int x,int y,bool holded){
}*/
SDL_Texture* Inventory::render_categoryButtons(){
    int cat_h = 130;
    int cat_w = 70;
    SDL_Texture* categorbutton_bg = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,cat_w,cat_h);
    if(categorbutton_bg ==NULL){
        cout<<"CategoryButton_bg is NULL"<<SDL_GetError()<<endl;
        return NULL;
    }
    else{
        int icon_size = 50;
        int icon_gap = 10;
        SDL_SetRenderTarget(renderer,categorbutton_bg);
        
        SDL_SetRenderDrawColor(renderer,0xDC,0xDC,0xDc,0xff);
        SDL_Rect bg_rect = {0,0,cat_w,cat_h};
        SDL_RenderFillRect(renderer,&bg_rect);
        
        int content_size = (icon_size+icon_gap)*buttons.size();
        int scrollbar_height = ((float)cat_h/(float)content_size)*130;
        category_slider_size = scrollbar_height;
        
        for(int i=0;i<buttons.size();i++){
            buttons[i].posx = 5;
            buttons[i].posy = (50+10)*i - category_slider_y;
            // cout<<buttons[i].posx<<"  "<<buttons[i].posy<<endl;
            buttons[i].draw();
            // SDL_RenderCopy(renderer,buttons[i].)
        }
        SDL_SetRenderDrawColor(renderer,0x80,0x80,0x80,0xff);
        SDL_Rect rect = {category_slider_x,category_slider_y,9,scrollbar_height};
        // cout<<"Height: "<<scrollbar_height<<endl;
        SDL_RenderFillRect(renderer,&rect);
        SDL_SetRenderTarget(renderer,NULL);
        return categorbutton_bg;
    }
}
void Inventory::draw(){
    int inv_w = 200,inv_h = 150;
    SDL_Texture* inventory_background = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,inv_w,inv_h);
    if(inventory_background == NULL)
        cout<<"Texture is Null :"<<SDL_GetError()<<endl;
    else{
        SDL_SetTextureBlendMode(inventory_background,SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer,inventory_background);
        SDL_SetRenderDrawColor(renderer,0xDC,0xDC,0xDC,0XFF);
        SDL_Rect fillrect = {0,0,inv_w,inv_h};
        SDL_RenderFillRect(renderer,&fillrect);

        SDL_SetRenderTarget(renderer,NULL);
        SDL_Texture* category_buttons = this->render_categoryButtons();
        SDL_SetRenderTarget(renderer,inventory_background);
        int w,h;
        SDL_QueryTexture(category_buttons,NULL,NULL,&w,&h);
        SDL_Rect category_buttons_rect = {5,5,w,h};
        SDL_RenderCopy(renderer,category_buttons, NULL,&category_buttons_rect);
        SDL_SetRenderTarget(renderer,NULL);

        SDL_Rect tar_rect = {posx,posy,inv_w,inv_h};
        SDL_RenderCopy(renderer,inventory_background,NULL,&tar_rect);
        SDL_DestroyTexture(category_buttons);
        SDL_DestroyTexture(inventory_background);
    }
}