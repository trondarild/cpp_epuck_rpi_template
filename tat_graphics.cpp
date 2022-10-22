#include "tat_graphics.h"
#include "tat_utils.h"
#include <iostream>


#include <cmath> 
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

void circle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex

    }
    glEnd();
}
void circle(float x, float y, float r) {
        float i = 0.0f;
        
        glBegin(GL_TRIANGLE_FAN);
        
        glVertex2f(x, y); // Center
        for(i = 0.0f; i <= 360; i++)
                glVertex2f(r*cos(M_PI * i / 180.0) + x, r*sin(M_PI * i / 180.0) + y);
        
        glEnd();
}

void rect(float x1, float y1, float x2, float y2)
{
    glRectf(x1, y1, x2, y2);
}

void square(float x, float y, float dim)
{
    rect(x, y, x+dim, y+dim);
}

void line(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
        glVertex3f(x1, y1, 0.0);
        glVertex3f(x2, y2, 0.0);
    glEnd();
}

void line(float *x, float *y, int size)
{
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<size; i++)
    {
        glVertex3f(x[i], y[i], 0.0);
    }
    glEnd();
}

void scatter(float *x, float *y, float r, int size)
{
    for(int i=0; i<size; i++)
    {
        circle(x[i], y[i], r);
    }
}

void fill_hsv(float h, float s, float v)
{
    // conv to rgb
    int r, g, b;
    //std::cout<< "h: "<< h << ", s: " << s << ", v: " << v << "\n";
    hsv_rgb(r, g, b, h, s, v);
    //std::cout<< "r: "<< r << ", g: " << g << ", b: " << b << "\n";
    glColor3ub(r, g, b);
}

void fill(int grey)
{
    glColor3ub(grey, grey, grey);
}

void drawColGrid(float x1, float y1, float dim, float margin, std::string title, float** top, int sizex, int sizey){
  /**
  x1 - topleft corner x
  y1 - topleft corner y
  dim - scale dim of individual square
  margin - between square margin
  top - array of values: pos and neg values drawn differently

  Example:
  float top[][] = {{0.1, 0.2}};
  drawColGrid(0, 0, 0.05, 0.025, "", top, 2, 1);
  */
  int exccol[] = {217, 122, 122};
  int inhcol[]= {122, 122, 217};
  
  // TODO implement text
  // glPushMatrix();
  // glTranslatef(0, -10, 0);
  // text(title, 0, 0);
  // popMatrix();
  
  //glPushStyle();
  //colorMode(HSB);
  glPushMatrix();
  float y=y1;
  // float margin = 10;
  for(int j=0; j<sizey; j++){
    float x=x1;
    for(int i=0; i<sizex; i++){
      //pushStyle();
      if(top[j][i] > 0) fill_hsv(hue(exccol), saturation(exccol), top[j][i]);
      else if(top[j][i] < 0) fill_hsv(hue(inhcol), saturation(inhcol), top[j][i]);
      else fill(0);
      square(x, y, dim);
      //popStyle();
      x+= dim+margin;
    }
    y+= dim+margin;
  }
  glPopMatrix();

}



 
