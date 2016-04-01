#include "Platform.h"
#include "ShaderProgram.h"
#include "Matrix.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SpriteSheet.h"
#include "Astral.h"
#include <SDL_image.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

//static ShaderProgram* program0;

Platform::Platform()
{
	keys = SDL_GetKeyboardState(nullptr);
	mapHeight = LEVEL_HEIGHT;
	mapWidth = LEVEL_WIDTH;
	count = 0;
	scaleFactor = 1;
	animationElapsed = 0.0f;
	fps = 30.0;
	animationIndex = 0;
	viewX = -4.0;
	viewY = 4.0;
	//gridX = 0;
	//gridY = 0;
	tileLength = TILE_SIZE * LEVEL_WIDTH;
	tileHeight = TILE_SIZE * LEVEL_HEIGHT;
	//program0 = program;
	//AstralEntity player;
	//cellmap = new bool[mapWidth][mapHeight];
	//playerAnimation[0].textureID = animations;
	playerAnimation[0].u = 67.0f / 508.0f;
	playerAnimation[0].v = 196.0f / 288.0f;
	playerAnimation[0].width = 66.0f / 508.0f;
	playerAnimation[0].height = 92.0f / 288.0f;
	playerAnimation[0].size = 0.2;

	//playerAnimation[1].textureID = animations;
	playerAnimation[1].u = 67.0f / 508.0f;
	playerAnimation[1].v = 196.0f / 288.0f;
	playerAnimation[1].width = 66.0f / 508.0f;
	playerAnimation[1].height = 97.0f / 288.0f;
	playerAnimation[1].size = 0.2;

	//playerAnimation[2].textureID = animations;
	playerAnimation[2].u = 67.0f / 508.0f;
	playerAnimation[2].v = 196.0f / 288.0f;
	playerAnimation[2].width = 66.0f / 508.0f;
	playerAnimation[2].height = 97.0f / 288.0f;
	playerAnimation[2].size = 0.2;

	//playerAnimation[3].textureID = animations;
	playerAnimation[3].u = 67.0f / 508.0f;
	playerAnimation[3].v = 196.0f / 288.0f;
	playerAnimation[3].width = 66.0f / 508.0f;
	playerAnimation[3].height = 97.0f / 288.0f;
	playerAnimation[3].size = 0.2;

	//playerAnimation[4].textureID = animations;
	playerAnimation[4].u = 67.0f / 508.0f;
	playerAnimation[4].v = 196.0f / 288.0f;
	playerAnimation[4].width = 66.0f / 508.0f;
	playerAnimation[4].height = 97.0f / 288.0f;
	playerAnimation[4].size = 0.2;

	//playerAnimation[5].textureID = animations;
	playerAnimation[5].u = 67.0f / 508.0f;
	playerAnimation[5].v = 196.0f / 288.0f;
	playerAnimation[5].width = 66.0f / 508.0f;
	playerAnimation[5].height = 97.0f / 288.0f;
	playerAnimation[5].size = 0.2;

	//playerAnimation[6].textureID = animations;
	playerAnimation[6].u = 67.0f / 508.0f;
	playerAnimation[6].v = 196.0f / 288.0f;
	playerAnimation[6].width = 66.0f / 508.0f;
	playerAnimation[6].height = 97.0f / 288.0f;
	playerAnimation[6].size = 0.2;

}


