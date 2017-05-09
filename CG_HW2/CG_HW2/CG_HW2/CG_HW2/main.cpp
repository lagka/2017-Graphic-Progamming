#include <stdio.h>
#include <stdlib.h>
#include <cmath>




#include <GL/glew.h>
#include <freeglut/glut.h>
#include "textfile.h"
#include "glm.h"

#include "Matrices.h"

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP   0x0003
# define GLUT_WHEEL_DOWN 0x0004
#endif

#ifndef GLUT_KEY_ESC
# define GLUT_KEY_ESC 0x001B
#endif

#ifndef max
# define max(a,b) (((a)>(b))?(a):(b))
# define min(a,b) (((a)<(b))?(a):(b))
#endif

// Shader attributes
GLint iLocPosition;
GLint iLocColor;
GLint iLocMVP;

char line[40] = "---------------------------------------";
char filename[][200] = { "ColorModels/bunny5KC.obj" ,"ColorModels/blitzcrank_incognito.obj","ColorModels/elephant16KC.obj","ColorModels/ziggs.obj" ,"ColorModels/cessna7KC.obj" ,"ColorModels/tigerC.obj" };
GLMmodel* OBJ, *OBJ1;
char modecommand[100];
char fname[200]="ColorModels/boxC.obj";
GLfloat* vertices,*vertices1;
GLfloat* colors,*colors1;
int index = 0, polygon = 0;
int colormode = 0;
float color[3],tx=0,ty=0;
GLuint color_link;
GLfloat maxVal[3],maxVal1[3];
GLfloat minVal[3],minVal1[3];
float scale,nx,ny,nz;
float tempx = 0, tempy = 0, tempz = 0, sx = 1, sy = 1, rotx = 0, roty = 0,rotz=0, rz = 0, r = 0;
bool start = false;
int mode = 1, dirx = 2, diry = 2;
float ex = 0, ey = 0, ez = 0, cx = 0, cy = 0, cz = 0,tez=0,px=0,py=0,pz=0;
bool fin = true;
float left = -1, right = 1, top = 1, bottom = -1;
float nea = 1.0f, fa = 10.0f;
Vector3 eye = Vector3(0.0f, 0.0f, 2.0f);//P1
Vector3 center = Vector3(0.0f, 0.0f, -5.0f); //P2
void traverseColorModel()
{
	int i;



	// TODO:
	//// You should traverse the vertices and the colors of each triangle, and 
	//// then normalize the model to unit size by using transformation matrices. 
	//// i.e. Each vertex should be bounded in [-1, 1], which will fit the camera clipping window.
	maxVal[0] = FLT_MIN;
	maxVal[1] = FLT_MIN;
	maxVal[2] = FLT_MIN;
	minVal[0] = FLT_MAX;
	minVal[1] = FLT_MAX;
	minVal[2] = FLT_MAX;

	// number of triangles
	OBJ->numtriangles;

	// number of vertices
	OBJ->numvertices;

	// The center position of the model 
	OBJ->position[0] = 0;
	OBJ->position[1] = 0;
	OBJ->position[2] = 0;

	vertices = new float[(OBJ->numtriangles) * 9];
	colors = new float[(OBJ->numtriangles) * 9];
	for(i=0; i<(int)OBJ->numtriangles; i++)
	{
		// the index of each vertex
		int indv1 = OBJ->triangles[i].vindices[0];
		int indv2 = OBJ->triangles[i].vindices[1];
		int indv3 = OBJ->triangles[i].vindices[2];

		// the index of each color
		int indc1 = indv1;
		int indc2 = indv2;
		int indc3 = indv3;

		// vertices
		GLfloat vx, vy, vz;
		vx = OBJ->vertices[indv1*3+0];
		vy = OBJ->vertices[indv1*3+1];
		vz = OBJ->vertices[indv1*3+2];
		maxVal[0] = max(maxVal[0], vx);
		minVal[0] = min(minVal[0], vx);
		maxVal[1] = max(maxVal[1], vy);
		minVal[1] = min(minVal[1], vy);
		maxVal[2] = max(maxVal[2], vz);
		minVal[2] = min(minVal[2], vz);
		vertices[i * 9 + 0] = vx;
		vertices[i * 9 + 1] = vy;
		vertices[i * 9 + 2] = vz;

		vx = OBJ->vertices[indv2*3+0];
		vy = OBJ->vertices[indv2*3+1];
		vz = OBJ->vertices[indv2*3+2];
		maxVal[0] = max(maxVal[0], vx);
		minVal[0] = min(minVal[0], vx);
		maxVal[1] = max(maxVal[1], vy);
		minVal[1] = min(minVal[1], vy);
		maxVal[2] = max(maxVal[2], vz);
		minVal[2] = min(minVal[2], vz);
		vertices[i * 9 + 3] = vx;
		vertices[i * 9 + 4] = vy;
		vertices[i * 9 + 5] = vz;

		vx = OBJ->vertices[indv3*3+0];
		vy = OBJ->vertices[indv3*3+1];
		vz = OBJ->vertices[indv3*3+2];
		maxVal[0] = max(maxVal[0], vx);
		minVal[0] = min(minVal[0], vx);
		maxVal[1] = max(maxVal[1], vy);
		minVal[1] = min(minVal[1], vy);
		maxVal[2] = max(maxVal[2], vz);
		minVal[2] = min(minVal[2], vz);
		vertices[i * 9 + 6] = vx;
		vertices[i * 9 + 7] = vy;
		vertices[i * 9 + 8] = vz;

		// colors
		GLfloat c1, c2, c3;
		c1 = OBJ->colors[indv1*3+0];
		c2 = OBJ->colors[indv1*3+1];
		c3 = OBJ->colors[indv1*3+2];
		colors[i * 9 + 0] = c1;
		colors[i * 9 + 1] = c2;
		colors[i * 9 + 2] = c3;

		c1 = OBJ->colors[indv2*3+0];
		c2 = OBJ->colors[indv2*3+1];
		c3 = OBJ->colors[indv2*3+2];
		colors[i * 9 + 3] = c1;
		colors[i * 9 + 4] = c2;
		colors[i * 9 + 5] = c3;

		c1 = OBJ->colors[indv3*3+0];
		c2 = OBJ->colors[indv3*3+1];
		c3 = OBJ->colors[indv3*3+2];
		colors[i * 9 + 6] = c1;
		colors[i * 9 + 7] = c2;
		colors[i * 9 + 8] = c3;
	}
	maxVal1[0] = FLT_MIN;
	maxVal1[1] = FLT_MIN;
	maxVal1[2] = FLT_MIN;
	minVal1[0] = FLT_MAX;
	minVal1[1] = FLT_MAX;
	minVal1[2] = FLT_MAX;

	// number of triangles
	OBJ1->numtriangles;

	// number of vertices
	OBJ1->numvertices;

	// The center position of the model 
	OBJ1->position[0] = 0;
	OBJ1->position[1] = 0;
	OBJ1->position[2] = 0;

	vertices1 = new float[(OBJ1->numtriangles) * 9];
	colors1 = new float[(OBJ1->numtriangles) * 9];
	for (i = 0; i<(int)OBJ1->numtriangles; i++)
	{
		// the index of each vertex
		int indv1 = OBJ1->triangles[i].vindices[0];
		int indv2 = OBJ1->triangles[i].vindices[1];
		int indv3 = OBJ1->triangles[i].vindices[2];

		// the index of each color
		int indc1 = indv1;
		int indc2 = indv2;
		int indc3 = indv3;

		// vertices
		GLfloat vx, vy, vz;
		vx = OBJ1->vertices[indv1 * 3 + 0];
		vy = OBJ1->vertices[indv1 * 3 + 1];
		vz = OBJ1->vertices[indv1 * 3 + 2];
		maxVal1[0] = max(maxVal1[0], vx);
		minVal1[0] = min(minVal1[0], vx);
		maxVal1[1] = max(maxVal1[1], vy);
		minVal1[1] = min(minVal1[1], vy);
		maxVal1[2] = max(maxVal1[2], vz);
		minVal1[2] = min(minVal1[2], vz);
		vertices1[i * 9 + 0] = vx;
		vertices1[i * 9 + 1] = vy;
		vertices1[i * 9 + 2] = vz;

		vx = OBJ1->vertices[indv2 * 3 + 0];
		vy = OBJ1->vertices[indv2 * 3 + 1];
		vz = OBJ1->vertices[indv2 * 3 + 2];
		maxVal1[0] = max(maxVal1[0], vx);
		minVal1[0] = min(minVal1[0], vx);
		maxVal1[1] = max(maxVal1[1], vy);
		minVal1[1] = min(minVal1[1], vy);
		maxVal1[2] = max(maxVal1[2], vz);
		minVal1[2] = min(minVal1[2], vz);
		vertices1[i * 9 + 3] = vx;
		vertices1[i * 9 + 4] = vy;
		vertices1[i * 9 + 5] = vz;

		vx = OBJ1->vertices[indv3 * 3 + 0];
		vy = OBJ1->vertices[indv3 * 3 + 1];
		vz = OBJ1->vertices[indv3 * 3 + 2];
		maxVal1[0] = max(maxVal1[0], vx);
		minVal1[0] = min(minVal1[0], vx);
		maxVal1[1] = max(maxVal1[1], vy);
		minVal1[1] = min(minVal1[1], vy);
		maxVal1[2] = max(maxVal1[2], vz);
		minVal1[2] = min(minVal1[2], vz);
		vertices1[i * 9 + 6] = vx;
		vertices1[i * 9 + 7] = vy;
		vertices1[i * 9 + 8] = vz;

		// colors
		GLfloat c1, c2, c3;
		c1 = OBJ1->colors[indv1 * 3 + 0];
		c2 = OBJ1->colors[indv1 * 3 + 1];
		c3 = OBJ1->colors[indv1 * 3 + 2];
		colors1[i * 9 + 0] = c1;
		colors1[i * 9 + 1] = c2;
		colors1[i * 9 + 2] = c3;

		c1 = OBJ1->colors[indv2 * 3 + 0];
		c2 = OBJ1->colors[indv2 * 3 + 1];
		c3 = OBJ1->colors[indv2 * 3 + 2];
		colors1[i * 9 + 3] = c1;
		colors1[i * 9 + 4] = c2;
		colors1[i * 9 + 5] = c3;

		c1 = OBJ1->colors[indv3 * 3 + 0];
		c2 = OBJ1->colors[indv3 * 3 + 1];
		c3 = OBJ1->colors[indv3 * 3 + 2];
		colors1[i * 9 + 6] = c1;
		colors1[i * 9 + 7] = c2;
		colors1[i * 9 + 8] = c3;
	}
	/*for (i = 0; i<3 * (int)OBJ->numtriangles; i++) {
		vertices[i * 3 + 0] -= (maxVal[0] + minVal[0]) / 2;
		vertices[i * 3 + 1] -= (maxVal[1] + minVal[1]) / 2;
		vertices[i * 3 + 2] -= (maxVal[2] + minVal[2]) / 2;
	}
	for (i = 0; i < 9 * (int)OBJ->numtriangles; i++) {
		vertices[i] = (2 * vertices[i] / scale);
	}*/
}

