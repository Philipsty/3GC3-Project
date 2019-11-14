// COMPSCI 3GC3
// Tyler Philips & Jakub Pawlikowski
// 400017512 & 400011899
// Final Project
#include <math.h>
#include <iostream>
#include "Paintball.h"
#include "Splatter.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

// //music
// #include <Windows.h>
// #include <MMSystem.h>

using namespace std;

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

//global variables
std::vector<Paintball> paintBallVec(0);
std::vector<Splatter> splatterVec(0);
//bool shot = false;
bool reset = true;
int cnt = 0;
bool axisToggle = false;

//total shots fired
float shotsFired=0;

//crosshair coordinates
float crossHairPos[3] = {0, 9, 47};

// One light for now
float lightPos[2][4] = {
    { 5, 20, 0, 1 },
    { -5, 20, 0, 1 }
};

//camera position
float eye[3] = {0, 10, 50};

//some basic lighting characteristics
float amb[4] = {0,0,0,0};
float spec[4] = {1,1,1,1};
float diff[4] = {0.7,0.7,0.7,1};

//Floor map coordinates
float floorCoord[4][3]={
    {-20, 0, -40},
    {20, 0, -40},
    {20, 0, 40},
    {-20, 0, 40}
};

float wallCoords[4][3]={
    {-15, 0, -35},
    {-15, 20, -35},
    {15, 20, -35},
    {15, 0, -35},
};

float tableCoords[4][3]={
    {-3, 7, 42},
    {3, 7, 42},
    {3, 7, 49},
    {-3, 7, 49}
};

//colour change
bool colourChanged = false;
float paintBallColour[3]={0,0,0};

//material variables
GLfloat materialAmbient[4] = {0,0,0,1};
GLfloat materialSpecular[4] = {.5,.5,.5,1};
GLfloat materialDiffuse[4] = {1,0,0,0};
GLfloat materialShininess[] = {10.0};

//this function is called once at the beginning of the program to print instructions for user
void instructions()
{
  cout<<"WELCOME TO 3D PAINTBALL SIMULATOR" <<endl;
  cout<<"======================================="<<endl;
  cout<<"arrow keys : move crosshairs"  <<endl;
  cout<<"space : shoot paintball"<<endl;
  cout<<"r : turn paintballs red"<<endl;
  cout<<"g : turn paintballs green"<<endl;
  cout<<"b : turn paintballs blue"<<endl;
  cout<<"w : turn paintballs to random colours"<<endl;
}

//temporarily removed from program
//display shots fired on top of wall
void textDisplay()
{ 
  //set properties of text
  //GLfloat dummaterialDiff[4]={.2,.8,.4,0};
  //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dummaterialDiff);
  
  //get length of shotsFired variable
  string number= to_string((int)shotsFired);
  int numSize=number.length();
  char word[]="Shots fired: ";
  char output[numSize+14];

  //create a char list that holds contents "shots fired: 125"
  //- 125 is example value for variablenumber
  //not working for some reason the output is just "shots fired: " and the number is completely ignored
  for(int i =0;i<numSize+14;i++){
    if(i<13){
        for(int j=0;j<13;j++){
            output[i]=word[j];           
            i++;
        }

    }else{
        for(int j=0;j<numSize;j++){
            // cout<<"output: "<<output<<endl;
            output[i]=number[j];
            
            i++;
        }
    }
 }
      
  glRasterPos3f(-6, 25,0);
  //int len  = (int)strlen(output); //issue on gpu1, temporarily removed
  int len = 0;

  for (int i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, output[i]);
  }
}

