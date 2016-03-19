#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include "ShaderProgram.h"


class SpriteSheet{

public:
	SpriteSheet();
	SpriteSheet(unsigned int textureID, float u, float v, float width, float height, float
		size);

	void Draw(ShaderProgram& program);
	void setupAndRender(ShaderProgram& program, float vertices[], float texCoords[], GLuint& texture);
	void BlendSprite(GLuint& texture);
	void moveMatrix(float x, float y, float z);
	void incrementYPos(float value);

	float size;
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
	float XPos,YPos;
	bool active;
	//ShaderProgram program;

};





#endif