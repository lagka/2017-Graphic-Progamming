#include <stdio.h>
#include <stdlib.h>

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
GLint iLocNormal;
GLint iLocMVP;
GLint iLocEye;
GLint iLocMDiffuse, iLocMAmbient, iLocMSpecular, iLocMShininess;
GLint iLocLDAmbient,iLocLDPosition,getmode0,getmode1,getmode2;
GLint iLocLDDiffuse, iLocDPosition,Normal, iLocLDDPosition,iLocSpecular,iLocDDAmbient;
GLint iLocDPAmbient, iLocLPDiffuse, iLocLPDPosition, iLocPSpecular, iLocPconstantAttenuation, iLocPlinearAttenuation, iLocPquadraticAttenuation;
GLint directionmode,MM;
GLint iflag;
GLint pointlight;
GLuint SPOT_EFFECT;
GLuint shader_type;
GLuint iLocDSAmbient, iLocLSDiffuse, iLocLSDPosition, iLocSSpecular, iLocSpotDirection, iLocSpotExponent, iLocSpotCutoff, iLocSpotCosCutoff;
char filename[][200] = { "NormalModels/Model/dragon10KN.obj" ,"NormalModels/Model/bunny5KN.obj","NormalModels/Model/cessna7KN.obj","NormalModels/Model/Dino20KN.obj","NormalModels/Model/elephant16KN.obj" };
GLMmodel* OBJ;
GLfloat* vertices;
GLfloat* normals;
float left = -1, right = 1, top = 1, bottom = -1;
float nea = 1.0f, fa = 10.0f;
Matrix4 N;
int flag = 1;
int shading_mode = 1;
int mode[3] = {0};
int change = 3;
int poi = 1;
int shad = 0;
int spoteffect = 1;
int distance;
int rotate = 1;
int index = 0;
float  cx = 0, cy = 0, cz = 0, tez = 0, px = 0, py = 0, pz = 0;
Vector3 eye = Vector3(0.0f, 0.0f, 2.0f);//P1
Vector3 center = Vector3(0.0f, 0.0f, -5.0f); //P2
float roty=0;
struct LightSourceParameters {
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float position[4];
	float halfVector[4];
	float spotDirection[4];
	float spotExponent;
	float spotCutoff; // (range: [0.0,90.0], 180.0)
	float spotCosCutoff; // (range: [1.0,0.0],-1.0)
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
}typedef LightSource;
LightSource lightsource[4];

