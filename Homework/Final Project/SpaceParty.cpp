#include "Astral.h"
#include "SpaceParty.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Projectile.h"
#include "SpriteSheet.h"
#include "Vector.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <SDL_mixer.h>
#include <SDL_audio.h>
using namespace std;

#define FIXED_TIMESTEP 0.0166666f 

GLuint SpacialArea::LoadTexture(const char* image_path) {

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

SpacialArea::SpacialArea() { //Ridiciously long initalizer
	score = 0;
	state = 0;
	keys = SDL_GetKeyboardState(nullptr);
	for (int i = 0; i < 5; i++) {
		player.push_back(AstralEntity());
	}
	for (int i = 0; i < 10; i++) {
		shots.push_back(Projectile());
	}
	//wordTexture = LoadTexture(fontSheetPath);
	//spriteSheetTexture = LoadTexture(spritepath);
	r_filter = 0.5;
	g_filter = 0.0;
	b_filter = 0.5;
	player[0].position.y = -1.5;
	player[0].position.x = 0.0;

	player[1].position.y = 1.5;
	player[1].position.x = 0.0;

	/*player[2].YPos = 1.5;
	player[2].XPos = 0.0;

	player[3].YPos = 1.0;
	player[3].XPos = 0.0;

	player[4].YPos = 1.0;
	player[4].XPos = 0.0;*/
	//sprites[0].textureID = spriteSheetTexture;
	sprites[0].u = 224.0 / 1024.0;
	sprites[0].v = 832.0 / 1024.0;
	sprites[0].width = 99.0 / 1024.0;
	sprites[0].height = 75.0 / 1024.0;
	sprites[0].size = 0.4;
	//sprites[1].textureID = spriteSheetTexture;
	sprites[1].u = 858.0 / 1024.0;
	sprites[1].v = 230.0 / 1024.0;
	sprites[1].width = 9.0 / 1024.0;
	sprites[1].height = 54.0 / 1024.0;
	sprites[1].size = 0.4;
	//sprites[2].textureID = spriteSheetTexture;
	sprites[2].u = 423.0 / 1024.0;
	sprites[2].v = 728.0 / 1024.0;
	sprites[2].width = 93.0 / 1024.0;
	player[1].width = sprites[2].width;
	player[3].width = sprites[2].width;
	sprites[2].height = 84.0 / 1024.0;
	player[1].height = sprites[2].height;
	player[3].height = sprites[2].height;
	sprites[2].size = 0.4;
	sprites[3].u = 423.0 / 1024.0;
	sprites[3].v = 728.0 / 1024.0;
	sprites[3].width = 93.0 / 1024.0;
	player[2].width = sprites[3].width;
	player[4].width = sprites[3].width;
	sprites[3].height = 84.0 / 1024.0;
	player[2].height = sprites[2].height;
	player[4].height = sprites[2].height;
	sprites[3].size = 0.4;
	shotIndex = 0;
	shots[shotIndex].position.y = -1.5;
	shots[shotIndex].width = sprites[1].width;
	shots[shotIndex].height = sprites[1].height;
	//player = new AstralEntity();
	numEnemies = player.size() - 1;
	
}


void SpacialArea::setup() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	int x_resolution = 800;
	int y_resolution = 600;
	displayWindow = SDL_CreateWindow("Astral Horizon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_resolution, y_resolution, SDL_WINDOW_OPENGL);// <-OUTER BOUND
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	//scored = Mix_LoadWAV("Score.ogg");

#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, x_resolution, y_resolution);//<- INNER BOUND
}

bool SpacialArea::windowCloseChecker(SDL_Event event) {
	bool done = false;

	if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) { //Toss this entire if statement into processInput function
		done = true;

	}

	return done;
}

