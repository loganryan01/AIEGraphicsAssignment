/*----------------------------------
	File Name: Mesh.h
	Purpose: Create mesh for objects
	Author: Logan Ryan
	Modified: 8 April 2021
------------------------------------
	Copyright 2021 Logan Ryan
----------------------------------*/
#pragma once
#include <glm/glm.hpp>

class Mesh
{
public:
	// Constructor
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {};
	// Destructor
	virtual ~Mesh();

	struct Vertex {
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	// Create mesh for plane
	void InitialiseQuad();
	// Create mesh for 3d object
	void Initialise(unsigned int a_vertexCount,
		const Vertex* a_verticies,
		unsigned int a_indexCount = 0,
		unsigned int* a_indices = nullptr);

	// Draw mesh
	virtual void Draw();

protected:
	unsigned int triCount;
	unsigned int vao, // Vertex Array Object
				 vbo, // Vertex Buffer Object
				 ibo; // Index Buffer Object
};

