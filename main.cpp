#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctime>

#include <GL/GLUT.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <windows.h>
#include <MMsystem.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include <string>
#include <sstream>

#define RAND_MAX=22


using namespace std;


float _cameraAngle = 0.0;



//cameraProperty
//float cameraX=-5.52;
float cameraX=-5.52;



//Mario Property
float marioPositionX=4;
float marioPositionY=-2.95;
bool jumpMarioKeyPressed=false;
bool jumpTopReached=false;
bool marioDirectionRight=true;
bool moveRight, moveLeft = false;
bool keyarr[4];


//texture variables
GLuint _textureFloor;
GLuint _textureBrick;
GLuint _textureScoreBlock;
GLuint _textureMario;
GLuint _textureHill;
GLuint _textureBlock;
GLuint _textureCloud;
GLuint _textureCylinder;
GLuint _textureEnemy;
GLuint _textureCastle;


//location store
bool storeLocation = true;

//Cloud Property
float cloudPositionX=4.0;


//collision information

int bottomAreaAssignCount=0;


float  bottomCollisionArea[500][5];

//float  bottomCollisionArea[2][4]={{5.0,-0.5,5.5,-0.5},{6.0,-0.5,6.5,-0.5}};


//collision property

bool marioCollisionOccured = false;
bool jumpBottomCollisionOccured = false;
bool jumpBottomCollisionOccuredCoin = false;


//coin collision
float collisionedCoinX;
float collisionedCoinY;
bool brickCollisionStatus[100]; // true for no collision, false for collision
int brickLocation[100]; //brickLocation value comes from bottomAreaAssignCount
int locationCounter = 0; //used as brickLocation array's counter
int tempBrickCounter = 0; //a temp counter used while calling drawbrick. Will be reset each time drawScene finishs looping



//goomba animation variables
float animateLegScaleX = 0.0;
float animateLegScaleY = 0.0;
bool legAnimationCycle = true;

//jump variables
int jumpCounter = 0;
bool onTheBrick = false;

//enemy property


float enemyX=20.0;


//text property
float textX=-2.5;
int score=0;


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void enableSound(string state){


    if(state=="jump"){
        PlaySound("sounds/jump.wav", NULL, NULL | SND_ASYNC);
       }

     if(state=="collite"){
        PlaySound("sounds/collite.wav", NULL, NULL | SND_ASYNC);
    }

     if(state=="coin"){
        PlaySound("sounds/coin.wav", NULL, NULL | SND_ASYNC);
    }

      if(state=="enemycollite"){
        PlaySound("sounds/enemycollite.wav", NULL, NULL | SND_ASYNC);
    }

       if(state=="mariodie"){
        PlaySound("sounds/mariodie.wav", NULL, NULL | SND_ASYNC);
    }


}

void printText(int x, int y,int z, char *string){
//set the position of the text in the window using the x, y and z coordinates
glRasterPos3f(x,y,z);
//get the length of the string to display
    int len = (int) strlen(string);

    //loop to display character by character
    for (int i = 0; i < len; i++)
    {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
    }
};

void delectCollisionWithEnemy(){


    float positionDifference = enemyX-marioPositionX;

    if(positionDifference<-0.25 && positionDifference>-1.0){
            if(marioPositionY<-2.95){
                  enableSound("mariodie");
                  cout<<"Mario died"<<endl;
            }
            else if(marioPositionY<-2.5){
                  enemyX=-50;
                  enableSound("enemycollite");
                  cout<<"Enemy died"<<endl;
            }
    }
}

int storeObjectPosition(float x, float y, int noOfObjects, int type){
    bottomCollisionArea[bottomAreaAssignCount][0]=x;
    bottomCollisionArea[bottomAreaAssignCount][1]=y;
    bottomCollisionArea[bottomAreaAssignCount][2]=x+0.5*noOfObjects;
    bottomCollisionArea[bottomAreaAssignCount][3]=y;
    bottomCollisionArea[bottomAreaAssignCount][4]=type;

    int returnValue = bottomAreaAssignCount;
    bottomAreaAssignCount++;
    return returnValue;
}

void enableTexture(GLuint textureName){

    GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);

}

void drawScoreBrick(float x, float y, int length, bool enabled){

     float translateFloorX=0.0;

       enableTexture(_textureScoreBlock);

      time_t seconds;
      seconds = time (NULL);

      if(enabled)
      {
          if(seconds%2==0){
            glColor3f(1.0f, 1.0f, 1.0f);
          }
          else{
            glColor3f(0.0f, 1.0f, 0.0f);
          }
      }

      else
      {
          glColor3f(1.0f, 0.0f, 0.0f);
      }

      if(storeLocation){
         brickLocation[locationCounter] = storeObjectPosition(x,y,length,1);
         locationCounter++;
      }


  glPushMatrix();
  glTranslatef(x,y,0);

    for(int i=0;i<length;i++){

    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            translateFloorX+=0.5;

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

            translateFloorX-=0.5;

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

             translateFloorX+=0.5;

        glEnd();



    glPopMatrix();

     }
    glPopMatrix();

     glDisable(GL_TEXTURE_2D);
}

