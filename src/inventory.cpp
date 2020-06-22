#include "classes.h"

Inventory::Inventory(string categoryfile){
        ifstream file(categoryfile);
        int ncategories;
        file>>ncategories;
        while(ncategories--){
            string categoryname;
            file>>categoryname;
            categories_names.push_back(categoryname);
            int nitems;
            file>>nitems;
            vector<string>items_group;
            while(nitems--){
                string itemname;
                file>>itemname;
                items_group.push_back(itemname);
            }
            items_names.push_back(items_group);
        }
    }
void Inventory::load_images(){
        for(int i=0;i<categories_names.size();i++){
            SDL_Surface*temp;
            string name = categories_names[i] +".png";
            cout<<name<<endl;
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
                }
                cout<<"Camer here 1"<<endl;
                vector<SDL_Texture*> texture_category_items;
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
                texture_items.push_back(texture_category_items);
            }
        }
    }
void Inventory::showInventory(){
        shown = true;
    }
bool Inventory::hideInventory(){
      return true;  
}
bool Inventory::checkAnyItemClicked(){
    return true;
}
void Inventory::draw(){
        
}