unsigned char level1Data[LEVEL_HEIGHT][LEVEL_WIDTH] =
{
	{ 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,97,98,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,97,98,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,97,98,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,97,98,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,6,6,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,21,21,21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
    { 7,21,21,21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 }, 
	{ 7,21,21,21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,7 },
	{ 7,18,18,18,18,18,18,18,18,5,5,5,4,4,4,5,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,7 },
	{ 7,33,34,34,34,34,34,36,34,12,12,12,12,12,12,12,34,34,34,34,33,34,34,34,34,34,34,34,34,34,33,7 },
	{ 7,34,34,33,34,33,34,34,34,101,101,101,101,101,101,101,34,34,33,34,34,34,34,33,34,33,34,33,36,34,33,7 },
	{ 7,36,34,36,34,36,34,33,34,34,34,36,34,34,36,34,34,33,34,34,36,34,33,34,34,34,34,33,34,34,34,7 },
	{ 7,7,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,7 },
	{ 7,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,7 },
	{ 7,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,106,106,106,106,106,106,106,90,90,90,90,7 },
	{ 7,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,7 },
	{ 7,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,7 },
	{ 7,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,7 },
	{ 7,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,109,109,109,109,109,109,7 },
	{ 7,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,124,124,124,124,124,124,124,124,109,7 },
	{ 7,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,7,7 },
	{ 7,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,7,7 },
	{ 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 }

};
void Platform::placeEntity(std::string type, float x, float y) {
	if (type == "Start") {
		player.XPos = x;
		player.YPos = y;
	}
	//player.setMatrices(*program0);
	
}
bool Platform::readEntityData(std::ifstream &stream, ShaderProgram& program) {
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
			//AstralEntity player;
			//player.setupAndRender(program, vertexData.data(), texCoordData.data(), texturez);
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}



bool Platform::readLayerData(std::ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < LEVEL_HEIGHT; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < LEVEL_WIDTH; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}



bool Platform::readHeader(std::ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while
		(getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height") {
			mapHeight = atoi(value.c_str());
		}
	}
	if
		(mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		solid = new bool*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
			solid[i] = new bool[mapWidth];
		}
		return true;
	}
}