void traverseColorModel()
{
	int i;
	
	GLfloat maxVal[3];
	GLfloat minVal[3];

	// TODO:
	//// You should traverse the vertices and the colors of each triangle, and 
	//// then normalize the model to unit size by using transformation matrices. 
	//// i.e. Each vertex should be bounded in [-1, 1], which will fit the camera clipping window.


	// number of triangles
	vertices = (GLfloat*)malloc(sizeof(GLfloat)*OBJ->numtriangles*9);
	normals = (GLfloat*)malloc(sizeof(GLfloat)*OBJ->numtriangles*9);

	float max_x = OBJ->vertices[3];
	float max_y = OBJ->vertices[4];
	float max_z = OBJ->vertices[5];
	float min_x = OBJ->vertices[3];
	float min_y = OBJ->vertices[4];
	float min_z = OBJ->vertices[5];

	for(i=0; i<(int)OBJ->numtriangles; i++)
	{
		// the index of each vertex
		int indv1 = OBJ->triangles[i].vindices[0];
		int indv2 = OBJ->triangles[i].vindices[1];
		int indv3 = OBJ->triangles[i].vindices[2];
		int nindv1 = OBJ->triangles[i].nindices[0];
		int nindv2 = OBJ->triangles[i].nindices[1];
		int nindv3 = OBJ->triangles[i].nindices[2];
		// the index of each color
		int indc1 = indv1;
		int indc2 = indv2;
		int indc3 = indv3;
		int ninidc1 = nindv1;
		int ninidc2 = nindv2;
		int ninidc3 = nindv3;
		// vertices
		
		vertices[i*9 + 0] = OBJ->vertices[indv1*3+0];
		vertices[i*9 + 1] = OBJ->vertices[indv1*3+1];
		vertices[i*9 + 2] = OBJ->vertices[indv1*3+2];
		if(vertices[i*9 + 0] > max_x) max_x = vertices[i*9 + 0];
		if(vertices[i*9 + 1] > max_y) max_y = vertices[i*9 + 1]; 
		if(vertices[i*9 + 2] > max_z) max_z = vertices[i*9 + 2];
		if(vertices[i*9 + 0] < min_x) min_x = vertices[i*9 + 0];
		if(vertices[i*9 + 1] < min_y) min_y = vertices[i*9 + 1];
		if(vertices[i*9 + 2] < min_z) min_z = vertices[i*9 + 2];

		vertices[i*9 + 3] = OBJ->vertices[indv2*3+0];
		vertices[i*9 + 4] = OBJ->vertices[indv2*3+1];
		vertices[i*9 + 5] = OBJ->vertices[indv2*3+2];
		if(vertices[i*9 + 3] > max_x) max_x = vertices[i*9 + 3];
		if(vertices[i*9 + 4] > max_y) max_y = vertices[i*9 + 4];
		if(vertices[i*9 + 5] > max_z) max_z = vertices[i*9 + 5];
		if(vertices[i*9 + 3] < min_x) min_x = vertices[i*9 + 3];
		if(vertices[i*9 + 4] < min_y) min_y = vertices[i*9 + 4];
		if(vertices[i*9 + 5] < min_z) min_z = vertices[i*9 + 5];
	
		vertices[i*9 + 6] = OBJ->vertices[indv3*3+0];
		vertices[i*9 + 7] = OBJ->vertices[indv3*3+1];
		vertices[i*9 + 8] = OBJ->vertices[indv3*3+2];
		if(vertices[i*9 + 6] > max_x) max_x = vertices[i*9 + 6];
		if(vertices[i*9 + 7] > max_y) max_y = vertices[i*9 + 7];
		if(vertices[i*9 + 8] > max_z) max_z = vertices[i*9 + 8];
		if(vertices[i*9 + 6] < min_x) min_x = vertices[i*9 + 6];
		if(vertices[i*9 + 7] < min_y) min_y = vertices[i*9 + 7];
		if(vertices[i*9 + 8] < min_z) min_z = vertices[i*9 + 8];

		// colors

		normals[i*9 + 0] = OBJ->normals[nindv1*3+0];
		normals[i*9 + 1] = OBJ->normals[nindv1*3+1];
		normals[i*9 + 2] = OBJ->normals[nindv1*3+2];

		normals[i*9 + 3] = OBJ->normals[nindv2*3+0];
		normals[i*9 + 4] = OBJ->normals[nindv2*3+1];
		normals[i*9 + 5] = OBJ->normals[nindv2*3+2];

		normals[i*9 + 6] = OBJ->normals[nindv3*3+0];
		normals[i*9 + 7] = OBJ->normals[nindv3*3+1];
		normals[i*9 + 8] = OBJ->normals[nindv3*3+2];


	}
	float normalize_scale = max(max(abs(max_x-min_x),abs(max_y-min_y)),abs(max_z-min_z));

	Matrix4 S,T;
	S.identity();
	T.identity();
	S[0] = 2/normalize_scale;
	S[5] = 2/normalize_scale;;
	S[10] = 2/normalize_scale;
	T[3] = -(min_x+max_x)/2;
	T[7] = -(min_y+max_y)/2;
	T[11] = -(min_z+max_z)/2;

	N = S*T;

}

