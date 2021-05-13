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
#include <stdio.h>
#include <stdlib.h>
//Changes!
#include "setGame.h"
#include "draw.h"
//#  include <OpenGL/gl.h>
#include <time.h>  

using namespace std;

drawObject::drawObject(){
}

void drawObject::drawPolygon(int a, int b, int c, int d, float v[8][3]){
    glBegin(GL_POLYGON);
    
    glTexCoord2f(0, 0);
    glVertex3fv(v[a]);
    
    glTexCoord2f(0, 1);
    glVertex3fv(v[b]);
    
    glTexCoord2f(1, 1);
    glVertex3fv(v[c]);
    
    glTexCoord2f(1, 0);
    glVertex3fv(v[d]);
    glEnd();
}

void drawObject::cube(float v[8][3],int t)
{
    glPushMatrix();
    glColor3f(0.4,0.2,0);
    glBindTexture(GL_TEXTURE_2D, textures[t]);
    drawPolygon(0, 3, 2, 1, v);
    drawPolygon(1, 0, 4, 5, v);
    drawPolygon(5, 1, 2, 6, v);
    drawPolygon(2, 3, 7, 6, v);
    drawPolygon(6, 5, 4, 7, v);
    drawPolygon(4, 0, 3, 7, v);
    glPopMatrix();
}

void drawObject::room(float v[8][3],int t)
{
    glPushMatrix();
    glColor3f(0.4,0.2,0);

    glBindTexture(GL_TEXTURE_2D, textures[t]);
        drawPolygon(1, 0, 4, 5, v);
    drawPolygon(0, 3, 2, 1, v);
    drawPolygon(5, 1, 2, 6, v);
   // glBindTexture(GL_TEXTURE_2D, textures[0]);
    drawPolygon(2, 3, 7, 6, v);
  //  drawPolygon(6, 5, 4, 7, v);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
    drawPolygon(4, 0, 3, 7, v);
    glPopMatrix();
}

GLubyte* drawObject::LoadPPM(char* file, int* width, int* height, int* max)
{
    GLubyte* img;
    FILE *fd;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    
    fd = fopen(file, "r");
    fscanf(fd,"%[^\n] ",b);
    if(b[0]!='P'|| b[1] != '3')
    {
        printf("%s is not a PPM file!\n",file);
        exit(0);
    }
    printf("%s is a PPM file\n", file);
    fscanf(fd, "%c",&c);
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n",b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    
    printf("%d rows  %d columns  max value= %d\n",n,m,k);
    
    nm = n*m;
    
    img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
    
    
    s=255.0/k;
    
    
    for(i=0;i<nm;i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }
    
    *width = n;
    *height = m;
    *max = k;
    
    return img;
}

void drawObject::textinit(){
    glMatrixMode(GL_TEXTURE);
    glScalef(1,-1,-1);
    
    tex1 = LoadPPM("wood1.ppm", &width1, &height1, &max1);
    
    tex2 = LoadPPM("wood3.ppm", &width2, &height2, &max2);
    
    tex3 = LoadPPM("tatami1.ppm", &width3, &height3, &max3);
    glEnable(GL_TEXTURE_2D);
    
    glGenTextures(3, textures);
    
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, tex2);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, textures[2]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, tex3);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
}

void drawObject::heightmap(void){
    srand(time(NULL));
    for (int i = 0; i < numCir; i++) {
        circles[i][0] = rand() % (terrainSize); 
        circles[i][1] = rand() % (terrainSize); 
        circles[i][2] = rand() % 10;  
        circles[i][3] = rand() % 20; 
    }
    for (int i = 0; i < numCir; i++){
        for (int x = 1; x <= terrainSize; x++){
            for (int z = 1; z <= terrainSize; z++){
                float pd = sqrt(pow(circles[i][0] - x, 2) + pow(circles[i][1]  - z, 2)) / circles[i][3] ;
                if (fabs(pd) <= 1.0)
                    heights[x][z] += circles[i][2]  / 2 + cos(pd * 3.14) * circles[i][2]  / 2;
            }
        }
    }
    defineNormals();
}

void drawObject::setColor(int x, int z){
    float y = heights[x + terrainSize / 2][z + terrainSize / 2]; 
    float r, g, b;
    float color = (y +2) / (22);
    b = color/2;
    g = 0.6 - color;
    r = 0;
    glColor3f(r, g, b);
    glVertex3f(x, y, z);
}

void drawObject::drawTerrain(void){
    glPushMatrix();
    glTranslatef(0,-10,-38);
        glBegin(GL_TRIANGLE_STRIP);
    for (int z = -terrainSize / 2; z <= terrainSize / 2; z++){
        for (int x = terrainSize / 2 - 1; x >= -terrainSize / 2; x--){
            setColor(x, z + 1);
            setColor(x, z);
            }
    }
    glEnd();
    glPopMatrix();
}

