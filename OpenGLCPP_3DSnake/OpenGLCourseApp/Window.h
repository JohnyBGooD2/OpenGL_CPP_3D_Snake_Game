#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#undef main  

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

	void getMovementDirectionFromKeyboard(); 
};
