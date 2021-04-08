/*---------------------------------
	File Name: GraphicsProjectApp.h
	Purpose: Launch App
	Author: Logan Ryan
	Modified: 8 April 2021
-----------------------------------
	Copyright 2021 Logan Ryan
---------------------------------*/
#pragma once

#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include "OBJMesh.h"
#include "Camera.h"

#include "Scene.h"
#include "ParticleEmitter.h"

class GraphicsProjectApp : public aie::Application {
public:
	// Constructor
	GraphicsProjectApp();

	// Destructor
	virtual ~GraphicsProjectApp();

	// Set up variables on startup
	virtual bool startup();
	// Destroy everything when shutting down
	virtual void shutdown();

	// Update function
	virtual void update(float deltaTime);
	// Draw function
	virtual void draw();

protected:
	// Cameras
	Camera m_camera;

	Camera m_flybyCamera;
	Camera m_stationaryCameraX;
	Camera m_stationaryCameraY;
	Camera m_stationaryCameraZ;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// === SHADER ===
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_normalMapShaders;
	aie::ShaderProgram m_particleShader;
	// ==============

	// === TEXTURE ===
	aie::Texture m_particleTexture;
	// ===============

	// Create a Dragon
	aie::OBJMesh	   m_dragonMesh;

	// Create an Object
	aie::OBJMesh	   m_gunMesh;

	// Create a Soulspear
	aie::OBJMesh	   m_spearMesh;

	// Particle Emitter
	ParticleEmitter* m_emitter;
	glm::vec3 m_emitterPosition;
	glm::vec4 m_emitterStartingColor;
	glm::vec4 m_emitterEndColor;

	Scene*			   m_scene;

	// Selected object
	int m_selectedItem = -1;

	// Position of selected object
	glm::vec3		   m_position;
	// Rotation of selected object
	glm::vec3		   m_rotation;
	// Scale of selected object
	float			   m_scale;

public:
	// Load shaders and meshes
	bool LoadShaderAndMeshLogic(Light a_light);
	// Setup IMGUI
	void IMGUI_Logic();
};