void Platform::BuildLevel() {
	memcpy(level, level1Data, LEVEL_HEIGHT*LEVEL_WIDTH);
	

}
void Platform::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY)
{
	*gridX = (int)(worldX / TILE_SIZE);
	*gridY = (int)(-worldY / TILE_SIZE);
}
void Platform::setupAndRender(ShaderProgram& program, float vertices[], float texCoords[], GLuint& texture) {
	blendSprite(texture);//Blend first? Why?
	glUseProgram(program.programID);



	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);

	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, LEVEL_HEIGHT * LEVEL_WIDTH * 6);
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);


}
void Platform::identityMatrix() {

	model.identity();
	view.identity();
}
void Platform::setMatrices(ShaderProgram& program) {
	program.setModelMatrix(model);
	program.setProjectionMatrix(proj);
	program.setViewMatrix(view);
}
void Platform::renderUpdate(ShaderProgram& program, GLuint& texture, AstralEntity& player, std::vector<SpriteSheet>& animations, float fixedElapsed) {
	player.width = 66.0 / 508.0;
	player.height = 92.0 / 288.0;
	/*for (int i = 0; i < 7; i++) {
		playerAnimation[i].textureID = animation;
	}*/

	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			if (levelData[y][x] != 0) {
			
				float u = (float)(((int)levelData[y][x] ) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x] ) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;

				spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;

				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,

					TILE_SIZE * x, -TILE_SIZE * y,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
					
				});
				texCoordData.insert(texCoordData.end(), {
					u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),

					u, v,
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});



				if (levelData[y][x] == '\x6') {
					solid[y][x] = true;
				}
				else {
					solid[y][x] = false;
				}
				
				/*if (player.XPos < -2.6) {
					player.XPos = -2.59999999999;
				}
				if (player.XPos > 2.5) {
					player.XPos = 2.49999999999;
				}*/
			}
			
			
		}
		
	}
	//bool x = solid[20][20];
	setupAndRender(program, vertexData.data(), texCoordData.data(), texture);
	//player.setupAndRender(program, vertexData.data(), texCoordData.data(), texture);
	//worldToTileCoordinates(LEVEL_WIDTH, LEVEL_HEIGHT, &gridX, &gridY);
	player.setMatrices(program);
	player.identityMatrix();
	player.moveMatrix(player.XPos, player.YPos, 0.0);
	//worldToTileCoordinates(player.XPos, player.YPos);
	//player.XPos = gridX;
	//player.YPos = gridY;
	//view.identity();
	//view.setOrthoProjection(0.0, 8.0, 0.0, 8.0, -1.0, 1.0);
	view.Translate(viewX, viewY, 0.0);
	//if ((-2.5 < player.XPos < 2.5) && (-4.0 < viewX < 4.0)) {
		view.Translate(-1 * (player.XPos + 2.5), -1 * (player.YPos + 0.75), 0.0);
		//incrementviewXPos(-1 * fixedElapsed);
		
		//view.identity();
	//}
	player.model.Scale(2 * scaleFactor, 2.0, 1.0);
	animationElapsed += fixedElapsed;
	
	if (keys[SDL_SCANCODE_D]) {
		//entites[28].HDirection *= -1;
		player.HDirection = 1;
		player.incrementXPos(0.75 * player.velocity * player.HDirection * fixedElapsed);
		//view.Translate(-1 * entites[28].XPos * 0.25, 0.0, 0.0);
		//incrementviewXPos(-0.25 * (player.XPos + 2.5));
		//incrementviewXPos(-1 * fixedElapsed);
		scaleFactor = 1;
		if (animationElapsed > 1.0 / fps) {
			animationIndex++;
			animationElapsed = 0.0f;
			if (animationIndex == animations.size() - 1) {
				animationIndex = 0;
			}
		}
	}
	else if (keys[SDL_SCANCODE_A]) {
		//entites[28].HDirection *= -1;
		player.HDirection = -1;
		player.incrementXPos(0.75 * player.velocity * player.HDirection * fixedElapsed);
		//view.Translate(-1 * entites[28].XPos * 0.25, 0.0, 0.0);
		//entites[28].model.Scale(-1.0, 1.0, 1.0);
		//entites[28].model.Scale(-1 * scaleXFactor, 1.0, 1.0);
		//incrementviewXPos(1 * fixedElapsed);
		scaleFactor = -1;
		if (animationElapsed > 1.0 / fps) {
			animationIndex++;
			animationElapsed = 0.0f;
			if (animationIndex == animations.size() - 1) {
				animationIndex = 0;
			}
		}
	}
	else if (keys[SDL_SCANCODE_S]) {
		player.VDirection = -1;
		player.incrementYPos(0.75 * player.velocity * player.VDirection * fixedElapsed);
	}
	else if (keys[SDL_SCANCODE_W]) {
		player.VDirection = 1;
		player.incrementYPos(0.75 * player.velocity * player.VDirection * fixedElapsed);
	}

	else {
		animationIndex = 0;
	}

	animations[animationIndex].Draw(program);

	/*for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			if ((player.XPos ) - (0.5 * player.width) < (TILE_SIZE * tileLength) + TILE_SIZE) {
				if (solid[y][x]) {
					player.XPos = -2.4;
				}
			}
		}
	}*/
	
	collisionHandler(player);
}
float Platform::collisionDetectionX(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	//float penetration;
	if (0 > gridX > mapWidth - 1) {
		penetrationX = 0.0;
	}
	
	/*else if (solid[gridY][gridX]) {
		penetrationX = (gridX + 1) * tileLength - x;
	}*/
	else if (x < (TILE_SIZE * tileLength) + TILE_SIZE) {
		//if (solid[gridY][gridX]) {
			penetrationX = (gridX + 1) * tileLength - x;
		//}
	}
	else if (x > (TILE_SIZE * tileLength)) {
		//if (solid[gridY][gridX]) {
			penetrationX = (gridX + 1) * tileLength - x;
		//}
	}
	else {
		penetrationX = 0.0;
	}



	return penetrationX;
}
float Platform::collisionDetectionY(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	//float penetration;

	if (0 > gridY > mapHeight - 1) {
		penetrationY = 0.0;
	}
	
	/*else if (solid[gridY][gridX]) {
		penetrationY = -1 * y - gridY * tileHeight;
	}*/
	else if (y > -(TILE_SIZE * tileHeight) ){
	if (solid[gridY][gridX]) {
			penetrationY = -1 * y - ( gridY * tileHeight);
	}
	}
	else if (y < (-TILE_SIZE * tileHeight) - TILE_SIZE) {
		if (solid[gridY][gridX]) {
			penetrationY = -1 * y - (gridY * tileHeight);
		}
	}
	else {
		penetrationY = 0.0;
	}


	
	return penetrationY;
}

