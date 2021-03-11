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

#define M_PI 3.14159265
#define GLM_ENABLE_EXPERIMENTAL

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp() {

}

GraphicsProjectApp::~GraphicsProjectApp() {

}

bool GraphicsProjectApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 
		getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	m_light.color = { 1, 1, 1 };
	m_light.direction = { 0, 0, 0 };
	m_ambientLight = { 0.25f, 0.25f , 0.25f };

	m_lucyRotation = { 0, 0, 0 };
	m_lucyPosition = { 0,0,0 };
	m_lucyScale = 0.5f;

	return LoadShaderAndMeshLogic();
}

void GraphicsProjectApp::shutdown() {

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

	/*float time = getTime();

	m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), 
												 glm::sin(time * 2),
												 0));*/

	// Rotate Lucy
	float xRot = (m_lucyRotation.x * M_PI) / 180;
	float yRot = (m_lucyRotation.y * M_PI) / 180;
	float zRot = (m_lucyRotation.z * M_PI) / 180;
	glm::vec3 eulerAngles(m_lucyPosition.x, m_lucyRotation.y, m_lucyRotation.z);
	glm::quat lucyQuaternion = glm::quat(eulerAngles);
	lucyQuaternion = glm::angleAxis(glm::degrees(xRot), glm::vec3(1.f, 0, 0));
	glm::mat4 rotationMatrix = glm::toMat4(lucyQuaternion);

	glm::mat4 scalingMatrix = glm::scale(glm::vec3(m_lucyScale));

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), m_lucyPosition);

	glm::mat4 newTransform = translationMatrix * rotationMatrix * scalingMatrix;
	m_lucyTransform = newTransform;

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	glm::mat4 projectionMatrix = m_camera.GetProjectionMatrix(getWindowWidth(), 
		(float)getWindowHeight());
	glm::mat4 viewMatrix = m_camera.GetViewMatrix();

	// update perspective based on screen size
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	DrawShaderAndMeshes(projectionMatrix, viewMatrix);

	Gizmos::draw(projectionMatrix * viewMatrix);
	
}

bool GraphicsProjectApp::LoadShaderAndMeshLogic()
{
#pragma region Quad
	// Load the vertex shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_simpleShader.link())
	{
		printf("Simple Shader had an error: %s\n", m_simpleShader.getLastError());
		return false;
	}

	// Define the 6 vertices for the 2 triangles that make the quad
	/*Mesh::Vertex vertices[6];
	vertices[0].position = { -0.5f, 0.f, 0.5f,  1.f };
	vertices[1].position = { 0.5f,  0.f, 0.5f,  1.f };
	vertices[2].position = { -0.5f, 0.f, -0.5f, 1.f };

	vertices[3].position = { -0.5f, 0.f, -0.5f, 1.f };
	vertices[4].position = { 0.5f,  0.f, 0.5f,  1.f };
	vertices[5].position = { 0.5f,  0.f, -0.5f, 1.f };*/

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.f, 0.5f,  1.f };
	vertices[1].position = { 0.5f,  0.f, 0.5f,  1.f };
	vertices[2].position = { -0.5f, 0.f, -0.5f, 1.f };
	vertices[3].position = { 0.5f,  0.f, -0.5f, 1.f };

	unsigned int indices[6] = { 0,1,2,2,1,3 };

	m_quadMesh.Initialise(4, vertices, 6, indices);

	// We will make the quad 10 units by 10 units
	m_quadTransform = {
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,0,1
	};

	double angleX = 90 * M_PI / 180.0;
	glm::mat4 rotationX = {
		1, 0,			 0,			  0,
		0, cos(angleX),  sin(angleX), 0,
		0, -sin(angleX), cos(angleX), 0,
		0, 0,			 0,           1
	};

	double angleY = 45 * M_PI / 180.0;
	glm::mat4 rotationY = {
		cos(angleY), 0, -sin(angleY), 0,
		0,			 1, 0,			  0,
		sin(angleY), 0, cos(angleY),  0,
		0,			 0,0,1
	};

	double angleZ = 90 * M_PI / 180.0;
	glm::mat4 rotationZ = {
		cos(angleZ), -sin(angleZ), 0, 0,
		sin(angleZ), cos(angleZ),  0, 0,
		0,			 0,			   1, 0,
		0,			 0,			   0, 1
	};

	/*m_quadTransform *= rotationY;
	m_quadTransform *= rotationX;*/
	//m_quadTransform *= rotationZ;
	
#pragma endregion
	
