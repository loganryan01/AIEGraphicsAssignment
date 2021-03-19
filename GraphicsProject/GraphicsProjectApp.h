#pragma once

#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include "OBJMesh.h"
#include "Camera.h"

#include "Scene.h"

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

	// === SHADER ===
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_normalMapShaders;
	// ==============

	// Create a Dragon
	aie::OBJMesh	   m_dragonMesh;

	// Create an Object
	aie::OBJMesh	   m_gunMesh;

	// Create a Soulspear
	aie::OBJMesh	   m_spearMesh;

	Scene*			   m_scene;

	int m_selectedItem = -1;
	bool m_dragonSelected = false;

	glm::vec3		   m_position;
	glm::vec3		   m_rotation;
	float			   m_scale;

public:
	bool LoadShaderAndMeshLogic(Light a_light);
	void IMGUI_Logic();
};