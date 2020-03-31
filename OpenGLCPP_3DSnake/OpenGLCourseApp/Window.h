#include <iostream>

#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <SDL.h>

//#include "MeshSpawner.h"
#undef main   // https://stackoverflow.com/questions/6847360/error-lnk2019-unresolved-external-symbol-main-referenced-in-function-tmainc
//#include <SDL_opengl.h>


#pragma once
class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int initialize();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }
	bool getShouldClose();

    enum MovementDirection {forward, backwards, left, right} MoveDirection;
	


	bool* getKeys() { return keys; } // pointer because array as output
	

	GLfloat getXChange(); 
	GLfloat getYChange();

	void swapBuffers() { SDL_GL_SwapWindow(mainWindow); };

	~Window();
private:
	SDL_Window* mainWindow;            
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat xChange;
	GLfloat yChange;
	int xPos;
	int yPos;

	bool mouseFirstMoved;

	void getMovementDirectionFromKeyboard(); // { return MoveDirection; };

};