void fallFromTopIfNoObstacle(){


    int countCol=0;

     for(int i = 0; i < 50; i++){
        //for(int j = 0; j < 4; j++){

            if(marioPositionX>=bottomCollisionArea[i][0]+0.5 && marioPositionX<=bottomCollisionArea[i][2]+0.75){

               if(marioPositionY==-0.5 || marioPositionY>-0.5){
                    countCol++;
               }
            }


             if(marioPositionY<-0.5 || jumpMarioKeyPressed==true){
                countCol=500;
             }


            //cout<<countCol<<endl;

    }

           if(countCol==0){
               jumpTopReached=true;
               //jumpCounter = 20;
               jumpMarioKeyPressed=true;
               //marioPositionY=-2.95;
               // countCol=0;
            }


}

void detectCollision(){


 for(int i = 0; i < 50; i++){
        //for(int j = 0; j < 4; j++){

            if(marioPositionX>=bottomCollisionArea[i][0]+0.5 && marioPositionX<=bottomCollisionArea[i][2]+0.75){

                if(marioPositionY>=bottomCollisionArea[i][1]-1 && marioPositionY<bottomCollisionArea[i][1]){


                    if(bottomCollisionArea[i][4]==0){
                         marioCollisionOccured=true;
                         jumpBottomCollisionOccured=true;
                    }else{
                         marioCollisionOccured=true;
                         jumpBottomCollisionOccuredCoin=true;
                         collisionedCoinX=bottomCollisionArea[i][0];
                         collisionedCoinY=bottomCollisionArea[i][1];
                    }
                     brickCollisionStatus[i] = false;

                }


                 else if(marioPositionY>bottomCollisionArea[i][1]){
                        if(!onTheBrick)
                        {
                            jumpCounter = 0;
                            jumpMarioKeyPressed=false;
                            //jumpTopReached = false;
                        }

                             //marioPositionY=bottomCollisionArea[i][1];
                             onTheBrick = true;
                            cout<<"On the brick: "<<marioPositionY<<endl;
                    }

            }


    }

    fallFromTopIfNoObstacle();


}

void colliteMario(float x, float y){

    if(jumpBottomCollisionOccured){
       enableSound("collite");
       jumpBottomCollisionOccured=false;
    }
    else if(jumpBottomCollisionOccuredCoin){
        enableSound("coin");
        jumpBottomCollisionOccuredCoin=false;

    }


      if(marioPositionY>-2.95){
           marioPositionY-= .2f;
           //detectCollision();
       }else{
         marioCollisionOccured=false;
         jumpCounter = 0;
         jumpMarioKeyPressed=false;
       }

}