//Draw the floor in the simulation
void floor(){
    //define materials
    GLfloat dummaterialDiffFloor[4]={0.2, 0.8, 0.4, 1.0};
    GLfloat dummaterialSpecFloor[4] = {0.0, 0.0, 0.0, 1.0};
    GLfloat dummaterialAmbFloor[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat dummaterialShinyFloor = 4.0;
    GLfloat materialEmit[4]={0,0,0,0};

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmit);
    //set material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dummaterialDiffFloor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dummaterialAmbFloor);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, dummaterialShinyFloor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dummaterialSpecFloor);

    //draw floor
    glBegin(GL_POLYGON);
    glNormal3f(0.0,1.0,0.0);
    for(int i=0;i<4;i++){
        glVertex3f(floorCoord[i][0],floorCoord[i][1],floorCoord[i][2]);
    }
    glEnd();
}
//Draw the wall which will be shot at
void wall() {
    GLfloat dummaterialDiffWall[4]={0.5, .2, 0.0, 0.0};
    GLfloat dummaterialSpecWall[4] = {0, 0, 0, 0};
    GLfloat dummaterialAmbWall[4] = {1.0, 1.0, 1.0, 0};
    GLfloat dummaterialShinyWall = 4.0;
    GLfloat materialEmit[4]={0,0,0,0};

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmit);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dummaterialDiffWall);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dummaterialAmbWall);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, dummaterialShinyWall);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dummaterialSpecWall);

    glBegin(GL_QUADS);
        glNormal3f(0.0,0.0,1.0);
        for(int i=0;i<4;i++){
            glVertex3f(wallCoords[i][0], wallCoords[i][1], wallCoords[i][2]);
        }
    glEnd();
}

//Draw a table which the shooter stands behind
void drawTable() {
    GLfloat dummaterialDiffTable[3]={0.9,0.6,0.3};
    GLfloat dummaterialSpecTable[4] = {0,0,0,0};
    GLfloat dummaterialAmbTable[4] = {1,1,1,0};
    GLfloat dummaterialShinyTable = 4;
    GLfloat emitTable[4]={0,0,0,0};

    GLfloat ballDiff1[3]={1,0,0};
    GLfloat ballDiff2[3]={0,1,0};
    GLfloat ballDiff3[3]={0,0,1};
    GLfloat ballEmit[4]={1,0,0,1};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dummaterialDiffTable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dummaterialAmbTable);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, dummaterialShinyTable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dummaterialSpecTable);
    glMaterialfv(GL_FRONT, GL_EMISSION, emitTable);

    glBegin(GL_POLYGON);
    glNormal3f(0,1.0,0);
        for(int i=0;i<4;i++){
            glVertex3f(tableCoords[i][0],tableCoords[i][1],tableCoords[i][2]);
        }
    glEnd();
    glPushMatrix();
        //red ball
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ballDiff1);
        glTranslatef(-2,7.25,42.5);
        if(paintBallColour[0]==1 and colourChanged){glMaterialfv(GL_FRONT, GL_EMISSION, ballEmit);}
        else{glMaterialfv(GL_FRONT, GL_EMISSION, emitTable);}
        glutSolidSphere(.25, 7, 70);
    glPopMatrix();
    glPushMatrix();
        //green ball
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ballDiff2);
        if(paintBallColour[1]==1 and colourChanged){glMaterialfv(GL_FRONT, GL_EMISSION, ballEmit);}
        else{glMaterialfv(GL_FRONT, GL_EMISSION, emitTable);}
        glTranslatef(0,7.25,42.5);
        glutSolidSphere(.25, 7, 70);
    glPopMatrix();
    glPushMatrix();
        //blue ball
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ballDiff3);
        if(paintBallColour[2]==1 and colourChanged){glMaterialfv(GL_FRONT, GL_EMISSION, ballEmit);}
        else{glMaterialfv(GL_FRONT, GL_EMISSION, emitTable);}
        glTranslatef(2,7.25,42.5);
        glutSolidSphere(.25, 7, 70);
    glPopMatrix();
}

//draw x,y,z axis, useful for perspective and testing (OPTIONAL)
void drawAxis() {
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0,0,0);
        glVertex3f(50,0,0);
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,50,0);
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,50);
    glEnd();
}

