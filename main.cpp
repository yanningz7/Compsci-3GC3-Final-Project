# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <math.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include "setGame.h"
#include "draw.h"
using namespace std;

#define MAX_PIECES 225
#define MAX_FIREWORKS 100
/*
todo:
text：计分表 player——win
棋子: drawFollowingMouse变色黑白红，//闪退问题
光线：阳光变化 （阴影）
整理代码 comments
*/

//camera
GLdouble eye[] = { 0, 3, 10 };
GLdouble lookAt[] = { 0, 3, 3 };
GLdouble up[] = { 0, 1, 0 };
double angle = 0;
bool playPos = false;
//light
bool lightOn = true;
float pos[4] = {0,80,70,1};
float amb[4] ={0.5,0.5,0.5,1};
float diff[4]= {1,1,1,0.5};
float spc[4] = {1,1,1,1};
float shiny = 0;
//fog
float density = 0.02;
float fogColor[4] = {0.5,0.5, 0.5, 1};
//Sun&Moon
double sunAngle = 10;
double moonAngle = 190;
//terrain
bool defineHeight = false;
//firework
float xp, yp, zp;
float xspeed,yspeed,zspeed;
float slowdown = 8.0f;
//pickupPiece
float passive_x;
float passive_y;
float passive_z;
int passive_row;
int passive_column;
bool isBlack = true; 
//placing piece 
int row, column;

gameData game;
drawObject obj;

//!!!!!!not working!!!!! text
char message[] = "Hello World";

//!!!!!!not working!!!!!! material
void setMat(GLfloat mat[3][4]) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat[1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat[2]);
    //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, outlineShiny);
}

void turnOnLight(void){
    if (lightOn) {
        glEnable(GL_LIGHTING);
        }else{
            glDisable(GL_LIGHTING);
        }
}

//change camera position to the top of the game board
void playPosition(void){
    if (playPos) {
            eye[0] = 0;
            eye[1] = 2.5;
            eye[2] = 0;
            lookAt[0] = 0;
            lookAt[1] = 0;
            lookAt[2] = 0;
            up[0] = 0;
            up[1] = 0;
            up[2] = -1;
        }else{
            eye[0] = 0;
            eye[1] = 3;
            eye[2] = 10;
            lookAt[0] = 0;
            lookAt[1] = 3;
            lookAt[2] = 0;
            up[0] = 0;
            up[1] = 1;
            up[2] = 0;
        }
}

//struct for piece
typedef struct{
    float px;
    float py;
    bool isBlack;
}pieces;
pieces piece[MAX_PIECES];

//struct for particles(firework)
typedef struct
{
  bool active;
  float life;
  float fade;
  float r,g,b;
  float x,y,z;
  float xi,yi,zi;
}particles;
#define MAX_PARTICLES 100
particles paticle[MAX_PARTICLES];
//init firework
int init(){
  for (int loop = 0;loop < MAX_PARTICLES;loop++)
  {
    paticle[loop].active = true;
    paticle[loop].life = 1.0f;
    paticle[loop].fade = (rand() % 100) / 1000.0f + 0.003f;
    paticle[loop].x = 0;
    paticle[loop].y = 0;
    paticle[loop].z = 0;
    paticle[loop].r =(rand()*256);
    paticle[loop].b =(rand()*256);
    paticle[loop].g =(rand()*256);
  }
  return true;
}

void drawFireworks(){
    glPushMatrix();
    glTranslatef(2,7,-11);
    for (int loop = 0;loop < MAX_PARTICLES;loop++){
        if (paticle[loop].active){
            float x = paticle[loop].x;
            float y = paticle[loop].y;
            float z = paticle[loop].z;
            float r = paticle[loop].r;
            float b = paticle[loop].b;
            float g = paticle[loop].g;
            glColor4f(r,g,b, paticle[loop].life);
            glPointSize(10);
            glBegin(GL_POINTS);
            glVertex3f(x +  0.0f,y + 0.0f,z+0.0f);
            glEnd();
            xp = paticle[loop].xi/(slowdown*1000);
            yp = paticle[loop].yi/(slowdown*1000);
            zp = paticle[loop].zi/(slowdown*1000);
            paticle[loop].x+=xp; 
            paticle[loop].y+=yp; 
            paticle[loop].z+=zp;   
            paticle[loop].life -= paticle[loop].fade;
            //set the particle to init if life < 0
            if (paticle[loop].life < 0.0f){
                paticle[loop].life = 1.0f;
                paticle[loop].fade = (rand() % 100) / 1000.0f + 0.003f; 
                paticle[loop].x = 0.0f;
                paticle[loop].y = 0.0f;
                paticle[loop].z = 0.0f;
                paticle[loop].xi=((rand()%400)-200.0f); 
                paticle[loop].yi=((rand()%400)-200.0f); 
                paticle[loop].zi=((rand()%400)-200.0f);
            }
        }
    }
    glPopMatrix();
}

