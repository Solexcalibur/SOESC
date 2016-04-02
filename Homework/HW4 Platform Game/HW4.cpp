#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include "Astral.h"//
//#include "SpaceParty.h"//
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SpriteSheet.h"
#include "Platform.h"
//#include "Projectile.h"//
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <SDL_mixer.h>
#include <SDL_audio.h>
//#include <string>
//#include <iostream>
//#include <sstream>


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
	
	
	
	GLuint tiles = game.LoadTexture("arne_sprites.png");
	game.texturez = tiles;
	GLuint playerModel = game.LoadTexture("p1_spritesheet.png");
	game.animation = playerModel;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
		Mix_Music *mysteriousSound;
		mysteriousSound = Mix_LoadMUS("Streets of Rage 2 - Dreamer.mp3");
	//<SubTexture name="metalHalf.png" x="0" y="280" width="70" height="70"/>
	//<SubTexture name="playerShip1_red.png" x="224" y="832" width="99" height="75"/>
	//SpriteSheet metal(tiles, 0.0f/256.0f, 280.0f/128.0f, 16.0f/256.0f, 16.0f/182.0f, 0.2);
	//SpriteSheet metal2(tiles, 0.0f / 512.0f, 280.0f / 512.0f, 70.0f / 512.0f, 70.0f / 512.0f, 0.2);
	Matrix model, projection, view;
	//vector<SpriteSheet> metaltiles;
	vector<SpriteSheet> playerAnimation;
	//SpriteSheet(tiles, )
	/*for (int i = 0; i < 28; i++) {
		metaltiles.push_back(SpriteSheet(tiles, 0.0f / 512.0f, 280.0f / 512.0f, 70.0f / 512.0f, 70.0f / 512.0f, 0.2));
	}*/
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
	/*playerAnimation[0].textureID = playerModel;
	playerAnimation[0].u = 67.0f / 508.0f;
	playerAnimation[0].v = 196.0f / 288.0f;
	playerAnimation[0].width = 66.0f / 508.0f;
	playerAnimation[0].height = 92.0f / 288.0f;
	playerAnimation[0].size = 0.2;*/
	AstralEntity something, something2;
	something.setOrthoProjection();
	//something.texID = tiles;
	vector<AstralEntity> entites;
	//something2.setOrthoProjection();
	//game.setOrthoProjection();
	
	bool done = false;
	float lastFrameTicks = 0.0;
	SDL_Event event;
	int animationIndex = 0;
	int scaleXFactor = 1;
	for (int i = 0; i < 29; i++) {
		entites.push_back(AstralEntity());
		//entites[i].XPos = rand() % 3;
		//entites[i].YPos = rand() % 3;
		//entites[i].XPos = rand() % 6;
		//entites[i].YPos = (int)entites[i].XPos % 6;
		
		entites[i].setOrthoProjection();
	}
	entites[28].YPos = -0.75;
	entites[28].XPos = -2.5;
	
	game.setOrthoProjection();
	//view.setOrthoProjection(-4.0, 4.0, -4.0, 4.0, -1.0, 1.0);
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	float animationElapsed = 0.0f;
	float fps = 30.0;
	game.readFile("some.txt", program);
	//view.setOrthoProjection(-3.0, 3.0, -2.0, 2.0, -1.0, 1.0);
	Mix_PlayMusic(mysteriousSound, -1);
	game.render(program);
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
					{ fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
		//game.renderUpdate(program, tiles, entites[28], playerAnimation, fixedElapsed);
		}
			
					while (fixedElapsed >= FIXED_TIMESTEP)
					{
						fixedElapsed -= FIXED_TIMESTEP;		
						//game.renderUpdate(program, tiles, entites[28], playerAnimation, FIXED_TIMESTEP);
						game.Update(FIXED_TIMESTEP, playerAnimation, entites[28], program);
				}

					game.clearScreen();
					
					
					game.setMatrices(program);
					game.identityMatrix();
					
					game.Update(fixedElapsed, playerAnimation, entites[28], program);
					//game.renderUpdate(program, tiles, entites[28], playerAnimation, fixedElapsed);
					
					

					game.windowSwapping();











	}
	
	Mix_FreeMusic(mysteriousSound);
	SDL_Quit();
	return 0;
}