void loadOBJModel()
{
	// read an obj model here
	if(OBJ != NULL){
		free(OBJ);
	}
	OBJ = glmReadOBJ(filename[index]);
	printf("%s\n", filename[index]);

	if (OBJ1 != NULL) {
		free(OBJ1);
	}
	OBJ1 = glmReadOBJ(fname);

	// traverse the color model
	traverseColorModel();
}

void onIdle()
{
	glutPostRedisplay();
}

void onDisplay(void)
{
	// clear canvas
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(iLocPosition);
	glEnableVertexAttribArray(iLocColor);

	// organize the arrays
	static GLfloat triangle_color[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	static GLfloat triangle_vertex[] = {
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};

	// TODO:
	//// Please define the model transformation matrix, viewing transformation matrix, 
	//// projection transformation matrix

	//MVP
	Matrix4 T;
	Matrix4 S;
	Matrix4 R;
	Matrix4 N;
	Matrix4 Tv, Rv;
	
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f); //P3
	Vector3 Forward,Right,UP,upn;
	
	if (tez == ez)ez = 0;
	eye += Vector3( ex,  ey, ez);
	center += Vector3(cx, cy, cz);
	Forward = center - eye;
	Right = Forward.cross(up);
	upn = Right.cross(Forward);
	upn = upn;
	up = eye + upn;
	Vector3 Rx, Ry, Rz;
	Rz = (center - eye )/ (center - eye).length();
	Rx = (center - eye).cross(up - eye) / ((center - eye).cross(up - eye)).length();
	Ry = Rx.cross(Rz);


	scale = max((maxVal[0] - minVal[0]), (maxVal[1] - minVal[1]));
	scale = max(scale, (maxVal[2] - minVal[2]));
	float sinx = sin(rotx);
	float cosx = cos(rotx);
	Matrix4 r1 = Matrix4(
		1, 0, 0, 0,
		0, cosx, -sinx, 0,
		0, sinx, cosx, 0,
		0, 0, 0, 1);
	float siny = sin(roty);
	float cosy = cos(roty);

	Matrix4 r2 = Matrix4(
		cosy, 0, siny, 0,
		0, 1, 0, 0,
		-siny, 0, cosy, 0,
		0, 0, 0, 1);
	float sinz = sin(rotz);
	float cosz = cos(rotz);

	Matrix4 r3 = Matrix4(
		cosz, -sinz, 0, 0,
		sinz, cosz, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	Matrix4 M = Matrix4(
						1, 0, 0, 0, 
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);
	Matrix4 V = Matrix4(
						1, 0, 0, 0, 
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);
	Matrix4 P;
	nx = -(maxVal[0] + minVal[0]) / 2, ny = -(maxVal[1] + minVal[1]) / 2, nz = -(maxVal[2] + minVal[2]) / 2;
	
	T = Matrix4(
		1, 0, 0, tx,
		0, 1, 0, ty+0.05,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	N = Matrix4(
		1, 0, 0, nx,
		0, 1, 0, ny,
		0, 0, 1, nz,
		0, 0, 0, 1
	);
	
	S = Matrix4(
		2/scale*sx, 0, 0, 0,
		0, 2/scale/sy, 0, 0,
		0, 0, 2/scale, 0,
		0, 0, 0, 1
	);
	Tv = Matrix4(
		1, 0, 0, -eye.x,
		0, 1, 0, -eye.y,
		0, 0, 1, -eye.z,
		0, 0, 0, 1
	);
	Rv = Matrix4(
		Rx.x, Rx.y, Rx.z, 0,
		Ry.x, Ry.y, Ry.z, 0,
		-Rz.x, -Rz.y, -Rz.z, 0,
		0, 0, 0, 1
	);
	
	if (fin) {
		P = Matrix4(
			2 / (right - left), 0, 0, -(right + left) / (right - left),
			0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
			0, 0, -2 / (fa - nea), -(fa + nea) / (fa - nea),
			0, 0, 0, 1
		);
	}
	else {
		P = Matrix4(
			2*nea / (right - left), 0, (right+left)/(right-left), 0,
			0, 2 *nea/ (top - bottom), (top + bottom) / (top - bottom), 0,
			0, 0, -(fa + nea) / (fa - nea), -2*fa*nea / (fa - nea),
			0, 0, -1, 0
		);
	}
	V = Rv*Tv;
	R = r1*r2*r3;
	M = T*R*S*N;
	
	//printf("%f %f %f\n", upn.x, upn.y, upn.z);
	Matrix4 MVP = P*V*M;
	
	GLfloat mvp[16];
	// row-major ---> column-major
	mvp[0] = MVP[0];  mvp[4] = MVP[1];   mvp[8]  = MVP[2];    mvp[12] = MVP[3];  
	mvp[1] = MVP[4];  mvp[5] = MVP[5];   mvp[9]  = MVP[6];    mvp[13] = MVP[7];  
	mvp[2] = MVP[8];  mvp[6] = MVP[9];   mvp[10] = MVP[10];   mvp[14] = MVP[11];  
	mvp[3] = MVP[12]; mvp[7] = MVP[13];  mvp[11] = MVP[14];   mvp[15] = MVP[15];

	// bind array pointers to shader
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(iLocColor, 3, GL_FLOAT, GL_FALSE, 0, colors);
	
	// bind uniform matrix to shader
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, mvp);
	
	// draw the array we just bound
	if (polygon) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	// draw the array we just bound
	glDrawArrays(GL_TRIANGLES, 0, (OBJ->numtriangles) * 3);
	float nx1, ny1, nz1;
	nx1 = -(maxVal1[0] + minVal1[0]) / 2, ny1 = -(maxVal1[1] + minVal1[1]) / 2, nz1 = -(maxVal1[2] + minVal1[2]) / 2;
	float scale1;
	scale1 = max((maxVal1[0] - minVal1[0]), (maxVal1[1] - minVal1[1]));
	scale1 = max(scale1, (maxVal1[2] - minVal1[2]));
	Matrix4 M1, T1, S1,N1;
	N1 = Matrix4(
		1, 0, 0, nx1,
		0, 1, 0, ny1,
		0, 0, 1, nz1,
		0, 0, 0, 1
	);
	T1 = Matrix4(
		1, 0, 0, 0,
		0, 1, 0, -1,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	S1 = Matrix4(
		2/scale1, 0, 0, 0,
		0, 2/scale1*0.03, 0, 0,
		0, 0, 2/scale1, 0,
		0, 0, 0, 1
	);
	M1 = T1*S1*N1;
	
	MVP = P*V*M1;
	mvp[0] = MVP[0];  mvp[4] = MVP[1];   mvp[8] = MVP[2];    mvp[12] = MVP[3];
	mvp[1] = MVP[4];  mvp[5] = MVP[5];   mvp[9] = MVP[6];    mvp[13] = MVP[7];
	mvp[2] = MVP[8];  mvp[6] = MVP[9];   mvp[10] = MVP[10];   mvp[14] = MVP[11];
	mvp[3] = MVP[12]; mvp[7] = MVP[13];  mvp[11] = MVP[14];   mvp[15] = MVP[15];
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices1);
	glVertexAttribPointer(iLocColor, 3, GL_FLOAT, GL_FALSE, 0, colors1);

	// bind uniform matrix to shader
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, mvp);

	// draw the array we just bound
	// draw the array we just bound
	tez = ez;
	glDrawArrays(GL_TRIANGLES, 0, (OBJ1->numtriangles) * 3);
	glutSwapBuffers();
}

