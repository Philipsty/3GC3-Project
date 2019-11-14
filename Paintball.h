#ifndef PAINTBALL_H
#define PAINTBALL_H

class Paintball {
public:
    Paintball();
    Paintball(float inX, float inY, float inZ);
    float mX;
    float mY;
    float mZ;
    float color[3]; //maybe have a color and a material as well
    float size;
    float speed;

    //set functions
    void setX(float xI){this->mX = xI;}
    void setY(float yI){this->mY = yI;}
    void setZ(float zI){this->mZ = zI;}

    //get functions
    float getX(){return mX;}
    float getY(){return mY;}
    float getZ(){return mZ;}
    float getR(){return color[0];}
    float getG(){return color[1];}
    float getB(){return color[2];}
};


#endif