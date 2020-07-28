#include "classes.h"

/*
class ResourceMeter{
    string image_path;
    int width;
    int height;
    int icon_width;
    int icon_height;
    int base=1000;
    
    public:

    SDL_Renderer *gRender;
    SDL_Texture *resource_image;
    SDL_Texture *texture_meter;
    TextRenderer* textRenderer;
    int resources_level;
    int usage;
    int consumption;
    ResourceMeter(string imagepath);
    void assignRenderer(SDL_Renderer* gRender);
    void update();
};
*/

ResourceMeter::ResourceMeter(string imagepath)
{
    this->imagepath = imagepath;
    cout << "Loading font for RM" << endl;
    textRenderer = new TextRenderer("../Assets/Fonts/Quicksand.ttf", 12);
    textRenderer->textcolor = {0x00, 0x00, 0x00, 0xff};
    cout << "Loading font for RM" << endl;
}
void ResourceMeter::setSize(int w, int h)
{
    width = w;
    height = h;
}
void ResourceMeter::assignRenderer(SDL_Renderer *gRender)
{
    this->gRender = gRender;
    textRenderer->renderer = gRender;
    SDL_Surface *surface = IMG_Load(imagepath.c_str());
    if (surface != NULL)
    {
        resource_image = SDL_CreateTextureFromSurface(gRender, surface);
        if (resource_image == NULL)
        {
            cout << "Error in Loading Resource Image" << endl;
        }
        texture_meter = SDL_CreateTexture(gRender, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, width, height);
    }
}
void ResourceMeter::setIconWidth(int w, int h)
{
        icon_width = w;
        icon_height = h;
}
void ResourceMeter::copyItemTexture(){
    item_texture = texture_meter;
}
void ResourceMeter::update(int u, int c)
{

    if (usage != u || consumption != c || texture_meter == NULL || current_usage != usage || current_consumption != consumption)
    {
        usage = u;
        consumption = c;

        //animation of bar increasing and decreasing

        int increase_rate = 5;
        // if (((float)prevtime - time(NULL)) > 0.5)
        // {
        prevtime = time(NULL);
        if (current_usage < usage)
        {
            if (current_usage + increase_rate > usage)
                current_usage = usage;
            else
                current_usage += increase_rate;
        }
        else if (current_usage > usage)
        {
            if (current_usage - increase_rate < usage)
                current_usage = usage;
            else
                current_usage -= increase_rate;
        }
        if (current_consumption < consumption)
        {
            if (current_consumption + increase_rate > consumption)
                current_consumption = consumption;
            else
                current_consumption += increase_rate;
        }
        else if (current_consumption > consumption)
        {
            if (current_consumption - increase_rate < consumption)
                current_consumption = consumption;
            else
                current_consumption -= increase_rate;
        }
        // }
        //end of code for bar animation

        SDL_SetRenderTarget(gRender, texture_meter);
        SDL_Rect fillRect = {0, 0, width, height};
        SDL_SetRenderDrawColor(gRender, 0xdc, 0xdc, 0xdc, 0xff);
        SDL_RenderFillRect(gRender, &fillRect);
        SDL_Rect resource_rect = {5, 5, icon_width, icon_height};
        SDL_RenderCopy(gRender, resource_image, NULL, &resource_rect);
        int length = width - (icon_width + 5 + 5) - 5;

        if (usage > base)
        {
            base = base * 2;
        }
        else if (usage < base / 5)
        {
            base = base / 5;
        }
        float usage_ratio = (float)current_usage / base;
        int usage_bar_width = length * usage_ratio;
        int bar_height = 15;
        SDL_Rect usage_bar_rect = {10 + icon_width, 5, usage_bar_width, bar_height};
        SDL_SetRenderDrawColor(gRender, 0x00, 0xff, 0x00, 0xff);
        SDL_RenderFillRect(gRender, &usage_bar_rect);

        float consumption_ratio = (float)current_consumption / base;
        int consumption_bar_width = length * consumption_ratio;
        SDL_Rect consumption_bar_rect = {10 + icon_width, 25, consumption_bar_width, bar_height};
        SDL_SetRenderDrawColor(gRender, 0xff, 0x00, 0x00, 0xff);
        SDL_RenderFillRect(gRender, &consumption_bar_rect);

        SDL_Texture *text = textRenderer->renderTexture(to_string(usage));
        int tw, ty;
        SDL_QueryTexture(text, NULL, NULL, &tw, &ty);
        SDL_Rect textrect = {10 + icon_width + 5 + 2, 5, tw, ty};
        SDL_RenderCopy(gRender, text, NULL, &textrect);
        SDL_DestroyTexture(text);

        text = textRenderer->renderTexture(to_string(consumption));
        SDL_QueryTexture(text, NULL, NULL, &tw, &ty);
        textrect = {10 + icon_width + 5 + 2, 25, tw, ty};
        SDL_RenderCopy(gRender, text, NULL, &textrect);
        SDL_DestroyTexture(text);

        SDL_SetRenderTarget(gRender, NULL);
        copyItemTexture();
    }
}
