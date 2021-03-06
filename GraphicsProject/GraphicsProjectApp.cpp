/*-----------------------------------
	File Name: GraphicsProjectApp.cpp
	Purpose: Launch App
	Author: Logan Ryan
	Modified: 8 April 2021
-------------------------------------
	Copyright 2021 Logan Ryan
-----------------------------------*/
#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext.hpp>
#include <math.h>
#include <vector>
#include <string>

#include "Scene.h"
#include "Instance.h"

#define M_PI 3.14159265
#define GLM_ENABLE_EXPERIMENTAL

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp() 
{
	// Set up flyby camera starting position
	m_flybyCamera = { glm::vec3(-10, 2, 0), 0, 0, false };

	// Set up stationary cameras position and rotation
	m_stationaryCameraX = { glm::vec3(15, 2, 0), 180, 0, true };
	m_stationaryCameraY = { glm::vec3(0, 25, 0), 0, -90, true };
	m_stationaryCameraZ = { glm::vec3(0, 2, 25), -90, 0, true };

	// Set up emitter starting position
	m_emitterPosition = glm::vec3(0);
}

GraphicsProjectApp::~GraphicsProjectApp() 
{
	// Destroy everything in the scene
	Gizmos::destroy();
	delete m_scene;
}

bool GraphicsProjectApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 
		getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	// Set up dynamic light
	Light light;
	light.m_color = { 1, 1, 1 };
	light.m_direction = { 1, -1, 1 };

	// Starting camera should be flyby camera
	m_camera = m_flybyCamera;

	return LoadShaderAndMeshLogic(light);
}

void GraphicsProjectApp::shutdown() {

	// Destroy everything in the app
	Gizmos::destroy();
}

void GraphicsProjectApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	m_camera.Update(deltaTime);

	IMGUI_Logic();

	// Get delta time
	float time = getTime();

	// Change direction of the dynamic light
	m_scene->GetLight().m_direction = glm::normalize(glm::vec3(glm::cos(time * 2),
													 glm::sin(time * 2),
													 0));

	// Control Model Transform
	if (m_selectedItem >= 0)
	{
		std::vector<Instance*>::iterator it = m_scene->GetInstances().begin();
		std::advance(it, m_selectedItem);
		const char* modelName = m_scene->GetInstances()[m_selectedItem]->GetName();
		aie::OBJMesh* modelMesh = m_scene->GetInstances()[m_selectedItem]->GetMesh();
		aie::ShaderProgram* modelShader = m_scene->GetInstances()[m_selectedItem]->GetShader();
		it = m_scene->GetInstances().erase(it);

		Instance* newModel = new Instance(modelName,
			m_position,
			m_rotation,
			glm::vec3(m_scale),
			modelMesh,
			modelShader);

		m_scene->GetInstances().insert(it, newModel);
	}

	// Get camera transform for particle emitter
	glm::mat4 cameraTransform = glm::translate(m_camera.GetPosition()) *
		glm::rotate(glm::mat4(1), m_camera.GetTheta(), glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1), m_camera.GetPhi(), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), m_camera.GetTheta(), glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(1));

	// Update transform of emitter
	m_emitter->update(deltaTime, cameraTransform);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	glm::mat4 projectionMatrix = m_scene->GetCamera()->GetProjectionMatrix(getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_scene->GetCamera()->GetViewMatrix();

	m_scene->Draw();

	// === Draw Particle emitter ===
	m_particleShader.bind();

	// Create particle transform
	// Get Euler angles from projection matrix
	float xAngle = atan2f(projectionMatrix[3][1], projectionMatrix[3][2]);
	float yAngle = acosf(projectionMatrix[3][3]);
	float zAngle = atan2(projectionMatrix[1][3], projectionMatrix[2][3]);

	// Build transform
	glm::mat4 particleTransform = glm::translate(m_emitterPosition) *
		glm::rotate(glm::mat4(1), zAngle, glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1), yAngle + 90.f, glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), xAngle, glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(1));

	// Set Starting Color
	m_emitter->SetStartingColor(m_emitterStartingColor);
	m_emitter->SetEndColor(m_emitterEndColor);

	// Bind particle transform
	auto pvm = projectionMatrix * viewMatrix * particleTransform;
	m_particleShader.bindUniform("ProjectionViewModel", pvm);

	m_emitter->draw();

	Gizmos::draw(projectionMatrix * viewMatrix);
}