void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled)
{
	glGetShaderiv(shader, GL_COMPILE_STATUS, shaderCompiled);
	if(GL_FALSE == (*shaderCompiled))
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character.
		GLchar *errorLog = (GLchar*) malloc(sizeof(GLchar) * maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		fprintf(stderr, "%s", errorLog);

		glDeleteShader(shader);
		free(errorLog);
	}
}

void setShaders()
{
	GLuint v, f, p;
	char *vs = NULL;
	char *fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("shader.vert");
	fs = textFileRead("shader.frag");

	glShaderSource(v, 1, (const GLchar**)&vs, NULL);
	glShaderSource(f, 1, (const GLchar**)&fs, NULL);

	free(vs);
	free(fs);

	// compile vertex shader
	glCompileShader(v);
	GLint vShaderCompiled;
	showShaderCompileStatus(v, &vShaderCompiled);
	if(!vShaderCompiled) system("pause"), exit(123);

	// compile fragment shader
	glCompileShader(f);
	GLint fShaderCompiled;
	showShaderCompileStatus(f, &fShaderCompiled);
	if(!fShaderCompiled) system("pause"), exit(456);

	p = glCreateProgram();

	// bind shader
	glAttachShader(p, f);
	glAttachShader(p, v);

	// link program
	glLinkProgram(p);

	iLocPosition = glGetAttribLocation (p, "av4position");
	iLocColor    = glGetAttribLocation (p, "av3color");
	iLocMVP		 = glGetUniformLocation(p, "mvp");
	color_link = glGetUniformLocation(p, "colorflag");
	glUseProgram(p);
}


