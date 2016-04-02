#pragma once

#include "ShaderProgram.h"
#include "Matrix.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SpriteSheet.h"
#include "Astral.h"
#include <string>
#include <vector>

#define LEVEL_HEIGHT 32
#define LEVEL_WIDTH 32
#define TILE_SIZE 0.4f
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8


class Platform {

public:
	Platform();

	

	void placeEntity(std::string type, float x, float y);

	void placeEntity(std::string type, float x, float y, AstralEntity & plr);

	

	bool readEntityData(std::ifstream & stream, ShaderProgram & program);

	bool readLayerData(std::ifstream & stream);



	bool readHeader(std::ifstream & stream);

	
	void worldToTileCoordinates(float worldX, float worldY, int * gridX, int * gridY);
	void setupAndRender(ShaderProgram & program, float vertices[], float texCoords[], GLuint & texture);
	void identityMatrix();
	void setMatrices(ShaderProgram & program);
	void render(ShaderProgram& program);
	void render(ShaderProgram & program, GLuint & texture);
	void renderUpdate(ShaderProgram & program, GLuint & texture, AstralEntity & player, std::vector<SpriteSheet>& animations, float fixedElasped);
	//void Update(float elapsed, std::vector<SpriteSheet>* animations, AstralEntity * player);
	void Update(float elapsed, std::vector<SpriteSheet>& animations, AstralEntity& player, ShaderProgram & program);
	float collisionDetectionX(float x, float y);
	float collisionDetectionY(float x, float y);
	
	void collisionHandler(AstralEntity & player);
	
	void setup();
	void clearScreen();
	void windowSwapping();
	void DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing);
	GLuint LoadTexture(const char* image_path);
	void blendSprite(GLuint& texture);
	GLuint LocateTexture();
	void setOrthoProjection();
	void moveViewMatrix(float x_value, float y_value, float z_value);
	void readFile(const char * levelFile, ShaderProgram & program);
	void incrementviewYPos(float value);
	void incrementviewXPos(float value);
	GLuint texturez, animation;
	unsigned char** levelData; //16 ROWS, 22 COLUMNS

private:
	SDL_Window* displayWindow;
	//unsigned char level[LEVEL_WIDTH][LEVEL_HEIGHT];
	
	const Uint8* keys;
	bool done = false;
	SDL_Event events;
	SpriteSheet playerAnimation[7];
	int mapHeight, mapWidth;
	//bool cellmap[LEVEL_WIDTH][LEVEL_HEIGHT];
	Matrix proj, view, model;
	int count, scaleFactor, animationIndex, tileLength, tileHeight;
	//int *gridX;
	//int *gridY;
	float animationElapsed, fps, viewX, viewY, spriteWidth, spriteHeight, penetrationX, penetrationY;
	bool **solid;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	AstralEntity player;
	//static ShaderProgram program0;


};



//#endif // !PLATFORM_H


