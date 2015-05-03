/*
    Copyright (C) Fabrizio Perria <fabrizio.perria@gmail.com> 
                                  <fabrizio.perria@hotmail.it>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

typedef struct snake{
	float x;
	float y;
	struct snake *next;
}snake;

void display();
void reshape(int w,int h);
void key(unsigned char event,int x,int y);
void idle();

const int width=500,height=500;
static int length=50;
static float headPos[2]={0,0};
static float foodPos[2];
static float speed=0.1;
static int vertical=0,horizontal=5;
static snake *head=NULL,*index=NULL;
static int needFood=1;
static int play=0;

int main(int argc, char ** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("SNAKE");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutMainLoop();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT);
	if(play){
		snake *tmp = (snake*)malloc(sizeof(snake));
		int i=0;
		headPos[0]+=(horizontal*speed);
		headPos[1]+=(vertical*speed);
	
		if(needFood==1){
			int n=rand();
			float r=(float)n/RAND_MAX;
			foodPos[0]=(r*(width-12))-((width-12)/2);
			n=rand();
			r=(float)n/RAND_MAX;
			foodPos[1]=(r*(height-12))-((height-12)/2);
			needFood=0;
		}
	
		glPushMatrix();
		glColor3ub(255,0,0);
		glTranslatef(foodPos[0], foodPos[1], 0);
		glRectf((width/2)-6, (height/2)+6, (width/2)+6, (height/2)-6);
		glPopMatrix();
	
		tmp->x=headPos[0];
		tmp->y=headPos[1];
		tmp->next=head;
		head=tmp;
		index=head;

		if(index->next==NULL){
			glPushMatrix();
			glColor3ub(0,0,255);
			glTranslatef(index->x, index->y, 0);
			glRectf((width/2)-6, (height/2)+6, (width/2)+6, (height/2)-6);
			glPopMatrix();
		} else {
			while(index->next!=NULL){
				i++;
				if(i > 1){
					if(headPos[0]==index->x && headPos[1]==index->y){
						length=50;
						head=NULL;
						headPos[0]=0;
						headPos[1]=0;
						needFood=1;
						play=0;
						speed=0.05;
						return;
					}
				}
				glPushMatrix();
				glColor3ub(0,0,255);
				glTranslatef(index->x, index->y, 0);
				glRectf((width/2)-6, (height/2)+6, (width/2)+6, (height/2)-6);
				glPopMatrix();
				if(index->next->next==NULL && i >= length){
					index->next=NULL;
					break;
				}
				index=index->next;
			}
		}
	
		if((headPos[0])<=(foodPos[0]+10) && (headPos[0])>=(foodPos[0]-10)){
			if((headPos[1])<=(foodPos[1]+10) && (headPos[1])>=(foodPos[1]-10)){
				length+=6;
				speed+=0.001;
				needFood=1;
			}
		}
		if((headPos[0] >= (width/2)+6) || (headPos[0] <= 6-(width/2)) || (headPos[1] >= (height/2)+6) || (headPos[1] < 6-(height/2))){
			length=50;
			head=NULL;
			headPos[0]=0;
			headPos[1]=0;
			needFood=1;
			play=0;
			speed=0.05;
		}
	} else {
		glColor3f(255, 0, 0);
		char *c="SNAKE Press Space to start...";
		int i=0;
		while (i<28){
			glRasterPos2f((i*13), height/2);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , c[i]);
			i++;
		}
	}
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
}

void key(unsigned char event,int x,int y){
	
	switch(event){
		case 's':
			if(vertical==0){
				vertical-=6;
				horizontal=0;
			}
			break;
		case 'w':
			if(vertical==0){
				vertical+=6;
				horizontal=0;
			}
			break;
		case 'a':
			if(horizontal==0){
				horizontal-=6;
				vertical=0;
			}
			break;
		case 'd':
			if(horizontal==0){
				horizontal+=6;
				vertical=0;
			}
			break;
		case ' ':
			play=1;
			break;
		default: break;
	}
	 glutPostRedisplay();
}

void idle(){
	glutPostRedisplay();
}


