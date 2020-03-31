#include <iostream>
#include <vector>
#include <random> 
#include "MeshSpawner.h"

std::random_device randDevice;
std::mt19937 randomGenerator(randDevice());

MeshSpawner::MeshSpawner()
{
	scaleFactorForCubes = 0.1f; // Scale of snake body cubes max = 0.2
	CalcALLCoordsList();
}

// Calculate all coordinates we can put cubes
void MeshSpawner::CalcALLCoordsList()
{
	planeCoords coordsForPickUp;

	int numberOfSteps = (int)(1 / scaleFactorForCubes);
	float tempX = 1.0f - scaleFactorForCubes;
	
	for (int i = 0; i <= numberOfSteps - 1; i += 1)
	{
		float tempY = 1.0f - scaleFactorForCubes;
		for (int j = 0; j <= numberOfSteps - 1; j += 1)
		{
			coordsForPickUp.xCord = tempX;
			coordsForPickUp.zCord = tempY;

			allLocationsList.push_back(coordsForPickUp);

			tempY -= (2 * scaleFactorForCubes);
		}
		tempX -= (2*scaleFactorForCubes);
	}
}

// Calculate all places not taken by snake body cubes
std::vector<planeCoords> MeshSpawner::CalcCurrentAvailableCoordsList()
{
	planeCoords coordsToRemove;
	std::vector<int> indexesToRemove;
	std::vector<planeCoords> currentAvailableLocationsList;

	for (Mesh* iter : snakeBodyMeshesList)
	{
		coordsToRemove.xCord = iter->X_Offset;
		coordsToRemove.zCord = iter->Z_Offset;

		int index = 0;

		for (planeCoords it : allLocationsList)
		{
			if (abs(it.xCord - coordsToRemove.xCord) < scaleFactorForCubes / 10)
			{
				if (abs(it.zCord - coordsToRemove.zCord) < scaleFactorForCubes / 10)
				{
					indexesToRemove.push_back(index);
				}
			}
			index++;
		}
	}

	int indexToCheck = 0;
	for (planeCoords it : allLocationsList)
	{
		bool allowToAdd = true;
		for (int indexToRemove : indexesToRemove)
		{
			if (indexToCheck == indexToRemove)
			{
				allowToAdd = false;
			}
		}
		if (allowToAdd)
		{
            currentAvailableLocationsList.push_back(it);
		}
		
		indexToCheck++;
	}

	return currentAvailableLocationsList;
}

// Find cube to follow
Mesh* MeshSpawner::GetPreviousSnakeCube(Mesh* currentSnakeCube)
{
	std::vector<Mesh*>::iterator it = std::find(snakeBodyMeshesList.begin(), snakeBodyMeshesList.end(), currentSnakeCube);
	return snakeBodyMeshesList[std::distance(snakeBodyMeshesList.begin(), it) - 1];
}

void MeshSpawner::CreateSnakeBodyCube(planeCoords coordsForCreatedCube)
{
	Mesh* newBodyCube = new Mesh();
	newBodyCube->CreateMesh(cubeVertices, cubeIndices, 40, 36);
	snakeBodyMeshesList.push_back(newBodyCube);
	newBodyCube->Y_Offset = 0.0f; 
	newBodyCube->X_Offset = coordsForCreatedCube.xCord; 
	newBodyCube->Z_Offset = coordsForCreatedCube.zCord; 
}

void MeshSpawner::CreateCubeToPickUp()
{
	cubeToPickUp = new Mesh();
	cubeToPickUp->CreateMesh(cubeVertices, cubeIndices, 40, 36);
	SetNewLocationForPickUpCube();
}

// Set new coordinates to move green cube to new location
void MeshSpawner::SetNewLocationForPickUpCube()
{
	if (cubeToPickUp)
	{
		std::vector<planeCoords> currentAvailableCoordsList = CalcCurrentAvailableCoordsList();

		std::uniform_int_distribution<> randomRangeInt(0, (currentAvailableCoordsList.size() - 1));

		planeCoords selectedCoords = currentAvailableCoordsList[randomRangeInt(randomGenerator)];

		cubeToPickUp->Y_Offset = 0.0f;
		cubeToPickUp->X_Offset = selectedCoords.xCord;
		cubeToPickUp->Z_Offset = selectedCoords.zCord;
	}
	else
	{
		std::cout << "No PickUp Cube To Set Location, void MeshSpawner::SetNewLocationForPickUpCube()" << std::endl;
	}
}

void MeshSpawner::CreateFloorPlane()
{
	floorPlane = new Mesh();
	floorPlane->CreateMesh(floorVertices, floorIndices, 20, 6);
	floorPlane->Y_Offset = 1 - scaleFactorForCubes; 
}

MeshSpawner::~MeshSpawner()
{

}