#pragma region FlatBunny
	// Load the vertex shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_bunnyShader.link())
	{
		printf("Bunny Shader had an error: %s\n", m_bunnyShader.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny Mesh Failed!\n");
	}

	m_bunnyTransform = {
		0.5f, 0,	0,	  0,
		0,	  0.5f, 0,	  0,
		0,	  0,	0.5f, 0,
		0,	  0,	0,    1
	};
#pragma endregion

#pragma region FlatDragon
	// Load the vertex shader from a file
	m_dragonShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_dragonShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_dragonShader.link())
	{
		printf("Dragon Shader had an error: %s\n", m_dragonShader.getLastError());
		return false;
	}

	if (m_dragonMesh.load("./stanford/dragon.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
	}

	m_dragonTransform = {
		0.5f, 0,	0,	  0,
		0,	  0.5f, 0,	  0,
		0,	  0,	0.5f, 0,
		0,	  0,	0,    1
	};
#pragma endregion

#pragma region FlatBuddha
	// Load the vertex shader from a file
	m_buddhaShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_buddhaShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_buddhaShader.link())
	{
		printf("Buddha Shader had an error: %s\n", m_buddhaShader.getLastError());
		return false;
	}

	if (m_buddhaMesh.load("./stanford/buddha.obj") == false)
	{
		printf("Buddha Mesh Failed!\n");
	}

	m_buddhaTransform = {
		0.5f, 0,	0,	  0,
		0,	  0.5f, 0,	  0,
		0,	  0,	0.5f, 0,
		0,	  0,	0,    1
	};
#pragma endregion

#pragma region FlatLucy
	// Load the vertex shader from a file
	m_lucyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_lucyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_lucyShader.link())
	{
		printf("Lucy Shader had an error: %s\n", m_lucyShader.getLastError());
		return false;
	}

	if (m_lucyMesh.load("./stanford/lucy.obj") == false)
	{
		printf("Lucy Mesh Failed!\n");
	}

	m_lucyTransform = {
		0.5f, 0,	0,	  0.f,
		0,	  0.5f, 0,	  0.f,
		0,	  0,	0.5f, 0.f,
		0,	  0,	0,    1
	};
#pragma endregion

#pragma region Phong
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");
	if (m_phongShader.link() == false)
	{
		printf("Phong Shader had an error: %s\n", m_phongShader.getLastError());
		return false;
	}
#pragma endregion
	
	return true;
}

void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
{
	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);

#pragma region Quad
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform of the mesh
	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform; // PVM = Projection View Matrix
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	//m_quadMesh.Draw();
#pragma endregion
	
#pragma region FlatBunny
	m_bunnyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
	m_bunnyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));

	// Draw bunny mesh
	//m_bunnyMesh.draw();
#pragma endregion

#pragma region FlatDragon
	m_dragonShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
	m_dragonShader.bindUniform("ProjectionViewModel", pvm);
	m_dragonShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));

	// Draw dragon mesh
	//m_dragonMesh.draw();
#pragma endregion

#pragma region FlatBuddha
	m_buddhaShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
	m_buddhaShader.bindUniform("ProjectionViewModel", pvm);
	m_buddhaShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));

	// Draw buddha mesh
	//m_buddhaMesh.draw();
#pragma endregion

#pragma region FlatLucy
	m_lucyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
	m_lucyShader.bindUniform("ProjectionViewModel", pvm);
	m_lucyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));

	// Draw lucy mesh
	//m_lucyMesh.draw();
#pragma endregion

#pragma region Phong
	// Bind the shader
	m_phongShader.bind();

	// Bind the camera position
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(a_viewMatrix)[3]));

	// Bind the light
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	m_phongShader.bindUniform("LightColor", m_light.color);
	m_phongShader.bindUniform("LightDirection", m_light.direction);

	// Bind the PVM
	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
	pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
	pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
	pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);
	m_phongShader.bindUniform("ModelMatrix", m_dragonTransform);
	m_phongShader.bindUniform("ModelMatrix", m_dragonTransform);
	m_phongShader.bindUniform("ModelMatrix", m_lucyTransform);
	
	//m_bunnyMesh.draw();
	//m_dragonMesh.draw();
	//m_buddhaMesh.draw();
	m_lucyMesh.draw();
#pragma endregion
}

void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.f, 1.f);
	ImGui::DragFloat3("Sunlight Color", &m_light.color[0], 0.1f, 0.f, 2.f);
	ImGui::End();

	ImGui::Begin("Lucy Transform Settings");
	ImGui::DragFloat3("Lucy Position", &m_lucyPosition[0], 0.1f, -10.f, 10.f);
	ImGui::DragFloat3("Lucy Rotation", &m_lucyRotation[0], 1.f, 0, 360.f);
	ImGui::DragFloat("Lucy Scale", &m_lucyScale, 0.1f, 0.f, 1.f);
	ImGui::End();
}
