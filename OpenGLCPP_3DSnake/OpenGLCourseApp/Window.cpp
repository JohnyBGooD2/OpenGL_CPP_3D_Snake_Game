#include "Window.h"
//#include "SDL.h"

Window::Window()
{
	width = 800;
	height = 600;
	bufferWidth = 0, bufferHeight = 0;
	mainWindow = 0;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	bufferWidth = 0, bufferHeight = 0;
	mainWindow = 0;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::initialize()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	mainWindow = SDL_CreateWindow("Main window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
	SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);


	if (!mainWindow)
	{
		std::string msg = "Window could not be created: ";
		msg.append(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Init Failed", msg.c_str(), nullptr);

		SDL_Quit();
		return 1;
	}

	// OpenGL viewport dimensions
	SDL_GetWindowSize(mainWindow, &bufferWidth, &bufferHeight); // https://metanit.com/cpp/tutorial/3.3.php     
                                                                //https://metanit.com/cpp/tutorial/4.6.php

	// Set context for GLEW to use
	SDL_GL_CreateContext(mainWindow);

    SDL_SetWindowGrab(mainWindow, SDL_TRUE); // Lock cursor to window !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
	SDL_ShowCursor(SDL_DISABLE);
	//SDL_CaptureMouse(SDL_TRUE); // Check outside the window

	glewExperimental = GLU_TRUE;
	SDL_GL_SetSwapInterval(0); // This removes camera lag

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cerr << "GLEW initialization failed...\n";
		SDL_DestroyWindow(mainWindow);
		SDL_Quit();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	return 0;
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

//Window::MovementDirection Window::getMovementDirectionFromKeyboard()
//{
//	return MovementDirection();
//}

void Window::getMovementDirectionFromKeyboard()
{
	//return x < a ? a : (x > b ? b : x);

	if (keys[153]) { (MoveDirection == left || MoveDirection == right) ? MoveDirection = forward : MoveDirection; }
	else if (keys[152])	{ (MoveDirection == left || MoveDirection == right) ? MoveDirection = backwards : MoveDirection;}
	else if (keys[151])	{ (MoveDirection == forward || MoveDirection == backwards) ? MoveDirection = left : MoveDirection;}
	else if (keys[150]) { (MoveDirection == forward || MoveDirection == backwards) ? MoveDirection = right : MoveDirection; }
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SDL events~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

bool Window::getShouldClose()
{
	//SDL_PumpEvents();

	SDL_Event event;

	if (SDL_PollEvent(&event))
	{

		if (event.type == SDL_QUIT)
		{
			return true;
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			if (!mouseFirstMoved)
			{
				SDL_WarpMouseInWindow(mainWindow, width / 2, height / 2);
				mouseFirstMoved = true;
			}
			else
			{

				SDL_GetMouseState(&xPos, &yPos); // if pointers are in the func argument, then pass refs to normal vars there
				//printf("%.5f, %.5\n", (float)xPos, (float)yPos);
				yChange = height / 2 - yPos;   //event.motion.y;
				xChange = xPos - width / 2;//event.motion.x - width/2;
			}

			SDL_WarpMouseInWindow(mainWindow, width / 2, height / 2); // Set Mouse Position to ZERO!!!!!!!!!!!!

			return false;
		}
		else
		{
			switch (event.type)
			{

			case SDL_KEYDOWN:

				// https://wiki.libsdl.org/SDLKeycodeLookup

				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					SDL_DestroyWindow(mainWindow);
					return true;
				}
				
				if (event.key.keysym.sym > 127)
				{
					keys[event.key.keysym.sym - 1073741753] = 1;
				}
				else
				{            
					keys[event.key.keysym.sym] = 1;
				}
                break;
			case SDL_KEYUP:
				if (event.key.keysym.sym > 127)
				{
					keys[event.key.keysym.sym - 1073741753] = 0;
				}
				else
				{
					keys[event.key.keysym.sym] = 0;
				}
				break;

			/*case SDL_MOUSEMOTION:

				if (!mouseFirstMoved)
				{
					SDL_WarpMouseInWindow(mainWindow, width / 2, height / 2);
				    mouseFirstMoved = true;
				}
				else
				{

				    SDL_GetMouseState(&xPos, &yPos); // if pointers are in the func argument, then pass refs to normal vars there
					//printf("%.5f, %.5\n", (float)xPos, (float)yPos);
					yChange = height / 2 - yPos;   //event.motion.y;
					xChange = xPos - width/2 ;//event.motion.x - width/2;
				}
				

				SDL_WarpMouseInWindow(mainWindow, width / 2, height / 2); // Set Mouse Position to ZERO!!!!!!!!!!!!

				break; */
			}
			getMovementDirectionFromKeyboard();

			return false;
		}
	}

}


Window::~Window()
{
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}