void loadOBJModel()
{
	// read an obj model here
	if(OBJ != NULL){
		free(OBJ);
	}
	OBJ = glmReadOBJ(filename[index]);
	printf("%s\n", filename[index]);

	glmFacetNormals(OBJ);
	glmVertexNormals(OBJ, 90.0);

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
	glEnableVertexAttribArray(iLocNormal);

	// organize the arrays
	static GLfloat triangle_normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f
	};

	static GLfloat triangle_vertex[] = {
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};

	static GLfloat ambient[]={0.500000, 0.500000, 0.500000};
	static GLfloat diffuse[]={0.784314, 0.470588, 0.752941};
	static GLfloat specular[]={1.000000,1.000000, 1.000000};
	static GLfloat shininess =100;

	// TODO:
	//// Please define the model transformation matrix, viewing transformation matrix, 
	//// projection transformation matrix

	//MVP
	Matrix4 T;
	Matrix4 S;
	Matrix4 R;

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
	Matrix4 P = Matrix4(
						1, 0, 0, 0, 
						0, 1, 0, 0,
						0, 0, -1, 0,
						0, 0, 0, 1);
	P = Matrix4(
		2 * nea / (right - left), 0, (right + left) / (right - left), 0,
		0, 2 * nea / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, -(fa + nea) / (fa - nea), -2 * fa*nea / (fa - nea),
		0, 0, -1, 0
	);
	Matrix4 Tv, Rv;

	Vector3 up = Vector3(0.0f, 1.0f, 0.0f); //P3
	Vector3 Forward, Right, UP, upn;
	
	center += Vector3(cx, cy, cz);
	Forward = center - eye;
	Right = Forward.cross(up);
	upn = Right.cross(Forward);
	upn = upn;
	up = eye + upn;
	Vector3 Rx, Ry, Rz;
	Rz = (center - eye) / (center - eye).length();
	Rx = (center - eye).cross(up - eye) / ((center - eye).cross(up - eye)).length();
	Ry = Rx.cross(Rz);
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
	
	float siny = sin(roty);
	float cosy = cos(roty);

	Matrix4 r2 = Matrix4(
		cosy, 0, siny, 0,
		0, 1, 0, 0,
		-siny, 0, cosy, 0,
		0, 0, 0, 1);
	if (rotate == 0) {
		roty += 0.01;
	}
	else {
		roty = 0;
	}
	V = Rv*Tv;
	R = r2;
	M = R*N;
	Matrix4 MVP = P*V*M;
	
	GLfloat mvp[16];
	// row-major ---> column-major
	mvp[0] = MVP[0];  mvp[4] = MVP[1];   mvp[8]  = MVP[2];    mvp[12] = MVP[3];  
	mvp[1] = MVP[4];  mvp[5] = MVP[5];   mvp[9]  = MVP[6];    mvp[13] = MVP[7];  
	mvp[2] = MVP[8];  mvp[6] = MVP[9];   mvp[10] = MVP[10];   mvp[14] = MVP[11];  
	mvp[3] = MVP[12]; mvp[7] = MVP[13];  mvp[11] = MVP[14];   mvp[15] = MVP[15];
	GLfloat N2[16];
	// row-major ---> column-major
	N2[0] = M[0];  N2[4] = M[1];   N2[8] = M[2];    N2[12] = M[3];
	N2[1] = M[4];  N2[5] = M[5];   N2[9] = M[6];    N2[13] = M[7];
	N2[2] = M[8];  N2[6] = M[9];   N2[10] = M[10];   N2[14] = M[11];
	N2[3] = M[12]; N2[7] = M[13];  N2[11] = M[14];   N2[15] = M[15];
	GLfloat M2[16];
	M2[0] = R[0];  M2[4] = R[1];   M2[8] = R[2];    M2[12] = R[3];
	M2[1] = R[4];  M2[5] = R[5];   M2[9] = R[6];    M2[13] = R[7];
	M2[2] = R[8];  M2[6] = R[9];   M2[10] = R[10];   M2[14] = R[11];
	M2[3] = R[12]; M2[7] = R[13];  M2[11] = R[14];   M2[15] = R[15];
	//pass model material value to the shader
	glUniform4fv(iLocMAmbient, 1, OBJ->materials[1].ambient);
	glUniform4fv(iLocMDiffuse, 1, OBJ->materials[1].diffuse);
	glUniform4fv(iLocMSpecular, 1, OBJ->materials[1].specular);
	glUniform1f(iLocMShininess, shininess);
	
	
	// bind array pointers to shader
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(iLocNormal, 3, GL_FLOAT, GL_FALSE, 0, normals);

	
	// bind uniform matrix to shader
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, mvp);
	glUniformMatrix4fv(Normal, 1, GL_FALSE, N2);
	glUniformMatrix4fv(MM, 1, GL_FALSE, M2);
	
	//printf("%d\n", flag);
	// draw the array we just bound
	glUniform1i(getmode0, mode[0]);
	glUniform1i(getmode1, mode[1]);
	glUniform1i(getmode2, mode[2]);
	
	
	
	glUniform4fv(iLocLSDPosition, 1, lightsource[3].position);
	glDrawArrays(GL_TRIANGLES, 0, OBJ->numtriangles*3);

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
void setLightingSource(){

	lightsource[0].position[0] = 0;
	lightsource[0].position[1] = 0;
	lightsource[0].position[2] = -1;
	lightsource[0].position[3] = 1;
	lightsource[0].ambient[0] = 0.75;
	lightsource[0].ambient[1] = 0.75;
	lightsource[0].ambient[2] = 0.75;
	lightsource[0].ambient[3] = 1;
	//To Do : Setup your own lighting source parameters;
	lightsource[1].ambient[0] = 0.7;
	lightsource[1].ambient[1] = 0.7;
	lightsource[1].ambient[2] = 0.7;
	lightsource[1].ambient[3] = 1;
	lightsource[1].diffuse[0] = 1;
	lightsource[1].diffuse[1] = 1;
	lightsource[1].diffuse[2] = 1;
	lightsource[1].diffuse[3] = 1;
	if (!change) {
		lightsource[1].position[0] = 0;
		lightsource[1].position[1] = 1;
		lightsource[1].position[2] = 0;
		lightsource[1].position[3] = 1;
	}
	else if (change == 1) {
		lightsource[1].position[0] = 1;
		lightsource[1].position[1] = 0;
		lightsource[1].position[2] = 1;
		lightsource[1].position[3] = 1;
	}
	else {
		lightsource[1].position[0] = 1;
		lightsource[1].position[1] = 1;
		lightsource[1].position[2] = 1;
		lightsource[1].position[3] = 1;
	}
	lightsource[1].specular[0] = 1;
	lightsource[1].specular[1] = 1;
	lightsource[1].specular[2] = 1;
	lightsource[1].specular[3] = 1;
	
	//point light
	lightsource[2].position[0] = 1;
	lightsource[2].position[1] = 0;
	lightsource[2].position[2] = 1;
	lightsource[2].position[3] = 1;
	lightsource[2].ambient[0] = 0.7;
	lightsource[2].ambient[1] = 0.7;
	lightsource[2].ambient[2] = 0.7;
	lightsource[2].ambient[3] = 1.0;
	lightsource[2].diffuse[0] = 1;
	lightsource[2].diffuse[1] = 1;
	lightsource[2].diffuse[2] = 1;
	lightsource[2].diffuse[3] = 1;
	lightsource[2].specular[0] = 1;
	lightsource[2].specular[1] = 1;
	lightsource[2].specular[2] = 1;
	lightsource[2].specular[3] = 1;
	lightsource[2].constantAttenuation = 1;
	lightsource[2].linearAttenuation = 4.5/100 ;
	lightsource[2].quadraticAttenuation = 75/100/100;

	//spotlight
	lightsource[3].position[0] = 0;
	lightsource[3].position[1] = 0;
	lightsource[3].position[2] = 1;
	lightsource[3].position[3] = 1;
	lightsource[3].ambient[0] = 0.7;
	lightsource[3].ambient[1] = 0.7;
	lightsource[3].ambient[2] = 0.7;
	lightsource[3].ambient[3] = 1.0;
	lightsource[3].diffuse[0] = 1;
	lightsource[3].diffuse[1] = 1;
	lightsource[3].diffuse[2] = 1;
	lightsource[3].diffuse[3] = 1;
	lightsource[3].specular[0] = 1;
	lightsource[3].specular[1] = 1;
	lightsource[3].specular[2] = 1;
	lightsource[3].specular[3] = 1;
	lightsource[3].spotCosCutoff = 0.96593502628;
	lightsource[3].spotExponent = 0.1;
	lightsource[3].spotCutoff = 45;
	
	lightsource[3].spotDirection[0] = 0;
	lightsource[3].spotDirection[1] = 0;
	lightsource[3].spotDirection[2] = -2;
	lightsource[3].spotDirection[3] = 0;

}