//draw aiming point
void drawCrossHair() {
    GLfloat dummaterialDiffCrossHair[4]={1,0,0,0};
    GLfloat materialEmit[4]={0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmit);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dummaterialDiffCrossHair);

    glBegin(GL_LINES);
        glVertex3f(crossHairPos[0] - 0.1, crossHairPos[1], crossHairPos[2]);
        glVertex3f(crossHairPos[0] + 0.1, crossHairPos[1], crossHairPos[2]);

        glVertex3f(crossHairPos[0], crossHairPos[1] + 0.1, crossHairPos[2]);
        glVertex3f(crossHairPos[0], crossHairPos[1] - 0.1, crossHairPos[2]);
    glEnd();

    //draw circle around cross
    int segments=100;
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < segments; i++)
    {   
        //calculate angle
        float theta = 2.0f * 3.1415926f * float(i) / segments;
        float r=.1;
        //calculate coordinates
        float x = r * cosf(theta)+crossHairPos[0];
        float y = r * sinf(theta)+crossHairPos[1];
        float z=crossHairPos[2];

        glVertex3f(x , y ,z);//output vertex
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < segments; i++)
    {
        //calculate angle
        float theta = 2.0f * 3.1415926f * float(i) / segments;
        float r=.12;
        //calculate coordinates
        float x = r * cosf(theta)+crossHairPos[0];
        float y = r * sinf(theta)+crossHairPos[1];
        float z=crossHairPos[2];

        glVertex3f(x , y ,z); //output vertex
    }
    glEnd();
}

//draw splatters on wall
void drawSplatters() {
    GLfloat dummaterialDiffSplatter[4]={0.5, 0.2, 0.0, 0.0};
    GLfloat dummaterialSpecSplatter[4]={0.0, 0.0, 0.0, 0.0};
    GLfloat dummaterialAmbSplatter[4] = {1.0, 1.0, 1.0, 0.0};
    GLfloat dummaterialShinySplatter = 4.0;
    
    for(int i = splatterVec.size()-1; i >=0 ; i--) { 
        GLfloat dummaterialDiffSplatter[4]={splatterVec[i].color[0], splatterVec[i].color[1], splatterVec[i].color[2], 0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dummaterialDiffSplatter);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dummaterialAmbSplatter);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, dummaterialShinySplatter);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dummaterialSpecSplatter);

        glBegin(GL_QUADS);
            glNormal3f(0.0,0.0,1.0);
            glVertex3f(splatterVec[i].mX - 2, splatterVec[i].mY - 2, -34.9);
            glVertex3f(splatterVec[i].mX - 2, splatterVec[i].mY + 2, -34.9);
            glVertex3f(splatterVec[i].mX + 2, splatterVec[i].mY + 2, -34.9);
            glVertex3f(splatterVec[i].mX + 2, splatterVec[i].mY - 2, -34.9);
        glEnd();
    }
}

//add to vector of splatters and remove from paintball vector, parameter is index of paintball vector for current hit
void wallInteraction(int p) {
    Splatter S(paintBallVec[p].mX, paintBallVec[p].mY, paintBallVec[p].color);   //create new 'splatter'
    splatterVec.push_back(S);
    paintBallVec.erase(paintBallVec.begin()+p); //remove paintball
}

//shooting a paintball ads it to the vector
void shootPaintBall(){
    shotsFired+=1;
    Paintball P(crossHairPos[0]*10, crossHairPos[1], crossHairPos[2]);
    if(colourChanged){
            P.color[0]=paintBallColour[0];
            P.color[1]=paintBallColour[1];
            P.color[2]=paintBallColour[2];
    }
    paintBallVec.push_back(P);
}

