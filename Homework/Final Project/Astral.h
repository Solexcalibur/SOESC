#ifndef ASTRAL_H
#define ASTRAL_H
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Projectile.h"
#include "SpriteSheet.h"
#include "Vector.h"
#include "ParticleEmitter.h"
#include <vector>



class AstralEntity{

public:
	AstralEntity();
	AstralEntity(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix);
	AstralEntity(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix, SpriteSheet& spritez);
	void setWidthAndHeight(float widthv, float heightv);
	float * grabVertices();
	float * grabTexCoords();
	void DrawUsingSpriteSheet(ShaderProgram& program, int index, int spriteCountX, int spriteCountY);
	bool lifeChecker();
	void setMatrices(ShaderProgram& program);
	void setOrthoProjection();
	void moveMatrix(float x, float y, float z);
	void moveViewMatrix(float x_value, float y_value, float z_value);
	void DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing);
	void identityMatrix();
	void BlendSprite(GLuint& texture);
	void setupAndRender(ShaderProgram& program, float vertices[], float texCoords[], GLuint& texture);
	void incrementXPos(float value);
	//void incrementXPos(Vector& position, float value);
	void incrementYPos(float value);
	//void incrementYPos(Vector& position, float value);
	void incrementAccelerationX(float value);
	void incrementAccelerationY(float value);
	float lerp(float start, float end, float time);
	//void renderWithNoTexture(ShaderProgram*program, float vertices[]);
	void shoot(ShaderProgram& program, Projectile ammo[], std::vector<SpriteSheet>& spriteSheets, float elapsed);
	void shoot();
	GLuint LoadTexture(const char* image_path);
	
	Matrix model, projection, view;
	GLuint texID;
	Vector position, direction, acceleration, friction, velocity, segment1, segment2;
	float XPos, YPos, HDirection, VDirection, width, height, spriteIndex, accelerationX, accelerationY, velocityX, velocityY
		, frictionX, frictionY, scaleFactor;
	
	bool alive, isStatic, collideTop, collideBottom, collideLeft, collideRight;
	int firstSpriteIndex, lastSpriteIndex, score, ammoIndex, maxshots, health, shields;
	ParticleEmitter party;
	std::vector<Projectile> shots;


};









#endif