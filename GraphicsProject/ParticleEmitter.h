#pragma once
#include <glm/glm.hpp>

struct Particle
{ 
	glm::vec3 position; 
	glm::vec3 velocity; 
	glm::vec4 colour; 
	float size; 
	float lifetime; 
	float lifespan; 
}; 

struct ParticleVertex
{ 
	glm::vec4 position; 
	glm::vec4 colour; 
};

class ParticleEmitter
{
public:
	ParticleEmitter(); 
	virtual ~ParticleEmitter();

	void initalise(unsigned int a_maxParticles, unsigned int a_emitRate,
		float a_lifetimeMin, float a_lifetimeMax, float a_velocityMin, float a_velocityMax,
		float a_startSize, float a_endSize, const glm::vec4& a_startColour,
		const glm::vec4& a_endColour);

	glm::vec3 GetPosition() { return m_position; }
	void SetStartingColor(glm::vec4 a_startingColor) { m_startColour = a_startingColor; }
	void SetEndColor(glm::vec4 a_endColor) { m_endColour = a_endColor; }

	void emit();
	void update(float a_deltaTime, const glm::mat4& a_cameraTransform);
	void draw();

	// more to come here!

protected:
	Particle* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;

	unsigned int m_vao, m_vbo, m_ibo;
	ParticleVertex* m_vertexData;

	glm::vec3 m_position; 

	float m_emitTimer; 
	float m_emitRate; 
	
	float m_lifespanMin; 
	float m_lifespanMax; 
	
	float m_velocityMin; 
	float m_velocityMax; 
	
	float m_startSize; 
	float m_endSize; 
	
	glm::vec4 m_startColour; 
	glm::vec4 m_endColour;
};

