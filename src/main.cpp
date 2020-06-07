#include<bits/stdc++.h>
#include "classes.h"
using namespace std;


class MyGame: public Game{
    
    void update(){

    }
}

int main(){
    Engine ox;
    MyGame game;
    if( !ox.init() )
    {
        printf( "Failed to initialize!\n" );
        ox.game = game;
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