bool GraphicsProjectApp::LoadShaderAndMeshLogic(Light a_light)
{
#pragma region LoadShader

	#pragma region Phong
		m_phongShader.loadShader(aie::eShaderStage::VERTEX,
			"./bin/shaders/phong.vert");
		m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
			"./bin/shaders/phong.frag");
		if (m_phongShader.link() == false)
		{
			printf("Phong Shader had an error: %s\n", m_phongShader.getLastError());
			return false;
		}
	#pragma endregion

	#pragma region NormalMapShader
		m_normalMapShaders.loadShader(aie::eShaderStage::VERTEX,
			"./bin/shaders/normalMap.vert");
		m_normalMapShaders.loadShader(aie::eShaderStage::FRAGMENT,
			"./bin/shaders/normalMap.frag");
		if (m_normalMapShaders.link() == false)
		{
			printf("Normal Map Shader had an error: %s\n",
				m_normalMapShaders.getLastError());
			return false;
		}
	#pragma endregion

	#pragma region Particles
		m_particleShader.loadShader(aie::eShaderStage::VERTEX,
			"./bin/shaders/particle.vert");
		m_particleShader.loadShader(aie::eShaderStage::FRAGMENT,
			"./bin/shaders/particle.frag");
		if (m_particleShader.link() == false)
		{
			printf("Particle Shader had an error: %s\n",
				m_particleShader.getLastError());
			return false;
		}
	#pragma endregion

#pragma endregion

#pragma region MeshLogic

	#pragma region Dragon
		if (m_dragonMesh.load("./bin/stanford/dragon.obj") == false)
		{
			printf("Dragon Mesh Failed!\n");
		}
	#pragma endregion

	#pragma region Soulspear
		if (m_spearMesh.load("./bin/soulspear/soulspear.obj", true, true) == false)
		{
			printf("Soulspear Mesh has had an error!\n");
			return false;
		}
	#pragma endregion

	#pragma region ShogunKnife
		if (m_gunMesh.load("./bin/knife/model.obj", true, true) == false)
		{
			printf("Gun Mesh Failed!\n");
		}
	#pragma endregion

#pragma endregion

	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), a_light,
		glm::vec3(0.25f));

	// Shogun Knife (Imported Model)
	m_scene->AddInstances(new Instance("Knife", 
		glm::vec3(5, 0, 5),
		glm::vec3(0, 0, 0),
		glm::vec3(0.01f), 
		&m_gunMesh, 
		&m_normalMapShaders));

	// Soul Spear
	m_scene->AddInstances(new Instance("Spear", 
		glm::vec3(-5, 0, -5),
		glm::vec3(0, 0, 0),
		glm::vec3(1),
		&m_spearMesh,
		&m_normalMapShaders));

	// Dragon (Stanford Model)
	m_position = glm::vec3(-5, 0, 5);
	m_rotation = glm::vec3(0, 0, 0);
	m_scale = 0.5f;

	m_scene->AddInstances(new Instance("Dragon", 
		m_position,
		m_rotation,
		glm::vec3(m_scale),
		&m_dragonMesh,
		&m_phongShader));

	// Particle Emitter
	m_emitter = new ParticleEmitter();
	m_emitter->initalise(1000, 500,
		0.1f, 1.0f,
		1, 5,
		1, 0.1f,
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
	m_emitterStartingColor = glm::vec4(1, 0, 0, 1);
	m_emitterEndColor = glm::vec4(1, 1, 0, 1);

	// Stationary Lights
	// Add a red light on the left side
	m_scene->GetPointLights().push_back(Light(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 50));
	// Add a green light on the right side
	m_scene->GetPointLights().push_back(Light(glm::vec3(-5, 3, 0), glm::vec3(0, 1, 0), 50));

	return true;
}

