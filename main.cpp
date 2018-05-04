#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <unistd.h>


#include <GL/GLUT.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <dos.h>

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


//game logic variables
bool isPaused = false;

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
int currentCollisionBrick;
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

//piranha plant animation variables
float piranhaPlantY = -1.0;
bool movePlantUp = true;
int piranhaWaitCounter = 40;

//jump variables
int jumpCounter = 0;
bool onTheBrick = false;


//enemy property
float emeneyPositionx[10]={10,15,20};


//Fire Property
bool fireBulletAbility = false;
bool fireBullet = false;
float bulletX=0;
float bulletY=-0.8;
bool bulletTouchedGround = false;


//Fireflower Property
bool enableFlowerCollision = false;
bool flowerUsed = false;
bool showFlower = false;
float flowerPositionY = -0.5; //initial Y position of the brick which the flower is in


//text property
int score=0;


//slideBrickProperty
float slideBrickX[3][2]={{59,0},{66,1}};
bool valueChanageForSlide=true;



//marioLife
int marioLife=3;


//Entered inside Cylinder
bool insideTheCylinder=false;
bool inCylinderArea=false;


//LevelProperty
int mariCrossedFlag=0;
int levelComplete=0;

//Flag Property
float flagY=0;


//GameMenu
bool gamePauseMenu = false;


//function declaration
void jumpMario();
void collisionEvents(int brickNumber);


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
        mciSendString("close background", 0, 0, 0);
        }

        if(state=="fire"){
        PlaySound("sounds/fire.wav", NULL, NULL | SND_ASYNC);

        }
        if(state=="powerupappears")
        {
        PlaySound("sounds/powerupappears.wav", NULL, NULL | SND_ASYNC);
        }

        if(state=="powerup")
        {
        PlaySound("sounds/powerup.wav", NULL, NULL | SND_ASYNC);
        }

        if(state=="pause")
        {
        PlaySound("sounds/pause.wav", NULL, NULL | SND_ASYNC);
        }

        if(state=="background")
        {
        PlaySound("sounds/background.wav", NULL, NULL | SND_ASYNC);
        }

        if(state=="pipetravel")
        {
        PlaySound("sounds/pipetravel.wav", NULL, NULL | SND_ASYNC);
        }
        if(state=="flagcrossed")
        {
        PlaySound("sounds/flagcrossed.wav", NULL, NULL | SND_ASYNC);
        }
        if(state=="levelclear")
        {
        PlaySound("sounds/levelclear.wav", NULL, NULL | SND_ASYNC);
        mciSendString("close background", 0, 0, 0);
        }




}

void drawMenu(){
         glPushMatrix();

            glColor4f(1.0, 1.0, 1.0,0.40);
            glEnable (GL_BLEND);
            glBlendFunc (GL_DST_COLOR,GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_POLYGON);
                glVertex3f(0,-3,0);
                glVertex3f(10,-3,0);
                glVertex3f(10,3,0);
                glVertex3f(0,3,0);
            glEnd();

    glPopMatrix();


}

void slideBrick(){




    if(slideBrickX[0][1]==0){
        slideBrickX[0][0]+=0.02f;
        if(slideBrickX[0][0]>63){
            slideBrickX[0][1]=1;
        }
    }else{
        slideBrickX[0][0]-=0.02f;
         if(slideBrickX[0][0]<59){
            slideBrickX[0][1]=0;
        }
    }


    if(valueChanageForSlide==true){
        slideBrickX[1][0]+=4;
        }


    if(slideBrickX[1][1]==0){
        slideBrickX[1][0]+=0.02f;
        if(slideBrickX[1][0]>70){
            slideBrickX[1][1]=1;
        }
    }

    else if(slideBrickX[1][1]==1){
        slideBrickX[1][0]-=0.02f;
         if(slideBrickX[1][0]<66){
            slideBrickX[1][1]=0;
        }
    }





    if(marioPositionX>slideBrickX[0][0] && marioPositionX<slideBrickX[0][0]+4.5 && onTheBrick==true && slideBrickX[0][1]==0){
        marioPositionX+=0.02f;
        cameraX-=0.02f;
    }
    else if(marioPositionX>slideBrickX[0][0] && marioPositionX<slideBrickX[0][0]+4.5 && onTheBrick==true && slideBrickX[0][1]==1){
        marioPositionX-=0.02f;
        cameraX+=0.02f;
//        cout<<"it will lessen"<<endl;
    }


        if(marioPositionX>slideBrickX[1][0] && marioPositionX<slideBrickX[1][0]+4.5 && onTheBrick==true && slideBrickX[1][1]==0){
        marioPositionX+=0.02f;
        cameraX-=0.02f;
    }
    else if(marioPositionX>slideBrickX[1][0] && marioPositionX<slideBrickX[1][0]+4.5 && onTheBrick==true && slideBrickX[1][1]==1){
        marioPositionX-=0.02f;
        cameraX+=0.02f;
    }








    if(marioPositionX>60 && marioPositionX<73.3 && marioPositionY<-2.45){
          enableSound("mariodie");
          marioPositionY=-500;
          Sleep(3000);
          marioPositionX=57;
          marioPositionY=-2.95;
          cameraX=-57;
    }




    valueChanageForSlide=false;
}

void generateRandomEnemy(){

 srand (time(NULL));
 int i=0;

 while(i<3){
     int  random = rand() % 30 + 10;
     emeneyPositionx[i]=random;
     i++;
 }


}

void drawFire(){

        glPushMatrix();

 glBegin(GL_POLYGON);
        glColor3f(1,0,0);
                glVertex3f(2,-1,0);
                glVertex3f(2.15,-1,0);
                glVertex3f(2.15,-1.15,0);
                glVertex3f(2,-1.15,0);
  glEnd();

    glPopMatrix();

}

void bulletCollisionWithEnemy(){


int arrayLength = sizeof(emeneyPositionx) / sizeof(int);

for(int i=0; i<arrayLength;i++){

     float positionDifference = emeneyPositionx[i]-bulletX;
     float enemyMarioDifference = emeneyPositionx[i]-marioPositionX;

        if(positionDifference<-0.25 && positionDifference>-1.0 && fireBullet==true){
                   if(enemyMarioDifference<10 && enemyMarioDifference>-10){
                          emeneyPositionx[i]=-50;
                          enableSound("enemycollite");
                          score += 100;
                          cout << "Score: " << score << endl;
                          cout<<"Enemy died by Bullet"<<endl;
                          fireBullet=false;
                    }
            }

}

}

void delectCollisionWithEnemy(){


int arrayLength = sizeof(emeneyPositionx) / sizeof(int);

for(int i=0; i<arrayLength;i++){

    float positionDifference = emeneyPositionx[i]-marioPositionX;

    if(positionDifference<-0.25 && positionDifference>-1.0){
            if(marioPositionY<-2.95){
                  enableSound("mariodie");
                  marioLife--;
                  cout<<"Mario died Life: "<<marioLife<<endl;
                  marioPositionY=-500;
                  Sleep(3000);
                  marioPositionX+=2;
                  cameraX-=2;
                  marioPositionY=-2.95;


            }
            else if(marioPositionY<-2.5){
                  emeneyPositionx[i]=-50;
                  enableSound("enemycollite");
                  score += 100;
                  cout << "Score: " << score << endl;
                  cout<<"Enemy died"<<endl;
            }
    }
}

}

void detectCollisionWithFireflower(){
    if(enableFlowerCollision)
    {
        float positionDifference = 25.3-marioPositionX;

        if((positionDifference<-0.25 && positionDifference >- 1.0) && (marioPositionY >= -0.95 && marioPositionY <= 0.25))
        {
            enableSound("powerup");
            score += 1000;
            cout << "Score: " << score << endl;
            flowerUsed = true;
        }
    }
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


         brickLocation[locationCounter] = storeObjectPosition(x,y,length,1);
         locationCounter++;



  glPushMatrix();
  glTranslatef(x,y+0.1,0);

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
               jumpCounter = 15;
               jumpMarioKeyPressed=true;
               //marioPositionY=-2.95;
               // countCol=0;
            }
            if(countCol==500){
                onTheBrick = false;
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
                     //currentCollisionBrick = i;
                     cout << "Collision of brick " << i << endl;

                     collisionEvents(i);
                     //brickCollisionStatus[i] = false;

                }