void onMouse(int who, int state, int x, int y)
{
	printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);

	switch(who)
	{
		case GLUT_LEFT_BUTTON:   
			printf("left button   "); 
			tempx = x;
			tempy = y;
			
			break;
		case GLUT_MIDDLE_BUTTON: 
			printf("middle button "); 
			
			break;
		case GLUT_RIGHT_BUTTON:  
			printf("right button  "); 
			tempx = x;
			tempy = y;
			break; 
		case GLUT_WHEEL_UP:      
			printf("wheel up      ");
			if(mode==3)rotz += 0.04;
			else if (mode == 4)ez += 0.04;
			else if (mode == 7) {
				pz += 0.04;
				nea += pz;
				fa += -pz;
				printf("%f %f\n", nea, fa);
			}
			break;
		case GLUT_WHEEL_DOWN:    
			printf("wheel down    "); 
			if(mode==3)rotz -= 0.04;
			else if (mode == 4) {
					ez -= 0.04;
				printf("%f\n",ez);
			}
			else if (mode == 7) {
				pz -= 0.04;
				nea += -pz;
				fa += pz;
				printf("%f %f\n", nea, fa);
			}
			break;
		default:                 printf("0x%02X          ", who); break;
	}

	switch(state)
	{
		case GLUT_DOWN: 
			printf("start "); 
			
			break;
		case GLUT_UP:   
			printf("end   "); 
			tempy = 0;
			tempx = 0;
			ex = 0;
			ey = 0;
			cx = 0;
			cy = 0;
			break;
	}

	printf("\n");
}