//increment z value of all active paintballs
void drawPaintBalls(){
    //declare materials
    GLfloat dummaterialDiffBall[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat dummaterialSpecBall[4] = {0.0, 0.0, 0.0, 1.0};
    GLfloat dummaterialAmbBall[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat dummaterialShinyBall = 2.0;
    for(int i = 0; i < paintBallVec.size(); i++){
        if(colourChanged){
            dummaterialDiffBall[0]=paintBallColour[0];
            dummaterialDiffBall[1]=paintBallColour[1];
            dummaterialDiffBall[2]=paintBallColour[2];
        }else{
            dummaterialDiffBall[0]=paintBallVec[i].color[0];
            dummaterialDiffBall[1]=paintBallVec[i].color[1];
            dummaterialDiffBall[2]=paintBallVec[i].color[2];
        }

        glPushMatrix();
            //set materials
            glMaterialfv(GL_FRONT, GL_DIFFUSE, dummaterialDiffBall);
            glMaterialfv(GL_FRONT, GL_AMBIENT, dummaterialAmbBall);
            glMaterialf(GL_FRONT, GL_SHININESS, dummaterialShinyBall);
            glMaterialfv(GL_FRONT, GL_SPECULAR, dummaterialSpecBall);
        
            paintBallVec[i].mZ = paintBallVec[i].mZ - paintBallVec[i].speed;    //"-" because shooting 'down' range
            glTranslatef(paintBallVec[i].mX, paintBallVec[i].mY, paintBallVec[i].mZ);

            if (paintBallVec[i].mZ <= -35 and paintBallVec[i].mX<15 
            and paintBallVec[i].mX>-15 and paintBallVec[i].mY<20 and paintBallVec[i].mY>0) {    //hit wall
                wallInteraction(i);
            }
         
            glutSolidSphere(1, 100, 100);
        glPopMatrix();
     }
}


//this function displays output to the window
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0],eye[1],eye[2], 0,10,0, 0,1,0);

    //if initial run of program
    //print instructions and reset particles
    if(reset == true){
        instructions();
        reset = false;
    }

    for (unsigned int j = 0; j < 2; j++) {
        glLightfv(GL_LIGHT0 +j, GL_POSITION, lightPos[j]);
        glLightfv(GL_LIGHT0 +j, GL_AMBIENT, amb);
        glLightfv(GL_LIGHT0 +j, GL_DIFFUSE, diff);
        glLightfv(GL_LIGHT0 +j, GL_SPECULAR, spec);
    }   

    if (axisToggle == true){
        drawAxis();
    }

    //textDisplay();

    floor();

    wall();

    drawTable();

    drawCrossHair();

    drawPaintBalls();

    drawSplatters();

    //switch our buffers for a smooth animation
    glutSwapBuffers();

    //force a redisplay, to keep the animation running
    glutPostRedisplay();
}

void projection(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,1,1,100);
}

//react to users keyboard input
void handleKeyboard(unsigned char key, int _x, int _y) {

    //close window
    if (key == 'q' or key == 'Q') {
        exit(0);
    }
    //set paintballs to red
    if (key == 'r' or key == 'R') {
        paintBallColour[0]=1;
        paintBallColour[1]=0;
        paintBallColour[2]=0;
        colourChanged=true;
    }
    //set paintballs to green
    if (key == 'g' or key == 'G') {
        paintBallColour[0]=0;
        paintBallColour[1]=1;
        paintBallColour[2]=0;
        colourChanged=true;
    }
    //set paintballs to blue
    if (key == 'b' or key == 'B') {
        paintBallColour[0]=0;
        paintBallColour[1]=0;
        paintBallColour[2]=1;
        colourChanged=true;
    }
    //set paintballs to blue
    if (key == 'w' or key == 'W') {
        colourChanged=false;
    }
    //shootpaintball
    if (key == ' '){
        shootPaintBall();
    }
    //toggle x, y, z axis display
    if (key == 'a' || key == 'A'){
        axisToggle = !axisToggle;
    }
}
void handleSpecialKeyboard(int key, int _x, int _y) {
    //crosshair position
    if(key==GLUT_KEY_LEFT){
        crossHairPos[0] = crossHairPos[0] - 0.05;
    }
    if(key==GLUT_KEY_RIGHT){
        crossHairPos[0] = crossHairPos[0] + 0.05;
    }
    if(key == GLUT_KEY_UP){
        crossHairPos[1] = crossHairPos[1] + 0.05;
    }
    if(key == GLUT_KEY_DOWN){
        crossHairPos[1] = crossHairPos[1] - 0.05;
    }
}

//no mouse driven interaction for now
// void OnMouseClick(int button, int state, int x, int y) {
//     //shoot paintball with mouse temporarily removed, spacebar instead
//     // if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//     //     shootPaintBall();
//     // }

//     if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
//         //empty for now
//     }

// }

int main(int argc, char** argv) {
	glutInit(&argc, argv);     //starts up GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //create window
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Paintball Range");  //creates the window
    
    //enable Z buffer test, otherwise things appear in the order they're drawn
    //enable lighting with 2 lights
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
    glEnable(GL_LIGHT1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    projection();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    //glutMouseFunc(OnMouseClick);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(handleSpecialKeyboard);
    glutDisplayFunc(display);
    // PlaySound("35 - Lost Woods.wav", NULL, SND_SYNC);

    glutMainLoop();             //starts the event glutMainLoop

    return 0;

}