void SpacialArea::clearScreen() {
	glClearColor(r_filter, g_filter, b_filter, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void SpacialArea::windowSwapping() {
	SDL_GL_SwapWindow(displayWindow);
}



void SpacialArea::DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing) {
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



void SpacialArea::blendSprite(GLuint& texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture); //Disabling this will result in the most recent image loaded to take over everything

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}


void SpacialArea::TitleScreen(ShaderProgram& program) {
	//Mix_PlayChannel(-1, start, 0);
	DrawText(program, wordTexture, "Astral Horizon! You are outnumbered, think you can win? (Press Enter)", 0.077, 0);
	//DrawText(program, fontTex, "2 v 1", 0.25, 0);
	//DrawText(program, fontTex, "Can you win?", 0.25, 0);


	blendSprite(wordTexture);

}

void SpacialArea::scoreBoard(ShaderProgram& program) {
	DrawText(program, wordTexture, "HEALTH:" + to_string(player[1].health), 0.15, 0);
	blendSprite(wordTexture);

}

void SpacialArea::gameOverScreen(ShaderProgram& program) {
	
	DrawText(program, wordTexture, "GAME OVER! DARE TO PLAY AGAIN? (Press Enter)", 0.11, 0);
	//DrawText(program, fontTex, "DARE TO PLAY AGAIN?", 0.1, 0);
	blendSprite(wordTexture);

}

void SpacialArea::VictoryScreen(ShaderProgram& program) {
	
	DrawText(program, wordTexture, "Good work! Go another round? (Press Enter)", 0.1, 0);
	//DrawText(program, fontTex, "DARE TO PLAY AGAIN?", 0.1, 0);
	blendSprite(wordTexture);
}

void SpacialArea::screenSelector(ShaderProgram& program) {

	switch (state) {
	case STATE_TITLE:
		TitleScreen(program);
		break;
	case STATE_GAME_LEVEL:
		scoreBoard(program);
		break;
	case STATE_GAME_OVER:
		gameOverScreen(program);
		break;
	case STATE_VICTORY:
		VictoryScreen(program);
		break;
	}
}

void SpacialArea::titleEvents(SDL_Event event, ShaderProgram& program) {

	while (SDL_PollEvent(&events)) {

		/*if (events.type == SDL_QUIT || events.type == SDL_WINDOWEVENT_CLOSE) { 
			done = true;

		}*/
		if (events.type == SDL_KEYDOWN) {
			if (events.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				Mix_PlayChannel(-1, start, 0);
				state = 1;
			}
			//shots[0].incrementYPos(4.0 * elapsed);

		}


	}


}


void SpacialArea::inGameEvents(SDL_Event event, ShaderProgram& program, float elapsed) {

	//numEnemies = player.size() - 1;
	/*time_t timer;
	if (time(&timer) % 5 == 0) {
		score += 1;
	}*/
	
	//score = 0;
	int ammoIndex = 0;
	//int maxshots = 10;

	if (shotIndex > maxshots - 1) {
		shotIndex = 0;

	}

	//objects[0].setMatrices(program);
	sprites[0].textureID = spriteSheetTexture;
	player[0].setMatrices(program);
	player[0].setOrthoProjection();
	player[0].identityMatrix();
	player[0].moveMatrix(player[0].position.x, player[0].position.y, 0.0);
	//spriteSheets[0].Draw(program);
	sprites[0].Draw(program);
	
	for (int k = 0; k < 2; k++) {
		player[k].velocity.x += player[k].acceleration.x * FIXED_TIMESTEP;
		player[k].velocity.x = player[k].lerp(player[k].velocity.x, 0.0, FIXED_TIMESTEP * player[k].friction.x);
		player[k].velocity.y += player[k].acceleration.y * FIXED_TIMESTEP;
		player[k].velocity.y = player[k].lerp(player[k].velocity.y, 0.0, FIXED_TIMESTEP * player[k].friction.y);
		//player[k].velocityY = player[k].lerp(player[k].velocityY, 0.0, FIXED_TIMESTEP * player[k].frictionY);

		
		//player[k].velocityY += player[k].accelerationY * FIXED_TIMESTEP;

		//player[k].incrementAccelerationX(FIXED_TIMESTEP);
	}
	//for (int indx = 1; indx < 5; indx++) {
		//if (player[indx].alive) {
			sprites[2].textureID = spriteSheetTexture;
			player[1].setMatrices(program);
			player[1].setOrthoProjection();
			player[1].identityMatrix();
			player[1].moveMatrix(player[1].position.x, player[1].position.y, 0.0);
			//player[indx].alive = true;
			//spriteSheets[2].Draw(program);
			sprites[2].Draw(program);
		//}
	//}



	sprites[1].textureID = spriteSheetTexture;
	shots[shotIndex].setOrthoProjection();
	shots[shotIndex].setMatrices(program);
	shots[shotIndex].identityMatrix();
	
	//shots[shotIndex].YPos = player[0].YPos;
	shots[shotIndex].incrementYPos(9.0 * elapsed);
	//spriteSheets[1].Draw(program);
	sprites[1].Draw(program);




	/*player[1].incrementXPos(1.0 * player[1].HDirection * player[1].velocityX * elapsed);
	player[2].incrementXPos(-1.0 * player[2].HDirection * player[2].velocityY * elapsed);
	player[3].incrementXPos(1.75 * player[3].HDirection * player[3].velocityY * elapsed);
	player[4].incrementXPos(-1.25 * player[4].HDirection * player[4].velocityY * elapsed);*/
	shots[shotIndex].moveMatrix(shots[shotIndex].position.x, shots[shotIndex].position.y + 0.5, 0.0);
	//while (SDL_PollEvent(&events)) {

	//	if (events.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) { 
	//		done = true;

	//	}
	//	else if (events.type == SDL_KEYDOWN) {
	//		if (events.key.keysym.scancode == SDL_SCANCODE_SPACE) {
	//			//player[0].shoot(program, ammo, spriteSheets, elapsed);
	//			//shoot(elapsed);
	//			//shotIndex++;
	//			//shots[0].incrementYPos(4.0 * elapsed);
	//			//shots[0].incrementYPos(4.0 * elapsed);
	//			//shots[0].moveMatrix(0.0, shots[0].YPos, 0.0);
	//			//DrawText(program, wordTexture, "Astral Horizon! You are outnumbered, think you can win?", 0.075, 0);

	//		}
	//		//shots[0].incrementYPos(4.0 * elapsed);

	//	}


	//}
	
	if (keys[SDL_SCANCODE_SPACE]) { //Hold to shoot. FULL AUTO
		shots[shotIndex].position.x = player[0].position.x;
		

		//shots[shotIndex].YPos = player[0].YPos;
		shoot(FIXED_TIMESTEP);
	}




	//for (int i = 1; i < 5; i++) {
		if (player[1].position.x > 2.0) {
			player[1].position.x = 2.0;
			//player[1].HDirection *= -1;
			//player[i].incrementYPos(-0.25 * player[i].velocityY);
		}
		if (player[1].position.x < -2.0) {
			player[1].position.x = -2.0;
			//player[1].HDirection *= -1;
			//player[i].incrementYPos(-0.25 * player[i].velocityY);
		}
		if (player[0].position.x > 2.0) {
			player[0].position.x = 2.0;
			//player[1].HDirection *= -1;
			//player[i].incrementYPos(-0.25 * player[i].velocityY);
		}
		if (player[0].position.x < -2.0) {
			player[0].position.x = -2.0;
			//player[1].HDirection *= -1;
			//player[i].incrementYPos(-0.25 * player[i].velocityY);
		}
		//if (objects[i].YPos == -1.5){
		//	//state = 2;
		//	objects[i].YPos = 1.5;
		//}
		

	//}
	
	if (player[0].position.y < -2.5) {
		player[0].position.y = -2.5;
	}
	if (player[0].position.y > 2.5) {
		player[0].position.y = 2.5;
	}
	if (player[1].position.y < -2.5) {
		player[1].position.y = -2.5;
	}
	if (player[1].position.y > 2.5) {
		player[1].position.y = 2.5;
	}
	//shots.erase(remove_if(shots.begin(), shots.end(), shouldRemoveBullet), shots.end());
	for (int i = 0; i < 2; i++) { //Bullet collision with enemy
		if (shots[shotIndex].position.y < player[i].position.y + player[i].height * 0.5
			&& shots[shotIndex].position.y > player[i].position.y - player[i].height * 0.5
			&& shots[shotIndex].position.x < player[i].position.x + player[i].width * 0.5
			&& shots[shotIndex].position.x > player[i].position.x - player[i].width * 0.5) {
			shots[shotIndex].position.y = player[0].position.y + 4.0; //still gotta remove bullet, just a temp fix
			player[i].health -= 10;
			//score += 5;

			if (player[i].health <= 0) {
				player[i].alive = false;
				//player[i].setWidthAndHeight(0.0, 0.0);
				//player.erase(remove_if(player.begin(), player.end(), player[i].alive), player.end());
				score += 100;
				state = 2;
				/*shots[shotIndex].remove = true;
				numEnemies -= 1;
				Mix_PlayChannel(-1, scored, 0);*/
				//shots.erase(remove_if(shots.begin(), shots.end(), shouldRemoveBullet), shots.end());

				//numEnemies--;
			}
		}
		/*if (player[i].YPos < -1.5) {
			Mix_PlayChannel(-1, gameover, 0);
			state = 2;
		}
		if (numEnemies == 0) {
			Mix_PlayChannel(-1, victory, 0);
			state = 3;
		}*/

	}



	if (keys[SDL_SCANCODE_B]) {
		state = 2;
		//player.XPos = 0.0;
		//objects[0].XPos = 0.0;
		player[0].position.x = 0.0;
	}
	if (keys[SDL_SCANCODE_LEFT]) {
		//player.incrementXPos(-2.0 * elapsed);
		player[0].incrementXPos(-1.5 * player[0].direction.x * player[0].velocity.x * elapsed);
		shots[shotIndex].incrementXPos(-1.5 * player[0].direction.x * player[0].velocity.x *  elapsed);
		//player[0].incrementAccelerationX(-1 * FIXED_TIMESTEP);
		//ammo[1].incrementXPos(-2.0 * elapsed);
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		//player.incrementXPos(2.0 * elapsed);
		player[0].incrementXPos(1.5 * player[0].direction.x *  player[0].velocity.x * elapsed);
		shots[shotIndex].incrementXPos(1.5 * player[0].direction.x * player[0].velocity.x *  elapsed);
		//player[0].incrementAccelerationX(FIXED_TIMESTEP);
		//score += 100;
		//ammo[1].incrementXPos(2.0 * elapsed);
	}
	if (keys[SDL_SCANCODE_UP]) {
		//player.incrementXPos(-2.0 * elapsed);
		player[0].incrementYPos(1.5 *player[0].direction.y * player[0].velocity.y * elapsed);
		shots[shotIndex].incrementYPos(1.5 *player[0].direction.y * player[0].velocity.y *  elapsed);
		//player[0].incrementAccelerationX(-1 * FIXED_TIMESTEP);
		//ammo[1].incrementXPos(-2.0 * elapsed);
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		//player.incrementXPos(2.0 * elapsed);
		player[0].incrementYPos(-1.5 *player[0].direction.y *  player[0].velocity.y * elapsed);
		shots[shotIndex].incrementYPos(-1.5 * player[0].direction.y * player[0].velocity.y *  elapsed);
		//player[0].incrementAccelerationX(FIXED_TIMESTEP);
		//score += 100;
		//ammo[1].incrementXPos(2.0 * elapsed);
	}
	if (keys[SDL_SCANCODE_A]) {
		player[1].incrementXPos(-2.0 * player[1].velocity.x * elapsed);
	}
	if (keys[SDL_SCANCODE_D]) {
		player[1].incrementXPos(2.0 * player[1].velocity.x * elapsed);
	}
	if (keys[SDL_SCANCODE_S]) {
		player[1].incrementYPos(-2.0 * player[1].velocity.y * elapsed);
	}
	if (keys[SDL_SCANCODE_W]) {
		player[1].incrementYPos(2.0 * player[1].velocity.y * elapsed);
	}



}

void SpacialArea::endGameEvents(SDL_Event event) {
	
	

		//if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) { //Toss this entire if statement into processInput function
		//	done = true;

		//}
		 if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) { //RESET
				player[1].alive = true;
				numEnemies = 1;
				state = 1;
				
				//player[1].YPos = 1.5;
				//SpacialArea();
			}
			//shots[0].incrementYPos(4.0 * elapsed);

		}


	

}