//                 else if(marioPositionY>=bottomCollisionArea[i][1]){
//                        //cout << "marioPositionY: " << marioPositionY << " bottomCollisionArea: " << bottomCollisionArea[i][1] << endl;
//                        if((marioPositionY<=-0.15 && marioPositionY >=-0.35))
//                        {
//                            jumpCounter = 0;
//                            cout << "Line 365 in detect collision else if jumpcounter 0" << endl;
//                            jumpMarioKeyPressed=false;
//                            jumpTopReached = false;
//                        }
//
//                             //marioPositionY=bottomCollisionArea[i][1];
//
//                             onTheBrick = true;
//                            //cout<<"On the brick: "<<marioPositionY<<endl;
//                    }
                    else if((marioPositionY<=-0.15 && marioPositionY >=-0.35) /*||(marioPositionY<=1.25 && marioPositionY >=1.05)*/){
                        //cout << "marioPositionY: " << marioPositionY << " bottomCollisionArea: " << bottomCollisionArea[i][1] << endl;

                            jumpCounter = 0;

                            jumpMarioKeyPressed=false;

                            jumpTopReached = false;



                             //marioPositionY=bottomCollisionArea[i][1];

                             onTheBrick = true;
                            //cout<<"On the brick: "<<marioPositionY<<endl;

                    }



                    //cout << "Mario Position Y: " << marioPositionY << endl;


            }


    }

    fallFromTopIfNoObstacle();


}

void colliteMario(float x, float y){

      if(marioPositionY>-2.95 && !onTheBrick){
           jumpTopReached = true;
           jumpMarioKeyPressed = true;

       }else{
         marioCollisionOccured=false;
         jumpCounter = 0;
         jumpMarioKeyPressed=false;
       }

}

void drawCastle(){
   glPushMatrix();
    glTranslatef(0, 1.49, 0);
    glScalef(0.2, 0.25, 1.0);
	//Draw Lower Castle door
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(0, 0, 0);

	glVertex3f(-2, -6, 0);
	glVertex3f(2, -6, 0);
	glVertex3f(2, -2, 0);
	glVertex3f(-2, -2, 0);

	glEnd();

	glPopMatrix();

    //Draw Lower Castle base
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(128, 64, 0);

	glVertex3f(-10, -6, 0);
	glVertex3f(10, -6, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(-10, 0, 0);

	glEnd();

	glPopMatrix();

    //Draw Lower Castle left block1
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(-10, 0, 0);
	glVertex3f(-9, 0, 0);
	glVertex3f(-9, 1, 0);
	glVertex3f(-10, 1, 0);

	glEnd();

	glPopMatrix();

    //Draw Lower Castle left block2
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(-7, 0, 0);
	glVertex3f(-5, 0, 0);
	glVertex3f(-5, 1, 0);
	glVertex3f(-7, 1, 0);

	glEnd();

	glPopMatrix();

    //Draw Lower Castle left block3
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(-3, 0, 0);
	glVertex3f(-1, 0, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(-3, 1, 0);

	glEnd();

	glPopMatrix();


    //Draw Lower Castle right block3
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(1, 0, 0);
	glVertex3f(3, 0, 0);
	glVertex3f(3, 1, 0);
	glVertex3f(1, 1, 0);

	glEnd();

	glPopMatrix();


	//Draw Lower Castle right block2
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(5, 0, 0);
	glVertex3f(7, 0, 0);
	glVertex3f(7, 1, 0);
	glVertex3f(5, 1, 0);

	glEnd();

	glPopMatrix();


	//Draw Lower Castle right block1
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(9, 0, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(10, 1, 0);
	glVertex3f(9, 1, 0);

	glEnd();

	glPopMatrix();


//Draw Upper Castle left window
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(0, 0, 0);

	glVertex3f(-4, 1, 0);
	glVertex3f(-2, 1, 0);
	glVertex3f(-2, 3, 0);
	glVertex3f(-4, 3, 0);

	glEnd();

	glPopMatrix();


	//Draw Upper Castle right window
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(0, 0, 0);

	glVertex3f(2, 1, 0);
	glVertex3f(4, 1, 0);
	glVertex3f(4, 3, 0);
	glVertex3f(2, 3, 0);

	glEnd();

	glPopMatrix();


	//Draw Upper Castle base
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(128, 64, 0);

	glVertex3f(-6, 0, 0);
	glVertex3f(6, 0, 0);
	glVertex3f(6, 4, 0);
	glVertex3f(-6, 4, 0);

	glEnd();

	glPopMatrix();


	//Draw Upper Castle left block1
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(-6, 4, 0);
	glVertex3f(-5, 4, 0);
	glVertex3f(-5, 5, 0);
	glVertex3f(-6, 5, 0);

	glEnd();

	glPopMatrix();


	//Draw Upper Castle left bock2
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(-3, 4, 0);
	glVertex3f(-1, 4, 0);
	glVertex3f(-1, 5, 0);
	glVertex3f(-3, 5, 0);

	glEnd();

	glPopMatrix();


	//Draw Upper Castle right block2
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(1, 4, 0);
	glVertex3f(3, 4, 0);
	glVertex3f(3, 5, 0);
	glVertex3f(1, 5, 0);

	glEnd();

	glPopMatrix();


	//Draw Upper Castle right block1
	glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);

	glVertex3f(6, 4, 0);
	glVertex3f(5, 4, 0);
	glVertex3f(5, 5, 0);
	glVertex3f(6, 5, 0);

	glEnd();

	glPopMatrix();

	glPopMatrix();
}

void drawFlag(){
    glPushMatrix();

    glTranslatef(0, 2.17, 0);
    glScalef(0.9, 0.9, 1);

        glPushMatrix();

	//Stand

	glPushMatrix();

	glColor3ub(0,255,0);

    glBegin(GL_POLYGON);

	glVertex3f(0.0, -2.0, 0.0);
	glVertex3f(0.1, -2.0, 0.0);
	glVertex3f(0.1, 2.0, 0.0);
	glVertex3f(0.0, 2.0, 0.0);

	glEnd();

	glPopMatrix();

    //Round SHape

    glPushMatrix();

    glTranslatef(0, 0.9, 0);
    glScalef(0.5, 0.5, 1);

    glColor3ub(255,255,255);

	glBegin(GL_POLYGON);
	glVertex3f(-0.1, 2.0, 0.0);
	glVertex3f(0.2, 2.0, 0.0);
	glVertex3f(0.2, 2.1, 0.0);
	glVertex3f(-0.1, 2.1, 0.0);

	glEnd();


	glColor3ub(255,255,255);

	glBegin(GL_POLYGON);

	glVertex3f(-0.1, 2.4, 0.0);
	glVertex3f(0.2, 2.4, 0.0);
	glVertex3f(0.2, 2.5, 0.0);
	glVertex3f(-0.1, 2.5, 0.0);

	glEnd();

	glColor3ub(0,255,0);

	glBegin(GL_POLYGON);

	glVertex3f(-0.1, 2.1, 0.0);
	glVertex3f(0.2, 2.1, 0.0);
	glVertex3f(0.2, 2.4, 0.0);
	glVertex3f(-0.1, 2.4, 0.0);

	glEnd();

	glColor3ub(255,255,255);

	glBegin(GL_POLYGON);

	glVertex3f(-0.2, 2.1, 0.0);
	glVertex3f(-0.1, 2.1, 0.0);
	glVertex3f(-0.1, 2.4, 0.0);
	glVertex3f(-0.2, 2.4, 0.0);

	glEnd();

	glColor3ub(255,255,255);

	glBegin(GL_POLYGON);

	glVertex3f(0.2, 2.1, 0.0);
	glVertex3f(0.3, 2.1, 0.0);
	glVertex3f(0.3, 2.4, 0.0);
	glVertex3f(0.2, 2.4, 0.0);

	glEnd();

    glPopMatrix();

    //Logo

    glPushMatrix();

    glTranslatef(0, flagY, 0);

    glColor3ub(0,255,0);

	glBegin(GL_POLYGON);

	glVertex3f(-0.4, 1.6, 0.0);
	glVertex3f(-0.1, 1.6, 0.0);
	glVertex3f(-0.1, 1.7, 0.0);
	glVertex3f(-0.4, 1.7, 0.0);

	glEnd();

	glColor3ub(0,255,0);

	glBegin(GL_POLYGON);

	glVertex3f(-0.4, 1.5, 0.0);
	glVertex3f(-0.3, 1.5, 0.0);
	glVertex3f(-0.3, 1.6, 0.0);
	glVertex3f(-0.4, 1.6, 0.0);

	glEnd();

	glColor3ub(0,255,0);

	glBegin(GL_POLYGON);

	glVertex3f(-0.4, 1.5, 0.0);
	glVertex3f(-0.3, 1.5, 0.0);
	glVertex3f(-0.3, 1.6, 0.0);
	glVertex3f(-0.4, 1.6, 0.0);

	glEnd();

	glColor3ub(0,255,0);

	glBegin(GL_POLYGON);

	glVertex3f(-0.2, 1.5, 0.0);
	glVertex3f(-0.1, 1.5, 0.0);
	glVertex3f(-0.1, 1.6, 0.0);
	glVertex3f(-0.2, 1.6, 0.0);

	glEnd();

	glColor3ub(0,255,0);

	glBegin(GL_POLYGON);

	glVertex3f(-0.3, 1.4, 0.0);
	glVertex3f(-0.2, 1.4, 0.0);
	glVertex3f(-0.2, 1.5, 0.0);
	glVertex3f(-0.3, 1.5, 0.0);

	glEnd();

	glPopMatrix();

    //Flag

    glPushMatrix();

    glTranslatef(0, flagY, 0);

    glColor3ub(255,255,255);

	glBegin(GL_POLYGON);

	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 1.8, 0.0);
	glVertex3f(-0.9, 1.8, 0.0);

	glEnd();

	glPopMatrix();


    //Block

    glPushMatrix();

    glColor3ub(255,255,255);

	glBegin(GL_POLYGON);

	glVertex3f(-0.2, -2.4, 0.0);
	glVertex3f(0.3, -2.4, 0.0);
	glVertex3f(0.3, -2.0, 0.0);
	glVertex3f(-0.2, -2.0, 0.0);

	glEnd();

	glPopMatrix();

	glPopMatrix();

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
            glVertex3f(1, 3.1, 0);



            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, 3.1, 0);


        glEnd();

   glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}

