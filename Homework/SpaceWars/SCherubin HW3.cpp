#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include "Astral.h"
#include "SpaceParty.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SpriteSheet.h"
#include "Projectile.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <math.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
#define FIXED_TIMESTEP 0.0166666f 
#define MAX_TIMESTEPS 6
#define MAX_SHOTS 10

float vertices_paddle [] = { -0.5, -0.5,
0.05, -0.5,
0.05, 0.5,
-0.5, -0.5,
0.05, 0.5,
-0.5, 0.5 };

float vertices_ball[] = { -0.04, -0.04,
0.04, -0.04,
0.04, 0.04,
-0.04, -0.04,
0.04, 0.04,
-0.04, 0.04 };
float texCoords[] = { 0.0, 1.0,
1.0, 1.0,
1.0, 0.0,
0.0, 1.0,
1.0, 0.0,
0.0, 0.0 };



int main(int argc, char *argv[]){
	SpacialArea environment;
	environment.setup();
	
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	//ShaderProgram program2(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	Matrix model, projection, view;
	vector<AstralEntity> objects;
	vector<AstralEntity> ships;
	AstralEntity text(model, projection, view);
	/*AstralEntity text2(model, projection, view);
	AstralEntity player(model, projection, view);
	AstralEntity enemy(model, projection, view);
	AstralEntity enemy2(model, projection, view);*/



	GLuint words = text.LoadTexture("font2.png");
	GLuint sprites = environment.LoadTexture("SpaceStuff.png");
	environment.wordTexture = words;
	environment.spriteSheetTexture = sprites;
	
	//<SubTexture name="playerShip1_red.png" x="224" y="832" width="99" height="75"/>
	
	//	<SubTexture name="laserRed01.png" x="858" y="230" width="9" height="54"/>
	
	//<SubTexture name="enemyBlack1.png" x="423" y="728" width="93" height="84"/>
	
	//<SubTexture name="laserBlue01.png" x="856" y="421" width="9" height="54"/>
	


	
	//make vector of spritesheets
	//make AstralEntity take in spritesheet as param?
	//by default, index 0 wll be space ship
	//index 1 to n will be enemies
	//press space to draw index
	
	objects.push_back(text);
	//ships.push_back(player);

	

	Projectile ammos[MAX_SHOTS];

	
	int ammoIndex = 0;
	
	text.setOrthoProjection();

	SDL_Event event;
	bool done = false;
	float lastFrameTicks = 0.0;
	//ships[0].YPos = -1.5;
	while (!done){
		float ticks = (float)SDL_GetTicks() / 1000.0f; float elapsed = ticks - lastFrameTicks; lastFrameTicks = ticks;
		float fixedElapsed = elapsed;
		//float fixedTimeStep = 0.0166666f;
		if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) 
		{ fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS; } 

		while (fixedElapsed >= FIXED_TIMESTEP)
		{
			fixedElapsed -= FIXED_TIMESTEP;
			//environment.updateThings(&program, &program2, player, ammos, event, something, FIXED_TIMESTEP);
			environment.updateThings(program, event, FIXED_TIMESTEP);
		}
		//Update(FIXED_TIMESTEP); }
		while (SDL_PollEvent(&event)) {
			
			//done = environment.windowCloseChecker(event, ships, ammos, spriteSheets, program, fixedElapsed);
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			
			
		}
		//environment.screenSelector(program);
		environment.clearScreen();

		//environment.screenSelector();
		environment.screenSelector(program);
		//ammos[0].incrementYPos(2.5 * elapsed);
		environment.updateThings(program, event, elapsed);
		environment.windowCloseChecker(event);
		
		
	
		


		text.setMatrices(program);
		//text.DrawText(program, words, "SCORE:" + to_string(text.score), 0.25, 0.0);
		text.identityMatrix();
		text.moveMatrix(-2.5, 1.6, 0.0);
		
		


		environment.windowSwapping();


	}
	_CrtDumpMemoryLeaks();
	SDL_Quit();
	return 0;

}