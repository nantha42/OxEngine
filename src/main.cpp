#include<bits/stdc++.h>
#include "classes.h"
using namespace std;

int main(){
    Engine ox;

    if( !ox.init() )
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