#include <GL/glew.h>

#pragma once
class Mesh
{
public:
	float Y_Offset;
	float X_Offset;
	float Z_Offset;
	float posMoveSpeed;
	bool Direction;
	Mesh();
	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	~Mesh();
	
private:
	GLuint VAO, VBO, IBO, indexCount;
};