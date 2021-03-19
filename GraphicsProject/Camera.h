#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera() {};
	Camera(glm::vec3 a_position, float a_theta, float a_phi, bool a_stationary);
	~Camera() {};

	void Update(float a_deltaTime);
	glm::vec3 GetPosition() { return m_position; }
	bool IsStationary() { return m_stationary; }

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float a_width, float a_height);
private:
	float m_theta; // In Degrees
	float m_phi;   // In Degrees
	glm::vec3 m_position;
	bool m_stationary;

	float m_lastMouseX, m_lastMouseY;
};