void GraphicsProjectApp::IMGUI_Logic()
{
	// Light settings
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.f, 1.f);
	ImGui::ColorEdit3("Sunlight Color", &m_scene->GetLight().m_color[0]);
	ImGui::DragFloat3("Light 1 Position", &m_scene->GetPointLights()[0].m_direction[0], 0.1f, -10.f, 10.f);
	ImGui::DragFloat3("Light 1 Color", &m_scene->GetPointLights()[0].m_color[0], 1.f, 0.f, 255.f);
	ImGui::DragFloat3("Light 2 Direction", &m_scene->GetPointLights()[1].m_direction[0], 0.1f, -10.f, 10.f);
	ImGui::DragFloat3("Light 2 Color", &m_scene->GetPointLights()[1].m_color[0], 1.f, 0.f, 255.f);
	ImGui::DragFloat3("Ambient Light", &m_scene->GetAmbientLight()[0], 0.25f, -1.f, 1.f);
	ImGui::End();

	// Model settings
	std::vector<const char*> nameVector;
	for (auto it = 0; it < m_scene->GetInstances().size(); it++)
	{
		nameVector.push_back(m_scene->GetInstances()[it]->GetName());
	}

	ImGui::Begin("Model Transform Settings");
	int i = 0;
	for (std::vector<const char*>::iterator it = nameVector.begin(); it != nameVector.end(); ++it)
	{
		std::string itemid = "##" + std::to_string(i);
		if (ImGui::Selectable(itemid.c_str(), i == m_selectedItem))
		{
			m_selectedItem = i;
		}
		ImGui::SameLine();
		ImGui::Text("Item: ");
		ImGui::SameLine();
		ImGui::Text((*it));
		i++;
	}

	if (m_selectedItem >= 0)
	{
		// Get the selected item position
		m_position = m_scene->GetInstances()[m_selectedItem]->GetPosition();

		// Get the selected item scale
		m_scale = m_scene->GetInstances()[m_selectedItem]->GetScale().x;

		// Get the selected item rotation
		m_rotation = m_scene->GetInstances()[m_selectedItem]->GetEulerAngles();

		for each (auto var in nameVector)
		{
			if (m_scene->GetInstances()[m_selectedItem]->GetName() == var)
			{
				ImGui::DragFloat3("Position", &m_position[0], 0.1f, -10.f, 10.f);
				ImGui::DragFloat3("Rotation", &m_rotation[0], 0.1f, 0.f, 360.f);
				ImGui::DragFloat("Scale", &m_scale, 0.1f, 0.f, 1.f);
			}
		}
	}
	ImGui::End();
	
	// Camera Controls
	ImGui::Begin("Camera Settings");
	if (ImGui::Button("Flyby"))
	{
		m_camera = m_flybyCamera;
	}
	if (ImGui::Button("Stationary X"))
	{
		m_camera = m_stationaryCameraX;
	}
	if (ImGui::Button("Stationary Y"))
	{
		m_camera = m_stationaryCameraY;
	}
	if (ImGui::Button("Stationary Z"))
	{
		m_camera = m_stationaryCameraZ;
	}
	ImGui::End();

	// Particle effects controls
	ImGui::Begin("Particle Emitter");
	ImGui::DragFloat3("Position", &m_emitterPosition[0], 0.1f, -10.f, 10.f);
	ImGui::ColorEdit3("Starting Color", &m_emitterStartingColor[0]);
	ImGui::ColorEdit3("Ending Color", &m_emitterEndColor[0]);
	ImGui::End();
}
