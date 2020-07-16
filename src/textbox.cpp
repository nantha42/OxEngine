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
void TextBox::setPos(int x,int y){
        this->posx = x-textBox_width-4;
        this->posy = y;
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
    this->renderSlider();
    cout<<"Successfully Loaded TextBox"<<endl;
}
void TextBox::assignRenderer(SDL_Renderer* grender){
    renderer = grender;
    textRenderer->renderer = grender;
}
void TextBox::renderSlider(){
    SDL_SetRenderTarget(renderer,textbox_texture);
    SDL_SetRenderDrawColor(renderer,0x80,0x80,0x80,0xff);
    SDL_Rect slider_rect = {textBox_width-9,slider_y,9,slider_size};
    SDL_RenderFillRect(renderer,&slider_rect);
    SDL_SetRenderTarget(renderer,NULL);
}
void TextBox::draw(){
    float r = ((float)content_size - 130)/((float)130-slider_size);
    cout<<"Content SIze "<<content_size<<" Slider size"<<slider_size<<" r: "<<r<<endl;
    cout<<"Position drawn "<<0<<" "<<-slider_y*r<<"r: "<<r<<endl
        <<"Content size "<<content_size<<endl;
    
    SDL_Rect rect = {0,(int)(0- slider_y*r),textBox_width,textBox_height};
    // cout<<"PPP"<<posx<<" "<<posy<<endl;
    SDL_Texture* temp_ = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,textBox_width,textBox_height);
    SDL_SetRenderTarget(renderer,temp_);
    SDL_SetRenderDrawColor(renderer,0xdc,0xdc,0xdc,0xff);
    SDL_Rect fill_rect = {0,0,textBox_width,textBox_height};
    SDL_RenderFillRect(renderer,&fill_rect);
    
    SDL_RenderCopy(renderer,textbox_texture,NULL,&rect);
    SDL_SetRenderTarget(renderer,NULL);
    SDL_Rect global_rect = {posx,posy,textBox_width,textBox_height};
    SDL_RenderCopy(renderer,temp_,NULL,&global_rect);
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
        if(x>= slider_x && x <slider_x + 9 && y>= slider_y && y<= slider_y+slider_size){
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
    
    cout<<"Slider y: "<<slider_y<<endl;

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