void drawPiranhaPlant()
{
glPushMatrix();

    glTranslatef(0.5, 0, 0);
    glScalef(0.4, 0.4, 1);

	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.1, 0.0, 0.0);
	glVertex3f(0.1, 0.8, 0.0);
	glVertex3f(0.0, 0.8, 0.0);
	glEnd();
    //2
    glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.1, 0.8, 0.0);
	glVertex3f(0.0, 0.8, 0.0);
	glVertex3f(0.0, 0.9, 0.0);
	glVertex3f(-0.1, 0.9, 0.0);
	glEnd();

    //3
    glColor3ub(217, 255, 2);
    glBegin(GL_POLYGON);
    glVertex3f(-0.1, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.8, 0.0);
	glVertex3f(-0.1, 0.8, 0.0);
	glEnd();

    //////////////////////////////
	//4
    glColor3ub(217, 255, 2);
    glBegin(GL_POLYGON);
    glVertex3f(0.3, 0.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.5, 0.1, 0.0);
	glVertex3f(0.3, 0.1, 0.0);
	glEnd();

	//5
    glColor3ub(217, 255, 2);
    glBegin(GL_POLYGON);
    glVertex3f(0.5, 0.1, 0.0);
	glVertex3f(0.6, 0.1, 0.0);
	glVertex3f(0.6, 0.2, 0.0);
	glVertex3f(0.5, 0.2, 0.0);
	glEnd();

	//6
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(0.6, 0.0, 0.0);
	glVertex3f(0.7, 0.0, 0.0);
	glVertex3f(0.7, 0.2, 0.0);
	glVertex3f(0.6, 0.2, 0.0);
	glEnd();

    //7
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(0.1, 0.4, 0.0);
	glVertex3f(0.2, 0.4, 0.0);
	glVertex3f(0.2, 0.5, 0.0);
	glVertex3f(0.1, 0.5, 0.0);
	glEnd();


	//8
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(0.3, 0.4, 0.0);
	glVertex3f(0.4, 0.4, 0.0);
	glVertex3f(0.4, 0.5, 0.0);
	glVertex3f(0.3, 0.5, 0.0);
	glEnd();

    //9
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(0.2, 0.5, 0.0);
	glVertex3f(0.3, 0.5, 0.0);
	glVertex3f(0.3, 0.6, 0.0);
	glVertex3f(0.2, 0.6, 0.0);
	glEnd();

	//10
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(0.4, 0.5, 0.0);
	glVertex3f(0.6, 0.5, 0.0);
	glVertex3f(0.6, 0.6, 0.0);
	glVertex3f(0.4, 0.6, 0.0);
	glEnd();
	//11
    glColor3ub(217, 255, 2);
    glBegin(GL_POLYGON);
    glVertex3f(0.2, 0.6, 0.0);
	glVertex3f(0.3, 0.6, 0.0);
	glVertex3f(0.3, 0.8, 0.0);
	glVertex3f(0.2, 0.8, 0.0);
	glEnd();

	//12
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(0.3, 0.6, 0.0);
	glVertex3f(0.4, 0.6, 0.0);
	glVertex3f(0.4, 0.8, 0.0);
	glVertex3f(0.3, 0.8, 0.0);
	glEnd();

	//13
	glBegin(GL_POLYGON);
    glVertex3f(0.4, 0.7, 0.0);
	glVertex3f(0.5, 0.7, 0.0);
	glVertex3f(0.5, 0.8, 0.0);
	glVertex3f(0.4, 0.8, 0.0);
	glEnd();

	//14
	glColor3ub(217, 255, 2);
	glBegin(GL_POLYGON);
    glVertex3f(0.3, 0.8, 0.0);
	glVertex3f(0.5, 0.8, 0.0);
	glVertex3f(0.5, 0.9, 0.0);
	glVertex3f(0.3, 0.9, 0.0);
	glEnd();

	//15
	glColor3ub(217, 255, 2);
	glBegin(GL_POLYGON);
    glVertex3f(0.5, 0.9, 0.0);
	glVertex3f(0.6, 0.9, 0.0);
	glVertex3f(0.6, 1.0, 0.0);
	glVertex3f(0.5, 1.0, 0.0);
	glEnd();

	//16
	glColor3ub(0, 255, 0);
	glBegin(GL_POLYGON);
    glVertex3f(0.6, 0.6, 0.0);
	glVertex3f(0.7, 0.6, 0.0);
	glVertex3f(0.7, 1.0, 0.0);
	glVertex3f(0.6, 1.0, 0.0);
	glEnd();

    //17
	glBegin(GL_POLYGON);
    glVertex3f(0.5, 0.6, 0.0);
	glVertex3f(0.6, 0.6, 0.0);
	glVertex3f(0.6, 0.7, 0.0);
	glVertex3f(0.5, 0.7, 0.0);
	glEnd();

    //Left side leaves
    glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.1, 0.0, 0.0);
	glVertex3f(0.1, 0.8, 0.0);
	glVertex3f(0.0, 0.8, 0.0);
	glEnd();
    //2
    glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.1, 0.8, 0.0);
	glVertex3f(0.0, 0.8, 0.0);
	glVertex3f(0.0, 0.9, 0.0);
	glVertex3f(-0.1, 0.9, 0.0);
	glEnd();

    //3
    glColor3ub(217, 255, 2);
    glBegin(GL_POLYGON);
    glVertex3f(-0.1, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.8, 0.0);
	glVertex3f(-0.1, 0.8, 0.0);
	glEnd();

    //////////////////////////////
	//4
    glColor3ub(217, 255, 2);
    glBegin(GL_POLYGON);
    glVertex3f(-0.3, 0.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);
	glVertex3f(-0.5, 0.1, 0.0);
	glVertex3f(-0.3, 0.1, 0.0);
	glEnd();

	//5
    glColor3ub(217, 255, 2);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, 0.1, 0.0);
	glVertex3f(-0.6, 0.1, 0.0);
	glVertex3f(-0.6, 0.2, 0.0);
	glVertex3f(-0.5, 0.2, 0.0);
	glEnd();

	//6
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.6, 0.0, 0.0);
	glVertex3f(-0.7, 0.0, 0.0);
	glVertex3f(-0.7, 0.2, 0.0);
	glVertex3f(-0.6, 0.2, 0.0);
	glEnd();

    //7
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.1, 0.4, 0.0);
	glVertex3f(-0.2, 0.4, 0.0);
	glVertex3f(-0.2, 0.5, 0.0);
	glVertex3f(-0.1, 0.5, 0.0);
	glEnd();


	//8
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.3, 0.4, 0.0);
	glVertex3f(-0.4, 0.4, 0.0);
	glVertex3f(-0.4, 0.5, 0.0);
	glVertex3f(-0.3, 0.5, 0.0);
	glEnd();

    //9
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.2, 0.5, 0.0);
	glVertex3f(-0.3, 0.5, 0.0);
	glVertex3f(-0.3, 0.6, 0.0);
	glVertex3f(-0.2, 0.6, 0.0);
	glEnd();

	//10
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.4, 0.5, 0.0);
	glVertex3f(-0.6, 0.5, 0.0);
	glVertex3f(-0.6, 0.6, 0.0);
	glVertex3f(-0.4, 0.6, 0.0);
	glEnd();
	//11
    glColor3ub(217, 255, 2);
    glBegin(GL_POLYGON);
    glVertex3f(-0.2, 0.6, 0.0);
	glVertex3f(-0.3, 0.6, 0.0);
	glVertex3f(-0.3, 0.8, 0.0);
	glVertex3f(-0.2, 0.8, 0.0);
	glEnd();

	//12
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.3, 0.6, 0.0);
	glVertex3f(-0.4, 0.6, 0.0);
	glVertex3f(-0.4, 0.8, 0.0);
	glVertex3f(-0.3, 0.8, 0.0);
	glEnd();

	//13
	glBegin(GL_POLYGON);
    glVertex3f(-0.4, 0.7, 0.0);
	glVertex3f(-0.5, 0.7, 0.0);
	glVertex3f(-0.5, 0.8, 0.0);
	glVertex3f(-0.4, 0.8, 0.0);
	glEnd();

	//14
	glColor3ub(217, 255, 2);
	glBegin(GL_POLYGON);
    glVertex3f(-0.3, 0.8, 0.0);
	glVertex3f(-0.5, 0.8, 0.0);
	glVertex3f(-0.5, 0.9, 0.0);
	glVertex3f(-0.3, 0.9, 0.0);
	glEnd();

	//15
	glColor3ub(217, 255, 2);
	glBegin(GL_POLYGON);
    glVertex3f(-0.5, 0.9, 0.0);
	glVertex3f(-0.6, 0.9, 0.0);
	glVertex3f(-0.6, 1.0, 0.0);
	glVertex3f(-0.5, 1.0, 0.0);
	glEnd();

	//16
	glColor3ub(0, 255, 0);
	glBegin(GL_POLYGON);
    glVertex3f(-0.6, 0.6, 0.0);
	glVertex3f(-0.7, 0.6, 0.0);
	glVertex3f(-0.7, 1.0, 0.0);
	glVertex3f(-0.6, 1.0, 0.0);
	glEnd();

    //17
	glBegin(GL_POLYGON);
    glVertex3f(-0.5, 0.6, 0.0);
	glVertex3f(-0.6, 0.6, 0.0);
	glVertex3f(-0.6, 0.7, 0.0);
	glVertex3f(-0.5, 0.7, 0.0);
	glEnd();

    ///////////////////////////////////////////////////
    //1
    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 1.2, 0.0);
	glVertex3f(0.4, 1.2, 0.0);
	glVertex3f(0.4, 1.3, 0.0);
	glVertex3f(0.0, 1.3, 0.0);
	glEnd();
    //2
	glBegin(GL_POLYGON);
    glVertex3f(0.0, 1.3, 0.0);
	glVertex3f(0.7, 1.3, 0.0);
	glVertex3f(0.7, 1.6, 0.0);
	glVertex3f(0.0, 1.6, 0.0);
	glEnd();

	//3
	glBegin(GL_POLYGON);
    glVertex3f(0.2, 1.6, 0.0);
	glVertex3f(0.8, 1.6, 0.0);
	glVertex3f(0.8, 1.7, 0.0);
	glVertex3f(0.2, 1.7, 0.0);
	glEnd();


	//4
	glBegin(GL_POLYGON);
    glVertex3f(0.3, 1.7, 0.0);
	glVertex3f(0.8, 1.7, 0.0);
	glVertex3f(0.8, 1.8, 0.0);
	glVertex3f(0.3, 1.8, 0.0);
	glEnd();

	//5
	glBegin(GL_POLYGON);
    glVertex3f(0.4, 1.8, 0.0);
	glVertex3f(0.7, 1.8, 0.0);
	glVertex3f(0.7, 1.9, 0.0);
	glVertex3f(0.4, 1.9, 0.0);
	glEnd();

	//6
	glBegin(GL_POLYGON);
    glVertex3f(0.5, 1.9, 0.0);
	glVertex3f(0.7, 1.9, 0.0);
	glVertex3f(0.7, 2.0, 0.0);
	glVertex3f(0.5, 2.0, 0.0);
	glEnd();

	//6.1
	glBegin(GL_POLYGON);
    glVertex3f(0.6, 2.0, 0.0);
	glVertex3f(0.7, 2.0, 0.0);
	glVertex3f(0.7, 2.1, 0.0);
	glVertex3f(0.6, 2.1, 0.0);
	glEnd();

	//7
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(0.0, 1.7, 0.0);
	glVertex3f(0.2, 1.7, 0.0);
	glVertex3f(0.2, 1.8, 0.0);
	glVertex3f(0.0, 1.8, 0.0);
	glEnd();

	//8
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(0.0, 1.8, 0.0);
	glVertex3f(0.3, 1.8, 0.0);
	glVertex3f(0.3, 1.9, 0.0);
	glVertex3f(0.0, 1.9, 0.0);
	glEnd();

    //9
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(0.1, 1.9, 0.0);
	glVertex3f(0.4, 1.9, 0.0);
	glVertex3f(0.4, 2.0, 0.0);
	glVertex3f(0.1, 2.0, 0.0);
	glEnd();

	//10
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(0.2, 2.0, 0.0);
	glVertex3f(0.5, 2.0, 0.0);
	glVertex3f(0.5, 2.1, 0.0);
	glVertex3f(0.2, 2.1, 0.0);
	glEnd();

	//12
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(0.3, 2.1, 0.0);
	glVertex3f(0.7, 2.1, 0.0);
	glVertex3f(0.7, 2.2, 0.0);
	glVertex3f(0.3, 2.2, 0.0);
	glEnd();

	//13
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(0.4, 2.2, 0.0);
	glVertex3f(0.7, 2.2, 0.0);
	glVertex3f(0.7, 2.3, 0.0);
	glVertex3f(0.4, 2.3, 0.0);
	glEnd();

	//14
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(0.5, 2.3, 0.0);
	glVertex3f(0.7, 2.3, 0.0);
	glVertex3f(0.7, 2.4, 0.0);
	glVertex3f(0.5, 2.4, 0.0);
	glEnd();
	//14
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(0.6, 2.4, 0.0);
	glVertex3f(0.7, 2.4, 0.0);
	glVertex3f(0.7, 2.5, 0.0);
	glVertex3f(0.6, 2.5, 0.0);
	glEnd();
    //////////Left sides/////////////
    //1
    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.0, 1.2, 0.0);
	glVertex3f(-0.4, 1.2, 0.0);
	glVertex3f(-0.4, 1.3, 0.0);
	glVertex3f(-0.0, 1.3, 0.0);
	glEnd();
    //2
	glBegin(GL_POLYGON);
    glVertex3f(-0.0, 1.3, 0.0);
	glVertex3f(-0.7, 1.3, 0.0);
	glVertex3f(-0.7, 1.6, 0.0);
	glVertex3f(-0.0, 1.6, 0.0);
	glEnd();

	//3
	glBegin(GL_POLYGON);
    glVertex3f(-0.2, 1.6, 0.0);
	glVertex3f(-0.8, 1.6, 0.0);
	glVertex3f(-0.8, 1.7, 0.0);
	glVertex3f(-0.2, 1.7, 0.0);
	glEnd();


	//4
	glBegin(GL_POLYGON);
    glVertex3f(-0.3, 1.7, 0.0);
	glVertex3f(-0.8, 1.7, 0.0);
	glVertex3f(-0.8, 1.8, 0.0);
	glVertex3f(-0.3, 1.8, 0.0);
	glEnd();

	//5
	glBegin(GL_POLYGON);
    glVertex3f(-0.4, 1.8, 0.0);
	glVertex3f(-0.7, 1.8, 0.0);
	glVertex3f(-0.7, 1.9, 0.0);
	glVertex3f(-0.4, 1.9, 0.0);
	glEnd();

	//6
	glBegin(GL_POLYGON);
    glVertex3f(-0.5, 1.9, 0.0);
	glVertex3f(-0.7, 1.9, 0.0);
	glVertex3f(-0.7, 2.0, 0.0);
	glVertex3f(-0.5, 2.0, 0.0);
	glEnd();

	//6.1
	glBegin(GL_POLYGON);
    glVertex3f(-0.6, 2.0, 0.0);
	glVertex3f(-0.7, 2.0, 0.0);
	glVertex3f(-0.7, 2.1, 0.0);
	glVertex3f(-0.6, 2.1, 0.0);
	glEnd();

	//7
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(-0.0, 1.7, 0.0);
	glVertex3f(-0.2, 1.7, 0.0);
	glVertex3f(-0.2, 1.8, 0.0);
	glVertex3f(-0.0, 1.8, 0.0);
	glEnd();

	//8
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(-0.0, 1.8, 0.0);
	glVertex3f(-0.3, 1.8, 0.0);
	glVertex3f(-0.3, 1.9, 0.0);
	glVertex3f(-0.0, 1.9, 0.0);
	glEnd();

    //9
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(-0.1, 1.9, 0.0);
	glVertex3f(-0.4, 1.9, 0.0);
	glVertex3f(-0.4, 2.0, 0.0);
	glVertex3f(-0.1, 2.0, 0.0);
	glEnd();

	//10
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(-0.2, 2.0, 0.0);
	glVertex3f(-0.5, 2.0, 0.0);
	glVertex3f(-0.5, 2.1, 0.0);
	glVertex3f(-0.2, 2.1, 0.0);
	glEnd();

	//12
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(-0.3, 2.1, 0.0);
	glVertex3f(-0.7, 2.1, 0.0);
	glVertex3f(-0.7, 2.2, 0.0);
	glVertex3f(-0.3, 2.2, 0.0);
	glEnd();

	//13
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(-0.4, 2.2, 0.0);
	glVertex3f(-0.7, 2.2, 0.0);
	glVertex3f(-0.7, 2.3, 0.0);
	glVertex3f(-0.4, 2.3, 0.0);
	glEnd();

	//14
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(-0.5, 2.3, 0.0);
	glVertex3f(-0.7, 2.3, 0.0);
	glVertex3f(-0.7, 2.4, 0.0);
	glVertex3f(-0.5, 2.4, 0.0);
	glEnd();

	//14
	glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
    glVertex3f(-0.6, 2.4, 0.0);
	glVertex3f(-0.7, 2.4, 0.0);
	glVertex3f(-0.7, 2.5, 0.0);
	glVertex3f(-0.6, 2.5, 0.0);
	glEnd();


	/***********Borders*************/
	glColor3ub(0,0,0);
	glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.2, 1.1, 0.0);
	glVertex3f(0.0, 1.1, 0.0);
	glEnd();


	glBegin(GL_POLYGON);
    glVertex3f(0.0, 1.1, 0.0);
	glVertex3f(0.4, 1.1, 0.0);
	glVertex3f(0.4, 1.2, 0.0);
	glVertex3f(0.0, 1.2, 0.0);
	glEnd();
	//B3

	glBegin(GL_POLYGON);
    glVertex3f(0.0, 1.2, 0.0);
	glVertex3f(0.8, 1.2, 0.0);
	glVertex3f(0.8, 1.8, 0.0);
	glVertex3f(0.0, 1.8, 0.0);
	glEnd();

	//B4

	glBegin(GL_POLYGON);
    glVertex3f(0.3, 1.8, 0.0);
	glVertex3f(0.8, 1.8, 0.0);
	glVertex3f(0.8, 2.4, 0.0);
	glVertex3f(0.3, 2.4, 0.0);
	glEnd();

	//B5

	glBegin(GL_POLYGON);
    glVertex3f(0.0, 1.9, 0.0);
	glVertex3f(0.1, 1.9, 0.0);
	glVertex3f(0.1, 2.0, 0.0);
	glVertex3f(0.0, 2.0, 0.0);
	glEnd();

	//B6

	glBegin(GL_POLYGON);
    glVertex3f(0.1, 2.0, 0.0);
	glVertex3f(0.2, 2.0, 0.0);
	glVertex3f(0.2, 2.1, 0.0);
	glVertex3f(0.1, 2.1, 0.0);
	glEnd();

	//B7

	glBegin(GL_POLYGON);
    glVertex3f(0.2, 2.1, 0.0);
	glVertex3f(0.3, 2.1, 0.0);
	glVertex3f(0.3, 2.2, 0.0);
	glVertex3f(0.2, 2.2, 0.0);
	glEnd();

    //B8

	glBegin(GL_POLYGON);
    glVertex3f(0.5, 2.4, 0.0);
	glVertex3f(0.8, 2.4, 0.0);
	glVertex3f(0.8, 2.6, 0.0);
	glVertex3f(0.5, 2.6, 0.0);
	glEnd();


	//B8

	glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.4, 0.0);
	glVertex3f(0.8, 0.4, 0.0);
	glVertex3f(0.8, 1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	//B9

	glBegin(GL_POLYGON);
    glVertex3f(0.5, 1.0, 0.0);
	glVertex3f(0.7, 1.0, 0.0);
	glVertex3f(0.7, 1.1, 0.0);
	glVertex3f(0.5, 1.1, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
    glVertex3f(0.3, 0.3, 0.0);
	glVertex3f(0.4, 0.3, 0.0);
	glVertex3f(0.4, 0.4, 0.0);
	glVertex3f(0.3, 0.4, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.2, 0.0);
	glVertex3f(0.0, 0.2, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
    glVertex3f(0.6, 0.2, 0.0);
	glVertex3f(0.7, 0.2, 0.0);
	glVertex3f(0.7, 0.3, 0.0);
	glVertex3f(0.6, 0.3, 0.0);
	glEnd();
    ////////////////////////////
    glColor3ub(0,0,0);
	glBegin(GL_POLYGON);
    glVertex3f(-0.0, 0.0, 0.0);
	glVertex3f(-0.2, 0.0, 0.0);
	glVertex3f(-0.2, 1.1, 0.0);
	glVertex3f(-0.0, 1.1, 0.0);
	glEnd();


	glBegin(GL_POLYGON);
    glVertex3f(-0.0, 1.1, 0.0);
	glVertex3f(-0.4, 1.1, 0.0);
	glVertex3f(-0.4, 1.2, 0.0);
	glVertex3f(-0.0, 1.2, 0.0);
	glEnd();
	//B3

	glBegin(GL_POLYGON);
    glVertex3f(-0.0, 1.2, 0.0);
	glVertex3f(-0.8, 1.2, 0.0);
	glVertex3f(-0.8, 1.8, 0.0);
	glVertex3f(-0.0, 1.8, 0.0);
	glEnd();

	//B4

	glBegin(GL_POLYGON);
    glVertex3f(-0.3, 1.8, 0.0);
	glVertex3f(-0.8, 1.8, 0.0);
	glVertex3f(-0.8, 2.4, 0.0);
	glVertex3f(-0.3, 2.4, 0.0);
	glEnd();

	//B5

	glBegin(GL_POLYGON);
    glVertex3f(-0.0, 1.9, 0.0);
	glVertex3f(-0.1, 1.9, 0.0);
	glVertex3f(-0.1, 2.0, 0.0);
	glVertex3f(-0.0, 2.0, 0.0);
	glEnd();

	//B6

	glBegin(GL_POLYGON);
    glVertex3f(-0.1, 2.0, 0.0);
	glVertex3f(-0.2, 2.0, 0.0);
	glVertex3f(-0.2, 2.1, 0.0);
	glVertex3f(-0.1, 2.1, 0.0);
	glEnd();

	//B7

	glBegin(GL_POLYGON);
    glVertex3f(-0.2, 2.1, 0.0);
	glVertex3f(-0.3, 2.1, 0.0);
	glVertex3f(-0.3, 2.2, 0.0);
	glVertex3f(-0.2, 2.2, 0.0);
	glEnd();

    //B8

	glBegin(GL_POLYGON);
    glVertex3f(-0.5, 2.4, 0.0);
	glVertex3f(-0.8, 2.4, 0.0);
	glVertex3f(-0.8, 2.6, 0.0);
	glVertex3f(-0.5, 2.6, 0.0);
	glEnd();


	//B8

	glBegin(GL_POLYGON);
    glVertex3f(-0.0, 0.4, 0.0);
	glVertex3f(-0.8, 0.4, 0.0);
	glVertex3f(-0.8, 1.0, 0.0);
	glVertex3f(-0.0, 1.0, 0.0);
	glEnd();

	//B9

	glBegin(GL_POLYGON);
    glVertex3f(-0.5, 1.0, 0.0);
	glVertex3f(-0.7, 1.0, 0.0);
	glVertex3f(-0.7, 1.1, 0.0);
	glVertex3f(-0.5, 1.1, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
    glVertex3f(-0.3, 0.3, 0.0);
	glVertex3f(-0.4, 0.3, 0.0);
	glVertex3f(-0.4, 0.4, 0.0);
	glVertex3f(-0.3, 0.4, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
    glVertex3f(-0.0, 0.0, 0.0);
	glVertex3f(-0.8, 0.0, 0.0);
	glVertex3f(-0.8, 0.2, 0.0);
	glVertex3f(-0.0, 0.2, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
    glVertex3f(-0.6, 0.2, 0.0);
	glVertex3f(-0.7, 0.2, 0.0);
	glVertex3f(-0.7, 0.3, 0.0);
	glVertex3f(-0.6, 0.3, 0.0);
	glEnd();

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

void drawFireFlower(float translateX, float translateY){
    glPushMatrix();

    glTranslatef(translateX+0.25, translateY+0.30, 0);
    glScalef(0.03, 0.03, 1);

//    glBegin(GL_QUADS);
//
//    glColor3ub(0, 240, 110);
//
//    glVertex3f(0, 0, 0);
//    glVertex3f(1, 0, 0);
//    glVertex3f(1, 1, 0);
//    glVertex3f(0, 1, 0);
//
//    glEnd();

    glPushMatrix(); //Save the current state of transformations

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-4, 0, 0);
	glVertex3f(4, 0, 0);
	glVertex3f(4, 1, 0);
	glVertex3f(-4, 1, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //red block2 left part1 with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(-3, 2, 0);
	glVertex3f(-6, 2, 0);
	glVertex3f(-6, 1, 0);
	glVertex3f(-3, 1, 0);

	glEnd();

	//glutSolidCube(1.0);


	glPopMatrix(); //Undo the move to the center of the pentagon


    //red block3 right part1 with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(3, 2, 0);
	glVertex3f(6, 2, 0);
	glVertex3f(6, 1, 0);
	glVertex3f(3, 1, 0);

	glEnd();

	//glutSolidCube(1.0);


	glPopMatrix(); //Undo the move to the center of the pentagon


    //red block4 left part2 with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(-5, 5, 0);
	glVertex3f(-7, 5, 0);
	glVertex3f(-7, 2, 0);
	glVertex3f(-5, 2, 0);

	glEnd();

	//glutSolidCube(1.0);


	glPopMatrix(); //Undo the move to the center of the pentagon


    //red block5 right part2 with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(5, 5, 0);
	glVertex3f(7, 5, 0);
	glVertex3f(7, 2, 0);
	glVertex3f(5, 2, 0);

	glEnd();

	//glutSolidCube(1.0);


	glPopMatrix(); //Undo the move to the center of the pentagon


    //red block6 left part3 with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(-6, 5, 0);
	glVertex3f(-3, 5, 0);
	glVertex3f(-3, 6, 0);
	glVertex3f(-6, 6, 0);

	glEnd();

	//glutSolidCube(1.0);


	glPopMatrix(); //Undo the move to the center of the pentagon


    //red block7 right part3 with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(6, 5, 0);
	glVertex3f(3, 5, 0);
	glVertex3f(3, 6, 0);
	glVertex3f(6, 6, 0);

	glEnd();

	//glutSolidCube(1.0);


	glPopMatrix(); //Undo the move to the center of the pentagon

    //red block8 upper part with plus minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-4, 6, 0);
	glVertex3f(4, 6, 0);
	glVertex3f(4, 7, 0);
	glVertex3f(-4, 7, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//gold block1 lower part with plus minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 128, 0);


	glVertex3f(-3, 1, 0);
	glVertex3f(3, 1, 0);
	glVertex3f(3, 2, 0);
	glVertex3f(-3, 2, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//gold block2 left part1 with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 128, 0);


	glVertex3f(-5, 2, 0);
	glVertex3f(-3, 2, 0);
	glVertex3f(-3, 3, 0);
	glVertex3f(-5, 3, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon




	//gold block3 right part1 with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 128, 0);


	glVertex3f(5, 2, 0);
	glVertex3f(3, 2, 0);
	glVertex3f(3, 3, 0);
	glVertex3f(5, 3, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


	//gold block4 left part2 with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 128, 0);


	glVertex3f(-5, 3, 0);
	glVertex3f(-4, 3, 0);
	glVertex3f(-4, 4, 0);
	glVertex3f(-5, 4, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//gold block5 right part2 with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 128, 0);


	glVertex3f(5, 3, 0);
	glVertex3f(4, 3, 0);
	glVertex3f(4, 4, 0);
	glVertex3f(5, 4, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//gold block6 left part3 with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 128, 0);


	glVertex3f(-5, 4, 0);
	glVertex3f(-3, 4, 0);
	glVertex3f(-3, 5, 0);
	glVertex3f(-5, 5, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//gold block7 right part3 with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 128, 0);


	glVertex3f(5, 4, 0);
	glVertex3f(3, 4, 0);
	glVertex3f(3, 5, 0);
	glVertex3f(5, 5, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//gold block8 upper part with plus minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 128, 0);


	glVertex3f(-3, 5, 0);
	glVertex3f(3, 5, 0);
	glVertex3f(3, 6, 0);
	glVertex3f(-3, 6, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//yellow block1 lower part with plus minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 0);


	glVertex3f(-1, 2, 0);
	glVertex3f(1, 2, 0);
	glVertex3f(1, 3, 0);
	glVertex3f(-1, 3, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//yellow block2 left part1 with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 0);


	glVertex3f(-3, 2, 0);
	glVertex3f(-2, 2, 0);
	glVertex3f(-2, 3, 0);
	glVertex3f(-3, 3, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//yellow block3 right part1 with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 0);


	glVertex3f(3, 2, 0);
	glVertex3f(2, 2, 0);
	glVertex3f(2, 3, 0);
	glVertex3f(3, 3, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


	//yellow block4 left part2 with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 0);


	glVertex3f(-4, 3, 0);
	glVertex3f(-3, 3, 0);
	glVertex3f(-3, 4, 0);
	glVertex3f(-4, 4, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//yellow block5 right part2 with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 0);


	glVertex3f(4, 3, 0);
	glVertex3f(3, 3, 0);
	glVertex3f(3, 4, 0);
	glVertex3f(4, 4, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//yellow block6 left part3 with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 0);


	glVertex3f(-3, 4, 0);
	glVertex3f(-2, 4, 0);
	glVertex3f(-2, 5, 0);
	glVertex3f(-3, 5, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//yellow block7 right part3 with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 0);


	glVertex3f(3, 4, 0);
	glVertex3f(2, 4, 0);
	glVertex3f(2, 5, 0);
	glVertex3f(3, 5, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//yellow block8 upper part with plus minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 0);


	glVertex3f(-1, 4, 0);
	glVertex3f(1, 4, 0);
	glVertex3f(1, 5, 0);
	glVertex3f(-1, 5, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//white block1 middle part with plus minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);


	glVertex3f(-1, 3, 0);
	glVertex3f(1, 3, 0);
	glVertex3f(1, 4, 0);
	glVertex3f(-1, 4, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//white block2 left part with minus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);


	glVertex3f(-3, 3, 0);
	glVertex3f(-2, 3, 0);
	glVertex3f(-2, 4, 0);
	glVertex3f(-3, 4, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//white block3 right part with plus side
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 255, 255);


	glVertex3f(3, 3, 0);
	glVertex3f(2, 3, 0);
	glVertex3f(2, 4, 0);
	glVertex3f(3, 4, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block1 middle part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(-1, 0, 0);
	glVertex3f(-1, -6, 0);
	glVertex3f(1, -6, 0);
	glVertex3f(1, 0, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block2 left part1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(-1, -6, 0);
	glVertex3f(-1, -5, 0);
	glVertex3f(-4, -5, 0);
	glVertex3f(-4, -6, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block3 right part1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(1, -6, 0);
	glVertex3f(4, -6, 0);
	glVertex3f(4, -5, 0);
	glVertex3f(1, -5, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block4 left part2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(-2, -5, 0);
	glVertex3f(-2, -4, 0);
	glVertex3f(-6, -4, 0);
	glVertex3f(-6, -5, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon




	//green block5 right part2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(2, -5, 0);
	glVertex3f(6, -5, 0);
	glVertex3f(6, -4, 0);
	glVertex3f(2, -4, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block6 left part3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(-2, -4, 0);
	glVertex3f(-2, -3, 0);
	glVertex3f(-7, -3, 0);
	glVertex3f(-7, -4, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block7 right part3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(2, -4, 0);
	glVertex3f(7, -4, 0);
	glVertex3f(7, -3, 0);
	glVertex3f(2, -3, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block8 left part4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(-4, -3, 0);
	glVertex3f(-4, -2, 0);
	glVertex3f(-7, -2, 0);
	glVertex3f(-7, -3, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block9 right part4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(3, -3, 0);
	glVertex3f(7, -3, 0);
	glVertex3f(7, -2, 0);
	glVertex3f(3, -2, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block10 left part5
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(-5, -2, 0);
	glVertex3f(-5, -1, 0);
	glVertex3f(-7, -1, 0);
	glVertex3f(-7, -2, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//green block11 right part5
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(0, 255, 0);


	glVertex3f(5, -2, 0);
	glVertex3f(7, -2, 0);
	glVertex3f(7, -1, 0);
	glVertex3f(5, -1, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix();

    glPopMatrix();
}

void moveMario(){


    if(moveRight == true && marioPositionX<87.5 && marioPositionX>2 && insideTheCylinder==false){

                    if(marioPositionX<40.2 || marioPositionX>42){
                        marioPositionX +=0.1f;
                        cameraX -=0.1f;
                    }
                    else if(marioPositionX>40.2 &&  marioPositionX<42 && marioPositionY>-0.45){
                              marioPositionX +=0.1f;
                              cameraX -=0.1f;
                    }

    }

    else if(moveRight == true && marioPositionX<227 && insideTheCylinder==true){


       if(marioPositionX<200 && marioPositionX>196){
           marioPositionX +=0.1f;
        }

        else if(marioPositionX>220.8){
              marioPositionX +=0.1f;
        }

        else{

             marioPositionX +=0.1f;
             cameraX -=0.1f;
        }



    }

    else if(moveLeft == true && marioPositionX<87.5 && marioPositionX>2 && insideTheCylinder==false){




               if(marioPositionX<40.3 || marioPositionX>42){
                    marioPositionX -=0.1f;
                    cameraX +=0.1f;

                }
                else{
                     if(marioPositionY>-0.45){
                        cout<<"Left Key Pressed"<<endl;
                        marioPositionX -=0.1f;
                        cameraX +=0.1f;
                    }

                }


}

    else if(moveLeft == true && insideTheCylinder==true){

        if(marioPositionX>196.5){

                    if(marioPositionX<200 && marioPositionX>196){
                        marioPositionX -=0.1f;
                    }

                    else if(marioPositionX>220.8){
                         marioPositionX -=0.1f;
                    }
                    else{
                         marioPositionX -=0.1f;
                         cameraX +=0.1f;
                    }

        }
    }


}

void drawFloor(int length){

    float translateFloorX=0.0;


    enableTexture(_textureFloor);


    if(insideTheCylinder){  glColor3f(0.0f, 0.0f, 1.0f);}


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

void drawVerticalFloor(int length){

    float translateFloorX=0.0;
    float translateFloorY=0.0;


    enableTexture(_textureFloor);
    glColor3f(0.0f, 0.0f, 1.0f);

    for(int i=0;i<length;i++){

    glPushMatrix();

       //glColor3f(1, 0, 0);

        float brickSize = 0.5;


        glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0, translateFloorY, 0);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.5, translateFloorY, 0);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.5, ++translateFloorY, 0);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0, translateFloorY, 0);
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


         brickLocation[locationCounter] = storeObjectPosition(x,y,length,0);
         locationCounter++;



    float translateFloorX=0.0;

    enableTexture(_textureBrick);

  glPushMatrix();
  glTranslatef(x,y+0.1,0);

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
            glTranslatef(marioPositionX-1.0, marioPositionY, 0);
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
            else if (marioPositionY <= -2.95)
            {
                jumpCounter = 0;
                jumpTopReached = false;
                jumpMarioKeyPressed = false;
            }

        }

    }


}

void collisionEvents(int brickNumber){
    if(brickNumber == 7 && !flowerUsed)
    {
        showFlower = true;
        enableSound("powerupappears");
    }
    if(brickNumber != 7)
    {
        if(jumpBottomCollisionOccuredCoin && brickCollisionStatus[brickNumber]==true && (brickNumber == 1 || brickNumber == 4 || brickNumber == 6 || brickNumber == 9 || brickNumber == 10 || brickNumber == 11 || brickNumber == 12 || brickNumber == 15 || brickNumber == 16 || brickNumber == 17))
        {
            enableSound("coin");
            score += 100;
            cout << "Score: " << score << endl;
            jumpBottomCollisionOccuredCoin=false;
            brickCollisionStatus[brickNumber]=false;
        }

        else
        {
            enableSound("collite");
            jumpBottomCollisionOccured=false;
            jumpBottomCollisionOccuredCoin = false;
        }
    }
}

void animatePiranhaPlant()
{
    if(piranhaWaitCounter <= 0)
    {
        if(movePlantUp == true)
        {
            if(piranhaPlantY>=0.0)
            {
                movePlantUp = false;
                piranhaWaitCounter = 40;
            }
            else
            {
                piranhaPlantY += 0.01;
            }
        }
        else
        {
            if(piranhaPlantY<=-1.0)
            {
                movePlantUp = true;
                piranhaWaitCounter = 40;
            }
            else
            {
                piranhaPlantY -= 0.01;
            }
        }
    }
    else
    {
        piranhaWaitCounter -= 1;
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

         if(key=='f' && fireBulletAbility){

               enableSound("fire");
               fireBullet=true;
               bulletX=marioPositionX-2.5;
               bulletY=marioPositionY+1.65;

               cout<<"Fired"<<endl;
         }

         if(key=='p')
         {
            if(isPaused)
            {
                isPaused = false;
            }
            else if(!isPaused)
            {
                isPaused = true;
            }
            enableSound("pause");
         }



         if(key=='s'){

            if(marioPositionX>40.5 && marioPositionX<41.7){
                enableSound("pipetravel");
                marioPositionX=200;
                cameraX=-200;
            }

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



    //DrawMenu
    glPushMatrix();
        //drawMenu();
    glPopMatrix();



       //draw Fire

         if(fireBullet==true){
                glPushMatrix();
                glTranslatef(bulletX,bulletY,0);
                drawFire();
                glPopMatrix();
            }



    //draw score
  glPushMatrix();

//    printText();

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
     drawBrick(25,-0.5,1);
     if(!flowerUsed)drawFireFlower(25, flowerPositionY);
    glPopMatrix();
    tempBrickCounter++;



    //block draw

//    glPushMatrix();
//        glTranslatef(25.5, -1.5, 0);
//        drawObstacleBlockByMyDefinedFunc(25.5,-1.5,5);
//    glPopMatrix();




    //draw floor
    glPushMatrix();
    glTranslatef(31, -3, 0);
        drawFloor(28);
    glPopMatrix();


    //draw cloud
          if(marioPositionX<150){
    glPushMatrix();
        glTranslatef(cloudPositionX, 2, 0);
        drawCloud(100);
    glPopMatrix();
            }

        //draw Cylinder
    glPushMatrix();
        glTranslatef(40, -3, 0);
        drawCylinder();
    glPopMatrix();

     glPushMatrix();
        drawBrick(40,-0.5,2);
    glPopMatrix();

    tempBrickCounter++;





  //draw Cylinder with piranha plant

    glPushMatrix();

    glTranslatef (32, -3, 0);
    drawCylinder();

    glPopMatrix();

    //draw piranta plant

    glPushMatrix();

    glTranslatef(32, piranhaPlantY, 0);
    drawPiranhaPlant();

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
            drawScoreBrick(47.5,1.0,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
            //glTranslatef(1.5, 0.8, 0);
            drawScoreBrick(47.5,1.0,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;

    //glPopMatrix();




    //drawenemy

    //drawenemy

    int arrayLength = sizeof(emeneyPositionx) / sizeof(int);

    if(storeLocation==true){generateRandomEnemy();}

    for(int i=0;i<arrayLength;i++){
        glPushMatrix();
                    glTranslatef(emeneyPositionx[i], -2.3, 0);
                    glScalef(0.25, 0.25, 1.0);
                    drawEnemy();
        glPopMatrix();
    }








    //drawCastle
       glPushMatrix();
                    glTranslatef(87, -2.5, 0);
                    drawCastle();
       glPopMatrix();



//draw flag
       glPushMatrix();
                    glTranslatef(82, -2.5, 0);
                    drawFlag();
       glPopMatrix();






       //drawSlide
       glPushMatrix();
                    drawBrick(slideBrickX[0][0],-0.5,3);
       glPopMatrix();
       tempBrickCounter++;


        glPushMatrix();
                    drawBrick(slideBrickX[1][0],-0.5,3);
       glPopMatrix();
       tempBrickCounter++;







    //scorebrickonslide

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        // glTranslatef(13, 0.5, 0);
         drawScoreBrick(61,1.0,1,true);
         glPopMatrix();
    }
    else
    {
        glPushMatrix();
        // glTranslatef(13, 0.5, 0);
         drawScoreBrick(61,1.0,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;







    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        // glTranslatef(13, 0.5, 0);
         drawScoreBrick(66,1.0,1,true);
         glPopMatrix();
    }
    else
    {
        glPushMatrix();
        // glTranslatef(13, 0.5, 0);
         drawScoreBrick(66,1.0,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;








    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        // glTranslatef(13, 0.5, 0);
         drawScoreBrick(69,1.0,1,true);
         glPopMatrix();
    }
    else
    {
        glPushMatrix();
        // glTranslatef(13, 0.5, 0);
         drawScoreBrick(69,1.0,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;








      //drawhfloor
    glPushMatrix();
    glTranslatef(73, -3, 0);
        drawFloor(25);
    glPopMatrix();






   glPushMatrix();
     glTranslatef(19, -2.5, 0);
     drawHill();
   glPopMatrix();









       //After Entering into Pipe Environment
      glPushMatrix();
        glTranslatef(194, 2.5, 0);
            drawFloor(5);
       glPopMatrix();

       glPushMatrix();
        glTranslatef(200, 2.5, 0);
            drawFloor(25);
       glPopMatrix();

         glPushMatrix();
       glTranslatef(194, -3, 0);
            drawFloor(34);
       glPopMatrix();


         glPushMatrix();
        glTranslatef(195, -2.5, 0);
            drawVerticalFloor(7);
        glPopMatrix();

         glPushMatrix();
        glTranslatef(194.5, -2.5, 0);
            drawVerticalFloor(7);
        glPopMatrix();


          glPushMatrix();
        glTranslatef(194, -2.5, 0);
            drawVerticalFloor(7);
        glPopMatrix();


           glPushMatrix();
        glTranslatef(225, -2, 0);
            drawVerticalFloor(7);
        glPopMatrix();
              glPushMatrix();
        glTranslatef(225.5, -2, 0);
            drawVerticalFloor(7);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(226, -2, 0);
            drawVerticalFloor(7);
        glPopMatrix();



   //After Entering into Pipe Environment





   storeLocation = false;
   bottomAreaAssignCount=0;
   tempBrickCounter = 0;
   glutSwapBuffers();
}

void update(int value) {


    if(mariCrossedFlag==2){
        if(flagY>-3){
            isPaused=true;
            flagY-=0.1f;
        }else{
                isPaused=false;
            }
    }


    if(mariCrossedFlag==1){
        enableSound("flagcrossed");
        mariCrossedFlag=2;
//        Sleep(4000);
    }

       if(levelComplete==1){
        enableSound("levelclear");
        levelComplete=2;
        Sleep(6000);
     }



    if(mariCrossedFlag==0 && marioPositionX>83.2 && marioPositionX<85){
        mariCrossedFlag=1;
    }

    if(levelComplete==0 && marioPositionX>87.5 && marioPositionX<89){
        levelComplete=1;
    }



    if(marioPositionX>226.5 && marioPositionX<230){
        marioPositionX=41;
        marioPositionY=-0.1;
        cameraX=-41;
    }


    if(marioPositionX>190){
        insideTheCylinder=true;
    }else{
        insideTheCylinder=false;
    }


    slideBrick();

    if(!isPaused)
    {
        if(marioPositionY >= -2.95 && jumpCounter==0 && !onTheBrick)
        {
            marioPositionY -= 0.2f;
        }


        moveMario();

        if(flowerUsed)
        {
            fireBulletAbility = true;
            showFlower = false;
            enableFlowerCollision = false;
        }

         bulletCollisionWithEnemy();


        if(marioCollisionOccured==true){
             colliteMario(0,0);
        }

        jumpMario();


        if(marioPositionY < -1.5)
        {
            onTheBrick = false;
        }


        detectCollision();

        delectCollisionWithEnemy();

        detectCollisionWithFireflower();

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




        int arrayLength = sizeof(emeneyPositionx) / sizeof(int);

        for(int i=0;i<arrayLength;i++){
                 emeneyPositionx[i]-=0.02;
        }






    //     textX+=marioPositionX;



            //bullet property change
              if(fireBullet){

                    if(bulletY>=-1.0){
                        bulletX+=0.2;
                        bulletY-=0.1;
                    }
                    else if(bulletY<=-0.8f){
                        bulletX+=0.2;
                        bulletY+=0.1;
                   }

              }


        if(!flowerUsed && showFlower)
        {
            if(flowerPositionY<=0)
            {
                flowerPositionY += 0.05f;
            }
        }

        if(flowerPositionY >= 0)
        {
            enableFlowerCollision = true;
        }
    }

    animatePiranhaPlant();





    glutPostRedisplay(); //Tell GLUT that the display has changed

    //Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(25, update, 0);
}

void playBackgroundMusic(){
    mciSendString("open ./sounds/background.wav alias background", 0, 0, 0);
    mciSendString("play background", 0, 0, 0);
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

    playBackgroundMusic();


    //PlaySound("sounds/background.wav", NULL, SND_LOOP | SND_ASYNC);
    //enableSound("background");

    //Set handler functions
    glutDisplayFunc(drawScene);

    glutReshapeFunc(handleResize);

    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeypressUp);

    glutTimerFunc(25, update, 0); //Add a timer

    glutMainLoop();


}
