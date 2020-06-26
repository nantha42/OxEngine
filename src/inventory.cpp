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
                for(int j=0;j<sub_buttons[i].size();j++){
                    sub_buttons[i][j].load_icon();
                }
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
    // cout<<x<<" "<<y<<" "<<mouse_holded<<endl;
    x-= posx+5;
    y-= posy;
    if(mouse_holded){
        cout<<category_slider_x<<" "<<9<<category_slider_y<<" "<<category_slider_size<<endl;
        cout<<(x>= category_slider_x && x <category_slider_x+9)<<" "<<(y>=category_slider_y && y< category_slider_y + category_slider_size)<<endl;
        if(x>= category_slider_x && x <category_slider_x+9 && y>=category_slider_y && y< category_slider_y + category_slider_size){
            if(!category_dragging){
                category_dragging = true;
                category_drag_startpoint = y;
            }
            
        }
        if(category_dragging){
            int t = y-category_drag_startpoint;
            if(t<0)
                category_slider_y = 0;
            if (t>(135-category_slider_size))
                category_slider_y = (135-category_slider_size);
            if(t>0 && t< (135-category_slider_size))
                category_slider_y = t;
                
        }
    }else{
        if(category_dragging)
            category_dragging = false;
    }
}
void Inventory::item_slider_clicked(int x,int y,bool mouse_holded){
    
}
void Inventory::handle_clicks(EventTriggered &et){
    if(!shown)
        return;
    int x = et.movx;
    int y = et.movy;
    bool mouse_holded = et.mouse_holded;
    //x -= posx+5;y-=posy;
    cout<<"Mouse Clicked "<<et.mouse_clicked<<endl;
    category_slider_clicked(x,y,mouse_holded);
    item_slider_clicked(x,y,mouse_holded);
    if(et.mouse_clicked){
        int selected = -1;
        for(int i=0;i<buttons.size();i++){
            if(buttons[i].handleClicks(et.mosx,et.mosy,et)){
                cout<<"MOuse clicked:  "<<et.mouse_clicked<<endl;
                selected = i;
                break;
            }
        }
        if(selected!=-1)
            for(int i=0;i<buttons.size();i++){
                if(i != selected)
                    buttons[i].state = false;
            }
    }
}
bool Inventory::checkAnyItemClicked(){
    return true;
}
/*void Inventory::handleClicks(int x,int y,bool holded){
}*/
SDL_Texture* Inventory::render_itemButtons(){
    int item_h = 130;
    int item_w = 130;
    SDL_Texture* item_button_bg = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,item_w,item_h);
    if(item_button_bg==NULL){
        cout<<"Item button bg is NULL"<<SDL_GetError()<<endl;
        return NULL;
    }else{
        int icon_size = 50;
        int icon_gap = 10;
        SDL_SetRenderTarget(renderer,item_button_bg);
        SDL_SetRenderDrawColor(renderer,0xDC,0xDC,0xDC,0xFF);
        SDL_Rect bg_rect = {0,0,item_w,item_h};
        SDL_RenderFillRect(renderer,&bg_rect);
        // SDL_RenderCopy(renderer,item_button_bg,NULL,&bg_rect);
        int content_size = (icon_size+icon_gap)*(sub_buttons.size()/2);
        item_slider_size = ((float)item_h/(float)content_size)*130;
        int selected = 0;
        for(int j=0;j<buttons.size();j++)
            if(buttons[j].state){
                selected = j;break;}
        // cout<<"Sub buttons:  "<<sub_buttons[selected].size()<<endl;
        for(int i=0;i<sub_buttons[selected].size();i+=2){
            sub_buttons[selected][i].posx = 5;
            sub_buttons[selected][i].posy = (50+10)*(i/2) - item_slider_y;
            // cout<<sub_buttons[selected][i].posx<<"  "<<sub_buttons[selected][i].posy<<endl;
            sub_buttons[selected][i].draw();
            if(i+1<sub_buttons[selected].size()){
                sub_buttons[selected][i+1].posx = 50+5+10;
                sub_buttons[selected][i+1].posy = (50+10)*(i/2) - item_slider_y;
                // cout<<sub_buttons[selected][i+1].posx<<"  "<<sub_buttons[selected][i+1].posy<<endl;
                sub_buttons[selected][i+1].draw();
            }
        }
        SDL_SetRenderDrawColor(renderer,0x80,0x80,0x80,0xff);
        SDL_Rect rect = {item_slider_x,item_slider_y,9,item_slider_size};
        SDL_RenderFillRect(renderer,&rect);
        SDL_SetRenderTarget(renderer,NULL);
        return item_button_bg;
    }
}
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

            buttons[i].offset_x = posx+5;
            buttons[i].offset_y = posy+5;
            buttons[i].posx = 5;
            buttons[i].posy = (50+10)*i - category_slider_y;
            buttons[i].draw();
        }
        SDL_SetRenderDrawColor(renderer,0x80,0x80,0x80,0xff);
        SDL_Rect rect = {category_slider_x,category_slider_y,9,scrollbar_height};
        SDL_RenderFillRect(renderer,&rect);
        SDL_SetRenderTarget(renderer,NULL);
        return categorbutton_bg;
    }
}
void Inventory::draw(){
    if(!shown)
        return;

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
        SDL_Texture* item_buttons = this->render_itemButtons();
        
        SDL_SetRenderTarget(renderer,inventory_background);
        int cw,ch,iw,ih;
        SDL_QueryTexture(category_buttons,NULL,NULL,&cw,&ch);
        SDL_QueryTexture(item_buttons,NULL,NULL,&iw,&ih);
        
        SDL_Rect category_buttons_rect = {5,5,cw,ch};
        SDL_Rect item_buttons_rect = {70,5,iw,ih};
        SDL_RenderCopy(renderer,category_buttons, NULL,&category_buttons_rect);
        SDL_RenderCopy(renderer,item_buttons, NULL,&item_buttons_rect);
        
        SDL_SetRenderTarget(renderer,NULL);

        SDL_Rect tar_rect = {posx,posy,inv_w,inv_h};
        SDL_RenderCopy(renderer,inventory_background,NULL,&tar_rect);
        SDL_DestroyTexture(category_buttons);
        SDL_DestroyTexture(inventory_background);
        SDL_DestroyTexture(item_buttons);
    }
}