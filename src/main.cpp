#include<bits/stdc++.h>
#include "classes.h"
using namespace std;


class MyGame: public Game{
    Sprite *watertile;
    Sprite *groundTile;
    Sprite *treeTile;
    Sprite *treeTile1;
    Sprite *simpletile;
    public:

    MyGame(bool isometric):Game(isometric){
        int n;
        cin>>n;
        while(n>0){
            string path;
            int n_images;
            bool animated;
            cin>>path;
            cin>>animated;
            cin>>n_images;
            Sprite tile(animated,true,NULL);
            simpletile = &tile;
            if(animated)
                simpletile->animation_images_path = path;
            else
                simpletile->image_path = path;
            simpletile->n_images = n_images;
            simpletile->rect.w = tile_size;
            simpletile->rect.h = tile_size;
            simpletile->rect.x = 0;
            simpletile->rect.y = 0;
            sprites.push_back(*simpletile);
        
            n--;
        }  
        /*
        Sprite tile(true,true,NULL);
        watertile = &tile;
        watertile->animation_images_path = "../Assets/Images/water_tile/water_tile";
        watertile->n_images = 8;
        watertile->rect.w = 128;
        watertile->rect.h = 128;
        watertile->rect.x = 0;
        watertile->rect.y = 0;
        sprites.push_back(*watertile);
        
        Sprite tile1(false,true,NULL);
        groundTile = &tile1;
        groundTile->image_path = "../Assets/Images/isometric_tile";
        groundTile->rect.w  = 128;
        groundTile->rect.h = 128;
        groundTile->rect.x = 0;
        groundTile->rect.y = 0;
        watertile->n_images = 1;
        sprites.push_back(*groundTile);
        
        
        Sprite tile2(false,true,NULL);
        treeTile = &tile2; 
        treeTile->image_path = "../Assets/Images/isometric_tree";
        treeTile->rect.w  = 128;
        treeTile->rect.h = 128;
        treeTile->rect.x = 0;
        treeTile->rect.y = 0;
        treeTile->n_images = 1;
        sprites.push_back(*treeTile);

        Sprite tile3(false,true,NULL);
        treeTile1 = &tile3; 
        treeTile1->image_path = "../Assets/Images/isometric_tree1";
        treeTile1->rect.w  = 128;
        treeTile1->rect.h = 128;
        treeTile1->rect.x = 0;
        treeTile1->rect.y = 0;
        treeTile1->n_images = 1;
        sprites.push_back(*treeTile1);
        */
        for(int i=0;i<grid_size;i++){
            for(int j=0;j<grid_size;j++){
                cin>>world_map[i][j];
            }
        }
    }   
    void update(){
        Uint32 curtime = SDL_GetTicks();
        if(curtime-unit_time >= 100){
                unit_time = curtime;
            
            for(int i=0;i<sprites.size();i++){
                
                    cout<<sprites[i].animated_sprite<<endl;
                    if(sprites[i].animated_sprite){    
                        sprites[i].curframe = (sprites[i].curframe+1)%sprites[i].n_images;
                    }       
                }
        }
    }
};

int main(){
    freopen("config.txt","r",stdin);
    // cin>>grid_size;
    // cin>>tile_size;
    Engine ox;
    MyGame game(true);

    if( !ox.init(game) )
    {
        printf( "Failed to initialize!\n" );

    }
    else
    {
        //Load media
        if( !ox.loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Apply the image
            cout<<"Noe Problem"<<endl;
            ox.run();
        }
    }
    ox.close();
    return 0;
}