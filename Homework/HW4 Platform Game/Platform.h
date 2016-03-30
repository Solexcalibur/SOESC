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

	void initalizeCell();

	int countAliveNeighbours(int x, int y);

	void doSimulationStep(ShaderProgram& program, GLuint& texture);

	bool readEntityData(std::ifstream & stream);

	void placeEntity(std::string type, float x, float y);

	void placeEntity(std::string type, float x, float y, AstralEntity & plr);

	bool readEntityData(std::ifstream & stream, ShaderProgram & program, AstralEntity & plr);

	bool readEntityData(std::ifstream & stream, ShaderProgram & program);

	bool readLayerData(std::ifstream & stream);

	void readFile(const char * levelFile);

	bool readHeader(std::ifstream & stream);

	void BuildLevel();
	void setupAndRender(ShaderProgram & program, float vertices[], float texCoords[], GLuint & texture);
	void identityMatrix();
	void setMatrices(ShaderProgram & program);
	void renderUpdate(ShaderProgram& program, GLuint& texture);
	void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);
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
	GLuint texturez;
	unsigned char** levelData; //16 ROWS, 22 COLUMNS

private:
	SDL_Window* displayWindow;
	unsigned char level[LEVEL_WIDTH][LEVEL_HEIGHT];
	
	const Uint8* keys;
	bool done = false;
	SDL_Event events;
	SpriteSheet sprites[LEVEL_WIDTH][LEVEL_HEIGHT];
	int mapHeight, mapWidth;
	//bool cellmap[LEVEL_WIDTH][LEVEL_HEIGHT];
	Matrix proj, view, model;
	int count, gridX, gridY;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	AstralEntity player;
	//static ShaderProgram program0;


};



//#endif // !PLATFORM_H