void Platform::collisionHandler(AstralEntity& player) {
	float penetrationXL, penetrationXR, penetrationYT, penetrationYB; //left, right, top, bottom

	penetrationXL = collisionDetectionX(player.XPos - ( 0.5  * player.width), player.YPos);
	penetrationXR = collisionDetectionX(player.XPos + (0.5  * player.width), player.YPos);
	penetrationYT = collisionDetectionY(player.XPos, player.YPos + (0.5 * player.height));
	penetrationYB = collisionDetectionY(player.XPos, player.YPos - (0.5 * player.height));


	if (penetrationXL > 0.0) {
		player.incrementXPos(0.2 * penetrationXL);
		//player.velocity = 0;
		player.collideLeft = true;
	}
	else if (penetrationXR > 0.0) {
		player.incrementXPos(0.2 * ( penetrationXR - tileLength));
		//player.velocity = 0;
		player.collideRight = true;
	}
	else if (penetrationYT > 0.0) {
		player.incrementYPos(0.2 * (penetrationYT));
		//player.velocity = 0;
		player.collideTop = true;
	}
	else if (penetrationYB > 0.0) {
		player.incrementYPos(0.2 * (penetrationYB - tileHeight));
		//player.velocity = 0;
		player.collideBottom = true;
	}
}


void Platform::setup()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	int x_resolution = 1024;
	int y_resolution = 800;
	displayWindow = SDL_CreateWindow("Oceanic Floor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_resolution, y_resolution, SDL_WINDOW_OPENGL);// <-OUTER BOUND
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	//scored = Mix_LoadWAV("Score.ogg");
	

#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, x_resolution, y_resolution);//<- INNER BOUND
}

void Platform::clearScreen()
{
	glClearColor(0.0, 0.25, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Platform::windowSwapping()
{
	SDL_GL_SwapWindow(displayWindow);
}

void Platform::DrawText(ShaderProgram & program, int fontTexture, std::string text, float size, float spacing)
{
	float texture_size = 1.0 / 16.0f;     std::vector<float> vertexData;     std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(),
		{ ((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size, });

		texCoordData.insert(texCoordData.end(),
		{ texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size,
			texture_y, texture_x + texture_size, texture_y + texture_size, texture_x + texture_size,
			texture_y, texture_x, texture_y + texture_size, });
	}
	glUseProgram(program.programID);
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program.texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);
}

GLuint Platform::LoadTexture(const char * image_path)
{
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels); //USE GL_RGB/A FOR WINDOWS, GL_BGR/A FOR MAC (.PNG files use RGBA, .JPG uses RGB)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//MUST USE THIS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//MUST USE THIS

	SDL_FreeSurface(surface);
	return textureID;

}

void Platform::blendSprite(GLuint& texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture); //Disabling this will result in the most recent image loaded to take over everything

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

GLuint Platform::LocateTexture()
{
	return texturez;
}
void Platform::setOrthoProjection() {
	proj.setOrthoProjection(-4.0f, 4.0f, -4.0f, 4.0f, -1.0f, 1.0f);
	//view.setOrthoProjection(0.0f, 8.0f, 0.0f, 8.0f, -1.0f, 1.0f);
}
void Platform::moveViewMatrix(float x_value, float y_value, float z_value) {
	view.Translate(x_value, y_value, z_value);
}

void Platform::readFile(const char* levelFile, ShaderProgram& program) {
	ifstream infile(levelFile);
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[People]") {
			readEntityData(infile, program);
		}
	}




}
void Platform::incrementviewYPos(float value) {
	viewY += value;
}
void Platform::incrementviewXPos(float value) {
	viewX += value;
}