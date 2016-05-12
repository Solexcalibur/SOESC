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
#include "ParticleEmitter.h"
//#include "Color.h"
#include "Cubic.h"
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

	Matrix model, projection, view;
	vector<AstralEntity> objects;
	AstralEntity text(model, projection, view);
	AstralEntity particletest(model, projection, view);
	AstralEntity text2(model, projection, view);
	
	unsigned int particlecnt = 10;
	Cubic cube;
	ParticleEmitter p(10);
	
	
	text.setOrthoProjection();
	
	
	environment.readFile("Metal Sheet Tiles.txt", program);
	environment.render(program);
	

	float animationTime = 0;
	bool done = false;
	float lastFrameTicks = 0.0;
	
	SDL_Event event;
	while (!done){
		float ticks = (float)SDL_GetTicks() / 1000.0f; float elapsed = ticks - lastFrameTicks; lastFrameTicks = ticks;
		float fixedElapsed = elapsed;
	
		done = environment.inputProcessor(program,fixedElapsed);
		if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) 
		{ fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS; } 

		while (fixedElapsed >= FIXED_TIMESTEP)
		{
			fixedElapsed -= FIXED_TIMESTEP;
			
			environment.updateThings(program, FIXED_TIMESTEP);
		}
		
		
		
		environment.clearScreen();

		
		environment.screenSelector(program, elapsed);
		
		environment.updateThings(program, fixedElapsed);
		
		
		

		text.setMatrices(program);
		text.identityMatrix();
		text.position.x = 0.0;
		text.moveMatrix(0.0, 2.5, 0.0);



		environment.windowSwapping();
		



	}
	
	return 0;

}