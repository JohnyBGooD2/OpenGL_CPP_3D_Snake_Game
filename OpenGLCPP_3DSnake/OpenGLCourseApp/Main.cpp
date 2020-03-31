#include <stdio.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, identity
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "MeshSpawner.h"
#include "Shader.h"
#include "Camera.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow = Window(1366, 768);
std::vector<Shader> shaderList;
Camera camera;

Window::MovementDirection currentMoveDirection = Window::forward; 

MeshSpawner meshSpawner; 

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

float delayCounter = 0.0f;

enum CubesType{snakeBody, pickUp};

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";
// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

float ClampFloat(float x, float a, float b)
{
	return x < a ? a : (x > b ? b : x);
};

// MAIN function~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	mainWindow.initialize();
	//_sleep(200000);

	camera = Camera(glm::vec3(0.0f, 1.51f, 1.05f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -60.0f, 5.0f, 0.2f); // Camera initial rotation here 4th value

	bool spawnOrdeleteNewObject;
	bool spawnOrdeleteDirection = true; // true - Spawn objects, false - delete objects

	float posOffsetMax = 0.8f;

	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColour = 0,
	uniformColorForPickUp = 0;

	glm::mat4 projection = glm::perspective(45.0f, 
    (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	meshSpawner.CreateSnakeBodyCube(planeCoords{-1 + meshSpawner.scaleFactorForCubes, 1 - meshSpawner.scaleFactorForCubes });
	
	meshSpawner.CreateFloorPlane();
	CreateShaders();
	bool createPickup = false;
    bool gameOver = false;

// MAIN LOOP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Loop until windows closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = SDL_GetTicks();
		deltaTime = (now - lastTime)/1000;
		lastTime = now;

		//printf("%.5f\n", (float)(1/deltaTime)); // print FPS
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();

		uniformColorForPickUp = shaderList[0].GetUniformColorForPickUpLocation();

		GLfloat colForPickUp[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glUniform4f(uniformColorForPickUp, colForPickUp[0], colForPickUp[1], colForPickUp[2], colForPickUp[3]); // pass value to shader

		// Render floor Plane
		
		glm::mat4 model = glm::identity<glm::mat4>();

		model = glm::translate(model, glm::vec3((meshSpawner.floorPlane->X_Offset), (meshSpawner.floorPlane->Y_Offset), (meshSpawner.floorPlane->Z_Offset)));//-2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		meshSpawner.floorPlane->RenderMesh();

        delayCounter += deltaTime;
		planeCoords coordsForNewSnakeCube = { meshSpawner.snakeBodyMeshesList.back()->X_Offset, meshSpawner.snakeBodyMeshesList.back()->Z_Offset };

		// Handle input
		if (delayCounter >= 0.5 && gameOver == false)
		{
			for (std::vector<Mesh*>::reverse_iterator iterator = meshSpawner.snakeBodyMeshesList.rbegin();
				iterator != meshSpawner.snakeBodyMeshesList.rend(); ++iterator) // reversed loop , we need last elements to copy cords from previous
			{
                Mesh* current = *iterator; // *i convert iterator to reference
				if (current == meshSpawner.snakeBodyMeshesList.front()) 
				{
					switch (mainWindow.MoveDirection)
					{
					case Window::forward:
						current->Z_Offset -= meshSpawner.scaleFactorForCubes * 2;
						break;
					case Window::backwards:
						current->Z_Offset += meshSpawner.scaleFactorForCubes * 2;
						break;
					case Window::left:
						current->X_Offset -= meshSpawner.scaleFactorForCubes * 2;
						break;
					case Window::right:
						current->X_Offset += meshSpawner.scaleFactorForCubes * 2;
						break;
					default: 
						break;
					}

					// Clamp object to floor plane bounds
					if (abs(current->Z_Offset) - (1 - meshSpawner.scaleFactorForCubes) >
						meshSpawner.scaleFactorForCubes / 10)
					{
						current->Z_Offset = ClampFloat(-(current->Z_Offset),
							-(1 - meshSpawner.scaleFactorForCubes), (1 - meshSpawner.scaleFactorForCubes));
					}

					if (abs(current->X_Offset) - (1 - meshSpawner.scaleFactorForCubes) >
						meshSpawner.scaleFactorForCubes / 10)
					{
						current->X_Offset = ClampFloat(-(current->X_Offset),
							-(1 - meshSpawner.scaleFactorForCubes), (1 - meshSpawner.scaleFactorForCubes));
					}
					// Clamp object to floor plane bounds
					if (!meshSpawner.cubeToPickUp)
					{
						meshSpawner.CreateCubeToPickUp();
					}
				}
				else
				{
					current->X_Offset = meshSpawner.GetPreviousSnakeCube(current)->X_Offset;
					current->Z_Offset = meshSpawner.GetPreviousSnakeCube(current)->Z_Offset;
					
				}
				/* Get index of element from iterator
				int index = std::distance(vecOfNums.begin(), it);*/
			}
			delayCounter = 0.0f;
		}
		// Iteration over snake body meshes list
		for (Mesh* iter : meshSpawner.snakeBodyMeshesList)
		{
			model = glm::identity<glm::mat4>(); // Set when render floor, but here set again to zero

			model = glm::translate(model, glm::vec3((iter->X_Offset), (iter->Y_Offset), (iter->Z_Offset))); 
			model = glm::scale(model, glm::vec3(meshSpawner.scaleFactorForCubes, meshSpawner.scaleFactorForCubes, meshSpawner.scaleFactorForCubes));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

			iter->RenderMesh();

			if (meshSpawner.snakeBodyMeshesList.size() > 1)
			{
				if (iter != meshSpawner.snakeBodyMeshesList.front())
				{
					if ((abs((meshSpawner.snakeBodyMeshesList[0]->X_Offset) - (iter->X_Offset)) <
						meshSpawner.scaleFactorForCubes / 10) &&
						(abs((meshSpawner.snakeBodyMeshesList[0]->Z_Offset) - (iter->Z_Offset)) <
						meshSpawner.scaleFactorForCubes / 10))
					{
						gameOver = true;
					}
				}
			}
		}
		
		// Check green cube exists
		if (meshSpawner.cubeToPickUp)
		{
			if (!gameOver)
			{
				// Render green cube
				GLfloat colForPickUp[] = { 0.0f, 0.7f, 0.0f, 1.0f }; // color not used in shader , only 4th value for if else

				model = glm::identity<glm::mat4>(); // Set transform to zero

				model = glm::translate(model, glm::vec3((meshSpawner.cubeToPickUp->X_Offset), (meshSpawner.cubeToPickUp->Y_Offset), (meshSpawner.cubeToPickUp->Z_Offset))); //-2.5f));
				model = glm::scale(model, glm::vec3(meshSpawner.scaleFactorForCubes, meshSpawner.scaleFactorForCubes, meshSpawner.scaleFactorForCubes));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
				glUniform4f(uniformColorForPickUp, colForPickUp[0], colForPickUp[1], colForPickUp[2], colForPickUp[3]);
				meshSpawner.cubeToPickUp->RenderMesh();

				// Collect green cube
				if ((abs((meshSpawner.cubeToPickUp->X_Offset) - (meshSpawner.snakeBodyMeshesList[0]->X_Offset)) <
					meshSpawner.scaleFactorForCubes / 10) &&
					(abs((meshSpawner.cubeToPickUp->Z_Offset) - (meshSpawner.snakeBodyMeshesList[0]->Z_Offset)) <
						meshSpawner.scaleFactorForCubes / 10))
				{
					meshSpawner.CreateSnakeBodyCube(coordsForNewSnakeCube);
					meshSpawner.SetNewLocationForPickUpCube();
				}
			}
			else
			{
				delete meshSpawner.cubeToPickUp;
				meshSpawner.cubeToPickUp = nullptr;
			}
		}
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}