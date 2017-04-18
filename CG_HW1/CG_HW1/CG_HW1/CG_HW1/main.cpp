#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <freeglut/glut.h>
#include "textfile.h"
#include "glm.h"

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
char line[40] = "---------------------------------------";
char filename[][200] = { "ColorModels/bunny5KC.obj" ,"ColorModels/blitzcrank_incognito.obj","ColorModels/elephant16KC.obj","ColorModels/ziggs.obj" ,"ColorModels/cessna7KC.obj" ,"ColorModels/tigerC.obj" };
GLMmodel* OBJ;
int index = 0,polygon=0;
int colormode = 0;
GLfloat* vertices;
GLfloat* colors;
float color[3];
GLuint color_link;
void traverseColorModel()
{
	int i;

	GLfloat maxVal[3];
	GLfloat minVal[3];
	maxVal[0] = -1000;
	maxVal[1] = -1000;
	maxVal[2] = -1000;
	minVal[0] = 1000;
	minVal[1] = 1000;
	minVal[2] = 1000;
	// TODO:
	//// You should traverse the vertices and the colors of each triangle, and 
	//// then normalize the model to unit size. i.e. Each vertex should be 
	//// bounded in [-1, 1], which will fit the camera clipping window.


	// number of triangles
	OBJ->numtriangles;

	// number of vertices
	OBJ->numvertices;

	// The center position of the model 
	OBJ->position[0] = 0;
	OBJ->position[1] = 0;
	OBJ->position[2] = 0;
	int id = 0;
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

		
		//printf("fuck");
		vertices[i * 9 + 0] = vx;
		//printf("fuck2");
		vertices[i * 9 + 1] = vy;
		vertices[i * 9 + 2] = vz;
		//printf("%f %f %f %d\n", vertices[i * 9 + 0], vertices[i * 9 + 1], vertices[i * 9 + 2],id*3);
		id++;
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
		//printf("%f %f %f\n", vertices[i * 9 + 3], vertices[i * 9 + 4], vertices[i * 9 + 5]);
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

		//printf("%f %f %f %d\n",vx,vy,vz,3*id);
		//printf("%f %f %f\n", vertices[i * 9 + 6], vertices[i * 9 + 7], vertices[i * 9 + 8]);
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
	float scale = max((maxVal[0] - minVal[0]), (maxVal[1] - minVal[1]));
	scale = max(scale, (maxVal[2] - minVal[2]));
	for (i = 0; i<3*(int)OBJ->numtriangles; i++) {
		vertices[i * 3 + 0]  -=(maxVal[0] + minVal[0]) / 2;
		vertices[i * 3 + 1]  -=(maxVal[1] + minVal[1]) / 2;
		vertices[i * 3 + 2]  -=(maxVal[2] + minVal[2]) / 2;
	}
	for (i = 0; i < 9 * (int)OBJ->numtriangles; i++) {
		vertices[i] = (2 * vertices[i] / scale);
	}
	
	
	
}

void loadOBJModel()
{
	// read an obj model here
	if(OBJ != NULL){
		free(OBJ);
	}
	OBJ = glmReadOBJ(filename[index]);
	printf("%s\n", filename[index]);

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
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	static GLfloat triangle_vertex[] = {
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f
	};
	
	// bind array pointers to shader
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(   iLocColor, 3, GL_FLOAT, GL_FALSE, 0, colors);
	if (polygon) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	// draw the array we just bound
	glDrawArrays(GL_TRIANGLES, 0, (OBJ->numtriangles)*3);

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
	
	glLinkProgram(p);

	iLocPosition = glGetAttribLocation (p, "av4position");
	iLocColor    = glGetAttribLocation (p, "av3color");
	color_link = glGetUniformLocation(p, "colorflag");
	
	//p->setUniformLocationWith1f();
	glUseProgram(p);
}


void onMouse(int who, int state, int x, int y)
{
	printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);

	switch(who)
	{
		case GLUT_LEFT_BUTTON:   printf("left button   "); break;
		case GLUT_MIDDLE_BUTTON: printf("middle button "); break;
		case GLUT_RIGHT_BUTTON:  printf("right button  "); break; 
		case GLUT_WHEEL_UP:      printf("wheel up      "); break;
		case GLUT_WHEEL_DOWN:    printf("wheel down    "); break;
		default:                 printf("0x%02X          ", who); break;
	}

	switch(state)
	{
		case GLUT_DOWN: printf("start "); break;
		case GLUT_UP:   printf("end   "); break;
	}

	printf("\n");
}

void onMouseMotion(int x, int y)
{
	printf("%18s(): (%d, %d) mouse move\n", __FUNCTION__, x, y);
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
			//printf("%d", index);
			break;
		case 'x':
			index = (index + 1) % 6;
			loadOBJModel();
			//printf("%d", index);
			break;
		case 'h':	
			printf("\n%s\nh -to show help menu\nz/x -switch different models\nw -switch solid/wireframe mode\nc -color filter function\n%s\n",line,line);
			break;
		case 'w':
			polygon =(polygon+1)%2;
			loadOBJModel();
			break;
		case 'c':
			colormode = (colormode + 1) % 4;
			//setShaders();
			//loadOBJModel();
			printf("%d\n", colormode);
			glUniform1i(color_link,colormode);
			break;
		
	}
	printf("\n");
}

void onKeyboardSpecial(int key, int x, int y){
	printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);
	switch(key)
	{
		case GLUT_KEY_LEFT:
			break;
			
		case GLUT_KEY_RIGHT:
			
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
	glutCreateWindow("10420 CS550000 CG HW1 TA");

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

