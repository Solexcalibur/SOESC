#pragma once
#include "ShaderProgram.h"
#include "Matrix.h"
#include <map>
#include <vector>
//#include "Particle.h"
#include "Vector.h"
#include "Color.h"
struct Particle {

	Particle();
	//std::pair<float, float> position;
	//std::pair<float, float> velocity;
	Vector position, velocity;
	float lifetime;
	float sizeDeviation;

};


class ParticleEmitter {

public:
	ParticleEmitter();
	ParticleEmitter(unsigned int ParticleCount);
	~ParticleEmitter();

	void Update(float elasped);
	void Render(ShaderProgram* program);
	void EmitXDirection(int number, bool toTheRight);
	void EmitYDirection(int number, bool toTheBottom);
	void EmitYDirectionWithOffsetX(int number, bool toTheRight, bool toTheBotton, float offset);

	float lerp(float start, float end, float time);

	Color startColor, endColor;
	Vector position, gravity, velocity, velocityDeviation;

	std::vector<Particle> particles;
	//std::vector<Color>colors;
	std::vector<float> vertices, particleColors, texCoords, colors;

	float minLifeTime, maxLifeTime, particleSize, decayRate, m, startSize, endSize, sizeDeviation;
};