void onMouseMotion(int x, int y)
{
	if (mode == 1) {
		start = false;
		printf("%18s(): (%d, %d) mouse move\n", __FUNCTION__, x, y);
		tx += (x - tempx) / 500;
		ty += -(y - tempy) / 500;
		tempx = x;
		tempy = y;
	}
	else if (mode == 2) {
		if (x - tempx > 0)
			sx = sx + ((x - tempx) / 1000);
		else {
			if(sx - ((tempx-x) / 1000)>0)sx = sx - ((tempx-x) / 1000);
		}
		if (y-tempy  > 0)
			sy = sy + ((y - tempy) / 1000);
		else {
			if (sy - ((tempy-y) / 1000)>0)sy = sy - ((tempy - y) / 1000);
		}
		tempx = x;
		tempy = y;
	}
	else if (mode == 3) {
		roty += (x - tempx)/100;
		rotx -= (y - tempy)/100;
		
		tempx = x;
		tempy = y;
	}
	else if (mode == 4) {
		ex = -(x - tempx) / 800;
		ey = (y - tempy) / 800;
		tempx = x;
		tempy = y;
	}
	else if (mode == 5) {
		cx = (x - tempx) / 800;
		cy = (y - tempy) / 800;
		tempx = x;
		tempy = y;
	}
	else if (mode == 7) {
		
		px =  + ((x - tempx) / 100);
		py =  - ((y - tempy) / 100);
		right += px;
		left -= px;
		top += -py;
		bottom -= -py;
		
		tempx = x;
		tempy = y;
	}
}