void drawPiece(){
    glPushMatrix();
        glFrontFace(GL_CCW);
	    for (int i = 0; i < 15; i++) {
		    for (int j = 0; j < 15; j++) {
			    float xp = (float)(j) / 10.0 - 0.7;
			    float yp = (float)(i) / 10.0 - 0.7;
			    if (game.gameMapArr[i][j] == -1) {
				    glColor3f(0, 0, 0);
				    glPushMatrix();
    				glTranslatef(xp, 0, yp);
	    			glTranslatef(0, 0.27, 0);
		    		glutSolidSphere(0.03, 10, 10);
			    	glPopMatrix();
			    }
			    else if (game.gameMapArr[i][j] == 1) {
				    glColor3f(1, 1, 1);
    				glPushMatrix();
	    			glTranslatef(xp, 0, yp);
		    		glTranslatef(0, 0.27, 0);
			    	glutSolidSphere(0.03, 10, 10);
				    glPopMatrix();
			    }
		    }	
	    }
    glPopMatrix();
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!comment part: 如果passive鼠标位置有棋子，画半透明红色棋子
void drawFollowingPiece(){
   if (game.gameMapArr[passive_row][passive_column] != 0 || !(passive_y >= 0.2 &&passive_y <= 0.3 && passive_x >=-0.75 && passive_x <=0.75 && passive_z >= -0.75 && passive_z <=0.75)){
        glPushMatrix();
            glColor4f(1,0,0,0.5);
            glTranslatef(passive_x, 0.28, passive_z);
            glutSolidSphere(0.03,5,5);
        glPopMatrix();
    }else{
        glPushMatrix();
        //每下一个字改颜色！！！！！！
        if (!game.isWhite){
            glColor3f(0,0,0);
        }else{
            glColor3f(1,1,1);
        }
    glTranslatef(passive_x, 0.27, passive_z);
    glutSolidSphere(0.03,5,5);
    glPopMatrix();
    }
}

void drawSunMoon(){   
    glPushMatrix();
    glColor3f(1,0,0);
    glRotatef(-1* sunAngle,0,0,1);
    glTranslatef(-15,-5,-25);
    glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1,1,1);
    glRotatef(-1* moonAngle,0,0,1);
    glTranslatef(-15,-5,-25);
    glutSolidSphere(2, 50, 50);
    glPopMatrix();
}

//!!!!!!!!!!not working!!!!!!!!!!!
void scoreTable(){
    glColor3f(0,0,0);
    glLineWidth(5);
    for (int i = 0 ; i <strlen(message);i++){
        glutStrokeCharacter(GLUT_STROKE_ROMAN, message[i]);
    }
}

void display(void){
    turnOnLight();
    glClearColor(0.5,0.5,0.5,0);
	glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();  
    glFrontFace(GL_CCW);

    glLightfv(GL_LIGHT0,GL_POSITION,pos);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0,GL_AMBIENT,amb);
    glLightfv(GL_LIGHT0,GL_SPECULAR, spc);
    //setMat(boardMaterial);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
    //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
   
	gluLookAt(eye[0],eye[1],eye[2],  lookAt[0],lookAt[1],lookAt[2], up[0],up[1],up[2]);   
    glRotatef(angle,0,1,0);
    drawSunMoon();
    obj.drawAll();
    drawPiece();
    drawFollowingPiece();
    if (game.checkWin(row, column)){
        drawFireworks();
    }
    //scoreTable();
    if(defineHeight ==false){
        obj.heightmap();
        defineHeight = true;
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glutSwapBuffers();
    glutPostRedisplay();
}

void handleKeyboard(unsigned char key, int _x, int _y){
    switch(key)
    {
        case 'q':
            exit(0);
            break;
        case '1':
            if (playPos){
                playPos = false;
                playPosition();
            }else{
                playPos = true;
                playPosition();
            }
            display();
            break;
        case 'l':
            if (lightOn){lightOn = false ;}
            else{lightOn = true;}
            display();
            break;
		case 'r':
			game.gameInit();
			break;
        case 'z':
            angle++;
            break;
        case 'x':
            angle--;
            break;
        case 'w':
            lookAt[1]+=0.1;
            break;
        case 's':
            lookAt[1]-=0.1;
            break;
        case 'a':
            lookAt[0]-=0.1;
            break;
        case 'd':
            lookAt[0]+=0.1;
            break;
        case '2':
            eye[0] = 0;
            eye[1] = 500;
            eye[2] = 0;
            lookAt[0] = 0;
            lookAt[1] = 0;
            lookAt[2] = 0;
            up[0] = 0;
            up[1] = 0;
            up[2] = -1;
            break;
		case 'p': // ******************************************
			game.singlePlayerFlag = !game.singlePlayerFlag;
			break;
		case 'u':
			if (!game.checkWin(row, column)) {
				game.undo();
				row = game.rowPosOrder.back();
				column = game.colPosOrder.back();
			}
			break;
    }
}