void drawCastle(){

        enableTexture(_textureCastle);
        glPushMatrix();



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(3, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(3, 3, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 3, 0);


        glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void drawEnemy(){

glPushMatrix();


	glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, 0.7, 0.0);
    glScalef(2.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //HEAD1

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, 0.6, 0.0);
    glScalef(3.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 2

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, 0.5, 0.0);
    glScalef(4.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 3

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, 0.4, 0.0);
    glScalef(5.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 4

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, 0.3, 0.0);
    glScalef(3.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 5

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.55, 0.3, 0.0);
    glScalef(0.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 5 Right

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.55, 0.3, 0.0);
    glScalef(0.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 5 Left

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.4, 0.3, 0.0);
    glScalef(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 5 Brow Left

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.4, 0.3, 0.0);
    glScalef(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 5 Brow Right

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, 0.2, 0.0);
    glScalef(2.0, 1.0, 1.0);

    glBegin(GL_POLYGON);

    glColor3ub(105, 45, 40); //Head 6

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.55, 0.2, 0.0);
    glScalef(1.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 6 Right

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.55, 0.2, 0.0);
    glScalef(1.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 6 Left

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, 0.1, 0.0);
    glScalef(2.0, 1.0, 1.0);

    glBegin(GL_POLYGON); // Head 7

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.55, 0.1, 0.0);
    glScalef(1.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 7 Right

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.55, 0.1, 0.0);
    glScalef(1.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 7 Left

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, 0.0, 0.0);
    glScalef(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON);

    glColor3ub(105, 45, 40); //Head 8

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.6, 0.0, 0.0);
    glScalef(2.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 8 right

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.6, 0.0, 0.0);
    glScalef(2.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 8 Left

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.25, 0.0, 0.0);
    glScalef(0.5, 3, 1.0);

    glBegin(GL_POLYGON); //Eye Pupil Left

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.25, 0.0, 0.0);
    glScalef(0.5, 3, 1.0);

    glBegin(GL_POLYGON); // Eye Pupil Right

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.15, 0.0, 0.0);
    glScalef(0.5, 2, 1.0);

    glBegin(GL_POLYGON); // Head 8 Eye White Left

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.15, 0.0, 0.0);
    glScalef(0.5, 2, 1.0);

    glBegin(GL_POLYGON); // Head 8 Eye white right

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, -0.1, 0.0);
    glScalef(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 9

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.6, -0.1, 0.0);
    glScalef(2.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 9 Right

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.6, -0.1, 0.0);
    glScalef(2.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //Head 9 Left

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.35, -0.1, 0.0);
    glScalef(0.5, 4, 1.0);

    glBegin(GL_POLYGON); //Head 9 Eye White vertical left

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.35, -0.1, 0.0);
    glScalef(0.5, 4, 1.0);

    glBegin(GL_POLYGON); //Head 9 Eye white vertical right

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.2, -0.1, 0.0);
    glScalef(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //head 9 eye white horizontal left

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.2, -0.1, 0.0);
    glScalef(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //head 9 eye white horizontal right

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, -0.2, 0.0);
    glScalef(8.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //head 10

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, -0.3, 0.0);
    glScalef(3.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //body 1

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix();

    glTranslatef(-0.5, -0.3, 0.0);
    glScalef(2, 1.0, 1.0);

    glBegin(GL_POLYGON); //head 11 left

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.5, -0.3, 0.0);
    glScalef(2, 1.0, 1.0);

    glBegin(GL_POLYGON); //head 11 right

    glColor3ub(105, 45, 40);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix();

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, -0.4, 0.0);
    glScalef(4.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //body 2

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, -0.5, 0.0);
    glScalef(4.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //body 3

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, -0.6, 0.0);
    glScalef(3.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //body 4

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.0, -0.7, 0.0);
    glScalef(1.5, 1.0, 1.0);

    glBegin(GL_POLYGON); //body 5

    glColor3ub(215, 185, 120);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //right leg animation matrix begin

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.5, -0.5, 0.0);
    glScalef(1.0, 1.0, 1.0);
    glTranslatef(animateLegScaleX, animateLegScaleY, 0.0);

    glBegin(GL_POLYGON); //leg 1 right

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.45, -0.6, 0.0);
    glScalef(2.5, 1.0, 1.0);
    glTranslatef(animateLegScaleX, animateLegScaleY, 0.0);

    glBegin(GL_POLYGON); //leg 2 right

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.4, -0.7, 0.0);
    glScalef(3, 1.0, 1.0);
    glTranslatef(animateLegScaleX, animateLegScaleY, 0.0);

    glBegin(GL_POLYGON); //leg 3 right

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(0.35, -0.8, 0.0);
    glScalef(2.5, 1.0, 1.0);
    glTranslatef(animateLegScaleX, animateLegScaleY, 0.0);

    glBegin(GL_POLYGON); //leg 4 right

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPopMatrix(); //Right Leg matrix end

    glPushMatrix(); //Left leg animation matrix begin

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.5, -0.5, 0.0);
    glScalef(1, 1.0, 1.0);
    glTranslatef(animateLegScaleX, -animateLegScaleY, 0.0);

    glBegin(GL_POLYGON); //leg 1 left

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.45, -0.6, 0.0);
    glScalef(2.5, 1.0, 1.0);
    glTranslatef(animateLegScaleX, -animateLegScaleY, 0.0);

    glBegin(GL_POLYGON); //leg 2 left

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix();

    glTranslatef(-0.4, -0.7, 0.0);
    glScalef(3, 1.0, 1.0);
    glTranslatef(animateLegScaleX, -animateLegScaleY, 0.0);

    glBegin(GL_POLYGON); //leg 3 left

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPushMatrix(); //Save the transformations performed thus far

    glTranslatef(-0.35, -0.8, 0.0);
    glScalef(2.5, 1.0, 1.0);
    glTranslatef(animateLegScaleX, -animateLegScaleY, 0.0);

    glBegin(GL_POLYGON); //leg 4 left

    glColor3ub(0, 0, 0);

    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);

    glEnd();

    glPopMatrix(); //Undo the move to the center of the triangle

    glPopMatrix(); // Left leg animation matrix end

    glPopMatrix();
}

