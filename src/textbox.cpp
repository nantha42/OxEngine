#include "classes.h"

TextBox::TextBox(string font,int size){
    text_size = size;
    renderer = NULL;
    textbox_texture = NULL;
    SDL_Color color = {0x00,0x00,0x00,0xff};
    textRenderer = new TextRenderer(font,text_size,color);   
    
}
void TextBox::setSize(int w,int h){
    textBox_width = w;
    textBox_height = h;
    slider_x = textBox_width-9;
}
void TextBox::LoadText(string s){
    text = s;
    renderText();
}
void TextBox::setPos(int x,int y){
        this->posx = x-textBox_width-4;
        this->posy = y;
}
void TextBox::renderText(){
    
    SDL_Surface* a = textRenderer->renderSurface("a");
    SDL_Surface* A = textRenderer->renderSurface("A");
    // int maxletters_perline = textBox_width/(a)
    
    vector<string> lines;
    int line_occupy = 0;
    int seg_start = 0;
    int i=0;

    vector<string> words;
    istringstream ss(text);
    do{
        string word;
        ss>>word;
        words.push_back(word);
    }while(ss);
    string line = "";
    for(int i=0;i<words.size();i++){
        if(words[i][0]=='`'){
            lines.push_back(line);
            line = "";
        }else{
            if( (line.size()+words[i].size())*a->w < textBox_width){
                line+=(words[i]+" ");
            }else{
                if(line !=""){
                    lines.push_back(line);
                    line = "";
                }else if(line==""){
                    lines.push_back(words[i]);
                }
            }
        }
    }
    if(line!="")
        lines.push_back(line);

    vector<SDL_Texture*> textures;
    for(string s:lines){
        textures.push_back(textRenderer->renderTexture(s));
    }
    textbox_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,textBox_width,textBox_height);
    SDL_SetRenderTarget(renderer,textbox_texture);
    SDL_Rect bg_rect = {0,0,textBox_width,textBox_height};
    SDL_SetRenderDrawColor(renderer,0xdc,0xdc,0xdc,0xff);
    SDL_RenderFillRect(renderer,&bg_rect);
    int x=5,y=5;
    content_size = 0;
    for(SDL_Texture* texture:textures){
        int texw,texh;
        SDL_QueryTexture(texture,NULL,NULL,&texw,&texh);
        content_size += (texh+texh*0.2f);
        SDL_Rect rect = {x,y,texw,texh};
        SDL_RenderCopy(renderer,texture,NULL,&rect);
        y+= texh+(texh)*0.2f;
    }
    slider_size = ((float)textBox_height/(float)content_size)*130;
    SDL_SetRenderTarget(renderer,NULL);
    
}
void TextBox::assignRenderer(SDL_Renderer* grender){
    renderer = grender;
    textRenderer->renderer = grender;
}

void TextBox::draw(){
    float r = ((float)content_size - 130)/((float)130-slider_size);
    // cout<<"Content SIze "<<content_size<<" Slider size"<<slider_size<<" r: "<<r<<endl;
    // cout<<"Position drawn "<<0<<" "<<-slider_y*r<<"r: "<<r<<endl
    //     <<"Content size "<<content_size<<endl;
    
    SDL_Rect rect = {0,(int)(0- slider_y*r),textBox_width,textBox_height};
    // cout<<"PPP"<<posx<<" "<<posy<<endl;
    SDL_Texture* temp_ = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,textBox_width,textBox_height);
    SDL_SetRenderTarget(renderer,temp_);
    SDL_SetRenderDrawColor(renderer,0xdc,0xdc,0xdc,0xff);
    SDL_Rect fill_rect = {0,0,textBox_width,textBox_height};
    SDL_RenderFillRect(renderer,&fill_rect);
    SDL_RenderCopy(renderer,textbox_texture,NULL,&rect);
    SDL_SetRenderDrawColor(renderer,0x80,0x80,0x80,0xff);
    SDL_Rect slider_rect = {textBox_width-slider_width,slider_y,slider_width,slider_size};
    SDL_RenderFillRect(renderer,&slider_rect); 
    SDL_SetRenderTarget(renderer,NULL);
    SDL_Rect global_rect = {posx,posy,textBox_width,textBox_height};
    SDL_RenderCopy(renderer,temp_,NULL,&global_rect);
    SDL_DestroyTexture(temp_);
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
    x-= posx+5;
    y-= posy;
    
    if(mouse_holded){
        if(x>= slider_x && x <slider_x + slider_width && y>= slider_y && y<= slider_y+slider_size){
            if(!slider_dragging){
                slider_dragging = true;
                slider_drag_startpoint = y;
            }
        }
        if(slider_dragging){
            int t = y - slider_drag_startpoint;
            if(slider_drag_startpoint > y){
                int t = slider_drag_startpoint - y;
                if(slider_y-t>0)
                    slider_y-= t;
                else slider_y = 0;
            }else{
                int t = y-slider_drag_startpoint;
                slider_drag_startpoint = y;
                if(slider_y+t<135 - slider_size)
                    slider_y += t;
                else slider_y = 135 - slider_size;
            }
        }
    }else if(slider_dragging)
        slider_dragging = false;
    

}

void TextBox::handle_clicks(EventTriggered &et){
    if(!shown)
        return;
    int x = et.movx;
    int y = et.movy;
    bool mouse_holded = et.mouse_holded;
    // cout<<"Mouse holded :"<<et.mouse_holded<<" "<<x<<" "<<y<<endl;
    slider_clicked(x,y,mouse_holded);
}