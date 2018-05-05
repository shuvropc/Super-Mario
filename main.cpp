#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <conio.h>
#include <string>

#include <sstream>

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
float cameraX=-4;
//float cameraX=-220;


//Mario Property
int marioState = 0; // 0 is idle, 1 is running, 2 is jump
int previousMarioState;
float marioRunCounter = 0.0;
float marioPositionX=4;
//float marioPositionX=220;
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
GLuint _textureMenu;


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
float piranhaPlantY = -4;
bool movePlantUp = true;
int piranhaWaitCounter = 20;

float piranhaPlantX[5]={32,206,213,220,224};

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


//Enemy Property
int generateRandomEnemyInsidePipe = 0;


//BackgroundColorProperty
float backgroundColor[1][3]={{0.48,0.47,1.0}};


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

void restartGame(){


        _cameraAngle = 0.0;



        //cameraProperty
        cameraX=-4;
        //float cameraX=-220;


        //Mario Property
        marioState = 0; // 0 is idle, 1 is running, 2 is jump
        previousMarioState;
        marioRunCounter = 0.0;
        marioPositionX=4;
        //float marioPositionX=220;
        marioPositionY=-2.95;
        jumpMarioKeyPressed=false;
        jumpTopReached=false;
        marioDirectionRight=true;
        moveRight, moveLeft = false;
        keyarr[4];

        //game logic variables
        isPaused = false;

        //location store
        storeLocation = true;

        //Cloud Property
        cloudPositionX=4.0;


        //collision information

        bottomAreaAssignCount=0;


        bottomCollisionArea[500][5];

        //float  bottomCollisionArea[2][4]={{5.0,-0.5,5.5,-0.5},{6.0,-0.5,6.5,-0.5}};


        //collision property

        marioCollisionOccured = false;
        jumpBottomCollisionOccured = false;
        jumpBottomCollisionOccuredCoin = false;


        //coin collision
        currentCollisionBrick;
        collisionedCoinX;
        collisionedCoinY;
        brickCollisionStatus[100]; // true for no collision, false for collision
        brickLocation[100]; //brickLocation value comes from bottomAreaAssignCount
        locationCounter = 0; //used as brickLocation array's counter
        tempBrickCounter = 0; //a temp counter used while calling drawbrick. Will be reset each time drawScene finishs looping



        //goomba animation variables
        animateLegScaleX = 0.0;
        animateLegScaleY = 0.0;
        legAnimationCycle = true;

        //piranha plant animation variables
        piranhaPlantY = -4;
        movePlantUp = true;
        piranhaWaitCounter = 20;

       float  piranhaPlantX[5]={32,206,213,220,224};

        //jump variables
        jumpCounter = 0;
        onTheBrick = false;


        //enemy property
       float emeneyPositionx[10]={10,15,20};


        //Fire Property
        fireBulletAbility = false;
        fireBullet = false;
        bulletX=0;
        bulletY=-0.8;
        bulletTouchedGround = false;


        //Fireflower Property
        enableFlowerCollision = false;
        flowerUsed = false;
        showFlower = false;
        flowerPositionY = -0.5; //initial Y position of the brick which the flower is in


        //text property
        score=0;


        //slideBrickProperty
float   slideBrickX[3][2]={{59,0},{66,1}};
        valueChanageForSlide=true;


        //marioLife
        marioLife=3;


        //Entered inside Cylinder
        insideTheCylinder=false;
        inCylinderArea=false;


        //LevelProperty
        mariCrossedFlag=0;
        levelComplete=0;

        //Flag Property
        flagY=0;


        //GameMenu
        gamePauseMenu = false;


        //Enemy Property
        generateRandomEnemyInsidePipe = 0;


        //BackgroundColorProperty
float         backgroundColor[1][3]={{0.48,0.47,1.0}};



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
        if(state=="startlevel")
        {
        PlaySound("sounds/startlevel.wav", NULL, NULL | SND_ASYNC);
        }

        if(state=="levelclear")
        {
        PlaySound("sounds/levelclear.wav", NULL, NULL | SND_ASYNC);
        mciSendString("close background", 0, 0, 0);
        }




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

void generateRandomEnemy2(){

        srand (time(NULL));

        for(int i=5;i<11;i++){
         int  random = 204 + (rand() % static_cast<int>(224 - 204 + 1));
         emeneyPositionx[i]=random;
        }
    }

void generateRandomEnemy(){

 srand (time(NULL));

    for(int i=0;i<5;i++){
         int  random = rand() % 39 + 10;
         emeneyPositionx[i]=random;
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

void marioCollisionWithPiranha(){
    int arrayLength = sizeof(piranhaPlantX) / sizeof(int);

    for(int i=0; i<arrayLength;i++){

       float verticalDistence= marioPositionY-piranhaPlantY;

       if(marioPositionX>piranhaPlantX[i]+0.5 && marioPositionX<piranhaPlantX[i]+1.8){

        if(verticalDistence>-1.05 && verticalDistence<0.75){

                  enableSound("mariodie");
                  marioLife--;
                  cout<<"Mario died Life: "<<marioLife<<endl;
//                  marioPositionY=-500;
                  Sleep(3000);
                  marioPositionX-=2;
                  cameraX+=2;
                  marioPositionY=-2.95;

        }

       }
    }

}

void bulletCollisionWithEnemy(){


    if((bulletX-marioPositionX)>20){
        bulletY=-500;
    }

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

void collideMario(float x, float y){

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

void drawMenu(){



         enableTexture(_textureMenu);
         glPushMatrix();
            glColor4f(1.0, 1, 1, 0.5);


            glBegin(GL_POLYGON);
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(marioPositionX-5.5,-3,0);

                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(marioPositionX+5.5,-3,0);

                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(marioPositionX+5.5,3,0);

                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(marioPositionX-5.5,3,0);
            glEnd();

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

void drawEnemy2(){
    glPushMatrix();

    glTranslatef(0,0,0);
    glRotatef(180, 0, 1, 0);
    glScalef(1,1,1);

	glPushMatrix();
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.4, 0.0, 0.0);
	glVertex3f(0.4, 0.2, 0.0);
	glVertex3f(0.0, 0.2, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//2
	glColor3ub(255,255,255);
    glBegin(GL_POLYGON);
	glVertex3f(0.4, 0.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.5, 0.1, 0.0);
	glVertex3f(0.4, 0.1, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
    //3
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.2, 0.2, 0.0);
	glVertex3f(0.3, 0.2, 0.0);
	glVertex3f(0.3, 0.3, 0.0);
	glVertex3f(0.2, 0.3, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	//Border
	glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
	glVertex3f(0.0, -0.1, 0.0);
	glVertex3f(0.6, -0.1, 0.0);
	glVertex3f(0.6, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.6, 0.0, 0.0);
	glVertex3f(0.6, 0.1, 0.0);
	glVertex3f(0.5, 0.1, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
	glVertex3f(0.4, 0.1, 0.0);
	glVertex3f(0.5, 0.1, 0.0);
	glVertex3f(0.5, 0.2, 0.0);
	glVertex3f(0.4, 0.2, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
	glVertex3f(0.3, 0.2, 0.0);
	glVertex3f(0.4, 0.2, 0.0);
	glVertex3f(0.4, 0.3, 0.0);
	glVertex3f(0.3, 0.3, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
    //Border
	glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
	glVertex3f(0.2, 0.2, 0.0);
	glVertex3f(0.3, 0.2, 0.0);
	glVertex3f(0.3, 1.0, 0.0);
	glVertex3f(0.2, 1.0, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
    //Border
	glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.2, 0.0);
	glVertex3f(0.2, 0.2, 0.0);
	glVertex3f(0.2, 0.3, 0.0);
	glVertex3f(0.0, 0.3, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.1, -0.1, 0.0);
	glVertex3f(0.0, -0.1, 0.0);
	glVertex3f(0.0, 0.3, 0.0);
	glVertex3f(-0.1, 0.3, 0.0);
	glEnd();

	glPopMatrix();
	//Border
	glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(-0.7, 0.1, 0.0);
	glVertex3f(-0.7, 0.2, 0.0);
	glVertex3f(0.0, 0.2, 0.0);
	glEnd();

	glPushMatrix();

    //4
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(-0.6, 0.0, 0.0);
	glVertex3f(-0.3, 0.0, 0.0);
	glVertex3f(-0.3, 0.1, 0.0);
	glVertex3f(-0.6, 0.1, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	//5
	glColor3ub(255,255,255);
    glBegin(GL_POLYGON);
	glVertex3f(-0.7, 0.0, 0.0);
	glVertex3f(-0.6, 0.0, 0.0);
	glVertex3f(-0.6, 0.1, 0.0);
	glVertex3f(-0.7, 0.1, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.8, 0.0, 0.0);
	glVertex3f(-0.7, 0.0, 0.0);
	glVertex3f(-0.7, 0.1, 0.0);
	glVertex3f(-0.8, 0.1, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.8, -0.1, 0.0);
	glVertex3f(-0.2, -0.1, 0.0);
	glVertex3f(-0.2, 0.0, 0.0);
	glVertex3f(-0.8, 0.0, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.3, 0.0, 0.0);
	glVertex3f(-0.2, 0.0, 0.0);
	glVertex3f(-0.2, 0.1, 0.0);
	glVertex3f(-0.3, 0.1, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();

	//6
	glColor3ub(255,255,255);
    glBegin(GL_POLYGON);
	glVertex3f(-0.7, 0.2, 0.0);
	glVertex3f(-0.1, 0.2, 0.0);
	glVertex3f(-0.1, 0.4, 0.0);
	glVertex3f(-0.7, 0.4, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();

	//7
	glColor3ub(255,255,255);
    glBegin(GL_POLYGON);
	glVertex3f(-0.1, 0.3, 0.0);
	glVertex3f(0.1, 0.3, 0.0);
	glVertex3f(0.1, 0.4, 0.0);
	glVertex3f(-0.1, 0.4, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();


	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(0.1, 0.3, 0.0);
	glVertex3f(0.2, 0.3, 0.0);
	glVertex3f(0.2, 0.4, 0.0);
	glVertex3f(0.1, 0.4, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();

	//8
	glColor3ub(255,255,255);
    glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.4, 0.0);
	glVertex3f(0.2, 0.4, 0.0);
	glVertex3f(0.2, 0.6, 0.0);
	glVertex3f(0.0, 0.6, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//9
	glColor3ub(255,255,255);
    glBegin(GL_POLYGON);
	glVertex3f(0.1, 0.6, 0.0);
	glVertex3f(0.2, 0.6, 0.0);
	glVertex3f(0.2, 1.1, 0.0);
	glVertex3f(0.1, 1.1, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//10
	glColor3ub(255,255,255);
    glBegin(GL_POLYGON);
	glVertex3f(0.0, 1.1, 0.0);
	glVertex3f(0.1, 1.1, 0.0);
	glVertex3f(0.1, 1.4, 0.0);
	glVertex3f(0.0, 1.4, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();

	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.8, 0.2, 0.0);
	glVertex3f(-0.7, 0.2, 0.0);
	glVertex3f(-0.7, 0.5, 0.0);
	glVertex3f(-0.8, 0.5, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.7, 0.4, 0.0);
	glVertex3f(-0.0, 0.4, 0.0);
	glVertex3f(-0.0, 0.5, 0.0);
	glVertex3f(-0.7, 0.5, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.1, 0.5, 0.0);
	glVertex3f(-0.0, 0.5, 0.0);
	glVertex3f(-0.0, 0.6, 0.0);
	glVertex3f(-0.1, 0.6, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.6, 0.0);
	glVertex3f(0.1, 0.6, 0.0);
	glVertex3f(0.1, 1.1, 0.0);
	glVertex3f(0.0, 1.1, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(0.1, 1.1, 0.0);
	glVertex3f(0.2, 1.1, 0.0);
	glVertex3f(0.2, 1.4, 0.0);
	glVertex3f(0.1, 1.4, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.1, 1.1, 0.0);
	glVertex3f(0.0, 1.1, 0.0);
	glVertex3f(0.0, 1.4, 0.0);
	glVertex3f(-0.1, 1.4, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();

	//11
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.2, 0.6, 0.0);
	glVertex3f(0.0, 0.6, 0.0);
	glVertex3f(0.0, 0.8, 0.0);
	glVertex3f(-0.2, 0.8, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//12
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.1, 0.8, 0.0);
	glVertex3f(0.0, 0.8, 0.0);
	glVertex3f(0.0, 0.9, 0.0);
	glVertex3f(-0.1, 0.9, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//13
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.3, 0.6, 0.0);
	glVertex3f(-0.2, 0.6, 0.0);
	glVertex3f(-0.2, 0.7, 0.0);
	glVertex3f(-0.3, 0.7, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//14
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.2, 0.5, 0.0);
	glVertex3f(-0.1, 0.5, 0.0);
	glVertex3f(-0.1, 0.6, 0.0);
	glVertex3f(-0.2, 0.6, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//15
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.4, 0.5, 0.0);
	glVertex3f(-0.3, 0.5, 0.0);
	glVertex3f(-0.3, 0.6, 0.0);
	glVertex3f(-0.4, 0.6, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//16
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.6, 0.5, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glVertex3f(-0.5, 0.6, 0.0);
	glVertex3f(-0.6, 0.6, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//17
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.7, 0.6, 0.0);
	glVertex3f(-0.5, 0.6, 0.0);
	glVertex3f(-0.5, 0.8, 0.0);
	glVertex3f(-0.7, 0.8, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//18
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.5, 0.6, 0.0);
	glVertex3f(-0.4, 0.6, 0.0);
	glVertex3f(-0.4, 0.7, 0.0);
	glVertex3f(-0.5, 0.7, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//19
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.7, 0.8, 0.0);
	glVertex3f(-0.6, 0.8, 0.0);
	glVertex3f(-0.6, 0.9, 0.0);
	glVertex3f(-0.7, 0.9, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//20
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.5, 0.8, 0.0);
	glVertex3f(-0.2, 0.8, 0.0);
	glVertex3f(-0.2, 1.1, 0.0);
	glVertex3f(-0.5, 1.1, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//21
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.4, 0.7, 0.0);
	glVertex3f(-0.3, 0.7, 0.0);
	glVertex3f(-0.3, 0.8, 0.0);
	glVertex3f(-0.4, 0.8, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//22
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.6, 0.9, 0.0);
	glVertex3f(-0.5, 0.9, 0.0);
	glVertex3f(-0.5, 1.0, 0.0);
	glVertex3f(-0.6, 1.0, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//23
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.4, 1.1, 0.0);
	glVertex3f(-0.3, 1.1, 0.0);
	glVertex3f(-0.3, 1.2, 0.0);
	glVertex3f(-0.4, 1.2, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();

	//24
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.2, 0.9, 0.0);
	glVertex3f(-0.1, 0.9, 0.0);
	glVertex3f(-0.1, 1.0, 0.0);
	glVertex3f(-0.2, 1.0, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//25
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.1, 1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 1.1, 0.0);
	glVertex3f(-0.1, 1.1, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//26
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.2, 1.1, 0.0);
	glVertex3f(-0.1, 1.1, 0.0);
	glVertex3f(-0.1, 1.4, 0.0);
	glVertex3f(-0.2, 1.4, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
		//27
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.3, 1.2, 0.0);
	glVertex3f(-0.2, 1.2, 0.0);
	glVertex3f(-0.2, 1.3, 0.0);
	glVertex3f(-0.3, 1.3, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
		//28
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.5, 1.2, 0.0);
	glVertex3f(-0.4, 1.2, 0.0);
	glVertex3f(-0.4, 1.3, 0.0);
	glVertex3f(-0.5, 1.3, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
		//29
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.4, 1.3, 0.0);
	glVertex3f(-0.3, 1.3, 0.0);
	glVertex3f(-0.3, 1.4, 0.0);
	glVertex3f(-0.4, 1.4, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
		//30
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.5, 1.4, 0.0);
	glVertex3f(-0.2, 1.4, 0.0);
	glVertex3f(-0.2, 1.5, 0.0);
	glVertex3f(-0.5, 1.5, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
		//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(0.1, 1.4, 0.0);
	glVertex3f(-0.6, 1.4, 0.0);
	glVertex3f(-0.6, 1.5, 0.0);
	glVertex3f(0.1, 1.5, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.5, 1.5, 0.0);
	glVertex3f(-0.2, 1.5, 0.0);
	glVertex3f(-0.2, 1.6, 0.0);
	glVertex3f(-0.5, 1.6, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

		//31
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.6, 1.1, 0.0);
	glVertex3f(-0.5, 1.1, 0.0);
	glVertex3f(-0.5, 1.4, 0.0);
	glVertex3f(-0.6, 1.4, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
    //32
	glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.7, 1.0, 0.0);
	glVertex3f(-0.6, 1.0, 0.0);
	glVertex3f(-0.6, 1.2, 0.0);
	glVertex3f(-0.7, 1.2, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//33
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.3, 0.6, 0.0);
	glVertex3f(0.4, 0.6, 0.0);
	glVertex3f(0.4, 1.1, 0.0);
	glVertex3f(0.3, 1.1, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//34
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.4, 0.8, 0.0);
	glVertex3f(0.6, 0.8, 0.0);
	glVertex3f(0.6, 0.9, 0.0);
	glVertex3f(0.5, 0.9, 0.0);
	glVertex3f(0.5, 1.0, 0.0);
	glVertex3f(0.4, 1.0, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//35
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.2, 1.1, 0.0);
	glVertex3f(0.3, 1.1, 0.0);
	glVertex3f(0.3, 1.9, 0.0);
	glVertex3f(0.2, 1.9, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//36
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.1, 1.4, 0.0);
	glVertex3f(0.2, 1.4, 0.0);
	glVertex3f(0.2, 2.1, 0.0);
	glVertex3f(0.1, 2.1, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//37
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.0, 1.6, 0.0);
	glVertex3f(0.1, 1.6, 0.0);
	glVertex3f(0.1, 2.0, 0.0);
	glVertex3f(0.0, 2.0, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	//38
	glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);
	glVertex3f(0.3, 1.7, 0.0);
	glVertex3f(0.6, 1.7, 0.0);
	glVertex3f(0.6, 1.8, 0.0);
	glVertex3f(0.5, 1.8, 0.0);
	glVertex3f(0.5, 2.4, 0.0);
	glVertex3f(0.3, 2.4, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//38
	glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);
	glVertex3f(0.5, 2.1, 0.0);
	glVertex3f(0.6, 2.1, 0.0);
	glVertex3f(0.6, 2.2, 0.0);
	glVertex3f(0.5, 2.2, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//39
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.3, 1.6, 0.0);
	glVertex3f(0.4, 1.6, 0.0);
	glVertex3f(0.4, 1.7, 0.0);
	glVertex3f(0.3, 1.7, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//40
	glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);
	glVertex3f(0.4, 1.6, 0.0);
	glVertex3f(0.5, 1.6, 0.0);
	glVertex3f(0.5, 1.7, 0.0);
	glVertex3f(0.4, 1.7, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//41
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.3, 1.5, 0.0);
	glVertex3f(0.5, 1.5, 0.0);
	glVertex3f(0.5, 1.6, 0.0);
	glVertex3f(0.3, 1.6, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	//42
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.5, 1.4, 0.0);
	glVertex3f(0.7, 1.4, 0.0);
	glVertex3f(0.7, 1.6, 0.0);
	glVertex3f(0.5, 1.6, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//43
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.6, 1.6, 0.0);
	glVertex3f(0.7, 1.6, 0.0);
	glVertex3f(0.7, 1.7, 0.0);
	glVertex3f(0.6, 1.7, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//44
	glColor3ub(255, 140, 2);
    glBegin(GL_POLYGON);
	glVertex3f(0.6, 1.3, 0.0);
	glVertex3f(0.7, 1.3, 0.0);
	glVertex3f(0.7, 1.4, 0.0);
	glVertex3f(0.6, 1.4, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(-0.7, 0.2, 0.0);
	glVertex3f(0.3, 0.2, 0.0);
	glVertex3f(0.3, 1.5, 0.0);
	glVertex3f(-0.7, 1.5, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(0.2, 1.5, 0.0);
	glVertex3f(0.6, 1.5, 0.0);
	glVertex3f(0.6, 2.5, 0.0);
	glVertex3f(0.2, 2.5, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(0.5, 1.3, 0.0);
	glVertex3f(0.6, 1.3, 0.0);
	glVertex3f(0.6, 1.4, 0.0);
	glVertex3f(0.5, 1.4, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	//Border
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(0.6, 1.2, 0.0);
	glVertex3f(0.8, 1.2, 0.0);
	glVertex3f(0.8, 1.7, 0.0);
	glVertex3f(0.7, 1.7, 0.0);
	glVertex3f(0.7, 2.3, 0.0);
	glVertex3f(0.6, 2.3, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(0.3, 0.5, 0.0);
	glVertex3f(0.4, 0.5, 0.0);
	glVertex3f(0.4, 0.6, 0.0);
	glVertex3f(0.3, 0.6, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
	glVertex3f(0.3, 0.6, 0.0);
	glVertex3f(0.5, 0.6, 0.0);
	glVertex3f(0.5, 1.1, 0.0);
	glVertex3f(0.4, 1.1, 0.0);
	glVertex3f(0.4, 1.5, 0.0);
	glVertex3f(0.3, 1.5, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.4, 1.4, 0.0);
	glVertex3f(0.5, 1.4, 0.0);
	glVertex3f(0.5, 1.5, 0.0);
	glVertex3f(0.4, 1.5, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

		glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.5, 0.7, 0.0);
	glVertex3f(0.7, 0.7, 0.0);
	glVertex3f(0.7, 1.0, 0.0);
	glVertex3f(0.5, 1.0, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.1, 1.6, 0.0);
	glVertex3f(0.3, 1.6, 0.0);
	glVertex3f(0.3, 2.0, 0.0);
	glVertex3f(-0.1, 2.0, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 2.0, 0.0);
	glVertex3f(0.1, 2.0, 0.0);
	glVertex3f(0.1, 2.1, 0.0);
	glVertex3f(0.0, 2.1, 0.0);
	glEnd();


	glPopMatrix();

	glPushMatrix();

	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.1, 2.1, 0.0);
	glVertex3f(0.2, 2.1, 0.0);
	glVertex3f(0.2, 2.2, 0.0);
	glVertex3f(0.1, 2.2, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 1.5, 0.0);
	glVertex3f(0.1, 1.5, 0.0);
	glVertex3f(0.1, 1.6, 0.0);
	glVertex3f(0.0, 1.6, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.8, 0.6, 0.0);
	glVertex3f(-0.7, 0.6, 0.0);
	glVertex3f(-0.7, 1.2, 0.0);
	glVertex3f(-0.8, 1.2, 0.0);
	glEnd();

	glPopMatrix();
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

void drawPiranhaPlant(){
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



                    if((marioPositionX<40.377 || marioPositionX>41.677)) {
                        marioPositionX +=0.1f;
                        cameraX -=0.1f;
                    }

                    else if(marioPositionX>40.377 &&  marioPositionX<41.677 && marioPositionY>-0.4777){
                              marioPositionX +=0.2f;
                              cameraX -=0.2f;
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

    else if(moveLeft == true && marioPositionX<87.5 && marioPositionX>4 && insideTheCylinder==false){



               if(marioPositionX<40.4 || marioPositionX>41.7){
                    marioPositionX -=0.1f;
                    cameraX +=0.1f;

                }

                else{
                     if(marioPositionY>-0.47){
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



glPushMatrix();


    glBegin(GL_POLYGON);

    glColor3ub(255, 0, 0);

    glVertex3f(0.41, 0.90, 0.0);
    glVertex3f(0.41, 0.93, 0.0);
    glVertex3f(0.26, 0.93, 0.0);
    glVertex3f(0.26, 0.90, 0.0);
    glVertex3f(0.23, 0.90, 0.0);
    glVertex3f(0.23, 0.87, 0.0);
    glVertex3f(0.50, 0.87, 0.0);
    glVertex3f(0.50, 0.90, 0.0);

    glEnd();

    glPopMatrix();


    //mario hair part1 under cap greenish block
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

    glVertex3f(0.23, 0.87, 0.0);
    glVertex3f(0.32, 0.87, 0.0);
    glVertex3f(0.32, 0.84, 0.0);
    glVertex3f(0.23, 0.84, 0.0);

    glEnd();

    glPopMatrix();


    //mario hair part2/chip greenish block
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

    glVertex3f(0.29, 0.81, 0.0);
    glVertex3f(0.29, 0.84, 0.0);
    glVertex3f(0.26, 0.84, 0.0);
    glVertex3f(0.26, 0.78, 0.0);
    glVertex3f(0.32, 0.78, 0.0);
    glVertex3f(0.32, 0.81, 0.0);

    glEnd();

    glPopMatrix();


    //mario face centre part skinnish block1
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.32, 0.84, 0.0);
    glVertex3f(0.29, 0.84, 0.0);
    glVertex3f(0.29, 0.81, 0.0);
    glVertex3f(0.32, 0.81, 0.0);
    glVertex3f(0.32, 0.72, 0.0);
    glVertex3f(0.38, 0.72, 0.0);
    glVertex3f(0.38, 0.87, 0.0);
    glVertex3f(0.32, 0.87, 0.0);


    glEnd();

    glPopMatrix();


    //mario ear skinnish block2
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.26, 0.84, 0.0);
    glVertex3f(0.23, 0.84, 0.0);
    glVertex3f(0.23, 0.78, 0.0);
    glVertex3f(0.26, 0.78, 0.0);

    glEnd();

    glPopMatrix();


    //mario back side hair part3 greenish block1 adjacent to ear
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

    glVertex3f(0.23, 0.84, 0.0);
    glVertex3f(0.20, 0.84, 0.0);
    glVertex3f(0.20, 0.78, 0.0);
    glVertex3f(0.23, 0.78, 0.0);


    glEnd();


    glPopMatrix();


    //mario back side hair part4 greenish block2
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

    glVertex3f(0.23, 0.78, 0.0);
    glVertex3f(0.26, 0.78, 0.0);
    glVertex3f(0.26, 0.75, 0.0);
    glVertex3f(0.23, 0.75, 0.0);

    glEnd();

    glPopMatrix();


    //mario goola/ghaar skinnish block3 under chip
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.26, 0.78, 0.0);
    glVertex3f(0.32, 0.78, 0.0);
    glVertex3f(0.32, 0.72, 0.0);
    glVertex3f(0.26, 0.72, 0.0);

    glEnd();

    glPopMatrix();

    //mario eye + goof of nose greenish block1
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

    glVertex3f(0.41, 0.81, 0.0);
    glVertex3f(0.41, 0.87, 0.0);
    glVertex3f(0.38, 0.87, 0.0);
    glVertex3f(0.38, 0.81, 0.0);
    glVertex3f(0.44, 0.81, 0.0);
    glVertex3f(0.44, 0.78, 0.0);
    glVertex3f(0.41, 0.78, 0.0);


    glEnd();

    glPopMatrix();

    //mario gaal/back end goof of nose side skinnish block4
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.41, 0.81, 0.0);
    glVertex3f(0.38, 0.81, 0.0);
    glVertex3f(0.38, 0.78, 0.0);
    glVertex3f(0.41, 0.78, 0.0);

    glEnd();


    glPopMatrix();


    //mario frontal nose side skinnish block5
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.44, 0.84, 0.0);
    glVertex3f(0.44, 0.87, 0.0);
    glVertex3f(0.41, 0.87, 0.0);
    glVertex3f(0.41, 0.81, 0.0);
    glVertex3f(0.44, 0.81, 0.0);
    glVertex3f(0.44, 0.78, 0.0);
    glVertex3f(0.50, 0.78, 0.0);
    glVertex3f(0.50, 0.84, 0.0);

    glEnd();


    glPopMatrix();


    //mario frontal nose side small skinnish block6
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.50, 0.81, 0.0);
    glVertex3f(0.50, 0.78, 0.0);
    glVertex3f(0.53, 0.78, 0.0);
    glVertex3f(0.53, 0.81, 0.0);


    glEnd();

    glPopMatrix();

    //mario big goof greenish block2
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

    glVertex3f(0.38, 0.75, 0.0);
    glVertex3f(0.50, 0.75, 0.0);
    glVertex3f(0.50, 0.78, 0.0);
    glVertex3f(0.38, 0.78, 0.0);

    glEnd();

    glPopMatrix();

    //mario goola/thutni skinnish block7
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.38, 0.75, 0.0);
    glVertex3f(0.38, 0.72, 0.0);
    glVertex3f(0.44, 0.72, 0.0);
    glVertex3f(0.44, 0.75, 0.0);


    glEnd();


    glPopMatrix();


    //mario right hand greenish block
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

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


    glPopMatrix();

    //mario dress red block2 right side shoulder + centre part
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 0, 0);

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


    glPopMatrix();

    //mario dress red block3 left shoulder joint adjacent skinnish button
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 0, 0);

    glVertex3f(0.38, 0.63, 0.0);
    glVertex3f(0.41, 0.63, 0.0);
    glVertex3f(0.41, 0.72, 0.0);
    glVertex3f(0.38, 0.72, 0.0);


    glEnd();


    glPopMatrix();


    //mario goola/chest/centre part greenish block
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

    glVertex3f(0.32, 0.66, 0.0);
    glVertex3f(0.32, 0.72, 0.0);
    glVertex3f(0.38, 0.72, 0.0);
    glVertex3f(0.38, 0.66, 0.0);

    glEnd();


    glPopMatrix();


    //mario leg hand greenish block
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

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


    glPopMatrix();


    //mario left side button skinnish block8
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.41, 0.63, 0.0);
    glVertex3f(0.38, 0.63, 0.0);
    glVertex3f(0.38, 0.60, 0.0);
    glVertex3f(0.41, 0.60, 0.0);

    glEnd();


    glPopMatrix();


    //mario right side button skinnish block9
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.32, 0.63, 0.0);
    glVertex3f(0.29, 0.63, 0.0);
    glVertex3f(0.29, 0.60, 0.0);
    glVertex3f(0.32, 0.60, 0.0);

    glEnd();


    glPopMatrix();

    //mario dress red block4 left leg part
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 0, 0);

    glVertex3f(0.44, 0.57, 0.0);
    glVertex3f(0.44, 0.63, 0.0);
    glVertex3f(0.41, 0.63, 0.0);
    glVertex3f(0.41, 0.60, 0.0);
    glVertex3f(0.38, 0.60, 0.0);
    glVertex3f(0.38, 0.51, 0.0);
    glVertex3f(0.47, 0.51, 0.0);
    glVertex3f(0.47, 0.57, 0.0);

    glEnd();


    glPopMatrix();

    //mario dress red block5 right leg part
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 0, 0);

    glVertex3f(0.26, 0.57, 0.0);
    glVertex3f(0.26, 0.63, 0.0);
    glVertex3f(0.29, 0.63, 0.0);
    glVertex3f(0.29, 0.60, 0.0);
    glVertex3f(0.32, 0.60, 0.0);
    glVertex3f(0.32, 0.51, 0.0);
    glVertex3f(0.23, 0.51, 0.0);
    glVertex3f(0.23, 0.57, 0.0);

    glEnd();


    glPopMatrix();


    //mario right hand finger skinnish block10
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.23, 0.60, 0.0);
    glVertex3f(0.23, 0.63, 0.0);
    glVertex3f(0.17, 0.63, 0.0);
    glVertex3f(0.17, 0.54, 0.0);
    glVertex3f(0.23, 0.54, 0.0);
    glVertex3f(0.23, 0.57, 0.0);
    glVertex3f(0.26, 0.57, 0.0);
    glVertex3f(0.26, 0.60, 0.0);

    glEnd();


    glPopMatrix();


    //mario left hand finger skinnish block11
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(255, 184, 77);

    glVertex3f(0.47, 0.60, 0.0);
    glVertex3f(0.47, 0.63, 0.0);
    glVertex3f(0.53, 0.63, 0.0);
    glVertex3f(0.53, 0.54, 0.0);
    glVertex3f(0.47, 0.54, 0.0);
    glVertex3f(0.47, 0.57, 0.0);
    glVertex3f(0.44, 0.57, 0.0);
    glVertex3f(0.44, 0.60, 0.0);

    glEnd();


    glPopMatrix();


    //mario right leg greenish block
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

    glVertex3f(0.20, 0.48, 0.0);
    glVertex3f(0.20, 0.51, 0.0);
    glVertex3f(0.29, 0.51, 0.0);
    glVertex3f(0.29, 0.45, 0.0);
    glVertex3f(0.17, 0.45, 0.0);
    glVertex3f(0.17, 0.48, 0.0);


    glEnd();


    glPopMatrix();


    //mario left leg greenish block
    glPushMatrix();

    glBegin(GL_POLYGON);

    glColor3ub(153, 115, 0);

    glVertex3f(0.50, 0.48, 0.0);
    glVertex3f(0.50, 0.51, 0.0);
    glVertex3f(0.41, 0.51, 0.0);
    glVertex3f(0.41, 0.45, 0.0);
    glVertex3f(0.53, 0.45, 0.0);
    glVertex3f(0.53, 0.48, 0.0);

    glEnd();


    glPopMatrix();


    glPopMatrix();
}

void drawMarioRun(){
    float runSpeedInc = 0.25;
    glPushMatrix();

    glTranslatef(0.45, 0.9, 0);

    if(marioDirectionRight==false){
            glTranslatef(marioPositionX-1.3, marioPositionY, 0);
            glRotatef(180,0.0,1.0,0.0);
        }else{
             glTranslatef(marioPositionX-1.1, marioPositionY, 0);
            glRotatef(0,0.0,0.0,0.0);
        }
    glScalef(0.02, 0.025, 1);


    if(marioRunCounter >= 0.0 && marioRunCounter < 1.0)
    {

        glPushMatrix(); //Save the current state of transformations



        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);

        glVertex3f(4, 0, 0);
        glVertex3f(4, 1.5, 0);
        glVertex3f(-3.5, 1.5, 0);
        glVertex3f(-3.5, 0, 0);

        glEnd();

        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon

        //mario red cap lower block2
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);

        glVertex3f(-5, -1.5, 0);
        glVertex3f(8.5, -1.5, 0);
        glVertex3f(8.5, 0, 0);
        glVertex3f(-5, 0, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix(); //Undo the move to the center of the pentagon


        //mario hair block1 under cap
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.65, 0.30, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(-5, -1.5, 0);
        glVertex3f(-5, -2.5, 0);
        glVertex3f(0, -2.5, 0);
        glVertex3f(0, -1.5, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix(); //Undo the move to the center of the pentagon

        //mario hair with chip block1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(-0.65, 0.30, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);


        glVertex3f(-3.5, -2.5, 0);
        glVertex3f(-3.5, -4, 0);
        glVertex3f(-2, -4, 0);
        glVertex3f(-2, -2.5, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix(); //Undo the move to the center of the pentagon

        //mario hair with chip block2
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(-0.65, 0.30, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(-3.5, -4, 0);
        glVertex3f(-3.5, -5.5, 0);
        glVertex3f(0, -5.5, 0);
        glVertex3f(0, -4, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix(); //Undo the move to the center of the pentagon

        //mario ear skinnish block
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.65, -0.15, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(-5, -2.5, 0);
        glVertex3f(-5, -5.5, 0);
        glVertex3f(-3.5, -5.5, 0);
        glVertex3f(-3.5, -2.5, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix(); //Undo the move to the center of the pentagon

        //mario hair block1 backside of ear skinnish block
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(-0.65, -0.15, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(-5, -2.5, 0);
        glVertex3f(-6.5, -2.5, 0);
        glVertex3f(-6.5, -5.5, 0);
        glVertex3f(-5, -5.5, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix(); //Undo the move to the center of the pentagon

        //mario hair block2 backside of ear skinnish block
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -0.85, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(-6.5, -5.5, 0);
        glVertex3f(-6.5, -7, 0);
        glVertex3f(-3.5, -7, 0);
        glVertex3f(-3.5, -5.5, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix(); //Undo the move to the center of the pentagon

        //mario kopal skinnish block infront chip block1 under cap
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(-0.20, -0.85, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(0, -2.5, 0);
        glVertex3f(3, -2.5, 0);
        glVertex3f(3, -1.5, 0);
        glVertex3f(0, -1.5, 0);

        glEnd();
        //glutSolidCube(1.0);

        glPopMatrix(); //Undo the move to the center of the pentagon

        //mario gaal/chowal block1 infront of chip block
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(-0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(3, -2.5, 0);
        glVertex3f(-3.5, -2.5, 0);
        glVertex3f(-3.5, -4, 0);
        glVertex3f(3, -4, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix(); //Undo the move to the center of the pentagon

        //mario eye greenish part under cap
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(3, -1.5, 0);
        glVertex3f(3, -4, 0);
        glVertex3f(4, -4, 0);
        glVertex3f(4, -1.5, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon



        //mario gaal/chowal block2 infront of chip block beside small goof nose
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(0, -4, 0);
        glVertex3f(0, -5.5, 0);
        glVertex3f(4, -5.5, 0);
        glVertex3f(4, -4, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon

        //mario gaal/chowal block3 under chip block beside goof big part
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(-3.5, -5.5, 0);
        glVertex3f(-3.5, -8, 0);
        glVertex3f(3, -8, 0);
        glVertex3f(3, -5.5, 0);

        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon


        //mario eye greenish block infront of kopal under cap
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(4, -1.5, 0);
        glVertex3f(4, -4, 0);
        glVertex3f(5.5, -4, 0);
        glVertex3f(5.5, -1.5, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon


        //mario nose with goof small part
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(4, -4, 0);
        glVertex3f(4, -5.5, 0);
        glVertex3f(5.5, -5.5, 0);
        glVertex3f(5.5, -4, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon


        //mario eye skinnish block infront gaal skinnish block1 adjacent small goof of nose
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(5.5, -5.5, 0);
        glVertex3f(7, -5.5, 0);
        glVertex3f(7, -2.5, 0);
        glVertex3f(5.5, -2.5, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon


        //mario eye infront of gaal skinnish block2 adjacent small goof of nose
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(7, -5.5, 0);
        glVertex3f(11, -5.5, 0);
        glVertex3f(11, -4, 0);
        glVertex3f(7, -4, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon


        //mario goof big part
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(10, -5.5, 0);
        glVertex3f(3, -5.5, 0);
        glVertex3f(3, -7, 0);
        glVertex3f(10, -7, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon


        //mario thutni under big goof part
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(3, -8, 0);
        glVertex3f(7, -8, 0);
        glVertex3f(7, -6.5, 0);
        glVertex3f(3, -6.5, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon



        //mario left hand greenish block1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(1, -8, 0);
        glVertex3f(1, -9, 0);
        glVertex3f(4, -9, 0);
        glVertex3f(4, -8, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon

        //mario left hand greenish block2
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(3, -9, 0);
        glVertex3f(3, -10, 0);
        glVertex3f(7, -10, 0);
        glVertex3f(7, -9, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon

        //mario left hand greenish block3
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(5.5, -10, 0);
        glVertex3f(5.5, -11, 0);
        glVertex3f(10, -11, 0);
        glVertex3f(10, -10, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon


        //mario left hand with finger skinnish block1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(7, -9, 0);
        glVertex3f(7, -10, 0);
        glVertex3f(12, -10, 0);
        glVertex3f(12, -9, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon




        //mario left hand with finger skinnish block2
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);

        glVertex3f(10, -10, 0);
        glVertex3f(10, -11, 0);
        glVertex3f(12, -11, 0);
        glVertex3f(12, -10, 0);


        glEnd();

        //glutSolidCube(1.0);

        glPopMatrix();//Undo the move to the center of the pentagon


        //mario right hand arm block1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);


        glVertex3f(-2, -8, 0);
        glVertex3f(-7.5, -8, 0);
        glVertex3f(-7.5, -10, 0);
        glVertex3f(-2, -10, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon


        //mario right hand arm block2
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(-2, -10, 0);
        glVertex3f(-5, -10, 0);
        glVertex3f(-5, -11, 0);
        glVertex3f(-2, -11, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right hand arm finger block1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);


        glVertex3f(-7.5, -9, 0);
        glVertex3f(-10, -9, 0);
        glVertex3f(-10, -12.5, 0);
        glVertex3f(-7.5, -12.5, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right hand arm finger block2
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);


        glVertex3f(-7.5, -10, 0);
        glVertex3f(-7.5, -11, 0);
        glVertex3f(-6.5, -11, 0);
        glVertex3f(-6.5, -10, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon




        //mario right leg finger block1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);


        glVertex3f(-6.5, -14.5, 0);
        glVertex3f(-8.5, -14.5, 0);
        glVertex3f(-8.5, -15.5, 0);
        glVertex3f(-6.5, -15.5, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right leg finger block2
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);


        glVertex3f(-8.5, -15.5, 0);
        glVertex3f(-8.5, -16.5, 0);
        glVertex3f(-5, -16.5, 0);
        glVertex3f(-5, -15.5, 0);



        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right leg finger block3
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(-7.5, -16.5, 0);
        glVertex3f(-7.5, -17.5, 0);
        glVertex3f(-3.5, -17.5, 0);
        glVertex3f(-3.5, -16.5, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right leg red block1 adjacent to right leg finger block1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);


        glVertex3f(-6.5, -14.5, 0);
        glVertex3f(-6.5, -15.5, 0);
        glVertex3f(-2, -15.5, 0);
        glVertex3f(-2, -14.5, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario left leg red block2 adjacent to left leg finger block1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);


        glVertex3f(7, -14.5, 0);
        glVertex3f(3, -14.5, 0);
        glVertex3f(3, -15.5, 0);
        glVertex3f(7, -15.5, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right left leg centre part red block3
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);


        glVertex3f(-7.5, -13.5, 0);
        glVertex3f(-7.5, -14.5, 0);
        glVertex3f(7, -14.5, 0);
        glVertex3f(7, -13.5, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right left leg centre part red block4
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);


        glVertex3f(-6.5, -12.5, 0);
        glVertex3f(-6.5, -13.5, 0);
        glVertex3f(7, -13.5, 0);
        glVertex3f(7, -12.5, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right left leg centre part red block5
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);


        glVertex3f(-5, -11, 0);
        glVertex3f(-5, -12.5, 0);
        glVertex3f(5.5, -12.5, 0);
        glVertex3f(5.5, -11, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right left leg centre part red block5 adjacent right hand block2 greenish part
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);


        glVertex3f(-2, -11, 0);
        glVertex3f(0, -11, 0);
        glVertex3f(0, -10, 0);
        glVertex3f(-2, -10, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario centre body skinish button1 beside centre part red block6
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 184, 77);


        glVertex3f(0, -11, 0);
        glVertex3f(1, -11, 0);
        glVertex3f(1, -10, 0);
        glVertex3f(0, -10, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right left leg centre body red block7 beside skinish button1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);


        glVertex3f(1, -11, 0);
        glVertex3f(5.5, -11, 0);
        glVertex3f(5.5, -10, 0);
        glVertex3f(1, -10, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right left leg centre body red block8
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);


        glVertex3f(-2, -10, 0);
        glVertex3f(3, -10, 0);
        glVertex3f(3, -9, 0);
        glVertex3f(-2, -9, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario right left leg centre body red block9
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(255, 0, 0);


        glVertex3f(-2, -9, 0);
        glVertex3f(1, -9, 0);
        glVertex3f(1, -8, 0);
        glVertex3f(-2, -8, 0);


        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario left leg greenish block1
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(7, -15.5, 0);
        glVertex3f(11, -15.5, 0);
        glVertex3f(11, -12.5, 0);
        glVertex3f(7, -12.5, 0);

        glEnd();


        //glutSolidCube(4.0);

        glPopMatrix(); //Undo the move to the center of the pentagon



        //mario left leg greenish block2
        glPushMatrix(); //Save the current state of transformations

        //glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
        //glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
        //glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

        glBegin(GL_QUADS);

        glColor3ub(153, 115, 0);

        glVertex3f(11, -12.5, 0);
        glVertex3f(11, -11, 0);
        glVertex3f(10, -11, 0);
        glVertex3f(10, -12.5, 0);

        glEnd();

        glPopMatrix();

    }

    else if(marioRunCounter >= 1.0 && marioRunCounter < 2.0)
    {
        glTranslatef(0, 1.8, 0);
        glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(4, 0, 0);
	glVertex3f(4, 1.5, 0);
	glVertex3f(-3.5, 1.5, 0);
	glVertex3f(-3.5, 0, 0);

	glEnd();

	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario red cap lower block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(-5, -1.5, 0);
	glVertex3f(8.5, -1.5, 0);
	glVertex3f(8.5, 0, 0);
	glVertex3f(-5, 0, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


    //mario hair block1 under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.65, 0.30, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-5, -1.5, 0);
	glVertex3f(-5, -2.5, 0);
	glVertex3f(0, -2.5, 0);
	glVertex3f(0, -1.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair with chip block1
    glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.65, 0.30, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-3.5, -2.5, 0);
	glVertex3f(-3.5, -4, 0);
	glVertex3f(-2, -4, 0);
	glVertex3f(-2, -2.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair with chip block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.65, 0.30, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-3.5, -4, 0);
	glVertex3f(-3.5, -5.5, 0);
	glVertex3f(0, -5.5, 0);
    glVertex3f(0, -4, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario ear skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.65, -0.15, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(-5, -2.5, 0);
	glVertex3f(-5, -5.5, 0);
	glVertex3f(-3.5, -5.5, 0);
	glVertex3f(-3.5, -2.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair block1 backside of ear skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.65, -0.15, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

    glVertex3f(-5, -2.5, 0);
	glVertex3f(-6.5, -2.5, 0);
	glVertex3f(-6.5, -5.5, 0);
	glVertex3f(-5, -5.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair block2 backside of ear skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -0.85, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-6.5, -5.5, 0);
    glVertex3f(-6.5, -7, 0);
    glVertex3f(-3.5, -7, 0);
	glVertex3f(-3.5, -5.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario kopal skinnish block infront chip block1 under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.20, -0.85, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

	glVertex3f(0, -2.5, 0);
    glVertex3f(3, -2.5, 0);
    glVertex3f(3, -1.5, 0);
	glVertex3f(0, -1.5, 0);

	glEnd();
	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario gaal/chowal block1 infront of chip block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(3, -2.5, 0);
    glVertex3f(-3.5, -2.5, 0);
	glVertex3f(-3.5, -4, 0);
	glVertex3f(3, -4, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario eye greenish part under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

    glVertex3f(3, -1.5, 0);
    glVertex3f(3, -4, 0);
	glVertex3f(4, -4, 0);
	glVertex3f(4, -1.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon



	//mario gaal/chowal block2 infront of chip block beside small goof nose
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(0, -4, 0);
    glVertex3f(0, -5.5, 0);
	glVertex3f(4, -5.5, 0);
	glVertex3f(4, -4, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon

    //mario gaal/chowal block3 under chip block beside goof big part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(-3.5, -5.5, 0);
    glVertex3f(-3.5, -8, 0);
	glVertex3f(3, -8, 0);
	glVertex3f(3, -5.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario eye greenish block infront of kopal under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(4, -1.5, 0);
	glVertex3f(4, -4, 0);
	glVertex3f(5.5, -4, 0);
	glVertex3f(5.5, -1.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario nose with goof small part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(4, -4, 0);
	glVertex3f(4, -5.5, 0);
	glVertex3f(5.5, -5.5, 0);
	glVertex3f(5.5, -4, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario eye skinnish block infront gaal skinnish block1 adjacent small goof of nose
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(5.5, -5.5, 0);
	glVertex3f(7, -5.5, 0);
	glVertex3f(7, -2.5, 0);
	glVertex3f(5.5, -2.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


    //mario eye infront of gaal skinnish block2 adjacent small goof of nose
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(7, -5.5, 0);
	glVertex3f(11, -5.5, 0);
	glVertex3f(11, -4, 0);
	glVertex3f(7, -4, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario goof big part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(10, -5.5, 0);
	glVertex3f(3, -5.5, 0);
	glVertex3f(3, -7, 0);
	glVertex3f(10, -7, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario thutni under big goof part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(3, -8, 0);
    glVertex3f(7, -8, 0);
	glVertex3f(7, -6.5, 0);
	glVertex3f(3, -6.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon



	//mario left hand with finger skinnish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

	glVertex3f(-5, -10, 0);
	glVertex3f(-5, -12, 0);
	glVertex3f(-6.5, -12, 0);
	glVertex3f(-6.5, -10, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon




	//mario left hand with finger skinnish block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

	glVertex3f(-5, -12, 0);
	glVertex3f(-8, -12, 0);
	glVertex3f(-8, -14, 0);
	glVertex3f(-5, -14, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario right hand arm greenish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-5, -8, 0);
	glVertex3f(-5, -10, 0);
	glVertex3f(2, -10, 0);
	glVertex3f(2, -8, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


    //mario right hand arm greenish block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-5, -10, 0);
	glVertex3f(-5, -12, 0);
	glVertex3f(4, -12, 0);
	glVertex3f(4, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario right hand arm greenish block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(4, -10, 0);
	glVertex3f(4, -8, 0);
	glVertex3f(3, -8, 0);
	glVertex3f(3, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario right hand arm greenish block4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-3.5, -12, 0);
	glVertex3f(-3.5, -14, 0);
	glVertex3f(4, -14, 0);
	glVertex3f(4, -12, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon*/



	//mario right hand arm finger block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(4, -14, 0);
	glVertex3f(7, -14, 0);
	glVertex3f(7, -10, 0);
	glVertex3f(4, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right hand arm finger block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(7, -10, 0);
	glVertex3f(7, -8, 0);
	glVertex3f(5.5, -8, 0);
	glVertex3f(5.5, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right hand arm finger block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(7, -10, 0);
	glVertex3f(7, -12, 0);
	glVertex3f(10, -12, 0);
	glVertex3f(10, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg red block1 adjacent to right leg finger blocks
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-2, -20, 0);
	glVertex3f(-6.5, -20, 0);
	glVertex3f(-6.5, -18, 0);
	glVertex3f(-2, -18, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario left leg red block2 adjacent to left leg finger blocks
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(0, -18, 0);
	glVertex3f(0, -20, 0);
	glVertex3f(4, -20, 0);
	glVertex3f(4, -18, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right left leg centre part red block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-6.5, -18, 0);
	glVertex3f(5.5, -18, 0);
	glVertex3f(5.5, -16, 0);
	glVertex3f(-6.5, -16, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right left leg centre part red block4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(5.5, -16, 0);
	glVertex3f(5.5, -14, 0);
	glVertex3f(-5, -14, 0);
	glVertex3f(-5, -16, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right left leg centre part red block5
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-5, -14, 0);
	glVertex3f(-3.5, -14, 0);
	glVertex3f(-3.5, -12, 0);
	glVertex3f(-5, -12, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right left leg centre part red block5 under thutni
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(3, -10, 0);
	glVertex3f(3, -8, 0);
	glVertex3f(2, -8, 0);
	glVertex3f(2, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg greenish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-5, -14, 0);
	glVertex3f(-8, -14, 0);
	glVertex3f(-8, -16, 0);
	glVertex3f(-5, -16, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg greenish block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-8, -16, 0);
	glVertex3f(-8, -20, 0);
	glVertex3f(-6.5, -20, 0);
	glVertex3f(-6.5, -16, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg greenish block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-8, -18, 0);
	glVertex3f(-9.5, -18, 0);
	glVertex3f(-9.5, -22, 0);
	glVertex3f(-8, -22, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario left leg greenish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-2, -20, 0);
	glVertex3f(-2, -24, 0);
	glVertex3f(3, -24, 0);
	glVertex3f(3, -20, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario left leg greenish block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(3, -24, 0);
	glVertex3f(4, -24, 0);
	glVertex3f(4, -22, 0);
	glVertex3f(3, -22, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    }

    else if (marioRunCounter >= 2.0 && marioRunCounter < 3.0)
    {
        glTranslatef(0, -1.8, 0);
        glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(4, 0, 0);
	glVertex3f(4, 1.5, 0);
	glVertex3f(-3.5, 1.5, 0);
	glVertex3f(-3.5, 0, 0);

	glEnd();

	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario red cap lower block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(-5, -1.5, 0);
	glVertex3f(8.5, -1.5, 0);
	glVertex3f(8.5, 0, 0);
	glVertex3f(-5, 0, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


    //mario hair block1 under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.65, 0.30, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-5, -1.5, 0);
	glVertex3f(-5, -2.5, 0);
	glVertex3f(0, -2.5, 0);
	glVertex3f(0, -1.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair with chip block1
    glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.65, 0.30, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-3.5, -2.5, 0);
	glVertex3f(-3.5, -4, 0);
	glVertex3f(-2, -4, 0);
	glVertex3f(-2, -2.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair with chip block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.65, 0.30, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-3.5, -4, 0);
	glVertex3f(-3.5, -5.5, 0);
	glVertex3f(0, -5.5, 0);
    glVertex3f(0, -4, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario ear skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.65, -0.15, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(-5, -2.5, 0);
	glVertex3f(-5, -5.5, 0);
	glVertex3f(-3.5, -5.5, 0);
	glVertex3f(-3.5, -2.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair block1 backside of ear skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.65, -0.15, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

    glVertex3f(-5, -2.5, 0);
	glVertex3f(-6.5, -2.5, 0);
	glVertex3f(-6.5, -5.5, 0);
	glVertex3f(-5, -5.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair block2 backside of ear skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -0.85, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-6.5, -5.5, 0);
    glVertex3f(-6.5, -7, 0);
    glVertex3f(-3.5, -7, 0);
	glVertex3f(-3.5, -5.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario kopal skinnish block infront chip block1 under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.20, -0.85, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

	glVertex3f(0, -2.5, 0);
    glVertex3f(3, -2.5, 0);
    glVertex3f(3, -1.5, 0);
	glVertex3f(0, -1.5, 0);

	glEnd();
	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario gaal/chowal block1 infront of chip block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(3, -2.5, 0);
    glVertex3f(-3.5, -2.5, 0);
	glVertex3f(-3.5, -4, 0);
	glVertex3f(3, -4, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario eye greenish part under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

    glVertex3f(3, -1.5, 0);
    glVertex3f(3, -4, 0);
	glVertex3f(4, -4, 0);
	glVertex3f(4, -1.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon



	//mario gaal/chowal block2 infront of chip block beside small goof nose
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(0, -4, 0);
    glVertex3f(0, -5.5, 0);
	glVertex3f(4, -5.5, 0);
	glVertex3f(4, -4, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon

    //mario gaal/chowal block3 under chip block beside goof big part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(-3.5, -5.5, 0);
    glVertex3f(-3.5, -8, 0);
	glVertex3f(3, -8, 0);
	glVertex3f(3, -5.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario eye greenish block infront of kopal under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(4, -1.5, 0);
	glVertex3f(4, -4, 0);
	glVertex3f(5.5, -4, 0);
	glVertex3f(5.5, -1.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario nose with goof small part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(4, -4, 0);
	glVertex3f(4, -5.5, 0);
	glVertex3f(5.5, -5.5, 0);
	glVertex3f(5.5, -4, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario eye skinnish block infront gaal skinnish block1 adjacent small goof of nose
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(5.5, -5.5, 0);
	glVertex3f(7, -5.5, 0);
	glVertex3f(7, -2.5, 0);
	glVertex3f(5.5, -2.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


    //mario eye infront of gaal skinnish block2 adjacent small goof of nose
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(7, -5.5, 0);
	glVertex3f(11, -5.5, 0);
	glVertex3f(11, -4, 0);
	glVertex3f(7, -4, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario goof big part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(10, -5.5, 0);
	glVertex3f(3, -5.5, 0);
	glVertex3f(3, -7, 0);
	glVertex3f(10, -7, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario thutni under big goof part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(3, -8, 0);
    glVertex3f(7, -8, 0);
	glVertex3f(7, -6.5, 0);
	glVertex3f(3, -6.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario left hand under thutni greenish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(0, -8, 0);
	glVertex3f(0, -9, 0);
	glVertex3f(4, -9, 0);
	glVertex3f(4, -8, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon

    //mario left hand greenish block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(3, -9, 0);
	glVertex3f(3, -10, 0);
	glVertex3f(5.5, -10, 0);
	glVertex3f(5.5, -9, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon



	//mario left hand with finger skinnish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

	glVertex3f(5.5, -10, 0);
	glVertex3f(5.5, -11, 0);
	glVertex3f(7, -11, 0);
	glVertex3f(7, -10, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon



	//mario right hand arm greenish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-6.5, -9, 0);
	glVertex3f(-6.5, -12, 0);
	glVertex3f(-5, -12, 0);
	glVertex3f(-5, -9, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


    //mario right hand arm greenish block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-5, -8, 0);
	glVertex3f(-5, -13, 0);
	glVertex3f(-3.5, -13, 0);
	glVertex3f(-3.5, -8, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario right hand arm greenish block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-3.5, -8, 0);
	glVertex3f(-3.5, -14, 0);
	glVertex3f(-2, -14, 0);
	glVertex3f(-2, -8, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario right hand arm greenish block4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-2, -9, 0);
	glVertex3f(-2, -10, 0);
	glVertex3f(0, -10, 0);
	glVertex3f(0, -9, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right hand arm greenish block5
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-2, -11, 0);
	glVertex3f(-2, -12, 0);
	glVertex3f(0, -12, 0);
	glVertex3f(0, -11, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right hand arm finger block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(-2, -12, 0);
	glVertex3f(-2, -13, 0);
	glVertex3f(3, -13, 0);
	glVertex3f(3, -12, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right hand arm finger block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(-2, -13, 0);
	glVertex3f(-2, -14, 0);
	glVertex3f(2, -14, 0);
	glVertex3f(2, -13, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon




	//mario right red block1 adjacent to/under right hand greenish blocks
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-6.5, -12, 0);
	glVertex3f(-6.5, -13, 0);
	glVertex3f(-5, -13, 0);
	glVertex3f(-5, -12, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right red block2 adjacent to/under right hand greenish blocks
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-5, -13, 0);
	glVertex3f(-5, -14, 0);
	glVertex3f(-3.5, -14, 0);
	glVertex3f(-3.5, -13, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre red block3 under thutni/goola adjacent right hand greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-2, -8, 0);
	glVertex3f(-2, -9, 0);
	glVertex3f(0, -9, 0);
	glVertex3f(0, -8, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right left hand centre part red block4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(0, -9, 0);
	glVertex3f(0, -10, 0);
	glVertex3f(3, -10, 0);
	glVertex3f(3, -9, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right left centre part red block5 beside right hand greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-2, -10, 0);
	glVertex3f(-2, -11, 0);
	glVertex3f(2, -11, 0);
	glVertex3f(2, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right left centre part red block6 adjacent left hand greenish part and skinnish finger
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(3, -10, 0);
	glVertex3f(3, -11, 0);
	glVertex3f(5.5, -11, 0);
	glVertex3f(5.5, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block7 beside right hand finger blocks
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(0, -11, 0);
	glVertex3f(0, -12, 0);
	glVertex3f(7, -12, 0);
	glVertex3f(7, -11, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block8
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(3, -12, 0);
	glVertex3f(3, -13, 0);
	glVertex3f(7, -13, 0);
	glVertex3f(7, -12, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block9 adjacent left leg finger blocks
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(2, -13, 0);
	glVertex3f(2, -14, 0);
	glVertex3f(5.5, -14, 0);
	glVertex3f(5.5, -13, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block10 adjacent right leg finger blocks
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-3.5, -14, 0);
	glVertex3f(-3.5, -15, 0);
	glVertex3f(2, -15, 0);
	glVertex3f(2, -14, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body skinish button1 under centre part red block4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(3, -10, 0);
	glVertex3f(2, -10, 0);
	glVertex3f(2, -11, 0);
	glVertex3f(3, -11, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario left leg greenish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(2, -14, 0);
	glVertex3f(2, -15, 0);
	glVertex3f(7, -15, 0);
	glVertex3f(7, -14, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg + left leg greenish block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-3.5, -15, 0);
	glVertex3f(-3.5, -16, 0);
	glVertex3f(7, -16, 0);
	glVertex3f(7, -15, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon




	//mario right leg leg greenish block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-3.5, -16, 0);
	glVertex3f(-3.5, -17, 0);
	glVertex3f(3, -17, 0);
	glVertex3f(3, -16, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


    }

    marioRunCounter += runSpeedInc;
    if(marioRunCounter >= 3.0)
    {
        marioRunCounter = 0.0;
    }

	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon
}

void drawMarioJump(){
    glPushMatrix(); //Save the current state of transformations
    glTranslatef(0.45, 0.9, 0);

    if(marioDirectionRight==false){
            glTranslatef(marioPositionX-1.3, marioPositionY, 0);
            glRotatef(180,0.0,1.0,0.0);
        }else{
             glTranslatef(marioPositionX-1.1, marioPositionY, 0);
            glRotatef(0,0.0,0.0,0.0);
        }
    glScalef(0.02, 0.025, 1);

    glPushMatrix();

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(4, 0, 0);
	glVertex3f(4, 1.5, 0);
	glVertex3f(-3.5, 1.5, 0);
	glVertex3f(-3.5, 0, 0);

	glEnd();

	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario red cap lower block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.80, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.50, 0.50, 0.40); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(-5, -1.5, 0);
	glVertex3f(8.5, -1.5, 0);
	glVertex3f(8.5, 0, 0);
	glVertex3f(-5, 0, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


    //mario hair block1 under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.65, 0.30, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-5, -1.5, 0);
	glVertex3f(-5, -2.5, 0);
	glVertex3f(0, -2.5, 0);
	glVertex3f(0, -1.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair with chip block1
    glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.65, 0.30, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-3.5, -2.5, 0);
	glVertex3f(-3.5, -4, 0);
	glVertex3f(-2, -4, 0);
	glVertex3f(-2, -2.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair with chip block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.65, 0.30, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-3.5, -4, 0);
	glVertex3f(-3.5, -5.5, 0);
	glVertex3f(0, -5.5, 0);
    glVertex3f(0, -4, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario ear skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.65, -0.15, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(-5, -2.5, 0);
	glVertex3f(-5, -5.5, 0);
	glVertex3f(-3.5, -5.5, 0);
	glVertex3f(-3.5, -2.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair block1 backside of ear skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.65, -0.15, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

    glVertex3f(-5, -2.5, 0);
	glVertex3f(-6.5, -2.5, 0);
	glVertex3f(-6.5, -5.5, 0);
	glVertex3f(-5, -5.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario hair block2 backside of ear skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -0.85, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-6.5, -5.5, 0);
    glVertex3f(-6.5, -7, 0);
    glVertex3f(-3.5, -7, 0);
	glVertex3f(-3.5, -5.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario kopal skinnish block infront chip block1 under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.20, -0.85, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.50, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

	glVertex3f(0, -2.5, 0);
    glVertex3f(3, -2.5, 0);
    glVertex3f(3, -1.5, 0);
	glVertex3f(0, -1.5, 0);

	glEnd();
	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario gaal/chowal block1 infront of chip block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(-0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(3, -2.5, 0);
    glVertex3f(-3.5, -2.5, 0);
	glVertex3f(-3.5, -4, 0);
	glVertex3f(3, -4, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario eye greenish part under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

    glVertex3f(3, -1.5, 0);
    glVertex3f(3, -4, 0);
	glVertex3f(4, -4, 0);
	glVertex3f(4, -1.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon



	//mario gaal/chowal block2 infront of chip block beside small goof nose
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(0, -4, 0);
    glVertex3f(0, -5.5, 0);
	glVertex3f(4, -5.5, 0);
	glVertex3f(4, -4, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon

    //mario gaal/chowal block3 under chip block beside goof big part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(-3.5, -5.5, 0);
    glVertex3f(-3.5, -8, 0);
	glVertex3f(3, -8, 0);
	glVertex3f(3, -5.5, 0);

	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario eye greenish block infront of kopal under cap
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(4, -1.5, 0);
	glVertex3f(4, -4, 0);
	glVertex3f(5.5, -4, 0);
	glVertex3f(5.5, -1.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario nose with goof small part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(4, -4, 0);
	glVertex3f(4, -5.5, 0);
	glVertex3f(5.5, -5.5, 0);
	glVertex3f(5.5, -4, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario eye skinnish block infront gaal skinnish block1 adjacent small goof of nose
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(5.5, -5.5, 0);
	glVertex3f(7, -5.5, 0);
	glVertex3f(7, -2.5, 0);
	glVertex3f(5.5, -2.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


    //mario eye infront of gaal skinnish block2 adjacent small goof of nose
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(7, -5.5, 0);
	glVertex3f(10, -5.5, 0);
	glVertex3f(10, -4, 0);
	glVertex3f(7, -4, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario goof big part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(10, -5.5, 0);
	glVertex3f(3, -5.5, 0);
	glVertex3f(3, -7, 0);
	glVertex3f(10, -7, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario thutni under big goof part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

    glVertex3f(3, -8, 0);
    glVertex3f(7, -8, 0);
	glVertex3f(7, -6.5, 0);
	glVertex3f(3, -6.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon



	//mario left hand greenish small part adjacent thutni under big goof block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(7, -8, 0);
	glVertex3f(8.5, -8, 0);
	glVertex3f(8.5, -6.5, 0);
	glVertex3f(7, -6.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario left hand elbow small part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(10, -5.5, 0);
	glVertex3f(11, -5.5, 0);
	glVertex3f(11, -4, 0);
	glVertex3f(10, -4, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon

    //mario left hand arm block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(11, -4, 0);
	glVertex3f(11, -1.5, 0);
	glVertex3f(7, -1.5, 0);
	glVertex3f(7, -4, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon

    //mario left hand arm block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(11, -1.5, 0);
	glVertex3f(11, 0, 0);
	glVertex3f(8, 0, 0);
	glVertex3f(8, -1.5, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario left hand with finger skinnish block
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.20, -1.10, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.40, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

	glVertex3f(11, 3, 0);
	glVertex3f(7, 3, 0);
	glVertex3f(7, 0, 0);
	glVertex3f(11, 0, 0);


	glEnd();

	//glutSolidCube(1.0);

	glPopMatrix();//Undo the move to the center of the pentagon


	//mario right hand arm block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-9, -9, 0);
	glVertex3f(-2, -9, 0);
	glVertex3f(-2, -8, 0);
	glVertex3f(-9, -8, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


    //mario right hand arm block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-10.5, -10, 0);
	glVertex3f(0, -10, 0);
	glVertex3f(0, -9, 0);
	glVertex3f(-10.5, -9, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario right hand arm block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-9, -12, 0);
	glVertex3f(0, -12, 0);
	glVertex3f(0, -10, 0);
	glVertex3f(-9, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

    //mario right hand arm block4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-3.5, -13, 0);
	glVertex3f(-2, -13, 0);
	glVertex3f(-2, -12, 0);
	glVertex3f(-3.5, -12, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right hand arm finger block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(-9, -10, 0);
	glVertex3f(-12, -10, 0);
	glVertex3f(-12, -13, 0);
	glVertex3f(-9, -13, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right hand arm finger block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(-9, -13, 0);
	glVertex3f(-10.5, -13, 0);
	glVertex3f(-10.5, -14, 0);
	glVertex3f(-9, -14, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right hand arm finger block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(-9, -13, 0);
	glVertex3f(-8, -13, 0);
	glVertex3f(-8, -12, 0);
	glVertex3f(-9, -12, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon




	//mario right leg finger block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-10.5, -18, 0);
	glVertex3f(-9, -18, 0);
	glVertex3f(-9, -15, 0);
	glVertex3f(-10.5, -15, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg finger block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-9, -16.5, 0);
	glVertex3f(-7.5, -16.5, 0);
	glVertex3f(-7.5, -14, 0);
	glVertex3f(-9, -14, 0);



	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg finger block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(-6.5, -16.5, 0);
	glVertex3f(-6.5, -13, 0);
	glVertex3f(-7.5, -13, 0);
	glVertex3f(-7.5, -16.5, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg finger block4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);


	glVertex3f(-6.5, -15, 0);
	glVertex3f(-5, -15, 0);
	glVertex3f(-5, -14, 0);
	glVertex3f(-6.5, -14, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg red block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-6.5, -14, 0);
	glVertex3f(-5, -14, 0);
	glVertex3f(-5, -12, 0);
	glVertex3f(-6.5, -12, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg red block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-5, -15, 0);
	glVertex3f(-6.5, -15, 0);
	glVertex3f(-6.5, -18, 0);
	glVertex3f(-5, -18, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg red block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-3.5, -12, 0);
	glVertex3f(-5, -12, 0);
	glVertex3f(-5, -18, 0);
	glVertex3f(-3.5, -18, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg red block4
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-2, -13, 0);
	glVertex3f(-3.5, -13, 0);
	glVertex3f(-3.5, -18, 0);
	glVertex3f(-2, -18, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario right leg red block5
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(0, -12, 0);
	glVertex3f(-2, -12, 0);
	glVertex3f(-2, -18, 0);
	glVertex3f(0, -18, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario dress joint with shoulder red small block1 under thutni
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(-2, -9, 0);
	glVertex3f(0, -9, 0);
	glVertex3f(0, -8, 0);
	glVertex3f(-2, -8, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario under goola greenish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

    glVertex3f(0, -9, 0);
	glVertex3f(4, -9, 0);
	glVertex3f(4, -8, 0);
	glVertex3f(0, -8, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario dress joint with shoulder red small block2 under thutni
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(4, -9, 0);
	glVertex3f(5.5, -9, 0);
	glVertex3f(5.5, -8, 0);
	glVertex3f(4, -8, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario under goola greenish small part beside joint red block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

    glVertex3f(5.5, -9, 0);
	glVertex3f(7, -9, 0);
	glVertex3f(7, -8, 0);
	glVertex3f(5.5, -8, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario under goola greenish block3
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

    glVertex3f(5.5, -9, 0);
	glVertex3f(2, -9, 0);
	glVertex3f(2, -10, 0);
	glVertex3f(5.5, -10, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block1 goolas greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(2, -9, 0);
	glVertex3f(0, -9, 0);
	glVertex3f(0, -16.5, 0);
	glVertex3f(2, -16.5, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block2 under goolas greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(2, -10, 0);
	glVertex3f(2, -12, 0);
	glVertex3f(3.5, -12, 0);
	glVertex3f(3.5, -10, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body skinish button1 under red block2 under goolas greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);


	glVertex3f(2, -12, 0);
	glVertex3f(2, -13, 0);
	glVertex3f(3.5, -13, 0);
	glVertex3f(3.5, -12, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block3 under goolas greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(2, -13, 0);
	glVertex3f(2, -16.5, 0);
	glVertex3f(3.5, -16.5, 0);
	glVertex3f(3.5, -13, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



    //mario centre body red block4 under goolas greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(3.5, -9, 0);
	glVertex3f(3.5, -16.5, 0);
	glVertex3f(4, -16.5, 0);
	glVertex3f(4, -9, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block5 under goolas greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);


	glVertex3f(3.5, -9, 0);
	glVertex3f(3.5, -15, 0);
	glVertex3f(5.5, -15, 0);
	glVertex3f(5.5, -9, 0);


	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block6 adjacent red block joint under goolas greenish small part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(5.5, -9, 0);
	glVertex3f(5.5, -12, 0);
	glVertex3f(7, -12, 0);
	glVertex3f(7, -9, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon


	//mario centre body skinish button2 under red block6 under goolas greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 184, 77);

	glVertex3f(5.5, -12, 0);
	glVertex3f(5.5, -13, 0);
	glVertex3f(7, -13, 0);
	glVertex3f(7, -12, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario centre body red block7 adjacent red block5 joint under skinnish button2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(5.5, -13, 0);
	glVertex3f(5.5, -15, 0);
	glVertex3f(7, -15, 0);
	glVertex3f(7, -13, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon




	//mario centre body red block8 adjacent red block7 adjacent left leg greenish part
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(255, 0, 0);

	glVertex3f(7, -12, 0);
	glVertex3f(7, -15, 0);
	glVertex3f(8.5, -15, 0);
	glVertex3f(8.5, -12, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario left leg greenish block1
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(8.5, -12, 0);
	glVertex3f(8.5, -15, 0);
	glVertex3f(11, -15, 0);
	glVertex3f(11, -12, 0);

	glEnd();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon



	//mario left leg greenish block2
	glPushMatrix(); //Save the current state of transformations

	//glTranslatef(0.0, 0.0, 0.0); //Move to the center of the pentagon
	//glRotatef(_angle, 0.0, 1.0, 0.0); //Rotate about the y-axis
	//glScalef(0.25, 0.30, 0.15); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_QUADS);

	glColor3ub(153, 115, 0);

	glVertex3f(11, -12, 0);
	glVertex3f(11, -9, 0);
	glVertex3f(10, -9, 0);
	glVertex3f(10, -12, 0);

	glEnd();

	glPopMatrix();


	//glutSolidCube(4.0);

	glPopMatrix(); //Undo the move to the center of the pentagon

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
                if(previousMarioState == 1)
                    marioState = 1;
                else
                    marioState = 0;
            }
            else if (marioPositionY <= -2.95)
            {
                jumpCounter = 0;
                jumpTopReached = false;
                jumpMarioKeyPressed = false;
                if(previousMarioState == 1)
                    marioState = 1;
                else
                    marioState = 0;
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
        if(jumpBottomCollisionOccuredCoin && brickCollisionStatus[brickNumber]==true && (brickNumber == 1 || brickNumber == 4 || brickNumber == 6 || (brickNumber >=9 && brickNumber <= 12) || brickNumber==15 || brickNumber == 16 || (brickNumber >= 19 && brickNumber <= 21) || (brickNumber >= 25 && brickNumber <=30) || (brickNumber >=33 && brickNumber <=35)))
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

void animatePiranhaPlant(){
    if(piranhaWaitCounter <= 0)
    {
        if(movePlantUp == true)
        {
            if(piranhaPlantY>=0)
            {
                movePlantUp = false;
                piranhaWaitCounter = 20;
            }
            else
            {
                piranhaPlantY += 0.1;
            }
        }
        else
        {
            if(piranhaPlantY<=-4)
            {
                movePlantUp = true;
                piranhaWaitCounter = 20;
            }
            else
            {
                piranhaPlantY -= 0.1;
            }
        }
    }
    else
    {
        piranhaWaitCounter -= 1;
    }

}

void writeHighscore(){
    ifstream scorefile ("highscore.txt");
    int previousScore;
    bool isEmpty = (scorefile.get(), scorefile.eof());
    if(isEmpty)
    {
        ofstream scorefile;
        scorefile.open("highscore.txt", ofstream::out | ofstream::trunc);
        scorefile << " " << score <<endl;
        scorefile.close();
    }
    else
    {
        scorefile >> previousScore;
        cout << "Previous Score: " << previousScore << endl;
        if(score > previousScore)
        {
            ofstream scorefile;
            scorefile.open("highscore.txt", ofstream::out | ofstream::trunc);
            scorefile << " " << score <<endl;
            scorefile.close();
        }
    }



}

void handleKeypress(unsigned char key, int x, int y) {


        if(key=='w' || key==' '){
               enableSound("jump");
               jumpMarioKeyPressed=true;
               previousMarioState = marioState;
               marioState = 2;
        }

        if(key=='d'){
               marioDirectionRight=true;
               moveRight = true;
               marioState = 1;
        }

        if(key=='a'){
                marioDirectionRight=false;
                moveLeft = true;
                marioState = 1;

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
                mciSendString("resume background", 0, 0, 0);
            }
            else if(!isPaused)
            {
                isPaused = true;
                mciSendString("pause background", 0, 0, 0);
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

           if(key=='r' && gamePauseMenu==true){

                restartGame();
                enableSound("startlevel");
         }



         if(key == 0x1B){


                if(gamePauseMenu==true){
                     gamePauseMenu=false;;
                }else{
                    gamePauseMenu=true;;
               }


                if(isPaused)
                {
                    isPaused = false;
                    mciSendString("resume background", 0, 0, 0);
                }
                else if(!isPaused)
                {
                    isPaused = true;
                    mciSendString("pause background", 0, 0, 0);
                }
                enableSound("pause");


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
               marioState = 0;
        }
         if(key=='a'){
                marioDirectionRight=false;
                moveLeft = false;
                marioState = 0;
         }


        glutPostRedisplay();

        }

void initValues(){
    for(int i=0; i<100; i++)
        {brickCollisionStatus[i] = true;};
}

void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void print(float x, float y, float z,char* text) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(1/552.0,1/552.0,1);
    glRotatef(z,0,0,1);

      for( char* p = text; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
}

//Initializes 3D rendering
void initRendering() {

    glClearColor(backgroundColor[0][0],backgroundColor[0][1],backgroundColor[0][2],1.0);

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


	Image* image11 = loadBMP("images/menu.bmp");
	_textureMenu = loadTexture(image11);
	delete image11;


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



const int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;


    //showScore
    glPushMatrix();
    glColor3f(1,1,1);
    print(abs(cameraX)-4,2,0,"Score: 5");
    glPopMatrix();

    //showScore
    glPushMatrix();
    glColor3f(1,1,1);
    print(abs(cameraX)-4,1.5,0,"Life: 3");
    glPopMatrix();




    //DrawMenu
    if(gamePauseMenu){
            glPushMatrix();
            drawMenu();
            glPopMatrix();
    }


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



    if(marioState==0)
    {
        glPushMatrix();

        drawMarioNew();

        glPopMatrix();
    }
    else if (marioState == 1)
    {
        glPushMatrix();

        drawMarioRun();

        glPopMatrix();
    }
    else if (marioState == 2)
    {
        glPushMatrix();

        drawMarioJump();

        glPopMatrix();
    }






//draw floor
    glPushMatrix();
    glTranslatef(-2, -3, 0);
        drawFloor(40);
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



//    glPushMatrix();
//    glTranslatef (32, -3, 0);
//    drawCylinder();
//    glPopMatrix();

//    draw piranta plant






    //draw brick+score  group





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







    //underground score bricks+ normal brick


    glPushMatrix();
        drawBrick(201.5,-0.5,1);
    glPopMatrix();
    tempBrickCounter++;

     glPushMatrix();
        drawBrick(202,-0.5,1);
    glPopMatrix();
    tempBrickCounter++;


    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(202.5,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(202.5,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;



    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(203,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(203,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;



    glPushMatrix();
        drawBrick(203.5,-0.5,1);
    glPopMatrix();
    tempBrickCounter++;
     glPushMatrix();
        drawBrick(204,-0.5,1);
    glPopMatrix();
    tempBrickCounter++;




    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(208.5,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(208.5,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(209,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(209,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;


    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(209.5,-0.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(209.5,-0.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;




        glPushMatrix();
            drawBrick(214,-0.5,1);
            glPopMatrix();
        tempBrickCounter++;

        glPushMatrix();
            drawBrick(214.5,-0.5,1);
            glPopMatrix();
        tempBrickCounter++;

        glPushMatrix();
            drawBrick(215,-0.5,1);
            glPopMatrix();
        tempBrickCounter++;







    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(214,1.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(214,1.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(214.5,1.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(214.5,1.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(215,1.5,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(215,1.5,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;






    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(220,0,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(220,0,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;

    if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(220.5,0,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(220.5,0,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;


     if(brickCollisionStatus[brickLocation[tempBrickCounter]])
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(221,0,1,true);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
        //glTranslatef(5.5, -0.5, 0);
            drawScoreBrick(221.5,0,1,false);
        glPopMatrix();
    }
    tempBrickCounter++;



    //glPopMatrix();




    //drawenemy

    //drawenemy

    int arrayLength = sizeof(emeneyPositionx) / sizeof(int);

    if(storeLocation==true){generateRandomEnemy();}

    for(int i=0;i<arrayLength;i++){

         if(i<5){
                glPushMatrix();
                glTranslatef(emeneyPositionx[i], -2.3, 0);
                glScalef(0.25, 0.25, 1.0);
                drawEnemy();
                glPopMatrix();
         }else{
                glPushMatrix();
                glTranslatef(emeneyPositionx[i], -2.5, 0);
                glScalef(0.25, 0.25, 1.0);
                drawEnemy2();
                glPopMatrix();
         }



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




   //drawPiranha

       int totalPiranha = sizeof(piranhaPlantX) / sizeof(int);

    for(int i=0 ;i <totalPiranha; i++){
            glPushMatrix();
                glTranslatef(piranhaPlantX[i], piranhaPlantY, 0);
                drawPiranhaPlant();
            glPopMatrix();
    }




   storeLocation = false;
   bottomAreaAssignCount=0;
   tempBrickCounter = 0;
   glutSwapBuffers();
}

void update(int value) {

//   if(marioPositionX>150){
//            backgroundColor[0][0]=0.4;
//            backgroundColor[0][1]=0.1;
//            backgroundColor[0][2]=0.8;
//    }


    marioCollisionWithPiranha();

    if(generateRandomEnemyInsidePipe==1){
        generateRandomEnemy2();
        generateRandomEnemyInsidePipe=2;
    }

    if(marioPositionX>199 && generateRandomEnemyInsidePipe==0){
          generateRandomEnemyInsidePipe=1;
      }


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
        gamePauseMenu=true;
        levelComplete=2;
        writeHighscore();
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
             collideMario(0,0);
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

    //glutFullScreen();           // making the window full screen

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
