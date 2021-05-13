#ifndef DRAW_H
#define DRAW_H

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

class drawObject{
public:
    double h = -2;
    double l = -20;

static const int terrainSize = 50;


float heights[terrainSize][terrainSize];  
float normals[terrainSize][terrainSize][3]; 
float minHeight = 0;
float maxHeight = 0;
static const int numCir = 40;
int circles[numCir][4];

GLubyte* tex2;
GLubyte* tex1;
GLubyte* tex3;
float cubeVerts[8][3] = { {-1,-0.25,1}, {-1,0.25,1}, {1,0.25,1}, {1,-0.25,1}, {-1,-0.25,-1}, {-1,0.25,-1}, {1,0.25,-1}, {1,-0.25,-1} };
//float cols[6][3] = { {1,0,0}, {0,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1} };

float tableVerts[8][3] = { {-0.4,-2,0.4}, {-0.4,-0.25,0.4}, {0.4,-0.25,0.4}, {0.4,-2,0.4}, {-0.4,-2,-0.4}, {-0.4,-0.25,-0.4}, {0.4,-0.25,-0.4}, {0.4,-2,-0.4} };
//float cols[6][3] = { {1,0,0}, {0,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1} };
float roomVerts[8][3] = { {-10,-2,10}, {-10,10,10}, {10,10,10}, {10,-2,10}, {-10,-2,-10}, {-10,10,-10}, {10,10,-10}, {10,-2,-10} };

GLuint textures[3];

int width1, height1, max1;
int width2, height2, max2;
int width3, height3, max3;


    drawObject();
GLfloat cubeVertices[8][3] = {
    { -1, 0.25, -1 },
    { -1, -0.25, -1 },
    { 1, -0.25, -1 },
    { 1, 0.25, -1 },

    { -1, 0.25, 1 },
    { -1, -0.25, 1 },
    { 1, -0.25, 1 },
    { 1, 0.25, 1 },
};

GLfloat tableVertices[8][3] = {
    { -0.4, -0.25, -0.4 },
    { -0.4, -2, -0.4 },
    { 0.4, -2, -0.4 },
    { 0.4, -0.25, -0.4 },

    { -0.4, -0.25, 0.4 },
    { -0.4, -2, 0.4 },
    { 0.4, -2, 0.4 },
    { 0.4, -0.25, 0.4 },
};

GLfloat roomVertices[8][3] = {
    { -10, 10, -10 },
    { -10, -2, -10 },
    { 10, -2, -10 },
    { 10, 10, -10 },

    { -10, 10, 10 },
    { -10, -2, 10 },
    { 10, -2, 10 },
    { 10, 10, 10 },
};

unsigned int cubeTriangles[12][3] = {
    // Back face
    { 0, 2, 1 },
    { 0, 3, 2 },
    // Left face
    { 0, 1, 4 },
    { 1, 5, 4 },
    // Front face
    { 4, 5, 6 },
    { 4, 6, 7 },
    // Right face
    { 6, 3, 7 },
    { 2, 3, 6 },
    // Top face
    { 0, 4, 3 },
    { 3, 4, 7 },
    // Bottom face
    { 1, 2, 5 },
    { 2, 6, 5 }
};

GLfloat cubeNormals[6][3] = {
    // Back face
    { 0, 0, -1 },
    // Left face
    { -1 ,0, 0 },
    // Front face
    { 0, 0, 1 },
    // Right face
    { 1, 0, 0 },
    // Top face
    { 0, 1, 0 },
    // Bottom face
    { 0, -1, 0 }
};

GLfloat roomNormals[6][3] = {
    // Back face
    { 0, 0, 1 },
    // Left face
    { 1 ,0, 0 },
    // Front face
    { 0, 0, -1 },
    // Right face
    { -1, 0, 0 },
    // Top face
    { 0, -1, 0 },
    // Bottom face
    { 0, 1, 0 }
};



GLfloat boardMaterial[3][4] = {
    {0.4,0.2,0,1},
    {0.4,0.2,0,1},
    {0.4,0.2,0,1}
}; 
    void drawRoom();
    void drawTable();
    void drawCube();
    void drawBoard();
    //void drawSits();
    void drawStick();
    void drawAll();
    void drawWire();
    void heightmap();
    void drawTerrain();
    void setColor(int x, int z);
    void defineNormals();
    void drawPolygon(int a, int b, int c, int d, float v[8][3]);
    void cube(float v[8][3],int t);
    void room(float v[8][3],int t);
    GLubyte* LoadPPM(char* file, int* width, int* height, int* max);
    void textinit();

};

#endif