void SpacialArea::updateThings(ShaderProgram& program, SDL_Event event, float elasped) {
	switch (state) {
	case STATE_TITLE:
		titleEvents(event, program);
		break;
	case STATE_GAME_LEVEL:
		inGameEvents(event, program, elasped);
		break;
	case STATE_GAME_OVER:
		endGameEvents(event);
		break;
	case STATE_VICTORY:
		endGameEvents(event);
		break;


	}


}

void SpacialArea::shoot(float elapsed) {
	//shots[0].setMatrices(program);
	////shots[0].YPos = -1.5;
	//sprite.Draw(program);
	//shots[0].incrementYPos(7.5 * elapsed);
	if (shots[shotIndex].position.y > 2.0) {
		shots[shotIndex].position.y = -1.5;
	}
	shotIndex++;

	if (shotIndex > maxshots - 1) {
		shotIndex = 0;

	}

}

bool SpacialArea::shouldRemoveBullet(Projectile& bullet)
{
	if (bullet.remove) {
		return true;
	}
	else {
		return false;
	}
}
//bool SpacialArea::raySegmentIntersect(const Vector &rayOrigin, const Vector &rayDirection, const
//	Vector &linePt1, const Vector &linePt2, float &dist)
//{
//	Vector seg1 = linePt1;
//	Vector segD;
//	segD.x = linePt2.x - seg1.x;
//	segD.y = linePt2.y - seg1.y;
//
//	float raySlope = rayDirection.y / rayDirection.x;
//	float n = ((seg1.x - rayOrigin.x)*raySlope + (rayOrigin.y - seg1.y)) / (segD.y -
//		segD.x*raySlope);
//
//	if (n < 0 || n > 1)
//		return false;
//
//	float m = (seg1.x + segD.x * n - rayOrigin.x) / rayDirection.x;
//	if (m < 0)
//		return false;
//
//	dist = m;
//	return true;
//}
