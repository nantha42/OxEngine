#include <bits/stdc++.h>
#include "classes.h"
using namespace std;

class Structures
{
private:
public:
    int mapsize = 50;
    int localsize = grid_size;
    float curx = 10;
    float cury = 10;

    vector<vector<int>> world;
    Structures()
    {
        cin>>mapsize;
        for (int i = 0; i < mapsize; i++)
        {
            vector<int> rows(mapsize, 1);
            world.push_back(rows);
        }
        //place trees
        for (int i = 0; i < mapsize; i++)
        {
            for (int j = 0; j < mapsize; j++)
            {
                int x = 1 + rand() % 10;
                switch(x){
                    case 1:
                    world[i][j] = x;
                    break;
                    case 2:
                    world[i][j]=x;
                    break;
                    case 3:
                    world[i][j] = x;
                    break;
                }
            }
        }
        //place river
        int x=0,y=0;
        while(x<mapsize && y <mapsize){
            int k = rand()%2;
            if(k==0)
                x++;
            if(k)
                y++;
            world[x][y] = 0;
        }
    }
};
class MyGame : public Game
{
    Sprite *watertile;
    Sprite *groundTile;
    Sprite *treeTile;
    Sprite *treeTile1;
    Sprite *simpletile;
    Structures structures;

    bool oup = false;
    bool odown = false;
    bool oleft = false;
    bool oright = false;

public:
    MyGame(bool isometric) : Game(isometric)
    {
        int n;
        cin >> n;
        while (n > 0)
        {
            string path;
            int n_images;
            bool animated;
            int animation_fps;
            cin >> path;
            cin >> animated;
            cin >> n_images;
            if (animated)
                cin >> animation_fps;
            Sprite tile(animated, true, NULL);
            simpletile = &tile;
            if (animated)
            {
                simpletile->animation_images_path = path;
                simpletile->animation_fps = animation_fps;
            }
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
        cout << "Size of sprites" << sprites.size() << endl;
    }
    void update()
    {
        Uint32 curtime = SDL_GetTicks();
        for (int i = 0; i < structures.localsize; i++)
        {
            for (int j = 0; j < structures.localsize; j++)
            {
                local_map[i][j] = structures.world[(int)structures.curx + i][(int)structures.cury + j];
            }
        }
        for (int i = 0; i < sprites.size(); i++)
        {
            sprites[i].update();
        }
    }
    void eventhandler(EventTriggered et)
    {
        if (!et.k_up)
            oup = false;

        if (!et.k_down)
            odown = false;
        if (!et.k_left)
            oleft = false;

        if (!et.k_right)
            oright = false;

        if (et.k_up && !oup)
        {
            oup = true;
            if (structures.curx > 0)
                structures.curx -= 1;
        }
        if (et.k_down && !odown)
        {
            odown = true;
            if(structures.curx<structures.mapsize-structures.localsize-1)
                structures.curx+=1;
            
        }
        if (et.k_left && !oleft)
        {
            oleft = true;
            if (structures.cury < structures.mapsize - structures.localsize - 1)
            {
                structures.cury += 1;
            }
            
        }
        if (et.k_right && !oright)
        {
            oright = true;
            if (structures.cury > 0)
                structures.cury -= 1;
        }
    }
};

int main()
{
    freopen("config.txt", "r", stdin);
    // cin>>grid_size;
    // cin>>tile_size;
    Engine ox;
    MyGame game(true);

    if (!ox.init(game))
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        //Load media
        if (!ox.loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            //Apply the image
            cout << "Noe Problem" << endl;
            ox.run();
        }
    }
    ox.close();
    return 0;
}