#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Vector.h"

class Projectile{


public:
	Projectile(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix);
	Projectile();

	Vector position, velocity, acceleration, direction;
	float XPos, YPos, flightTime, width, height, velocityX, velocityY, accelertionX, accelerationY;
	float vertices[12];
	Matrix model, projection, view;
	GLuint texID;
	bool remove, visible;

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
	//void removeBullet();
	bool shouldRemoveBullet(Projectile& bulle);







};



#endif