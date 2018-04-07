#include <iostream>
#include <stdlib.h>
#include <math.h>

#include <GL/GLUT.h>

#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"

using namespace std;


float _cameraAngle = 0.0;


//cameraProperty
float cameraX=-5.52;


//Mario Property
float marioPositionX=3;
float marioPositionY=-2.95;
bool jumpMarioKeyPressed=false;
bool jumpTopReached=false;
bool marioDirectionRight=true;



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

GLuint _textureId;

//The id of the texture


void moveRight(){
          marioPositionX +=0.04f;
         cameraX -=0.04f;
}

void moveLeft(){
            if(marioPositionX>2.8){
                marioPositionX -=0.04f;
                cameraX +=0.04f;
            }
}


void handleKeypress(unsigned char key, int x, int y) {

	switch (key) {

case 'w':
case ' ':
      jumpMarioKeyPressed=true;
      break;
case 'd':
       moveRight();
      break;
 case 'a':
        moveLeft();
      break;


glutPostRedisplay();


	}
}


void drawFloor(int length){

    float translateFloorX=0.0;


    GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);


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


void drawBrick(int length){


    float translateFloorX=0.0;


    GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 0.0f, 0.0f);


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


void drawMario(){
    glPushMatrix();

    glTranslatef(marioPositionX, marioPositionY, 0);

    //glRotatef(-180,0,1,0);
    //glFrustum(l,r,b,t,n,f)


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

    if(jumpMarioKeyPressed){
        if(jumpTopReached==false){
            if(marioPositionY<0.6){
                marioPositionY += .2f;
            }
            else{
                jumpTopReached = true;
            }
        }
        else if(jumpTopReached==true){

            if(marioPositionY>-2.95){
                marioPositionY-= .2f;
            }
            else{
                jumpMarioKeyPressed = false;
                jumpTopReached=false;
            }
        }
     }

}


//Initializes 3D rendering
void initRendering() {
    glClearColor(0.56, 0.56, 0.96, 0.0);
    glEnable(GL_DEPTH_TEST);



    //texture
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	Image* image = loadBMP("images/brick.bmp");
	_textureId = loadTexture(image);
	delete image;
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






    glPushMatrix();
        drawMario();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0, -3, 0);
        drawFloor(40);
    glPopMatrix();


    //Draw Brick
    glPushMatrix();
    glTranslatef(5, -0.5, 0);
        drawBrick(1);
    glPopMatrix();

        glPushMatrix();
    glTranslatef(7, -0.5, 0);
        drawBrick(2);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(10, -0.5, 0);
        drawBrick(2);
    glPopMatrix();





    glutSwapBuffers();
}

void update(int value) {


    jumpMario();


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
    glutCreateWindow("Transformations");
    initRendering();

    //Set handler functions
    glutDisplayFunc(drawScene);

    glutReshapeFunc(handleResize);

    glutKeyboardFunc(handleKeypress);

    glutTimerFunc(25, update, 0); //Add a timer

    glutMainLoop();

    return 0;
}