void onKeyboard(unsigned char key, int x, int y) 
{
	printf("%18s(): (%d, %d) key: %c(0x%02X) ", __FUNCTION__, x, y, key, key);
	switch(key) 
	{
		case GLUT_KEY_ESC: /* the Esc key */ 
			exit(0); 
			break;
		case 'z':
			index = (index - 1) % 6;
			if (index < 0)index = 5;
			loadOBJModel();
			printf("%d", index);
			break;
		case 'x':
			index = (index + 1) % 6;
			loadOBJModel();
			printf("%d", index);
			break;
		case 'h':
			printf("----------Help Menu----------\n*****keyboard control*****"
				"\nh: to show help menu\nz/x: switch different models\nw: switch solid/wireframe mode\nc: color filter function\n"
				"o: switch between orthogonal / perspective projection\ni: showcurrent model name and current control mode\n"
				"\nMODE SWITCHING\n"
				"t: go to OBJECT translate mode\ns: go to OBJECT scale mode\nr: go to OBJECT rotate mode\ne: go to EYE translate mode"
				"\nl: go to CENTER( look at )translate mode\np: go to PROJECTION mode\n*****end of keyboard control*****"
				"*****mouse control*****\nVALUE CONTROL\nmouse drag LEFT: decrease the value on X axis, mouse drag RIGHT: increase the value on X axis\n"
				"mouse drag DOWN: decrease the value on Y axis, mouse drag UP: increase the value on Y axis\n"
				"mouse wheel DOWN: deccrease the value on Z axis, mouse wheel UP: increase the value on Z axis\n*****end of mouse control*****\n\n\n"
				"----------Help Menu----------\n");
			break;
		case 'i':
			printf("\nmodel name : %s\n", filename[index]);
			if (mode == 1)printf("actived action mode : object translation\n");
			else if (mode == 2)printf("actived action mode : object scaling\n");
			else if (mode == 3)printf("actived action mode : object rotation\n");
			else if (mode == 4)printf("actived action mode : eye translation\n");
			else if (mode == 5)printf("actived action mode : center ( look at)translation\n");
			else if (mode == 6)printf("switch projection mode\n");
			else if (mode == 7)printf("actived action mode : projection control\n");
			break;
		case 'w':
			polygon = (polygon + 1) % 2;
			loadOBJModel();
			break;
		case 'c':
			colormode = (colormode + 1) % 4;
			
			printf("%d\n", colormode);
			glUniform1i(color_link, colormode);
			break;
		case 't':
			mode = 1;
			break;
		case 's':
			mode = 2;
			break;
		case 'r':
			mode = 3;
			break;
		case 'e':
			mode = 4;
			break;
		case 'l':
			mode = 5;
			break;
		case 'o':
			mode = 6;
			fin = !fin;
			break;
		case 'p':
			mode = 7;
			break;
	}
	printf("\n");
}

