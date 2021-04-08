/*---------------------------------------
	File Name: Instance.h
	Purpose: Create objects for the scene
	Author: Logan Ryan
	Modified: 8 April 2021
-----------------------------------------
	Copyright 2021 Logan Ryan
---------------------------------------*/
#pragma once
#include <glm/glm.hpp>

class Camera;
class Scene;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	// Constructor with transform
	Instance(const char* a_name, glm::mat4 a_transform, aie::OBJMesh* a_mesh,
		aie::ShaderProgram* a_shader);
	// Constructor with position, euler angles and scale
	Instance(const char* a_name, glm::vec3 a_position, glm::vec3 a_eulerAngles,
		glm::vec3 a_scale, aie::OBJMesh* a_mesh, 
		aie::ShaderProgram* a_shader);

	// Draw Function
	void Draw(Scene* a_scene);

	// Get Functions
	const char* GetName() { return m_name; }
	glm::mat4 GetTransform() { return m_transform; }
	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 GetEulerAngles() { return m_eulerAngles; }
	glm::vec3 GetScale() { return m_scale; }
	aie::OBJMesh* GetMesh() { return m_mesh; }
	aie::ShaderProgram* GetShader() { return m_shader; }

	// Create transform
	static glm::mat4 MakeTransform(glm::vec3 a_position,
		glm::vec3 a_eulerAngles, glm::vec3 a_scale);

protected:
	glm::mat4			m_transform;
	aie::OBJMesh*		m_mesh;
	aie::ShaderProgram* m_shader;
	const char*			m_name;
	glm::vec3			m_position;
	glm::vec3			m_eulerAngles;
	glm::vec3			m_scale;
};

