#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Matrix.h"
#include "ShaderProgram.h"

class Projectile{


public:
	Projectile(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix);
	Projectile();


	float XPos, YPos, velocity, flightTime;
	float vertices[12];
	Matrix model, projection, view;
	GLuint texID;

	void setMatrices(ShaderProgram program);
	void setOrthoProjection();
	void moveMatrix(float x, float y, float z);
	void identityMatrix();
	void incrementXPos(float value);
	void incrementYPos(float value);
	void renderWithNoTexture(ShaderProgram program, float vertices[]);
	void BlendSprite(GLuint& texture);
	void setupAndRender(ShaderProgram& program, float vertices[], float texCoords[], GLuint& texture);
	void scaleMatrix(float x, float y, float z);







};



#endif