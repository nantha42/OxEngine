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
    research,
    elements_size
};
struct Record
{
    int id;
    int structure_id;
    string structure_name;
    int type;
    int x, y;
    time_t founded;
    int product_id;
    vector<int> consumes_id;
};
struct Request
{
    int x, y;
    int structure_id;
};
class RecordManager
{
public:
    int stock[elements_size];
    int available[elements_size];
    vector<Record *> records;
    
    int last_id = 0;
    map<string, int> elements_ids;
    map<string, int> structures_ids;
    RecordManager()
    {
        ifstream file("../Data/elements_info.txt");
        int n = 0, i = 0;
        file >> n;

        while (n--)
        {
            string s;
            file >> s;
            elements_ids.insert(pair<string, int>(s, i++));
        }
    }
    void create_record(Request request)
    {
        Record *record = new Record;
        record->x = request.x;
        record->y = request.y;
        record->structure_id = request.structure_id;
        for(auto it:structures_ids){
            if(it.second == request.structure_id){
                record->structure_name = it.first;
            }
        }
        record->founded = time(NULL);
        record->id = last_id++;
        records.push_back(record);
        cout << "Record Created" << endl;
    }
    void modify_record(Request request)
    {
        for (Record *r : records)
        {
            if (r->x == request.x && r->y == request.y)
            {
                r->structure_id = request.structure_id;
                cout << "Modified Record" << endl;
                return;
            }
        }
    }
    void delete_record(Request request)
    {
        int pos = 0;
        bool record_found = false;
        for (; pos < records.size(); pos++)
        {
            if (records[pos]->x == request.x && records[pos]->y == request.y)
            {
                record_found = true;
                break;
            }
        }

        if (record_found)
        {
            cout << "Deleted Record" << endl;
            records.erase(records.begin() + pos, records.begin() + pos + 1);
        }
        else
            cout << "Deleted No Record" << endl;
    }
    void update()
    {
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
    }
};
class MyGame : public Game
{
    Sprite *simpletile;
    Structures structures;

    bool oup = false;
    bool odown = false;
    bool oleft = false;
    bool oright = false;
    RecordManager *manager;

public:
    MyGame(bool isometric) : Game(isometric)
    {
        string font;
        manager = new RecordManager();
        int size;
        cin >> font >> size;
        SDL_Color color = {0x00, 0x00, 0x00, 0xFF};
        textRenderer = new TextRenderer(font, size, color);
        int n;
        for (int i = 0; i < grid_size; i++)
        {
            for (int j = 0; j < grid_size; j++)
                selected_tile[i][j] = false;
        }
        cin >> n;
        while (n > 0)
        {
            string path,name;
            int n_images;
            bool animated;
            int animation_fps;
            int size;
            
            cin >> path >> name;
            cin >> animated;
            cin >> n_images;
            if (animated)
                cin >> animation_fps;
            cin >> size;
            Sprite tile(animated, true, NULL);

            simpletile = &tile;
            simpletile->name = name;
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
            structural_sprites.push_back(*simpletile);
            n--;
        }
        for(int i=0;i<structural_sprites.size();i++){
            manager->structures_ids.insert(pair<string,int>(structural_sprites[i].name,i));
        }
        
        create_buttons();
        cout << "Building Inventory" << endl;
        build_inventory = new Inventory("build_inventory.txt");
        build_inventory->add_attached_button(&buttons[1]);
        build_inventory->add_attached_button(&buttons[2]);
        build_inventory->textRenderer = textRenderer;
        level_status_bar = new LevelStatusBar(screen_width / 2 - 100, 0);
    }
    void create_buttons()
    {
        Button build_button(10, 100, "../Assets/Images/buttons/build");
        Button build_cancel_button(70, 100, "../Assets/Images/buttons/cancel");
        Button build_place_button(70, 100, "../Assets/Images/buttons/place");
        Button resources_stats_button(10, 50, "../Assets/Images/buttons/resources_stats_button");
        Button research_button(10, 150, "../Assets/Images/buttons/research_button");
        Button solar_button(10, 200, "../Assets/Images/buttons/solar_button");

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
            for (int i = 0; i < local_map_changed_pos.size(); i++)
            {
                int x = local_map_changed_pos[i].first;
                int y = local_map_changed_pos[i].second;
                int id_in_world = structures.world[(int)structures.curx + x][(int)structures.cury + y];
                if (id_in_world == local_map[x][y])
                    continue;

                if (id_in_world != 1 && id_in_world != 0)
                {
                    // Modifying existing record
                    if (local_map[x][y] != 1)
                    {
                        Request req;
                        req.x = x;
                        req.y = y;
                        req.structure_id = local_map[x][y];
                        manager->modify_record(req);
                    }
                    else
                    {
                        //sending a delete request because the new localmap contains empty ground
                        Request req;
                        req.x = x;
                        req.y = y;
                        req.structure_id = id_in_world;
                        manager->delete_record(req);
                    }
                }
                else
                {
                    //Creating a new record because the world contained only a ground or water
                    if (local_map[x][y] != 1) // Condition for preventing records for empty ground
                    {
                        Request req;
                        req.x = x;
                        req.y = y;
                        req.structure_id = local_map[x][y];
                        manager->create_record(req);
                    }
                }
            }

            local_map_changed_pos.clear();
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

        for (int i = 0; i < structural_sprites.size(); i++)
            structural_sprites[i].update();

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