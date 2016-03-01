#ifndef ASTRAL_H
#define ASTRAL_H
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Projectile.h"
#include "SpriteSheet.h"
#include <vector>



class AstralEntity{

public:
	AstralEntity(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix);
	AstralEntity(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix, SpriteSheet& spritez);
	void setWidthAndHeight(float width, float height);
	void DrawUsingSpriteSheet(ShaderProgram *program, int index, int spriteCountX, int spriteCountY);
	bool lifeChecker();
	void setMatrices(ShaderProgram* program);
	void setOrthoProjection();
	void moveMatrix(float x, float y, float z);
	void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing);
	void identityMatrix();
	void BlendSprite(GLuint& texture);
	void setupAndRender(ShaderProgram& program, float vertices[], float texCoords[], GLuint& texture);
	void incrementXPos(float value);
	void incrementYPos(float value);
	void renderWithNoTexture(ShaderProgram* program, float vertices[]);
	void shoot(ShaderProgram* program, Projectile ammo[],  float elapsed);
	




	
	Matrix model, projection, view;
	GLuint texID;
	float XPos, YPos, HDirection, VDirection, health, width, height, velocity, spriteIndex;
	bool alive;
	int firstSpriteIndex, lastSpriteIndex, score, ammoIndex, maxshots;
	


};









#endif