void drawCylinder(){

        enableTexture(_textureCylinder);

        glPushMatrix();



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1, 2.0, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 2, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void drawCloud(){

        enableTexture(_textureCloud);

        glPushMatrix();



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(3, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(3.0, 1.0, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 1, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void drawCloud(int length){

    float translateCloudX=0.0f;

        enableTexture(_textureCloud);

        for(int i=0;i<length;i++){

            glPushMatrix();

            glTranslatef(translateCloudX, 0, 0);

            glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(0, 0, 0);



                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(3, 0, 0);

                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(3.0, 1.0, 0);



                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(0, 1, 0);


            glEnd();
        glPopMatrix();

        translateCloudX += 10.0f;
        }


        glDisable(GL_TEXTURE_2D);
}

void drawBlock(int length){

float translateFloorX=0.0;


    enableTexture(_textureBrick);



    for(int i=0;i<length;i++){


    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            translateFloorX+=0.5;

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

            translateFloorX-=0.5;

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

             translateFloorX+=0.5;

        glEnd();



    glPopMatrix();

     }

     glDisable(GL_TEXTURE_2D);

}

void drawObstacleBlockByMyDefinedFunc(float xx, float yy, int length){

    glPushMatrix();


    glPushMatrix();

              float x=2.0;
              float y=-1.0;

              for(int i=length;i>0;i--){





                    glPushMatrix();
                        glTranslatef(x, y, 0);
                        drawBlock(i);
                    glPopMatrix();

                     x+=0.5;
//                     y+=0.55;
                     y+=0.5;
              }

    glPopMatrix();

    glPushMatrix();

      glTranslatef(length*2,0.0,0.0);
      glRotatef(180,0.0,1.0,0.0);

              x=2.0;
              y=-1.0;

              for(int i=length;i>0;i--){




                    glPushMatrix();
                        glTranslatef(x, y, 0);
                        drawBlock(i);
                    glPopMatrix();

                     x+=0.5;
//                     y+=0.55;
                     y+=0.5;
              }

    glPopMatrix();



    glPopMatrix();


}

void drawHill(){


    enableTexture(_textureHill);

    glPushMatrix();



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(3, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(3.0, 1.0, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 1, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void moveMario(){
    if(moveRight == true && marioPositionX<80 && marioPositionX>2){
         marioPositionX +=0.1f;
         cameraX -=0.1f;
    }
    else if(moveLeft == true){
            if(marioPositionX>4){
                marioPositionX -=0.1f;
                cameraX +=0.1f;
            }
    }

}

void drawFloor(int length){

    float translateFloorX=0.0;


    enableTexture(_textureFloor);


    for(int i=0;i<length;i++){

    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(translateFloorX++, 0, 0);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(translateFloorX--, 0.5, 0);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(translateFloorX++, 0.5, 0);
        glEnd();



    glPopMatrix();

     }

     glDisable(GL_TEXTURE_2D);
}

void drawOldBrick(int length){


    float translateFloorX=0.0;

    enableTexture(_textureBrick);


    for(int i=0;i<length;i++){

    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            translateFloorX+=0.5;

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

            translateFloorX-=0.5;

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

             translateFloorX+=0.5;

        glEnd();



    glPopMatrix();


     }

     glDisable(GL_TEXTURE_2D);
}

void drawBrick(float x, float y, int length){

      if(storeLocation){
         brickLocation[locationCounter] = storeObjectPosition(x,y,length,0);
         locationCounter++;
      }


    float translateFloorX=0.0;

    enableTexture(_textureBrick);

  glPushMatrix();
  glTranslatef(x,y,0);

    for(int i=0;i<length;i++){

    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            translateFloorX+=0.5;

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(translateFloorX, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

            translateFloorX-=0.5;

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(translateFloorX, 0.5, 0);

             translateFloorX+=0.5;

        glEnd();

    glPopMatrix();
}
     glPopMatrix();


     glDisable(GL_TEXTURE_2D);
}

void drawMario(){


    enableTexture(_textureMario);


    glPushMatrix();


        if(marioDirectionRight==false){
            glTranslatef(marioPositionX-0.5, marioPositionY, 0);
            glRotatef(180,0.0,1.0,0.0);
        }else{
             glTranslatef(marioPositionX-1.0, marioPositionY, 0);
            glRotatef(0,0.0,0.0,0.0);
        }



        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, 0, 0);



            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.5, 0, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.5, 0.5, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 0.5, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void drawMarioNew(){


    glPushMatrix();


        if(marioDirectionRight==false){
            glTranslatef(marioPositionX-0.5, marioPositionY, 0);
            glRotatef(180,0.0,1.0,0.0);
        }else{
             glTranslatef(marioPositionX-1.0, marioPositionY, 0);
            glRotatef(0,0.0,0.0,0.0);
        }



        glBegin(GL_POLYGON);

        glBegin(GL_POLYGON);
glColor3f(0.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.41, 0.90, 0.0);
glVertex3f(0.41, 0.93, 0.0);
glVertex3f(0.26, 0.93, 0.0);
glVertex3f(0.26, 0.90, 0.0);
glVertex3f(0.23, 0.90, 0.0);
glVertex3f(0.23, 0.87, 0.0);
glVertex3f(0.50, 0.87, 0.0);
glVertex3f(0.50, 0.90, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.23, 0.87, 0.0);
glVertex3f(0.32, 0.87, 0.0);
glVertex3f(0.32, 0.84, 0.0);
glVertex3f(0.23, 0.84, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.29, 0.81, 0.0);
glVertex3f(0.29, 0.84, 0.0);
glVertex3f(0.26, 0.84, 0.0);
glVertex3f(0.26, 0.78, 0.0);
glVertex3f(0.32, 0.78, 0.0);
glVertex3f(0.32, 0.81, 0.0);



glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.32, 0.84, 0.0);
glVertex3f(0.29, 0.84, 0.0);
glVertex3f(0.29, 0.81, 0.0);
glVertex3f(0.32, 0.81, 0.0);
glVertex3f(0.32, 0.72, 0.0);
glVertex3f(0.38, 0.72, 0.0);
glVertex3f(0.38, 0.87, 0.0);
glVertex3f(0.32, 0.87, 0.0);




glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.26, 0.84, 0.0);
glVertex3f(0.23, 0.84, 0.0);
glVertex3f(0.23, 0.78, 0.0);
glVertex3f(0.26, 0.78, 0.0);



glEnd();


glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.23, 0.84, 0.0);
glVertex3f(0.20, 0.84, 0.0);
glVertex3f(0.20, 0.78, 0.0);
glVertex3f(0.23, 0.78, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.23, 0.78, 0.0);
glVertex3f(0.26, 0.78, 0.0);
glVertex3f(0.26, 0.75, 0.0);
glVertex3f(0.23, 0.75, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.26, 0.78, 0.0);
glVertex3f(0.32, 0.78, 0.0);
glVertex3f(0.32, 0.72, 0.0);
glVertex3f(0.26, 0.72, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 0.0, 0.0);//(R,G,B)

glVertex3f(0.41, 0.81, 0.0);
glVertex3f(0.41, 0.87, 0.0);
glVertex3f(0.38, 0.87, 0.0);
glVertex3f(0.38, 0.81, 0.0);
glVertex3f(0.44, 0.81, 0.0);
glVertex3f(0.44, 0.78, 0.0);
glVertex3f(0.41, 0.78, 0.0);



glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.41, 0.81, 0.0);
glVertex3f(0.38, 0.81, 0.0);
glVertex3f(0.38, 0.78, 0.0);
glVertex3f(0.41, 0.78, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.44, 0.84, 0.0);
glVertex3f(0.44, 0.87, 0.0);
glVertex3f(0.41, 0.87, 0.0);
glVertex3f(0.41, 0.81, 0.0);
glVertex3f(0.44, 0.81, 0.0);
glVertex3f(0.44, 0.78, 0.0);
glVertex3f(0.50, 0.78, 0.0);
glVertex3f(0.50, 0.84, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.50, 0.81, 0.0);
glVertex3f(0.50, 0.78, 0.0);
glVertex3f(0.53, 0.78, 0.0);
glVertex3f(0.53, 0.81, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 0.0, 0.0);//(R,G,B)

glVertex3f(0.38, 0.75, 0.0);
glVertex3f(0.50, 0.75, 0.0);
glVertex3f(0.50, 0.78, 0.0);
glVertex3f(0.38, 0.78, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.38, 0.75, 0.0);
glVertex3f(0.38, 0.72, 0.0);
glVertex3f(0.44, 0.72, 0.0);
glVertex3f(0.44, 0.75, 0.0);



glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.26, 0.63, 0.0);
glVertex3f(0.29, 0.63, 0.0);
glVertex3f(0.29, 0.72, 0.0);
glVertex3f(0.23, 0.72, 0.0);
glVertex3f(0.23, 0.69, 0.0);
glVertex3f(0.20, 0.69, 0.0);
glVertex3f(0.20, 0.66, 0.0);
glVertex3f(0.17, 0.66, 0.0);
glVertex3f(0.17, 0.63, 0.0);
glVertex3f(0.23, 0.63, 0.0);
glVertex3f(0.23, 0.60, 0.0);
glVertex3f(0.26, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 0.0, 1.0);//(R,G,B)

glVertex3f(0.32, 0.63, 0.0);
glVertex3f(0.32, 0.72, 0.0);
glVertex3f(0.29, 0.72, 0.0);
glVertex3f(0.29, 0.63, 0.0);
glVertex3f(0.32, 0.63, 0.0);
glVertex3f(0.32, 0.54, 0.0);
glVertex3f(0.38, 0.54, 0.0);
glVertex3f(0.38, 0.66, 0.0);
glVertex3f(0.32, 0.66, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 0.0, 1.0);//(R,G,B)

glVertex3f(0.38, 0.63, 0.0);
glVertex3f(0.41, 0.63, 0.0);
glVertex3f(0.41, 0.72, 0.0);
glVertex3f(0.38, 0.72, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.32, 0.66, 0.0);
glVertex3f(0.32, 0.72, 0.0);
glVertex3f(0.38, 0.72, 0.0);
glVertex3f(0.38, 0.66, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.44, 0.63, 0.0);
glVertex3f(0.41, 0.63, 0.0);
glVertex3f(0.41, 0.72, 0.0);
glVertex3f(0.47, 0.72, 0.0);
glVertex3f(0.47, 0.69, 0.0);
glVertex3f(0.50, 0.69, 0.0);
glVertex3f(0.50, 0.66, 0.0);
glVertex3f(0.53, 0.66, 0.0);
glVertex3f(0.53, 0.63, 0.0);
glVertex3f(0.47, 0.63, 0.0);
glVertex3f(0.47, 0.60, 0.0);
glVertex3f(0.44, 0.60, 0.0);


glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.41, 0.63, 0.0);
glVertex3f(0.38, 0.63, 0.0);
glVertex3f(0.38, 0.60, 0.0);
glVertex3f(0.41, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.32, 0.63, 0.0);
glVertex3f(0.29, 0.63, 0.0);
glVertex3f(0.29, 0.60, 0.0);
glVertex3f(0.32, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 0.0, 1.0);//(R,G,B)

glVertex3f(0.44, 0.57, 0.0);
glVertex3f(0.44, 0.63, 0.0);
glVertex3f(0.41, 0.63, 0.0);
glVertex3f(0.41, 0.60, 0.0);
glVertex3f(0.38, 0.60, 0.0);
glVertex3f(0.38, 0.51, 0.0);
glVertex3f(0.47, 0.51, 0.0);
glVertex3f(0.47, 0.57, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.0, 0.0, 1.0);//(R,G,B)

glVertex3f(0.26, 0.57, 0.0);
glVertex3f(0.26, 0.63, 0.0);
glVertex3f(0.29, 0.63, 0.0);
glVertex3f(0.29, 0.60, 0.0);
glVertex3f(0.32, 0.60, 0.0);
glVertex3f(0.32, 0.51, 0.0);
glVertex3f(0.23, 0.51, 0.0);
glVertex3f(0.23, 0.57, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.23, 0.60, 0.0);
glVertex3f(0.23, 0.63, 0.0);
glVertex3f(0.17, 0.63, 0.0);
glVertex3f(0.17, 0.54, 0.0);
glVertex3f(0.23, 0.54, 0.0);
glVertex3f(0.23, 0.57, 0.0);
glVertex3f(0.26, 0.57, 0.0);
glVertex3f(0.26, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(1.0, 1.0, 0.0);//(R,G,B)

glVertex3f(0.47, 0.60, 0.0);
glVertex3f(0.47, 0.63, 0.0);
glVertex3f(0.53, 0.63, 0.0);
glVertex3f(0.53, 0.54, 0.0);
glVertex3f(0.47, 0.54, 0.0);
glVertex3f(0.47, 0.57, 0.0);
glVertex3f(0.44, 0.57, 0.0);
glVertex3f(0.44, 0.60, 0.0);

glEnd();

glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.20, 0.48, 0.0);
glVertex3f(0.20, 0.51, 0.0);
glVertex3f(0.29, 0.51, 0.0);
glVertex3f(0.29, 0.45, 0.0);
glVertex3f(0.17, 0.45, 0.0);
glVertex3f(0.17, 0.48, 0.0);


glEnd();


glBegin(GL_POLYGON);
glColor3f(0.33, 0.0, 0.0);//(R,G,B)

glVertex3f(0.50, 0.48, 0.0);
glVertex3f(0.50, 0.51, 0.0);
glVertex3f(0.41, 0.51, 0.0);
glVertex3f(0.41, 0.45, 0.0);
glVertex3f(0.53, 0.45, 0.0);
glVertex3f(0.53, 0.48, 0.0);

        glEnd();


    glPopMatrix();
}

//void jumpMario(){
//
//
//    if(jumpMarioKeyPressed){
//        if(jumpTopReached==false){
//            if(marioPositionY<0.6){
//                marioPositionY += .2f;
//            }
//            else{
//                jumpTopReached = true;
//            }
//        }
//        else if(jumpTopReached==true){
//
//            if(marioPositionY>-2.95){
//                marioPositionY-= .2f;
//            }
//            else{
//                jumpMarioKeyPressed = false;
//                jumpTopReached=false;
//            }
//        }
//     }
//
//
//}

void jumpMario(){

    if(jumpMarioKeyPressed)
    {
        if(!jumpTopReached)
        {
            marioPositionY += 0.2f;
            jumpCounter++;
            if(jumpCounter>=15)
            {
                jumpTopReached = true;
            }
        }
        else if (jumpTopReached)
        {
            marioPositionY -= 0.2f;
            jumpCounter--;


            if(jumpCounter<=0)
            {
                jumpTopReached = false;
                jumpMarioKeyPressed = false;
            }

        }

    }


}

void handleKeypress(unsigned char key, int x, int y) {

if(key=='w' || key==' '){
       enableSound("jump");
       jumpMarioKeyPressed=true;
}
if(key=='d'){
       marioDirectionRight=true;
       moveRight = true;
}
 if(key=='a'){
        marioDirectionRight=false;
        moveLeft = true;

 }

glutPostRedisplay();
}

void handleKeypressUp(unsigned char key, int x, int y) {

if(key=='w' || key==' '){
      // enableSound("jump");
}
if(key=='d'){
       marioDirectionRight=true;
       moveRight = false;
}
 if(key=='a'){
        marioDirectionRight=false;
        moveLeft = false;
 }



glutPostRedisplay();

}

void initValues(){
    for(int i=0; i<100; i++)
        {brickCollisionStatus[i] = true;};
}

//Initializes 3D rendering
void initRendering() {

    glClearColor(0.48,0.47,1.0,1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);





    //texture
//    glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_NORMALIZE);
//	glEnable(GL_COLOR_MATERIAL);

	Image* image = loadBMP("images/baseWall.bmp");
	_textureFloor = loadTexture(image);
	delete image;


	Image* image2 = loadBMP("images/question.bmp");
	_textureScoreBlock = loadTexture(image2);
	delete image2;

	Image* image3 = loadBMP("images/brick.bmp");
	_textureBrick = loadTexture(image3);
	delete image3;


	Image* image4 = loadBMP("images/mario.bmp");
	_textureMario = loadTexture(image4);
	delete image4;

	Image* image5 = loadBMP("images/hill.bmp");
	_textureHill = loadTexture(image5);
	delete image5;


	Image* image6 = loadBMP("images/block.bmp");
	_textureBlock = loadTexture(image6);
	delete image6;


	Image* image7 = loadBMP("images/cloud.bmp");
	_textureCloud = loadTexture(image7);
	delete image7;


	Image* image8 = loadBMP("images/cylinder.bmp");
	_textureCylinder = loadTexture(image8);
	delete image8;

	Image* image9 = loadBMP("images/enemy.bmp");
	_textureEnemy = loadTexture(image9);
	delete image9;



	Image* image10 = loadBMP("images/castle.bmp");
	_textureCastle = loadTexture(image10);
	delete image10;



}

//Called when the window is resized
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

//Draws the 3D scene
void drawScene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glLoadIdentity(); //Reset the drawing perspective
    glRotatef(-_cameraAngle, 0.0, 1.0, 0.0); //Rotate the camera

   // glTranslatef(0, 0.0, -7.0); //Move forward 5 units

    glTranslatef(cameraX, 0.0, -7.0); //Move forward 5 units





    //draw score
  glPushMatrix();

    printText(textX+marioPositionX,1.5,0,"Score: ");

  glPopMatrix();





    glPushMatrix();
        drawMarioNew();
    glPopMatrix();

//draw floor
    glPushMatrix();
    glTranslatef(0, -3, 0);
        drawFloor(38);
    glPopMatrix();





//firsst power
    glPushMatrix();
//    glTranslatef(5, -0.5, 0);
//        drawBrick(1);
    drawBrick(5,-0.5,1);
    glPopMatrix();
    tempBrickCounter++;


    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(5.5,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(5.5,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;



    glPushMatrix();
//    glTranslatef(6, -0.5, 0);
//        drawBrick(1);
  drawBrick(6,-0.5,1);
    glPopMatrix();
    tempBrickCounter++;



//second power
    glPushMatrix();
    //    glTranslatef(11, -0.5, 0);
    //        drawBrick(2);
    drawBrick(11,-0.5,2);
    glPopMatrix();
    tempBrickCounter++;

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
    //glTranslatef(12, -0.5, 0);
        drawScoreBrick(12,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
    //glTranslatef(12, -0.5, 0);
        drawScoreBrick(12,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;



      glPushMatrix();
//    glTranslatef(12.5, -0.5, 0);
//        drawBrick(2);

    drawBrick(12.5,-0.5,2);
    glPopMatrix();
    tempBrickCounter++;

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        // glTranslatef(13, 0.5, 0);
         drawScoreBrick(13,1,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        // glTranslatef(13, 0.5, 0);
         drawScoreBrick(13,1,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;



   glPushMatrix();
     glTranslatef(19, -2.5, 0);
     drawHill();
    glPopMatrix();



    //block draw

//    glPushMatrix();
//        glTranslatef(25.5, -1.5, 0);
//        drawObstacleBlockByMyDefinedFunc(25.5,-1.5,5);
//    glPopMatrix();




    //draw floor
    glPushMatrix();
    glTranslatef(31, -3, 0);
        drawFloor(35);
    glPopMatrix();


    //draw cloud
    glPushMatrix();
        glTranslatef(cloudPositionX, 2, 0);
        drawCloud(100);
    glPopMatrix();

        //draw Cylinder
    glPushMatrix();
        glTranslatef(40, -3, 0);
        drawCylinder();
    glPopMatrix();



    //draw score brick group

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
            //glTranslatef(0, -0.9, 0);
            drawScoreBrick(46,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
            //glTranslatef(0, -0.9, 0);
            drawScoreBrick(46,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
          //glTranslatef(1.5, -0.9, 0);
            drawScoreBrick(47.5,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
          //glTranslatef(1.5, -0.9, 0);
            drawScoreBrick(47.5,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
            //glTranslatef(3, -0.9, 0);
            drawScoreBrick(49,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
            //glTranslatef(3, -0.9, 0);
            drawScoreBrick(49,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
            //glTranslatef(1.5, 0.8, 0);
            drawScoreBrick(47.5,0.8,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
            //glTranslatef(1.5, 0.8, 0);
            drawScoreBrick(47.5,0.8,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;

    //glPopMatrix();




    //drawenemy

        glPushMatrix();
                    glTranslatef(enemyX, -2.3, 0);
                    glScalef(0.25, 0.25, 1.0);
                    drawEnemy();
        glPopMatrix();




    //drawCastle
       glPushMatrix();
                    glTranslatef(75, -2.5, 0);
                    drawCastle();
       glPopMatrix();





       //drawSlide
       glPushMatrix();
                    drawBrick(58,-0.5,3);
       glPopMatrix();

       glPushMatrix();
                    drawBrick(62,-0.3,3);
       glPopMatrix();





   storeLocation = false;
   tempBrickCounter = 0;
   glutSwapBuffers();
}

void update(int value) {


    moveMario();


    if(marioCollisionOccured==true){
         colliteMario(0,0);
    }else{
         jumpMario();
    }

    if(marioPositionY < -2)
    {
        onTheBrick = false;
    }


    detectCollision();

    delectCollisionWithEnemy();

    //cloud animation begin
    cloudPositionX -= 0.02f;


    if(cloudPositionX < -100.0f)
    {
        cloudPositionX = 4.0f;
    }
    //cloud animation end

    //goomba animation begin
	if (legAnimationCycle == true)
    {
        animateLegScaleX -= 0.01f;
        animateLegScaleY -= 0.01f;
        if (animateLegScaleX <= -0.05f)
        {
            legAnimationCycle = false;
        }
    }
    else if (legAnimationCycle == false)
    {
        animateLegScaleX += 0.01f;
        animateLegScaleY += 0.01f;
        if (animateLegScaleX >= 0.05f)
        {
            legAnimationCycle = true;
        }
    }
    //goomba animation end

     enemyX-=0.02;
//     textX+=marioPositionX;

    glutPostRedisplay(); //Tell GLUT that the display has changed

    //Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(200,100);

    //Create the window
    glutCreateWindow("Super Mario");
    initValues();
    initRendering();


    //PlaySound("sounds/background.wav", NULL, SND_LOOP | SND_ASYNC);

    //Set handler functions
    glutDisplayFunc(drawScene);

    glutReshapeFunc(handleResize);

    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeypressUp);

    glutTimerFunc(25, update, 0); //Add a timer

    glutMainLoop();

}








