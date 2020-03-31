#pragma once

#include <GL\glew.h>
#include <vector>
#include "CommonValues.h"
#include "Mesh.h"

class MeshSpawner
{
public:
	MeshSpawner();
    float scaleFactorForCubes; 
    std::vector<Mesh*> snakeBodyMeshesList;
	Mesh* cubeToPickUp;
	Mesh* floorPlane;

    void CalcALLCoordsList();
	std::vector<planeCoords> CalcCurrentAvailableCoordsList();
	void CreateSnakeBodyCube(planeCoords coordsForCreatedCube);
	void CreateCubeToPickUp();
	void SetNewLocationForPickUpCube();
	void CreateFloorPlane();

	Mesh* GetPreviousSnakeCube(Mesh* currentSnakeCube);

	~MeshSpawner();

private:
	std::vector<planeCoords> allLocationsList;

	unsigned int cubeIndices[36] =
	{
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	GLfloat cubeVertices[40] = {
		-1.0f, 1.0f, -1.0f,    0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,     0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,    0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,    0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,     0.0f, 0.0f,
	};

	unsigned int floorIndices[6] = {

		// bottom
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[20] = {
		//-1.0f, 1.0f, -1.0f,    0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
		//1.0f, 1.0f, -1.0f,     0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,    0.0f, 0.0f,

		//-1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
		//1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,    0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,     0.0f, 0.0f,
	};

};

