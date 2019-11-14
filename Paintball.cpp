#include <math.h>
#include "Paintball.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Constructor for Paintball
Paintball::Paintball(){
	this -> mX = 0;
    this -> mY = 0;
    this -> mZ = 0;
    //make ball size adjustable in menu
    this -> size = 2;
    this -> color[0] = (rand() % 10) / 10.0;
    this -> color[1] = (rand() % 10) / 10.0;
    this -> color[2] = (rand() % 10) / 10.0;
    //speed, static for now
    this -> speed = 0.5;
    
}

//Constructor with Parameters for Paintball
Paintball::Paintball(float inX, float inY, float inZ){
	this -> mX = inX;
	this -> mY = inY;
	this -> mZ = inZ;

	this -> size = 2;
    this -> color[0] = (rand() % 10) / 10.0;
    this -> color[1] = (rand() % 10) / 10.0;
    this -> color[2] = (rand() % 10) / 10.0;

    this -> speed = 0.5;

}