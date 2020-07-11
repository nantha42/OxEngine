#include "classes.h"
using namespace std;

enum Elements
{
    energy,
    hydrogen,
    helium,
    carbon,
    aluminium,
    silicon,
    titanium,
    uranium,
    plutonium,
    elements_size
};
class Item
{
    
public:
    int posx;
    int posy;
    time_t begin_time;
    time_t  prev_time;
    int id;
    int produces_element;
    int produces_per_sec;
    int produced = 0;
    
    Item(int x, int y)
    {
        posx = x;
        posy = y;
        begin_time = time(NULL);
        prev_time = (int)time(NULL);    
    }
    int transfer_produced(){
        int t = produced;
        produced = 0;
        return t;
    }
    void update(){
        // if(begin_time-time(NULL)>)
        time_t curtime = time(NULL);
        if((int)(curtime-prev_time) >= 1){
            produced += (curtime-prev_time)*produces_per_sec;
            prev_time = curtime;
        }
        // cout<<prev_time<<" "<<curtime<<endl;
    }
    double getLifeTime()
    {
        return (double)(time(NULL) - begin_time);
    }
};
class Manager
{
public:
    int stock[elements_size];
    int available[elements_size];
    map<pair<int, int>, bool> buildings_existed;
    vector<Item> catalog;
    vector<vector<int>> *world;
    Manager(vector<vector<int>> &map)
    {
        //   istream avaliable("available.txt","r");
        world = &map;
        for (int i = 0; i < elements_size; i++)
            stock[i] = 0;
        cout<<"Traversing world"<<endl;
        cout<<"Finished"<<endl;
        Item reactor(1,7);
        reactor.produces_element = energy;
        reactor.produces_per_sec = 1;
        reactor.id = 1;
        catalog.push_back(reactor);
    }
    void update()
    {
        for(auto &item:catalog){
            item.update();
            stock[item.produces_element] += item.transfer_produced();   
        }
        for(int i=0;i<elements_size;i++){
            cout<<stock[i]<<" ";
        }
        cout<<endl;
    }
};

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
        cin >> mapsize;
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
                switch (x)
                {
                case 1:
                    world[i][j] = x;
                    break;
                case 2:
                    world[i][j] = x;
                    break;
                case 3:
                    world[i][j] = x;
                    break;
                }
            }
        }
        //place river
        int x = 0, y = 0;
        while (x < mapsize && y < mapsize)
        {
            int k = rand() % 2;
            if (k == 0)
                x++;
            if (k)
                y++;
            world[x][y] = 0;
        }
        
        world[1][7] = 5;
        world[1 + 1][7] = -1;
        world[1][7 + 1] = -1;
        world[1 + 1][7 + 1] = -1;
        
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
    Manager *manager;

public:
    MyGame(bool isometric) : Game(isometric)
    {
        string font;

        manager = new Manager(structures.world);
        int size;
        cin >> font >> size;
        textRenderer = new TextRenderer(font, size);

        int n;
        for (int i = 0; i < grid_size; i++)
        {
            for (int j = 0; j < grid_size; j++)
            {
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
            cin >> size;
            Sprite tile(animated, true, NULL);
            simpletile = &tile;

            if (animated)
            {
                cout << path << endl;
                simpletile->animation_images_path = path;
                simpletile->animation_fps = animation_fps;
            }
            else
                simpletile->image_path = path;
            simpletile->size = size;
            simpletile->n_images = n_images;
            simpletile->rect.w = tile_size * size;
            simpletile->rect.h = tile_size * size;
            simpletile->rect.x = 0;
            simpletile->rect.y = 0;
            sprites.push_back(*simpletile);
            n--;
        }
        create_buttons();
        cout << "Building Inventory" << endl;
        build_inventory = new Inventory("build_inventory.txt");
        build_inventory->add_attached_button(&buttons[1]);
        build_inventory->add_attached_button(&buttons[2]);
        build_inventory->textRenderer = textRenderer;
    }
    void create_buttons()
    {
        Button build_button(10, 100, "../Assets/Images/buttons/build");
        Button build_cancel_button(70, 100, "../Assets/Images/buttons/cancel");
        Button build_place_button(70, 100, "../Assets/Images/buttons/place");
        Button resources_stats_button(10,50,"../Assets/Images/buttons/resources_stats_button");        
        Button research_button(10,150,"../Assets/Images/buttons/research_button");
        Button solar_button(10,200,"../Assets/Images/buttons/solar_button");
        buttons.push_back(build_button);
        buttons.push_back(build_cancel_button);
        buttons.push_back(build_place_button);
        buttons.push_back(resources_stats_button);
        buttons.push_back(research_button);
        buttons.push_back(solar_button);
    }
    void update()
    {
        Uint32 curtime = SDL_GetTicks();
        //updating the local map changes into world map
        manager->update();
        if (local_map_changed)
        {
            local_map_changed = false;
            for (int i = 0; i < grid_size; i++)
            {
                for (int j = 0; j < grid_size; j++)
                {
                    structures.world[(int)structures.curx + i][(int)structures.cury + j] = local_map[i][j];
                }
            }
        }

        for (int i = 0; i < structures.localsize; i++)
            for (int j = 0; j < structures.localsize; j++)
                local_map[i][j] = structures.world[(int)structures.curx + i][(int)structures.cury + j];

        for (int i = 0; i < sprites.size(); i++)
            sprites[i].update();

        if (buttons[1].isPressed())
        {
            buttons[1].stablize();
            for (int i = 0; i < grid_size; i++)
                for (int j = 0; j < grid_size; j++)
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
            if (structures.curx < structures.mapsize - structures.localsize - 1)
                structures.curx += 1;
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
        cout << "Main " << game.buttons.size() << endl;
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