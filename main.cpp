
/******************************************************************************


*******************************************************************************/

#include <iostream>
#include <string>
#include "tat_math.h"
#include "tat_utils.h"
#include "tat_graphics.h"
#include "tat_neurosim.h"
#include "tat_epuck.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

int winx = 500;
int winy = 500;
float t = 0;
int dx = 10;
int dy = 10;
int inh_y = 2;
float sqsz = 0.05;
//SpikingPopulation pop("TestPop", dx*dy, NeuronType::eRegular_spiking, 10);
//SpikingPopulation inhpop("Inhibitory", inh_y, NeuronType::eRegular_spiking, 10);
//SpikingPopulation pop;
//float** topology;
//float** inh_topology;
EPuck puck;
bool ledon = true;
void setup()
{
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(winx, winy);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Test");

    //topology = rand_topology(0.075, dx*dy, dx*dy);
    //inh_topology = rand_topology(0.35, dx*dy, inh_y);
    //inhpop.setInternalTopology(zeros(inh_y, inh_y), inh_y, inh_y);
    
    
}

void update()
{
    t += 1;
    
    FloatList dir(inh_y, 50.f);

    //int ix = (int)random(0, dx*dy-1);
    //dir[ix] = (20.f);
    //dir[(int)random(0, dx*dy-1)] = (20.f);
    //inhpop.setDirect(dir);
    
    //inhpop.tick();
    

    //pop.inhibit(inhpop.getOutput(), inh_topology, dx*dy, inh_y);
    
    //if((int)t % 20 == 0) apply_rand_topology(topology, 0.075, dx*dy, dx*dy);
    //topology[1][1] = 0.5;
    //pop.setInternalTopology(topology, dx*dy, dx*dy);
    //pop.tick();
    if((int)t % 50 == 0){
	    ledon = !ledon;
    }
    puck.setLED1(ledon);
    if(ledon)
        puck.setLED2(100,0,0);
    else
        puck.setLED2(0, 100,0);
    
    puck.tick();
    
    glutPostRedisplay();
    
}

void draw()
{
    
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_LINE_SMOOTH);

    glLineWidth(4.f);
    glPushMatrix();
	circle(0, 0, 0.2);
    	/*
        float **grid = zeros(dx, dy);
        int count = 0;
        for (int j = 0; j < dy; j++)
            for (int i = 0; i < dx; i++)
                grid[j][i] = pop.getNormOutput()[count++];
                
        drawColGrid(-.9, 0.2, sqsz, 0.025, "", grid, dx, dy);   
        destroy_matrix(grid);      
	*/
    glPopMatrix();

    glFlush();
    glutSwapBuffers();

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    
    setup();
    glutDisplayFunc(draw);
    glutIdleFunc(update);
    
    glutMainLoop();    
    return 0;
}
