#include <math.h>
#include "Splatter.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Constructor for Splatter
Splatter::Splatter(float inX, float inY, float col[3]){
	this -> mX = inX;
	this -> mY = inY;
    this -> color[0] = col[0];
    this -> color[1] = col[1];
    this -> color[2] = col[2];

}

Splatter::Splatter(){
    this -> mX = 0;
    this -> mY = 0;
    this -> color[0] = (rand() % 10) / 10.0;
    this -> color[1] = (rand() % 10) / 10.0;
    this -> color[2] = (rand() % 10) / 10.0;

}