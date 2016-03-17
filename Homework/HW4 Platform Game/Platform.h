#pragma once
#include "ShaderProgram.h"
#include "Matrix.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SpriteSheet.h"

#define LEVEL_HEIGHT 32
#define LEVEL_WIDTH 32
#define TILE_SIZE 0.5f
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 9


class Platform {

public:
	Platform();

	void initalizeCell();

	bool readEntityData(std::ifstream & stream);

	bool readLayerData(std::ifstream & stream);

	void readFile(const char * levelFile);

	bool readHeader(std::ifstream & stream);

	void BuildLevel();
	void setupAndRender(ShaderProgram & program, float vertices[], float texCoords[], GLuint & texture);
	void renderUpdate(ShaderProgram& program, GLuint texture);
	void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);
	void setup();
	void clearScreen();
	void windowSwapping();
	void DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing);
	GLuint LoadTexture(const char* image_path);
	void blendSprite(GLuint& texture);
	GLuint LocateTexture();
	void setOrthoProjection();
	GLuint texturez;
	unsigned char** levelData; //16 ROWS, 22 COLUMNS

private:
	SDL_Window* displayWindow;
	unsigned char level[LEVEL_WIDTH][LEVEL_HEIGHT];
	const Uint8* keys;
	bool done = false;
	SDL_Event events;
	SpriteSheet sprites[10];
	int mapHeight, mapWidth;
	bool cellmap[LEVEL_WIDTH][LEVEL_HEIGHT];
	Matrix proj, view;



};

