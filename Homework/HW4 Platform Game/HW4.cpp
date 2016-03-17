#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include "Astral.h"//
//#include "SpaceParty.h"//
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SpriteSheet.h"//
#include "Platform.h"
//#include "Projectile.h"//
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <math.h>
#include <SDL_mixer.h>
#include <SDL_audio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>


using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
#define FIXED_TIMESTEP 0.0166666f 
#define MAX_TIMESTEPS 6

int main(int argc, char *argv[]) {
	Platform game;
	game.setup();
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	GLuint tiles = game.LoadTexture("MetalSheet.png");
	game.texturez = tiles;
	GLuint playerModel = game.LoadTexture("p1_spritesheet.png");
	//	<SubTexture name="metalHalf.png" x="0" y="280" width="70" height="70"/>
	//<SubTexture name="playerShip1_red.png" x="224" y="832" width="99" height="75"/>
	//SpriteSheet metal(tiles, 0.0f/256.0f, 280.0f/128.0f, 16.0f/256.0f, 16.0f/182.0f, 0.2);
	//SpriteSheet metal2(tiles, 0.0f / 512.0f, 280.0f / 512.0f, 70.0f / 512.0f, 70.0f / 512.0f, 0.2);
	Matrix model, projection, view;
	vector<SpriteSheet> metaltiles;
	vector<SpriteSheet> playerAnimation;

	for (int i = 0; i < 28; i++) {
		metaltiles.push_back(SpriteSheet(tiles, 0.0f / 512.0f, 280.0f / 512.0f, 70.0f / 512.0f, 70.0f / 512.0f, 0.2));
	}
	/*p1_stand = 67 196 66 92
p1_walk01 = 0 0 72 97
p1_walk02 = 73 0 72 97
p1_walk03 = 146 0 72 97
p1_walk04 = 0 98 72 97
p1_walk05 = 73 98 72 97
p1_walk06 = 146 98 72 97
p1_walk07 = 219 0 72 97
p1_walk08 = 292 0 72 97
p1_walk09 = 219 98 72 97
p1_walk10 = 365 0 72 97
p1_walk11 = 292 98 72 97*/
	playerAnimation.push_back(SpriteSheet(playerModel, 67.0f / 508.0f, 196.0f / 288.0f, 66.0f / 508.0f, 92.0f / 288.0f, 0.2));
	playerAnimation.push_back(SpriteSheet(playerModel, 0.0f / 508.0f, 0.0f / 288.0f, 72.0f / 508.0f, 97.0f / 288.0f, 0.2));
	playerAnimation.push_back(SpriteSheet(playerModel, 73.0f / 508.0f, 0.0f / 288.0f, 72.0f / 508.0f, 97.0f / 288.0f, 0.2));
	playerAnimation.push_back(SpriteSheet(playerModel, 146.0f / 508.0f, 0.0f / 288.0f, 72.0f / 508.0f, 97.0f / 288.0f, 0.2));
	playerAnimation.push_back(SpriteSheet(playerModel, 0.0f / 508.0f, 98.0f / 288.0f, 72.0f / 508.0f, 97.0f / 288.0f, 0.2));
	playerAnimation.push_back(SpriteSheet(playerModel, 73.0f / 508.0f, 98.0f / 288.0f, 72.0f / 508.0f, 97.0f / 288.0f, 0.2));
	playerAnimation.push_back(SpriteSheet(playerModel, 146.0f / 508.0f, 98.0f / 288.0f, 72.0f / 508.0f, 97.0f / 288.0f, 0.2));
	AstralEntity something, something2;
	//something.setOrthoProjection();
	//something.texID = tiles;
	vector<AstralEntity> entites;
	//something2.setOrthoProjection();
	//game.setOrthoProjection();
	
	bool done = false;
	float lastFrameTicks = 0.0;
	SDL_Event event;
	int animationIndex = 0;
	for (int i = 0; i < 29; i++) {
		entites.push_back(AstralEntity());
		//entites[i].setOrthoProjection();
	}
	view.setOrthoProjection(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0);
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	float animationElapsed = 0.0f;
	float fps = 30.0;
	//game.readFile("Metal Sheet Tiles.txt");
	while (!done) {
		while (SDL_PollEvent(&event)) {
						if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
							done = true;
						}
					}
		float ticks = (float)SDL_GetTicks() / 1000.0f; float elapsed = ticks - lastFrameTicks; lastFrameTicks = ticks;
		float fixedElapsed = elapsed;
		animationElapsed += fixedElapsed;
		if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS)
					{ fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS; } 
			
					while (fixedElapsed >= FIXED_TIMESTEP)
					{
						fixedElapsed -= FIXED_TIMESTEP;		
				}

					game.clearScreen();
					
					//game.blendSprite(tiles);
					//game.BuildLevel();
					//game.BuildLevel();
					//something.setMatrices(program);
					//something.identityMatrix();
					//something.moveMatrix(-2.7, 2.7, 0.0);
					//something.moveMatrix(0.0, -0.25 * elapsed, 0.0);
					//metal.Draw(program);
					program.setViewMatrix(view);
					for (int i = 0; i < 28; i++) {
						entites[i].setMatrices(program);
						metaltiles[i].Draw(program);
					}
					entites[0].identityMatrix();
					entites[0].XPos = -2.8;
					entites[0].moveMatrix(entites[0].XPos, 0.0, 0.0);
					for (int i = 1; i < 28; i++) {
						entites[i].XPos = entites[i - 1].XPos + metaltiles[i].width + 0.07;
						entites[i].identityMatrix();
						entites[i].moveMatrix(entites[i].XPos, 0.0, 0.0);
					}
					entites[28].setMatrices(program);
					entites[28].identityMatrix();
					entites[28].moveMatrix(entites[28].XPos, 0.0, 0.0);
					view.identity();
					view.Translate(-1 * entites[28].XPos * 0.25, 0.0, 0.0);
					if (keys[SDL_SCANCODE_D]) {
						//entites[28].HDirection *= -1;
						entites[28].incrementXPos(0.75 * entites[28].velocity * entites[28].HDirection * elapsed);
						//view.Translate(-1 * entites[28].XPos * 0.25, 0.0, 0.0);
						if (animationElapsed > 1.0 / fps) {
							animationIndex++;
							animationElapsed = 0.0f;
							if (animationIndex == playerAnimation.size() - 1) {
								animationIndex = 0;
							}
						}
					}
					if (keys[SDL_SCANCODE_A]) {
						//entites[28].HDirection *= -1;
						entites[28].incrementXPos(-1 * 0.75 * entites[28].velocity * entites[28].HDirection * elapsed);
						//view.Translate(-1 * entites[28].XPos * 0.25, 0.0, 0.0);
						if (animationElapsed > 1.0 / fps) {
							animationIndex++;
							animationElapsed = 0.0f;
							if (animationIndex == playerAnimation.size() - 1) {
								animationIndex = 0;
							}
						}
					}
					playerAnimation[animationIndex].Draw(program);
					//view.Translate(0.25* elapsed, 0.0, 0.0);
					//entites[28].model.Rotate(-3.14 / 2);

					//entites[28].view.Translate(0.25* elapsed, 0.0, 0.0);
					//entites[28].moveMatrix(-0.25 * elapsed, 0.0, 0.0);
					//something.moveMatrix(-1.25 * fixedElapsed, 0.0, 0.0);
					//game.renderUpdate(program, tiles);
					//something2.setMatrices(program);
					//something2.identityMatrix();
					//game.initalizeCell();
					//game.renderUpdate(program, tiles);

					game.windowSwapping();











	}
	

	SDL_Quit();
	return 0;
}