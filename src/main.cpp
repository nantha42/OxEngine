#include<bits/stdc++.h>
#include "classes.h"
using namespace std;

int main(){
    Engine iso;

    if( !iso.init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !iso.loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Apply the image

            cout<<"Noe Problem"<<endl;
            iso.run();
        }
    }
    iso.close();
    return 0;
}