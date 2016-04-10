#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLShaderLoader.h"
#include "PPMImage.h"

// window size
#define WIDTH 1200
#define HEIGHT 800

// delay const for 60fps
#define FPS 60
#define FPS_DELAY (1/FPS)*1000

// control sensitvity
#define ROT_SENSITVITY 1.0

// camera/light Zpos
#define Z_POS 5.0

using std::cout;

using glm::vec3;
using glm::mat4;
using glm::lookAt;
using glm::perspective;

void init();
void renderScene();
void renderTimer(int val);
void keyPress(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mousePosTracker(int x, int y);
void drawCube(vec3 scale, const mat4& rot);
void drawWindow();

// min and max pixel found
float minPx, maxPx;

// programs
GLuint solidCubeProg;
GLuint initWindow;
GLuint eqWindow;
GLuint thrsWindow;

// texture
GLuint texture;

// projection matrix
mat4 proj;

// key press buffer
bool keyStates[256];

// light pos
float lightX, lightY;

int main(int argc, char* args[])
{
	// create the window
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Opengl Assignment 5");
	glewInit();

	init();

	glutDisplayFunc(renderScene);
	glutTimerFunc(FPS_DELAY, renderTimer, 0);
	glutPassiveMotionFunc(mousePosTracker);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutMainLoop();

	// clean up
	OpenGLShaderLoader::getInstance().deleteProgram(solidCubeProg);
	OpenGLShaderLoader::getInstance().deleteProgram(initWindow);
	OpenGLShaderLoader::getInstance().deleteProgram(eqWindow);
	OpenGLShaderLoader::getInstance().deleteProgram(thrsWindow);

	return 0;
}

void renderScene()
{
	static float xRot = 0.0;
	static float yRot = 0.0;
	static float zRot = 0.0;

	// check for key press
	if (keyStates['w'])
		xRot += ROT_SENSITVITY;
	if (keyStates['s'])
		xRot -= ROT_SENSITVITY;
	if (keyStates['d'])
		yRot += ROT_SENSITVITY;
	if (keyStates['a'])
		yRot -= ROT_SENSITVITY;
	if (keyStates['q'])
		zRot += ROT_SENSITVITY;
	if (keyStates['e'])
		zRot -= ROT_SENSITVITY;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// calc the rot for the outer cube
	mat4 rot = glm::rotate(mat4(), glm::radians(xRot), vec3(1.0, 0.0, 0.0));
	rot = glm::rotate(rot, glm::radians(yRot), vec3(0.0, 1.0, 0.0));
	rot = glm::rotate(rot, glm::radians(zRot), vec3(0.0, 0.0, 1.0));
	drawCube(vec3(1.0), rot);
	drawWindow();
	glutSwapBuffers();
}

void init()
{
	// set init light pos
	lightX = 0.0;
	lightY = 0.0;

	glClearColor(0.75f, 0.75f, 0.75f, 1.0f); // Set background color to light grey
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	proj = perspective(45.0f, (float)(WIDTH - (WIDTH / 4)) / (float)HEIGHT, 0.1f, 1000.0f);

	// load the programs and textures
	try
	{
		PPMImage img("Img\\doctored_image.ppm");	// texture image
		minPx = img.getMin();
		maxPx = img.getMax();

		// programs
		solidCubeProg = OpenGLShaderLoader::getInstance().createProgram("Shaders\\SquareVertexShader.glsl", "Shaders\\thresholdFrgtShader.glsl");
		initWindow = OpenGLShaderLoader::getInstance().createProgram("Shaders\\WindowVertexShader.glsl", "Shaders\\windowFrgtShader.glsl");
		eqWindow = OpenGLShaderLoader::getInstance().createProgram("Shaders\\WindowVertexShader.glsl", "Shaders\\equalizedFrgtShader.glsl");
		thrsWindow = OpenGLShaderLoader::getInstance().createProgram("Shaders\\WindowVertexShader.glsl", "Shaders\\thresholdFrgtShader.glsl");

		// config and bind textures
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, img.getWidth(), img.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, img.image());
	}
	catch (std::exception& msg)
	{
		cout << msg.what();
		exit(-1);
	}
}

void renderTimer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(FPS_DELAY, renderTimer, 0);
}

void keyPress(unsigned char key, int x, int y)
{
	// set key as pressed
	keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y)
{
	// set key as not pressed
	keyStates[key] = false;
}

void mousePosTracker(int x, int y)
{
	// translate screen to world cords
	lightX = ((float)x / (WIDTH/2-1)) - 1.0;
	lightY = 1.0 - ((float)y / (HEIGHT/2-1));
}

void drawCube(vec3 scale, const mat4& rot)
{
	GLuint scaleUni;
	GLuint projecUni;
	GLuint viewUni;
	GLuint rotUni;
	GLuint maxUni;
	GLuint minUni;
	mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	static const mat4 viewMatrix = lookAt(vec3(0.0, 0.0, Z_POS), vec3(0.0), vec3(0.0, 1.0, 0.0));
	
	// set up the solid cube
	glUseProgram(solidCubeProg);
	glViewport((WIDTH / 4), 0, WIDTH-(WIDTH/4), HEIGHT);
	rotUni = glGetUniformLocation(solidCubeProg, "rot");
	scaleUni = glGetUniformLocation(solidCubeProg, "scale");
	projecUni = glGetUniformLocation(solidCubeProg, "projection");
	viewUni = glGetUniformLocation(solidCubeProg, "viewMatrix");
	maxUni = glGetUniformLocation(solidCubeProg, "maxPx");
	minUni = glGetUniformLocation(solidCubeProg, "minPx");
	
	// set the uniforms
	glUniformMatrix4fv(scaleUni, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
	glUniformMatrix4fv(projecUni, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(viewUni, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(rotUni, 1, GL_FALSE, glm::value_ptr(rot));
	glUniform1f(maxUni, maxPx);
	glUniform1f(minUni, minPx);

	// draw
	glDrawArrays(GL_QUADS, 0, 24);
}

void drawWindow()
{
	GLuint maxUni;
	GLuint minUni;

	glUseProgram(initWindow);	// set the program
	glViewport(0, 800 - (HEIGHT / 3)*1, WIDTH / 4, HEIGHT / 3);

	// config the texturer
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// draw
	glDrawArrays(GL_QUADS, 0, 4);

	glUseProgram(eqWindow);
	// draw
	glViewport(0, 800 - (HEIGHT / 3)*2, WIDTH / 4, HEIGHT / 3);
	maxUni = glGetUniformLocation(eqWindow, "maxPx");
	minUni = glGetUniformLocation(eqWindow, "minPx");
	glUniform1f(maxUni, maxPx);
	glUniform1f(minUni, minPx);
	glDrawArrays(GL_QUADS, 0, 4);

	glUseProgram(thrsWindow);
	// draw
	glViewport(0, 800 - (HEIGHT / 3)*3, WIDTH / 4, HEIGHT / 3);
	maxUni = glGetUniformLocation(thrsWindow, "maxPx");
	minUni = glGetUniformLocation(thrsWindow, "minPx");
	glUniform1f(maxUni, maxPx);
	glUniform1f(minUni, minPx);
	glDrawArrays(GL_QUADS, 0, 4);
}