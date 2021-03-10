#pragma once

#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include "OBJMesh.h"

class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// === SHADER ===
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	// ==============
	Mesh			   m_quadMesh;
	glm::mat4		   m_quadTransform;

	aie::OBJMesh	   m_bunnyMesh;
	glm::mat4		   m_bunnyTransform;

	// Solar System
	glm::vec3 m_mercuryPosition = glm::vec3(2.f, 0, 0);
	float m_mercuryRotationSpeed = 0;

	glm::vec3 m_venusPosition = glm::vec3(3.f, 0, 0);
	float m_venusRotationSpeed = 0;

	glm::vec3 m_earthPosition = glm::vec3(4.f, 0, 0);
	float m_earthRotationSpeed = 0;

	glm::vec3 m_marsPosition = glm::vec3(5.f, 0, 0);
	float m_marsRotationSpeed = 0;

	glm::vec3 m_jupiterPosition = glm::vec3(6.f, 0, 0);
	float m_jupiterRotationSpeed = 0;

public:
	bool LoadShaderAndMeshLogic();
	void DrawShaderAndMeshes(glm::mat4, glm::mat4);
};