#include<bits/stdc++.h>
#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include "classes.h"
using namespace std;

vector<double> Vector::add(vector<double> a,vector<double> b){
    vector<double> c(2);
    c[0] = a[0]+b[0];
    c[1] = a[1]+b[1];
    return c;
}
vector<double> Vector::sub(vector<double> a,vector<double>b){
    vector<double> c(2);
    c[0] = a[0]-b[0];
    c[1] = a[1]-b[1];
    return c;
}
vector<int> Vector::add(vector<int> a,vector<int> b){
    vector<int> c(2);
    c[0] = a[0]+b[0];
    c[1] = a[1]+b[1];
    return c;
}
vector<int> Vector::sub(vector<int> a,vector<int> b){
    vector<int> c(2);
    c[0] = a[0]-b[0];
    c[1] = a[1]-b[1];
    return c;
}
