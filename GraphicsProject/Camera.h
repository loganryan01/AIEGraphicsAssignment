/*--------------------------------
	File Name: Camera.h
	Purpose: Create camera objects
	Author: Logan Ryan
	Modified: 8 April 2021
----------------------------------	
	Copyright 2021 Logan Ryan
--------------------------------*/
#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	// Default Constructor
	Camera() {};

	// Constructor
	Camera(glm::vec3 a_position, float a_theta, float a_phi, bool a_stationary);

	// Destructor
	~Camera() {};

	// Update function
	void Update(float a_deltaTime);

	// Get position of camera
	glm::vec3 GetPosition() { return m_position; }

	// Is the camera stationary
	bool IsStationary() { return m_stationary; }

	// Get camera theta angle
	float GetTheta() { return m_theta; }

	// Get camera phi angle
	float GetPhi() { return m_phi; }

	// Get camera view matrix
	glm::mat4 GetViewMatrix();

	// Get camera projection matrix
	glm::mat4 GetProjectionMatrix(float a_width, float a_height);
private:
	float m_theta; // In Degrees
	float m_phi;   // In Degrees
	glm::vec3 m_position;
	bool m_stationary;

	float m_lastMouseX, m_lastMouseY;
};

