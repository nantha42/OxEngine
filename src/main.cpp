#include<bits/stdc++.h>
#include "classes.h"
using namespace std;


class MyGame: public Game{
    public:
    MyGame(bool isometric):Game(isometric){

    }
    void update(){

    }
};

int main(){
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