void setShaders()
{
	GLuint v, f, p;
	char *vs = NULL;
	char *fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	if (shading_mode) {
		vs = textFileRead("sample.vert");
		fs = textFileRead("sample.frag");
	}
	else if (shading_mode == 0) {
		vs = textFileRead("shader.vert");
		fs = textFileRead("shader.frag");
	}
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
	iLocNormal = glGetAttribLocation(p, "av3normal");
	iLocMVP		 = glGetUniformLocation(p, "mvp");
	Normal = glGetUniformLocation(p, "Normal");

	iLocMDiffuse = glGetUniformLocation(p, "Material.diffuse");
	iLocMAmbient = glGetUniformLocation(p, "Material.ambient");
	iLocMSpecular = glGetUniformLocation(p, "Material.specular");
	iLocMShininess = glGetUniformLocation(p, "Material.shininess");

	iLocLDAmbient = glGetUniformLocation(p, "LightSource[0].ambient");
	iLocLDPosition = glGetUniformLocation(p, "LightSource[0].position");
	iLocDDAmbient = glGetUniformLocation(p, "LightSource[1].ambient");
	iLocLDDiffuse = glGetUniformLocation(p, "LightSource[1].diffuse");
	iLocLDDPosition = glGetUniformLocation(p, "LightSource[1].position");
	iLocSpecular = glGetUniformLocation(p, "LightSource[1].specular");
	
	MM = glGetUniformLocation(p, "MM");
	iLocDPAmbient = glGetUniformLocation(p, "LightSource[2].ambient");
	iLocLPDiffuse = glGetUniformLocation(p, "LightSource[2].diffuse");
	iLocLPDPosition = glGetUniformLocation(p, "LightSource[2].position");
	iLocPSpecular = glGetUniformLocation(p, "LightSource[2].specular");
	
	iLocPconstantAttenuation = glGetUniformLocation(p, "LightSource[2].constantAttenuation");
	iLocPlinearAttenuation = glGetUniformLocation(p, "LightSource[2].linearAttenuation");
	iLocPquadraticAttenuation = glGetUniformLocation(p, "LightSource[2].quadraticAttenuation");
	iLocDSAmbient = glGetUniformLocation(p, "LightSource[3].ambient");
	iLocLSDiffuse = glGetUniformLocation(p, "LightSource[3].diffuse");
	iLocLSDPosition = glGetUniformLocation(p, "LightSource[3].position");
	iLocSSpecular = glGetUniformLocation(p, "LightSource[3].specular");
	iLocSpotDirection = glGetUniformLocation(p, "LightSource[3].spotDirection");
	iLocSpotExponent = glGetUniformLocation(p, "LightSource[3].spotExponent");
	iLocSpotCutoff = glGetUniformLocation(p, "LightSource[3].spotCutoff");
	iLocSpotCosCutoff = glGetUniformLocation(p, "LightSource[3].spotCosCutoff");
	
	directionmode = glGetUniformLocation(p, "dl");
	getmode0 = glGetUniformLocation(p, "mod[0]");
	getmode1 = glGetUniformLocation(p, "mod[1]");
	getmode2 = glGetUniformLocation(p, "mod[2]");
	pointlight = glGetUniformLocation(p, "poi");
	iLocEye = glGetUniformLocation(p, "EyePoint");
	SPOT_EFFECT = glGetUniformLocation(p, "spoteffect");
	shader_type = glGetUniformLocation(p, "shad");

	glUseProgram(p);
	GLfloat eye2[3];
	eye2[0] = eye[0]; eye2[1] = eye[1]; eye2[2] = eye[2]; 
	glUniform4fv(iLocLDAmbient, 1, lightsource[0].ambient);
	glUniform4fv(iLocLDPosition, 1, lightsource[0].position);
	glUniform4fv(iLocDDAmbient, 1, lightsource[1].ambient);
	glUniform4fv(iLocLDDiffuse, 1, lightsource[1].diffuse);
	glUniform4fv(iLocLDDPosition, 1, lightsource[1].position);
	glUniform4fv(iLocSpecular, 1, lightsource[1].specular);
	
	glUniform1i(directionmode, change);
	glUniform4fv(iLocLPDPosition, 1, lightsource[2].position);
	glUniform4fv(iLocDPAmbient, 1, lightsource[2].ambient);
	glUniform4fv(iLocLPDiffuse, 1, lightsource[2].diffuse);
	glUniform1i(pointlight, poi);
	glUniform1i(SPOT_EFFECT, spoteffect);
	
	glUniform4fv(iLocPSpecular, 1, lightsource[2].specular);
	glUniform4fv(iLocDSAmbient, 1, lightsource[3].ambient);
	glUniform4fv(iLocLSDiffuse, 1, lightsource[3].diffuse);
	glUniform4fv(iLocLSDPosition, 1, lightsource[3].position);
	glUniform4fv(iLocSSpecular, 1, lightsource[3].specular);
	glUniform4fv(iLocSpotDirection,1, lightsource[3].spotDirection);
	glUniform1f(iLocSpotExponent, lightsource[3].spotExponent);
	glUniform1f(iLocSpotCosCutoff, lightsource[3].spotCosCutoff);
	glUniform1f(iLocSpotCutoff, lightsource[3].spotCutoff);
	glUniform1f(iLocPconstantAttenuation, lightsource[2].constantAttenuation);
	glUniform1f(iLocPlinearAttenuation,  lightsource[2].linearAttenuation);
	glUniform1f(iLocPquadraticAttenuation,  lightsource[2].quadraticAttenuation);
	glUniform1i(shader_type, shad);
	
	
	glUniform3fv(iLocEye, 1, eye2);

}