void specialKeys(int key, int x, int y)
{
switch (key)
{
    case GLUT_KEY_LEFT:
        angle--;
        break;
    case GLUT_KEY_RIGHT:
        angle++;
        break;
    case GLUT_KEY_UP:
        if (eye[2]> -10){
            eye[2]-=0.1;
            lookAt[2]-=0.1;
        }
        break;
    case GLUT_KEY_DOWN:
        if (eye[2]< 10){
            eye[2]+=0.1;
            lookAt[2]+=0.1;
        }
        break;
    }
}

void passive (int x, int y){
    static int number = 1;
        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winX, winY, winZ;
        GLdouble object_x, object_y, object_z;
        int mouse_x = x;
        int mouse_y = y;
        
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        winX = (float)mouse_x;
        winY = (float)viewport[3] - (float)mouse_y - 1.0f;
        glReadBuffer(GL_BACK);
        glReadPixels(mouse_x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        gluUnProject((GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, modelview, projection, viewport, &object_x, &object_y, &object_z);
        passive_x = object_x;
        passive_y = object_y;
        passive_z = object_z;
        passive_row = static_cast<int>(((floor(object_z*10+0.5)/10) + 0.7) * 10 + 0.1);
		passive_column = static_cast<int>(((floor(object_x*10+0.5)/10 + 0.7)) * 10 + 0.1);
}

void handleMouse(int btn, int state, int x, int y){
    static int number = 1;
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && game.gameFlag){
        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winX, winY, winZ;
        GLdouble object_x, object_y, object_z;
        int mouse_x = x;
        int mouse_y = y;
        
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        winX = (float)mouse_x;
        winY = (float)viewport[3] - (float)mouse_y - 1.0f;
        glReadBuffer(GL_BACK);
        glReadPixels(mouse_x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        gluUnProject((GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, modelview, projection, viewport, &object_x, &object_y, &object_z);
        if (object_y >= 0.2 &&object_y <= 0.3 && object_x >=-0.7 && object_x <=0.7 && object_z >= -0.7 && object_z <=0.7){
		    row = static_cast<int>(((floor(object_z*10+0.5)/10) + 0.7) * 10 + 0.1);
		    column = static_cast<int>(((floor(object_x*10+0.5)/10 + 0.7)) * 10 + 0.1);
			game.setPieces(row, column);
			game.isWin(row, column);
			game.isDraw(row, column);
			if (game.singlePlayerMode) {
				game.setVal(row, column);
				/*for (int i = 0; i < 15; i++) {
					for (int j = 0; j < 15; j++) {
						printf("%d ", game.gameMapVal[i][j]);
					}
					printf("\n");
				}
				printf("\n");*/
				game.AISetPiece();
			}
        }

    }
}

void FPS(int val){
        glutPostRedisplay();
        sunAngle+=0.1;
        moonAngle+=0.1;
    glutTimerFunc(17, FPS, 0); // 1sec = 1000, 60fps = 1000/60 = ~17
}

void handleReshape(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,1,1,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void callBackInit(){
    glutDisplayFunc(display);
    glutTimerFunc(0, FPS, 0);
}

// !!!!!!!!!!!!!!!!!
void printOut(){
    std::cout<<"Commands avariable: \n"
    <<"1: switch to the camera on the game\n"
    <<"w,a,s,d: change view point \n"
    <<"arrow up, down: \n"
    <<"arrow left, right: \n"
    <<"r: restart game\n"
    <<"mouse left click: \n"
    <<"press u to undo(recommend to do it with 2-player mode\n"
    <<"press p to toggle between single player and 2 players mode\n"
	<< "  (Only activates after press r to restart the game)\n"
    <<"p: \n"
    <<"p: \n"
    <<"p: \n"
    <<std::endl;
}

int main(int argc, char** argv){
    printOut();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_RGB |GLUT_DOUBLE| GLUT_DEPTH);
    glutInitWindowSize(800,800);
    glutInitWindowPosition(50,50);
	game.gameInit();
    glutCreateWindow("Project: five in a row");
    callBackInit();
    init();
    obj.textinit();

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    turnOnLight();
    glEnable (GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glFogi(GL_FOG_MODE,GL_EXP2);
    glFogfv(GL_FOG_COLOR,fogColor);
    glFogf(GL_FOG_DENSITY,density);
    glHint(GL_FOG_HINT,GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_FOG);
    
    glutPassiveMotionFunc(passive);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(handleMouse);
    glutDisplayFunc(display);
    glutReshapeFunc(handleReshape);
    glutMainLoop();
    return(0);
}