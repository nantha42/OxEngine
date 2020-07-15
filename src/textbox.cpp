#include "classes.h"

TextBox::TextBox(string font,int size){
    text_size = size;
    renderer = NULL;
    textbox_texture = NULL;
    SDL_Color color = {0x00,0x00,0x00,0xff};
    textRenderer = new TextRenderer(font,text_size,color);   
    
}
void TextBox::LoadText(string s){
    text = s;
    cout<<"Rendering Text"<<endl;
    renderText();
}

void TextBox::renderText(){
    cout<<"TextRenderer"<<endl;
    SDL_Surface* a = textRenderer->renderSurface("a");
    SDL_Surface* A = textRenderer->renderSurface("A");
    // int maxletters_perline = textBox_width/(a)
    cout<<"Lines"<<endl;
    vector<string> lines;
    int line_occupy = 0;
    int seg_start = 0;
    cout<<"A a size"<<A->w<<" "<<a->w<<endl;
    for(int i=0;i<text.size();i++){
        char c = text[i];
        
        if( (c>='a' && c<= 'z') || (c>='0' && c<='9')){
            line_occupy += a->w;
        }
        else if(c>='A' && c<='Z'){
            line_occupy += A->w;
        }else{
            line_occupy+=A->w;
        }                     
        if(line_occupy+A->w>= textBox_width){
            line_occupy = 0;
            string newstring = text.substr(seg_start,(i-seg_start+1));
            lines.push_back(newstring);
            seg_start = i+1;
        }
    }
    cout<<"Text is parsed"<<endl;
    vector<SDL_Texture*> textures;
    for(string s:lines){
        textures.push_back(textRenderer->renderTexture(s));
    }
    cout<<"Textures for lines is created"<<endl;
    SDL_SetRenderTarget(renderer,textbox_texture);
    SDL_Rect bg_rect = {0,0,textBox_width,textBox_height};
    SDL_SetRenderDrawColor(renderer,0x80,0x80,0x80,0xff);
    SDL_RenderFillRect(renderer,&bg_rect);
    int x=5,y=5;
    for(SDL_Texture* texture:textures){
        int texw,texh;
        SDL_QueryTexture(texture,NULL,NULL,&texw,&texh);
        SDL_Rect rect = {x,y,texw,texh};
        SDL_RenderCopy(renderer,texture,NULL,&rect);
        y+= texh+(texh)*0.2f;
    }
    SDL_SetRenderTarget(renderer,NULL);
    cout<<"Successfully Loaded TextBox"<<endl;
}
void TextBox::assignRenderer(SDL_Renderer* grender){
    renderer = grender;
    textRenderer->renderer = grender;
}
void TextBox::draw(){
    SDL_Rect rect = {posx,posy,textBox_width,textBox_height};
    cout<<"PPP"<<posx<<posy<<endl;
    SDL_RenderCopy(renderer,textbox_texture,NULL,&rect);
}
void TextBox::clearTexture(){
    SDL_DestroyTexture(textbox_texture);
    textbox_texture = NULL;
}
bool TextBox::isTextBoxClear(){
    return (textbox_texture!=NULL?false:true);
}
void TextBox::showTextBox(){ shown = true;}

void TextBox::hideTextBox(){ shown = false;}

void TextBox::slider_clicked(int x,int y,bool mouse_holded){
    
}

void TextBox::handle_clicks(EventTriggered &et){
    if(!shown)
        return;
    int x = et.movx;
    int y = et.movy;
    bool mouse_holded = et.mouse_holded;
    slider_clicked(x,y,mouse_holded);
    
}