void onMouse(int who, int state, int x, int y)
{
	//printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);

	switch(who)
	{
		case GLUT_LEFT_BUTTON:   
			printf("left button   "); 
			lightsource[3].spotExponent -= 10;
			if (lightsource[3].spotExponent <- 50)lightsource[3].spotExponent = -50;
			glUniform1f(iLocSpotExponent, lightsource[3].spotExponent);
			break;
		case GLUT_MIDDLE_BUTTON: printf("middle button "); break;
		case GLUT_RIGHT_BUTTON:  
			lightsource[3].spotExponent += 10;
			if (lightsource[3].spotExponent > 50)lightsource[3].spotExponent = 50;
			glUniform1f(iLocSpotExponent, lightsource[3].spotExponent);
			printf("right button  "); 
			break; 
		case GLUT_WHEEL_UP:     
			lightsource[3].spotCosCutoff -= 0.05;
			glUniform1f(iLocSpotCosCutoff, lightsource[3].spotCosCutoff);
			printf("wheel up      "); 
			break;
		case GLUT_WHEEL_DOWN:
			lightsource[3].spotCosCutoff += 0.005;
			glUniform1f(iLocSpotCosCutoff, lightsource[3].spotCosCutoff);
			printf("wheel down    "); 
			break;
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
	//printf("hello\n\n%18s(): fuckyou(%d, %d) fuckyou\n", __FUNCTION__, x, y);
	lightsource[3].position[0] = (float)(x-400) / 800 * 2;
	lightsource[3].position[1] = (float)-(y-400) / 800 * 2;
	//printf("%f %f\n", lightsource[3].position[0], lightsource[3].position[1]);
	glUniform4fv(iLocLSDPosition, 1, lightsource[3].position);
	
}

void onKeyboard(unsigned char key, int x, int y) 
{
	//printf("%18s(): (%d, %d) key: %c(0x%02X) ", __FUNCTION__, x, y, key, key);
	switch(key) 
	{
		case GLUT_KEY_ESC: /* the Esc key */ 
			exit(0); 
			break;
		case 'a':
			mode[0]=(mode[0]+1)%2;
			if (mode[0])printf("Ambient off");
			else printf("Ambient on");
			break;
		case 's':
			mode[1] = (mode[1]+1)%2;
			if (mode[1])printf("Diffuse off");
			else printf("Diffuse on");
			break;
		case 'd':
			mode[2] = (mode[2]+1)%2;
			if (mode[2])printf("Specular off");
			else printf("Specular on");
			break;
		case 'q':
			change = (change + 1) % 4;
			if (change == 3)printf("Directional Light off");
			else printf("Directional Light on");
			setLightingSource();
			glUniform4fv(iLocLDDPosition, 1, lightsource[1].position);
			glUniform1i(directionmode, change);
			break;
		case 'w':
			poi = (poi + 1) % 2;
			if (poi)printf("Point Light off");
			else printf("Point Light on");
			glUniform1i(pointlight, poi);
			break;
		case 'e':
			spoteffect = (spoteffect + 1) % 2;
			if (spoteffect)printf("Spot Light off");
			else printf("Spot Light on");
			glUniform1i(SPOT_EFFECT, spoteffect);
			break;
		case 'r':
			rotate = (rotate + 1) % 2;
			if (rotate) {
				printf("Auto Rotation on");
			}
			else if (!rotate) {
				printf("Auto Rotation off");
			}
			break;
		case 'z':
			index = (index - 1) % 5;
			if (index < 0)index = 5;
			printf("Current Model:%s\n", filename[index]);
			loadOBJModel();
			break;
		case 'x':
			index = (index + 1) % 5;
			printf("Current Model:%s\n", filename[index]);
			loadOBJModel();
			break;
		case 'f':
			shading_mode = (shading_mode + 1) % 2;
			if (shading_mode) {
				printf("\nper vertor\n");
			}
			else if (!shading_mode) {
				printf("\nper-pixel\n");
			}
			setShaders();
			loadOBJModel();
			break;
		case 'g':
			shad = (shad + 1) % 2;
			if (shad) {
				printf("\nOriginal Phong Reflection Model\n");
			}
			else if (shad == 0) {
				printf("\nBlinn-Phong Reflection Model\n");
			}
			glUniform1i(shader_type, shad);
			break;
		case 'h':
			printf("\n----------Help Menu----------\npress 'q' 'w' 'e' to toggle the light source\npress 'r' to toggle auto rotation\n"
				"press 'a' 's' 'd' to toggle the light attribute\npress 'f' to toggle per-pixel rendering\npress 'z' 'x' to change model"
				"press 'g' to change between Original Phong Reflection Model and Blinn-Phong Reflection Model\n"
				"use arrow button to move the point light\nhover mouse to move the spot light\nclick mouse to tune EXP\nscroll mouse to tune CUT_OFF_ANGLE"
			);
			break;

	}
	printf("\n");
}

void onKeyboardSpecial(int key, int x, int y){
	//printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);
	switch(key)
	{
		case GLUT_KEY_LEFT:
			printf("key: LEFT ARROW");
			lightsource[2].position[0] -= 0.01;
			
			glUniform4fv(iLocLPDPosition, 1, lightsource[2].position);
			break;
			
		case GLUT_KEY_RIGHT:
			printf("key: RIGHT ARROW");
			lightsource[2].position[0] += 0.01;
			
			glUniform4fv(iLocLPDPosition, 1, lightsource[2].position);
			break;
		case GLUT_KEY_UP:
			lightsource[2].position[2] += 0.01;
			
			glUniform4fv(iLocLPDPosition, 1, lightsource[2].position);
			break;
		case GLUT_KEY_DOWN:
			lightsource[2].position[2] -= 0.01;
			
			glUniform4fv(iLocLPDPosition, 1, lightsource[2].position);
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
	glutCreateWindow("10420 CS550000 CG HW2 TA");

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
	glutPassiveMotionFunc(onMouseMotion);
	//set up lighting parameters
	setLightingSource();

	// set up shaders here
	setShaders();
	
	glEnable(GL_DEPTH_TEST);

	// main loop
	glutMainLoop();

	// free
	glmDelete(OBJ);

	return 0;
}

