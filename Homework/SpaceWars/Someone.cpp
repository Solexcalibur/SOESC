#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include "Astral.h"
#include "SpaceParty.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SpriteSheet.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <math.h>
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
	ShaderProgram program2(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	Matrix model, projection, view;

	GLuint words = environment.LoadTexture("font2.png");
	GLuint sprites = environment.LoadTexture("SpaceStuff.png");
	vector<SpriteSheet> spriteSheets;
	//<SubTexture name="playerShip1_red.png" x="224" y="832" width="99" height="75"/>
	SpriteSheet something(sprites, 224.0 / 1024.0, 832.0 / 1024.0, 99.0 / 1024.0, 75.0 / 1024.0, 0.4);
	//	<SubTexture name="laserRed01.png" x="858" y="230" width="9" height="54"/>
	SpriteSheet something2(sprites, 858.0 / 1024.0, 230.0 / 1024.0, 9.0 / 1024.0, 54.0 / 1024.0, 0.4);
	//<SubTexture name="enemyBlack1.png" x="423" y="728" width="93" height="84"/>
	SpriteSheet something3(sprites, 423.0 / 1024.0, 728.0 / 1024.0, 93.0 / 1024.0, 84.0 / 1024.0, 0.4);
	//<SubTexture name="laserBlue01.png" x="856" y="421" width="9" height="54"/>
	SpriteSheet something4(sprites, 856.0 / 1024.0, 421.0 / 1024.0, 9.0 / 1024.0, 84.0 / 1024.0, 0.4);


	spriteSheets.push_back(something);
	spriteSheets.push_back(something2);
	spriteSheets.push_back(something3);
	spriteSheets.push_back(something4);
	//make vector of spritesheets
	//make AstralEntity take in spritesheet as param?
	//by default, index 0 wll be space ship
	//index 1 to n will be enemies
	//press space to draw index
	vector<AstralEntity> objects;
	AstralEntity text(model, projection, view);
	AstralEntity player(model, projection, view);
	AstralEntity enemy(model, projection, view);
	objects.push_back(text);
	objects.push_back(player);
	objects.push_back(enemy);

	Projectile ammos[MAX_SHOTS];


	//vector<Projectile*> ammo;
	for (int i = 0; i < 10; i++){
		//ammo.push_back(new Projectile(model, projection, view));
		//ammo[i]->setOrthoProjection();
		ammos[i].setOrthoProjection();
		ammos[i].XPos = player.XPos;
		ammos[0].YPos = -1.5;

	}
	//ammos[1].XPos = objects[2].XPos;
	//ammos[1].YPos = objects[2].YPos;
	int ammoIndex = 0;
	//AstralEntity bullet(model, projection, view);
	//player.setMatrices(&program);
	//player.texID = environment.LoadTexture("ghost.png");
	//player.texID = sprites;
	text.setOrthoProjection();
	//player.setOrthoProjection();
	//enemy.setOrthoProjection();
	//ammos[0].texID = sprites;
	for (int i = 1; i < objects.size(); i++){
		objects[i].setOrthoProjection();
	}
	
	//ammo[0]->setOrthoProjection();
	//bullet.setOrthoProjection();

	SDL_Event event;
	bool done = false;
	float lastFrameTicks = 0.0;
	while (!done){
		float ticks = (float)SDL_GetTicks() / 1000.0f; float elapsed = ticks - lastFrameTicks; lastFrameTicks = ticks;
		float fixedElapsed = elapsed;
		float fixedTimeStep = 0.0166666f;
		if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) 
		{ fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS; } 

		while (fixedElapsed >= FIXED_TIMESTEP)
		{
			fixedElapsed -= FIXED_TIMESTEP;
			//environment.updateThings(&program, &program2, player, ammos, event, something, FIXED_TIMESTEP);
		}
		//Update(FIXED_TIMESTEP); }
		while (SDL_PollEvent(&event)) {
			done = environment.windowCloseChecker(event);
		}
		environment.clearScreen();
		environment.screenSelector(&program);
		environment.updateThings(&program, objects, ammos, event, spriteSheets, fixedElapsed);
		

		//player.identityMatrix();
		objects[1].identityMatrix();
		objects[1].moveMatrix(objects[1].XPos, -1.5, 0.0);
		//player.moveMatrix(player.XPos, -1.5, 0.0);

		//enemy.identityMatrix();
		objects[2].identityMatrix();
		objects[2].moveMatrix(objects[2].XPos, 1.5, 0.0);
		objects[2].moveMatrix(0.0, objects[2].YPos, 0.0);
		//enemy.moveMatrix(0.0, 1.5, 0.0);



		text.setMatrices(&program);
		//text.DrawText(&program, words, "SCORE:" + to_string(text.score), 0.25, 0.0);
		text.identityMatrix();
		text.moveMatrix(-2.5, 1.6, 0.0);
		environment.blendSprite(words);

		//for (int ammoIndex = 0; ammoIndex < 10; ammoIndex++){

			//ammos[ammoIndex].setMatrices(&program2);
			//ammo[i].setOrthoProjection();
		///}
		ammos[0].identityMatrix();
		//ammos[ammoIndex].renderWithNoTexture(&program);
		ammos[0].moveMatrix(ammos[0].XPos, ammos[0].YPos, 0.0);
		ammos[1].identityMatrix();
		//ammos[ammoIndex].renderWithNoTexture(&program);
		ammos[1].moveMatrix(ammos[1].XPos, ammos[1].YPos, 0.0);

		//ammos[0].scaleMatrix(1.0, 1.0, 1.0);
		//ammos[0].texID = something2.textureID;
		ammoIndex++;
		if (ammoIndex > MAX_SHOTS - 1){
			ammoIndex = 0;

		}
		//ammo[ammoIndex]->setMatrices(&program2);
		/*ammo[0]->setMatrices(&program2);
		ammo[0]->renderWithNoTexture(&program2);
	    ammo[0]->identityMatrix();
		ammo[0]->XPos = player.XPos;
		ammo[0]->YPos = -1.0;
		ammo[0]->moveMatrix(0.0, ammo[ammoIndex]->YPos, 0.0);
		ammoIndex++;*/
		/*bullet.identityMatrix();
		bullet.XPos = player.XPos;
		bullet.moveMatrix(player.XPos, -1.0, 0.0);
		bullet.moveMatrix(0.0, bullet.YPos, 0.0);*/


		environment.windowSwapping();


	}
	SDL_Quit();
	return 0;
}