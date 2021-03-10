#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <math.h>

#define M_PI 3.14159265

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
	
	// Solar System
	//// The sun
	//Gizmos::addSphere(glm::vec3(0, 0, 0), 1, 10, 10, glm::vec4(1, 1, 0, 1));

	//// Mercury
	//float mercuryColour = 169 / 255;
	//m_mercuryRotationSpeed += 6 * deltaTime;

 //	m_mercuryPosition.x = ((2.f) * sin(m_mercuryRotationSpeed));
	//m_mercuryPosition.z = ((2.f) * cos(m_mercuryRotationSpeed));

	//Gizmos::addSphere(m_mercuryPosition, 0.1f, 10, 10, glm::vec4(mercuryColour, mercuryColour, mercuryColour, 1));

	//// Venus
	//m_venusRotationSpeed += 4.375 * deltaTime;

	//m_venusPosition.x = ((3.f) * sin(m_venusRotationSpeed));
	//m_venusPosition.z = ((3.f) * cos(m_venusRotationSpeed));

	//Gizmos::addSphere(m_venusPosition, 0.2f, 10, 10, glm::vec4(1, 1, 1, 1));

	//// Earth
	//m_earthRotationSpeed += 3.75 * deltaTime;

	//m_earthPosition.x = ((4.f) * sin(m_earthRotationSpeed));
	//m_earthPosition.z = ((4.f) * cos(m_earthRotationSpeed));

	//Gizmos::addSphere(m_earthPosition, 0.2f, 10, 10, glm::vec4(0, 0, 1, 1));

	//// Mars
	//m_marsRotationSpeed += 3 * deltaTime;

	//m_marsPosition.x = ((5.f) * sin(m_marsRotationSpeed));
	//m_marsPosition.z = ((5.f) * cos(m_marsRotationSpeed));

	//Gizmos::addSphere(m_marsPosition, 0.1f, 10, 10, glm::vec4(1, 0, 0, 1));

	//// Jupiter
	//float jupiterColour = 69 / 255;
	//m_jupiterRotationSpeed += 1.625 * deltaTime;

	//m_jupiterPosition.x = ((6.f) * sin(m_jupiterRotationSpeed));
	//m_jupiterPosition.z = ((6.f) * cos(m_jupiterRotationSpeed));

	//Gizmos::addSphere(m_jupiterPosition, 0.5f, 10, 10, glm::vec4(1, jupiterColour, 0, 1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	DrawShaderAndMeshes(m_projectionMatrix, m_viewMatrix);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
	
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

	m_quadMesh.Draw();
#pragma endregion
	
#pragma region FlatBunny
	m_bunnyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
	m_bunnyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));

	m_bunnyMesh.draw();
#pragma endregion
}