void onKeyboardSpecial(int key, int x, int y){
	printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);
	switch(key)
	{
		case GLUT_KEY_LEFT:
			printf("key: LEFT ARROW");
			break;
			
		case GLUT_KEY_RIGHT:
			printf("key: RIGHT ARROW");
			break;

		default:
			printf("key: 0x%02X      ", key);
			break;
	}
	printf("\n");
}


void onWindowReshape(int width, int height)
{
	printf("%18s(): %dx%d\n", __FUNCTION__, width, height);
}

int main(int argc, char **argv) 
{
	// glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// create window
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG HW2_103062315 ");

	glewInit();
	if(glewIsSupported("GL_VERSION_2_0")){
		printf("Ready for OpenGL 2.0\n");
	}else{
		printf("OpenGL 2.0 not supported\n");
		system("pause");
		exit(1);
	}

	// load obj models through glm
	loadOBJModel();

	// register glut callback functions
	glutDisplayFunc (onDisplay);
	glutIdleFunc    (onIdle);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc (onKeyboardSpecial);
	glutMouseFunc   (onMouse);
	glutMotionFunc  (onMouseMotion);
	glutReshapeFunc (onWindowReshape);

	// set up shaders here
	setShaders();
	
	glEnable(GL_DEPTH_TEST);

	// main loop
	glutMainLoop();

	// free
	glmDelete(OBJ);

	return 0;
}

