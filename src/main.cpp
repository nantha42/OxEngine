#include "classes.h"
using namespace std;

class Structures
{
private:
public:
    int mapsize = 50;
    int localsize = grid_size;
    float curx = 0;
    float cury = 0;

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
        int c = 3;
        world[1+c][3+c]=5;
        world[c][3+c] = 3;
        world[c][3+c+1]=3;
        world[1+1+c][3+1+c] = -1;
        world[1+c][3+1+c] = -1;
        world[1+1+c][3+c] = -1;

        world[1][7] = 5;
        world[1+1][7] = -1;
        world[1][7+1] = -1;
        world[1+1][7+1] = -1;
        world[0][8] = 2;

        world[1][4]= 4;
        world[1][5] = 4;
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
    {   string font;
        int size;
        cin>>font>>size;
        textRenderer = new TextRenderer(font,size);
        // textRenderer->textcolor = {0xff,0xff,0xff,0xff};
        // cout<<"Renderint Text"<<endl;
        // Text *t = new Text(textRenderer->renderText("SSSe"));
        // t->x = screen_width/2 - t->w/2;
        // t->y = 80;
        
        // TextRenderer* textRenderer1 = new TextRenderer(font,10);
        // textRenderer1->textcolor = {0xff,0xff,0xff,0xff};
        // Text *t1 = new Text(textRenderer1->renderText("SSSe"));
        // t1->x = screen_width/2 + t1->w/2 +2;
        // t1->y = 80;
        // texts.push_back(t1);
        // texts.push_back(t);
        
        int n;
        for(int i=0;i<grid_size;i++){
            for(int j=0;j<grid_size;j++){
                selected_tile[i][j] = false;
            }
        }
        cin >> n;
        while (n > 0)
        {
            string path;
            int n_images;
            bool animated;
            int animation_fps;
            int size;
            cin >> path;
            cin >> animated;
            cin >> n_images;
            if (animated)
                cin >> animation_fps;
            cin>>size;
            Sprite tile(animated, true, NULL);
            simpletile = &tile;
            
            if (animated)
            {
                cout<<path<<endl;
                simpletile->animation_images_path = path;
                simpletile->animation_fps = animation_fps;
            }
            else
                simpletile->image_path = path;
            simpletile->size = size;
            simpletile->n_images = n_images;
            simpletile->rect.w = tile_size*size;
            simpletile->rect.h = tile_size*size;
            // cout<<tile_size*size<<endl;
            simpletile->rect.x = 0;
            simpletile->rect.y = 0;
            sprites.push_back(*simpletile);
            n--;
        }
        create_buttons();
        cout<<"Building Inventory"<<endl;
        build_inventory = new Inventory("build_inventory.txt");
        build_inventory->add_attached_button(&buttons[1]);
        build_inventory->add_attached_button(&buttons[2]);
        build_inventory->textRenderer = textRenderer;
    }
    void create_buttons(){
        Button build_button(10,100,"../Assets/Images/buttons/build");
        Button build_cancel_button(70,100,"../Assets/Images/buttons/cancel");
        Button build_place_button(70,100,"../Assets/Images/buttons/place");
        buttons.push_back(build_button);
        buttons.push_back(build_cancel_button);
        buttons.push_back(build_place_button);

    }
    void update()
    {
        Uint32 curtime = SDL_GetTicks();
        //updating the local map changes into world map
        if(local_map_changed){
            local_map_changed= false;
            for(int i=0;i<grid_size;i++){
                for(int j=0;j<grid_size;j++){
                    structures.world[(int)structures.curx+i][(int)structures.cury+j] = local_map[i][j];
                }
            }
        }
        
        for (int i = 0; i < structures.localsize; i++)
            for (int j = 0; j < structures.localsize; j++)
                local_map[i][j] = structures.world[(int)structures.curx + i][(int)structures.cury + j];
                
        for (int i = 0; i < sprites.size(); i++)
            sprites[i].update();
        
        if(buttons[1].isPressed()){
            buttons[1].stablize();
            for(int i=0;i<grid_size;i++)
                for(int j=0;j<grid_size;j++)
                    selected_tile[i][j] = false;
        }
    }
    void eventhandler(EventTriggered &et)
    {
        if (!et.k_w)
            oup = false;

        if (!et.k_s)
            odown = false;

        if (!et.k_a)
            oleft = false;

        if (!et.k_d)
            oright = false;

        if (et.k_w && !oup)
        {
            oup = true;
            if (structures.curx > 0)
                structures.curx -= 1;
        }
        if (et.k_s && !odown)
        {
            odown = true;
            if(structures.curx<structures.mapsize-structures.localsize-1)
                structures.curx+=1;
        }
        if (et.k_a && !oleft)
        {
            oleft = true;
            if (structures.cury < structures.mapsize - structures.localsize - 1)
            {
                structures.cury += 1;
            }
        }
        if (et.k_d && !oright)
        {
            oright = true;
            if (structures.cury > 0)
                structures.cury -= 1;
        }
        build_inventory->handle_clicks(et);
    }
};

int main()
{
    freopen("config.txt", "r", stdin);
    // cin>>grid_size;
    // cin>>tile_size;
    Engine ox;
    
    if (!ox.init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        MyGame game(true);
        cout<<"Main "<<game.buttons.size()<<endl;    
        ox.assignGame(game);
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