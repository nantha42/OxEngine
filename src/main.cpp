#include "classes.h"
using namespace std;

enum Elements
{
    energy,
    hydrogen,
    helium,
    carbon,
    oxygen,
    aluminium,
    silicon,
    titanium,
    uranium,
    plutonium,
    research,
    oil,
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
    int unit_production;
    vector<int> consumes_id;
    vector<int> consumption_units;
};
struct Request
{
    int x, y;
    int structure_id;
};
class RecordManager
{
    time_t prevtime = time(NULL);

public:
    int stock[elements_size];
    int available[elements_size];
    int stock_production[elements_size];
    int stock_consumption[elements_size];
    vector<Record *> records;

    int last_id = 0;
    map<string, int> elements_ids;
    map<string, int> structures_ids;
    map<string, vector<string>> consumptions;
    map<string, string> production;
    RecordManager()
    {
        for (int i = 0; i < elements_size; i++)
        {
            stock[i] = 10000;
        }
        ifstream file("../Assets/Data/elements_info.txt");
        int n = 0, i = 0;
        file >> n;

        while (n--)
        {
            string s;
            file >> s;
            // elements_ids.insert(pair<string, int>(s, i++));
            elements_ids[s] = i++;
        }
        file.close();

        ifstream file1("../Assets/Data/structure_info.txt");
        file1 >> n;
        while (n--)
        {
            string name, product;
            vector<string> consumables;
            file1 >> name;
            cout << name << endl;
            int n_consum;
            file1 >> n_consum;
            cout << n_consum;
            for (int i = 0; i < n_consum; i++)
            {
                string consumable;
                file1 >> consumable;
                consumables.push_back(consumable);
                cout << consumable << endl;
            }
            consumptions.insert(pair<string, vector<string>>(name, consumables));
            file1 >> product;
            cout << product << endl;
            production.insert(pair<string, string>(name, product));
        }
        file1.close();
    }
    void create_record(Request request)
    {
        Record *record = new Record;
        record->x = request.x;
        record->y = request.y;
        record->structure_id = request.structure_id;
        for (auto it : structures_ids)
        {
            if (it.second == request.structure_id)
            {
                record->structure_name = it.first;
                string produces = production[it.first];
                // cout<<produces<<endl;
                int k = produces.find("-");
                string element_produces = produces.substr(0, k);
                // cout<<element_produces<<endl;
                record->product_id = elements_ids[element_produces];
                record->unit_production = stoi(produces.substr(k + 1, produces.size() - k - 1));

                vector<string> consumables = consumptions[it.first];
                vector<int> consumptions_units(elements_size, 0);

                for (string consumable : consumables)
                {
                    int k = consumable.find("-");
                    string consumable_name = consumable.substr(0, k);
                    int unit = stoi(consumable.substr(k + 1, consumable.size() - (k + 1)));
                    consumptions_units[elements_ids[consumable_name]] = unit;
                }
                record->consumption_units = consumptions_units;
                for (auto x : consumptions_units)
                    cout << x << " ";
                cout << endl;
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
    void print_elements()
    {
        for (auto x : elements_ids)
        {
            cout << x.first << " " << x.second << endl;
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
        time_t curtime = time(NULL);

        if ((int)(curtime - prevtime) >= 1)
        {
            //energy should not keep increasing
            //but it should be equal to amount of production
            //so equalling it to 0 for each update
            for (int i = 0; i < elements_size; i++)
            {
                stock_production[i] = 0;
                stock_consumption[i] = 0;
            }

            stock[0] = 0;
            for (auto record : records)
            {

                if (record->product_id == 0)
                {
                    bool require_available = true;
                    for (int i = 0; i < elements_size; i++)
                    {
                        if (stock[i] < record->consumption_units[i])
                        {
                            require_available = false;
                            break;
                        }
                    }
                    if (require_available)
                    {
                        stock[0] += record->unit_production;
                    }
                }
            }

            for (auto record : records)
            {
                bool require_available = true;
                if (record->product_id == 0)
                {
                    stock_production[0] += record->unit_production;
                    continue;
                }
                for (int i = 0; i < elements_size; i++)
                {
                    if (stock[i] < record->consumption_units[i])
                    {
                        require_available = false;
                        break;
                    }
                }
                if (require_available)
                {
                    for (int i = 0; i < elements_size; i++)
                    {
                        stock[i] -= record->consumption_units[i];
                        stock_consumption[i] += record->consumption_units[i];
                    }
                    stock[record->product_id] += record->unit_production;
                    stock_production[record->product_id] += record->unit_production;
                }
            }

            // for (int i = 0; i < 1; i++)
            // {
            //     cout << "Energy: " << stock_production[i] << " " << stock_consumption[i] << endl;
            // }
            // cout << endl;
            prevtime = curtime;
        }
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
        manager->print_elements();
        resourceMeter = new ResourceMeter("../Assets/Images/buttons/power0.png");
        resourceMeter->setSize(150, 42);
        resourceMeter->setIconWidth(32, 32);

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
            string path, name;
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
        for (int i = 0; i < structural_sprites.size(); i++)
        {
            manager->structures_ids.insert(pair<string, int>(structural_sprites[i].name, i));
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
        resourceMeter->update(manager->stock_production[energy], manager->stock_consumption[energy]);
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