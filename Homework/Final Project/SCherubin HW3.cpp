/*
HW3: Space Invaders
Done by Stefan Cherubin
March 2nd, 2016
Be sure to include the headerfiles and corresponding source files listed below! (marked with double slash)

The bulk of the functions are in SpaceParty.cpp


*/
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include "Astral.h"//
#include "SpaceParty.h"//
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SpriteSheet.h"//
#include "Projectile.h"//
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <math.h>
#include <SDL_mixer.h>
#include <SDL_audio.h>

using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
#define FIXED_TIMESTEP 0.0166666f 
#define MAX_TIMESTEPS 6
//#define MAX_SHOTS 10




int main(int argc, char *argv[]){
	SpacialArea environment;
	environment.setup();
	
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	//ShaderProgram program2(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	Matrix model, projection, view;
	vector<AstralEntity> objects;
	AstralEntity text(model, projection, view);
	/*AstralEntity text2(model, projection, view);
	AstralEntity player(model, projection, view);
	AstralEntity enemy(model, projection, view);
	AstralEntity enemy2(model, projection, view);*/


	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Music *mysteriousSound;
	mysteriousSound = Mix_LoadMUS("ASDF.mp3");
	//
	//Mix_Chunk *scores;
	Mix_Chunk *scores, *gameOver, *victory, *begin;
	scores = Mix_LoadWAV("Score.ogg");
	environment.scored = scores;
	gameOver = Mix_LoadWAV("Game Over.ogg");
	environment.gameover = gameOver;
	victory = Mix_LoadWAV("Victory.ogg");
	environment.victory = victory;
	begin = Mix_LoadWAV("Skyjack.ogg");
	environment.start = begin;

	
	
	GLuint words = text.LoadTexture("font2.png");
	GLuint sprites = environment.LoadTexture("SpaceStuff.png");
	environment.wordTexture = words;
	environment.spriteSheetTexture = sprites;
	
	//<SubTexture name="playerShip1_red.png" x="224" y="832" width="99" height="75"/>
	
	//	<SubTexture name="laserRed01.png" x="858" y="230" width="9" height="54"/>
	
	//<SubTexture name="enemyBlack1.png" x="423" y="728" width="93" height="84"/>
	
	//<SubTexture name="laserBlue01.png" x="856" y="421" width="9" height="54"/>
	
	//<SubTexture name="laserBlue10.png" x="740" y="724" width="37" height="37"/>

	
	//make vector of spritesheets
	//make AstralEntity take in spritesheet as param?
	//by default, index 0 wll be space ship
	//index 1 to n will be enemies
	//press space to draw index
	
	objects.push_back(text);
	//ships.push_back(player);

	

	//Projectile ammos[MAX_SHOTS];

	
	//int ammoIndex = 0;
	
	text.setOrthoProjection();

	
	Mix_PlayMusic(mysteriousSound, -1);
	bool done = false;
	float lastFrameTicks = 0.0;
	SDL_Event event;
	while (!done){
		float ticks = (float)SDL_GetTicks() / 1000.0f; float elapsed = ticks - lastFrameTicks; lastFrameTicks = ticks;
		float fixedElapsed = elapsed;
		//while (SDL_PollEvent(&event)) {

		//	//done = environment.windowCloseChecker(event, ships, ammos, spriteSheets, program, fixedElapsed);
		//	if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
		//		done = true;
		//	}


		//}
		done = environment.inputProcessor(program,fixedElapsed);
		if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) 
		{ fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS; } 

		while (fixedElapsed >= FIXED_TIMESTEP)
		{
			fixedElapsed -= FIXED_TIMESTEP;
			//environment.updateThings(&program, &program2, player, ammos, event, something, FIXED_TIMESTEP);
			environment.updateThings(program, FIXED_TIMESTEP);
		}
		//Update(FIXED_TIMESTEP); }
		
		
		environment.clearScreen();

		
		environment.screenSelector(program);
		
		environment.updateThings(program, fixedElapsed);
		//environment.windowCloseChecker(event);
		
		
	
		


		text.setMatrices(program);
		text.identityMatrix();
		text.moveMatrix(-2.5, 2.6, 0.0);
		
		


		environment.windowSwapping();


	}
	Mix_FreeMusic(mysteriousSound);
	Mix_FreeChunk(environment.scored);
	Mix_FreeChunk(environment.gameover);
	Mix_FreeChunk(environment.victory);
	Mix_FreeChunk(environment.start);
	//Mix_FreeChunk(scores);
	//Mix_FreeChunk(victory);
	//Mix_FreeChunk(gameOver);
	//Mix_FreeChunk(begin);
	SDL_Quit();
	return 0;

}