void drawObject::defineNormals()
{
    float x1, y1, z1, x2, y2, z2, xc, yc, zc;
    for (int x = 0; x < terrainSize - 1; x++)
    {
        for (int z = 0; z < terrainSize - 1; z++)
        {
            x1 = x + 1;
            y1 = heights[x + 1][z] - heights[x][z];
            z1 = z;

            x2 = x + 1;
            y2 = heights[x + 1][z + 1] - heights[x][z];
            z2 = z + 1;

            xc = y1 * z2 - z1 * y2;
            yc = z1 * x2 - x1 * z2;
            zc = x1 * y2 - y1 * x2;
            float n = sqrtf(xc * xc + yc * yc + zc * zc);

            normals[x][z][0] = xc / n;
            normals[x][z][1] = yc / n;
            normals[x][z][2] = zc / n;
        }
    }
}

void drawObject:: drawStick(){
    glPushMatrix();
        glColor3f(0.6,0.3,0);
        for (int i = -2; i < 10; i++){
            glBegin(GL_QUADS);
            glVertex3f(-10,i+0.1,-10);
            glVertex3f(-5, i+0.1, -10);
            glVertex3f(-5,i-0.1,-10);
            glVertex3f(-10,i-0.1,-10);
            glEnd();
        }
        for (int i = -2; i < 10; i++){
            glBegin(GL_QUADS);
            glVertex3f(10,i+0.15,-10);
            glVertex3f(5, i+0.15, -10);
            glVertex3f(5,i-0.15,-10);
            glVertex3f(10,i-0.15,-10);
            glEnd();
        }
        for (int i = -1; i <= 1; i++){
            glBegin(GL_QUADS);
            glVertex3f(i*5+0.15, 10, -10);
            glVertex3f(i*5+0.15, -2, -10);
            glVertex3f(i*5-0.15,-2,-10);
            glVertex3f(i*5-0.15,10,-10);
            glEnd();
        }
        for (int i = -5; i <= 5; i++){
            glColor3f(0,0.2,0.6);
            glBegin(GL_QUADS);
            glVertex3f(i*2+0.8, 10, -10.1);
            glVertex3f(i*2+0.8, 8, -10.1);
            glVertex3f(i*2-0.8,8,-10.1);
            glVertex3f(i*2-0.8,10,-10.1);
            glEnd();
        }
    glPopMatrix();
}

/*void drawObject::drawSits(){
    GLUquadricObj *objCylinder = gluNewQuadric();
    glPushMatrix();
        glColor3f(0.8,0.8,0);
        glPushMatrix();
            glTranslatef(3,-1.75,0);
            glRotatef(90,1,0,0);
            gluCylinder(objCylinder, 1, 1, 0.25, 100, 5);
            //glNormal3f(0,1,0);
            //gluDisk(objCylinder,0,1,100,1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-3,-1.75,0);
            glRotatef(90,1,0,0);
            //gluCylinder(objCylinder, 1, 1, 0.25, 100, 5);
            //glNormal3f(0,1,0);
            //gluDisk(objCylinder,0,1,100,1);
        glPopMatrix();

    glPopMatrix();
}*/

void drawObject::drawRoom(){
    glPushMatrix();
        glColor3f(1,0.9,0.7);
        glBegin(GL_TRIANGLES);
        for (int i = 2; i < 12; i++){
            glNormal3fv(roomNormals[i/2]);
            for (int j = 0; j < 3; j++){
                glVertex3fv(roomVertices[cubeTriangles[i][j]]);
            }
        }
    glPopMatrix();
}

void drawObject::drawBoard(){
    glPushMatrix();
        glNormal3f(0,1,0);
        glBegin(GL_QUADS);
            glColor3f(0.8,0.8,0.6);
            glVertex3f(-0.8,0.26,0.8);
            glVertex3f(0.8, 0.26, 0.8);
            glVertex3f(0.8,0.26,-0.8);
            glVertex3f(-0.8,0.26,-0.8);
        glEnd();
        glPushMatrix;
            for (int i = 0; i < 15; i++){
                glColor3f(0,0,0);
                glBegin(GL_LINES);
                    glVertex3f(-0.7,0.27, -0.7+0.1*i);
                    glVertex3f(0.7,0.27, -0.7+0.1*i);
                glEnd();
            }
            for (int i = 0; i < 15; i++){
                glBegin(GL_LINES);
                    glVertex3f(-0.7+0.1*i,0.27, 0.7);
                    glVertex3f(-0.7+0.1*i,0.27, -0.7);
                glEnd();
            }
        glPopMatrix();
    glPopMatrix();
}

void drawObject::drawAll(){
    drawBoard();
    drawStick();
    drawTerrain();
    //draw with texture
    cube(cubeVerts, 0);
    //table with texture
    cube(tableVerts, 0);
    //room with texture
    room(roomVerts, 1);
    
}

