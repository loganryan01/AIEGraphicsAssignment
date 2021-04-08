/*-------------------------------------
	File Name: Scene.h
	Purpose: Create a scene for the app
	Author: Logan Ryan
	Modified: 8 April 2021
---------------------------------------
	Copyright 2021 Logan Ryan
-------------------------------------*/
#pragma once
#include <list>
#include <vector>
#include <glm/glm.hpp>

class Camera;
class Instance;

const int MAX_LIGHTS = 4;

struct Light 
{
	Light() 
	{
		m_direction = glm::vec3(1);
		m_color = glm::vec3(1);
	}
	Light(glm::vec3 a_pos, glm::vec3 a_color, float a_intensity)
	{
		m_direction = a_pos;
		m_color = a_color * a_intensity;
	}

	glm::vec3 m_direction;
	glm::vec3 m_color;
};

class Scene
{
public:
	// Constructor
	Scene(Camera* a_camera, glm::vec2 a_windowSize,
		Light& a_light, glm::vec3 a_ambientLight);
	// Destructor
	~Scene();

	// Add object to scene
	void AddInstances(Instance* a_instances);
	// Draw objects in scene
	void Draw();

	// Getters
	Camera* GetCamera()			{ return m_camera; }
	glm::vec2 GetWindowSize()	{ return m_windowSize; }
	Light& GetLight()			{ return m_light; }
	glm::vec3& GetAmbientLight() { return m_ambientLight; }
	std::vector<Instance*>& GetInstances() { return m_instances; }

	int GetNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColor() { return &m_pointLightColors[0]; }

	std::vector<Light>& GetPointLights() { return m_pointLights; }

protected:
	Camera*					m_camera;
	glm::vec2				m_windowSize;
	Light					m_light;
	std::vector<Light>		m_pointLights;
	glm::vec3				m_ambientLight;
	std::vector<Instance*>	m_instances;

	glm::vec3				m_pointLightPositions[MAX_LIGHTS];
	glm::vec3				m_pointLightColors[MAX_LIGHTS];
};

