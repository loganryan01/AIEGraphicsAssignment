#pragma once

#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include "OBJMesh.h"
#include "Camera.h"

class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	Camera m_camera;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture	   m_gridTexture;
	aie::Texture	   m_bladeTexture;

	// === SHADER ===
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_dragonShader;
	aie::ShaderProgram m_buddhaShader;
	aie::ShaderProgram m_lucyShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShaders;
	// ==============
	// Basic Plane
	Mesh			   m_quadMesh;
	glm::mat4		   m_quadTransform;
	// Create a Bunny with a flat color
	aie::OBJMesh	   m_bunnyMesh;
	glm::mat4		   m_bunnyTransform;
	// Create a Dragon
	aie::OBJMesh	   m_dragonMesh;
	glm::mat4		   m_dragonTransform;
	// Create a Buddha
	aie::OBJMesh	   m_buddhaMesh;
	glm::mat4		   m_buddhaTransform;
	// Create a Lucy
	aie::OBJMesh	   m_lucyMesh;
	glm::mat4		   m_lucyTransform;
	glm::vec3		   m_lucyPosition;
	glm::vec3		   m_lucyRotationAxis;
	float              m_lucyRotationAngle;
	float			   m_lucyScale;
	// Create an Infinity Blade
	aie::OBJMesh	   m_infinityMesh;
	glm::mat4		   m_infinityTransform;

	// Create a Soulspear
	aie::OBJMesh	   m_spearMesh;
	glm::mat4		   m_spearTransform;

	struct Light {
		glm::vec3 direction;
		glm::vec3 color;
	};

	Light			   m_light;
	glm::vec3		   m_ambientLight;

	float			   m_value;

public:
	bool LoadShaderAndMeshLogic();
	void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void IMGUI_Logic();
};