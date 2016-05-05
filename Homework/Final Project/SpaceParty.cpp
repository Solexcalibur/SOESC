//#include "Astral.h"
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
#define JOYSTICK_DEADZONE 8000

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
	playerOne = nullptr;
	playerTwo = nullptr;
	for (int i = 0; i < 5; i++) {
		player.push_back(AstralEntity());
	}
	for (int i = 0; i < 10; i++) {
		shots.push_back(Projectile());
	}
	for (int i = 0; i < 2; i++) {
		party.push_back(ParticleEmitter(10));
	}
	//wordTexture = LoadTexture(fontSheetPath);
	//spriteSheetTexture = LoadTexture(spritepath);
	r_filter = 0.5;
	g_filter = 0.0;
	b_filter = 0.5;
	player[0].position.y = -1.5;
	player[0].position.x = 1.5;

	player[1].position.y = 1.5;
	player[1].position.x = -1.5;
	
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
	//Blue spacship
	sprites[2].u = 211.0 / 1024.0;
	sprites[2].v = 941.0 / 1024.0;
	sprites[2].width = 99.0 / 1024.0;
	player[1].width = sprites[2].width;
	player[3].width = sprites[2].width;
	sprites[2].height = 75.0 / 1024.0;
	player[1].height = sprites[2].height;
	player[3].height = sprites[2].height;
	sprites[2].size = 0.4;
	//<SubTexture name="playerShip1_green.png" x="237" y="377" width="99" height="75"/>
	sprites[3].u = 237.0 / 1024.0;
	sprites[3].v = 377.0 / 1024.0;
	sprites[3].width = 99.0 / 1024.0;
	player[2].width = sprites[3].width;
	player[4].width = sprites[3].width;
	sprites[3].height = 75.0 / 1024.0;
	player[2].height = sprites[2].height;
	player[4].height = sprites[2].height;
	sprites[3].size = 0.4;
	shotIndex = 0;
	//<SubTexture name="laserBlue01.png" x="856" y="421" width="9" height="54"/>
	sprites[4].u = 856.0 / 1024.0;
	sprites[4].v = 421.0 / 1024.0;
	sprites[4].width = 9.0 / 1024.0;
	sprites[4].height = 54.0 / 1024.0;
	sprites[4].size = 0.4;
	//<SubTexture name="laserGreen11.png" x="849" y="310" width="9" height="54"/>
	sprites[5].u = 849.0 / 1024.0;
	sprites[5].v = 310.0 / 1024.0;
	sprites[5].width = 9.0 / 1024.0;
	sprites[5].height = 54.0 / 1024.0;
	sprites[5].size = 0.4;
	//<SubTexture name="shield1.png" x="0" y="412" width="133" height="108"/>
	player[0].shots[shotIndex].width = sprites[1].width;
	player[0].shots[shotIndex].height = sprites[1].height;
	player[0].shots[shotIndex].position.y = player[0].position.y;
	player[0].shots[shotIndex].position.x = player[0].position.x;
	player[0].shots[shotIndex].direction.x = player[0].direction.x;
	player[0].shots[shotIndex].direction.y = 1.0;
	//player = new AstralEntity();
	//shots[shotIndex].position.y = player[0].position.y;
	numEnemies = player.size() - 1;
	player[1].scaleFactor = -1.0;
	mapHeight = LEVEL_HEIGHT;
	mapWidth = LEVEL_WIDTH;
	tileLength = TILE_SIZE * LEVEL_WIDTH;
	tileHeight = TILE_SIZE * LEVEL_HEIGHT;
	//particles = vector<ParticleEmitter>(10);
	playerIndexOne = 0;
	playerIndexTwo = 0;
	laserIndexOne = 1;
	laserIndexTwo = 1;
	
}

SpacialArea::~SpacialArea()
{
	SDL_JoystickClose(playerOne);
	playerOne = nullptr;
	SDL_JoystickClose(playerTwo);
	playerTwo = nullptr;
	Mix_FreeChunk(laserShot);
	Mix_FreeChunk(select);
	SDL_Quit();
}


void SpacialArea::setup() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
	int x_resolution = 800;
	int y_resolution = 600;
	displayWindow = SDL_CreateWindow("Astral Horizon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_resolution, y_resolution, SDL_WINDOW_OPENGL);// <-OUTER BOUND
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	//SDL_SetWindowFullscreen(displayWindow, SDL_WINDOW_FULLSCREEN);
	SDL_JoystickEventState(SDL_ENABLE);
	playerOne = SDL_JoystickOpen(0);
	//SDL_JoystickEventState(SDL_ENABLE);
	playerTwo = SDL_JoystickOpen(1);
	metalTex = LoadTexture("MetalSheet.png");
	//scored = Mix_LoadWAV("Score.ogg");
	//party.texture = LoadTexture("fire.png");
	
	particle.party = ParticleEmitter(2);
	wordTexture = LoadTexture("font2.png");
	particletex = LoadTexture("fire.png");
	party[0].texture = particletex;
	particle.texID = LoadTexture("fire.png");
	//party.SetTex("fire.png");
	spriteSheetTexture = LoadTexture("SpaceStuff.png");
	laserShot = Mix_LoadWAV("Laser_Shoot26.wav");
	select = Mix_LoadWAV("Blip_Select.wav");

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
	DrawText(program, wordTexture, "Astral Horizon! 1 V 1! Fight, kill, survive!", 0.077, 0);
	//DrawText(program, fontTex, "2 v 1", 0.25, 0);
	//DrawText(program, fontTex, "Can you win?", 0.25, 0);


	blendSprite(particletex);

}

void SpacialArea::scoreBoard(ShaderProgram& program) {
	DrawText(program, wordTexture,  "P2 HEALTH:" + to_string(player[1].health) + " SHIELDS:" + to_string(player[1].shields), 0.12, 0);
	//DrawText(program, wordTexture, "P1 HEALTH:" + to_string(player[0].health) + " SHIELDS:" + to_string(player[0].shields), 0.12, 0);
	blendSprite(wordTexture); //Blend Particle Sprite

}

void SpacialArea::gameOverScreen(ShaderProgram& program) {
	
	DrawText(program, wordTexture, "GAME OVER! DARE TO PLAY AGAIN? (Press Enter)", 0.11, 0);
	//DrawText(program, fontTex, "DARE TO PLAY AGAIN?", 0.1, 0);
	blendSprite(wordTexture);

}

void SpacialArea::VictoryScreen(ShaderProgram& program) {
	
	DrawText(program, wordTexture, "Good work! Go another round? (Press Enter)", 0.1, 0);
	//DrawText(program, fontTex, "DARE TO PLAY AGAIN?", 0.1, 0);
	//blendSprite(wordTexture);
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
	case STATE_CHARACTER_SELECT:
		SelectCharacter(program);
		break;
	}
}

void SpacialArea::SelectCharacter(ShaderProgram& program) {
	DrawText(program, wordTexture, "Select your ship", 0.077, 0);
	sprites[playerIndexOne].textureID = spriteSheetTexture;
	player[0].setMatrices(program);
	player[0].setOrthoProjection();
	player[0].identityMatrix();
	player[0].moveMatrix(2.5, 0.0, 0.0);
	//spriteSheets[0].Draw(program);
	sprites[playerIndexOne].Draw(program);
	sprites[playerIndexTwo].textureID = spriteSheetTexture;
	player[1].setMatrices(program);
	player[1].setOrthoProjection();
	player[1].identityMatrix();
	player[1].moveMatrix(-2.5, 0.0, 0.0);
	//spriteSheets[0].Draw(program);
	sprites[playerIndexTwo].Draw(program);


}
void SpacialArea::titleEvents(SDL_Event event, ShaderProgram& program) {

	//while (SDL_PollEvent(&events)) {

		/*if (events.type == SDL_QUIT || events.type == SDL_WINDOWEVENT_CLOSE) { 
			done = true;

		}*/
		//if (events.type == SDL_KEYDOWN) {
		//	if (events.key.keysym.scancode == SDL_SCANCODE_RETURN) {
		//		Mix_PlayChannel(-1, start, 0);
		//		state = 1;
		//	}
		//	//shots[0].incrementYPos(4.0 * elapsed);

		//}


	//}


}

void SpacialArea::setupThings(ShaderProgram& program) {
	sprites[playerIndexOne].textureID = spriteSheetTexture;
	player[0].setMatrices(program);
	player[0].setOrthoProjection();
	player[0].identityMatrix();
	player[0].moveMatrix(player[0].position.x, player[0].position.y, 0.0);
	//spriteSheets[0].Draw(program);
	sprites[playerIndexOne].Draw(program);
	if (playerIndexOne == 0)
		laserIndexOne = 1;
	else if (playerIndexOne == 2)
		laserIndexOne = 4;
	else if (playerIndexOne == 3)
		laserIndexOne = 5;

	sprites[laserIndexOne].textureID = spriteSheetTexture;
	/*shots[shotIndex].setOrthoProjection();
	shots[shotIndex].setMatrices(program);
	shots[shotIndex].identityMatrix();*/
	player[0].shots[player[0].ammoIndex].setOrthoProjection();
	player[0].shots[player[0].ammoIndex].setMatrices(program);
	player[0].shots[player[0].ammoIndex].identityMatrix();
	sprites[laserIndexOne].Draw(program);
	if (playerIndexTwo == 0)
		laserIndexTwo = 1;
	else if (playerIndexTwo == 2)
		laserIndexTwo = 4;
	else if (playerIndexTwo == 3)
		laserIndexTwo = 5;
	sprites[playerIndexTwo].textureID = spriteSheetTexture;
	player[1].setMatrices(program);
	player[1].setOrthoProjection();
	player[1].identityMatrix();
	player[1].moveMatrix(player[1].position.x, player[1].position.y, 0.0);
	//player[indx].alive = true;
	//spriteSheets[2].Draw(program);
	sprites[playerIndexTwo].Draw(program);

	sprites[laserIndexTwo].textureID = spriteSheetTexture;
	/*shots[shotIndex].setOrthoProjection();
	shots[shotIndex].setMatrices(program);
	shots[shotIndex].identityMatrix();*/
	player[1].shots[player[1].ammoIndex].setOrthoProjection();
	player[1].shots[player[1].ammoIndex].setMatrices(program);
	player[1].shots[player[1].ammoIndex].identityMatrix();
	sprites[laserIndexTwo].Draw(program);
}
void SpacialArea::inGameEvents(SDL_Event event, ShaderProgram& program, float elapsed) {
	
	setupThings(program);


	/*particle.setOrthoProjection();
	particle.setMatrices(program);
	particle.identityMatrix();*/
	

	//party[0].Update(elapsed);
	//blendSprite(party[0].texture);

	//numEnemies = player.size() - 1;
	/*time_t timer;
	if (time(&timer) % 5 == 0) {
		score += 1;
	}*/
	/*for (int i = 0; i < particles.size(); i++) {
		particles[i].Render(&program);
		particles[i].Update(elapsed);

		
	}
	particles[0].EmitXDirection(2, true);*/
	//render(program);
	//setupAndRender(program, vertexData.data(), texCoordData.data(), metalTex);
	int ammoIndex = 0;
	//int maxshots = 10;

	if (shotIndex > maxshots - 1) {
		shotIndex = 0;

	}
	
	//objects[0].setMatrices(program);
	//sprites[0].textureID = spriteSheetTexture;
	//player[0].setMatrices(program);
	//player[0].setOrthoProjection();
	//player[0].identityMatrix();
	//player[0].moveMatrix(player[0].position.x, player[0].position.y, 0.0);
	////spriteSheets[0].Draw(program);
	//sprites[0].Draw(program);
	
	//for (int k = 0; k < 2; k++) {
	//	player[k].velocity.x += player[k].acceleration.x * FIXED_TIMESTEP;
	//	player[k].velocity.x = player[k].lerp(player[k].velocity.x, 0.0, FIXED_TIMESTEP * player[k].friction.x);
	//	player[k].velocity.y += player[k].acceleration.y * FIXED_TIMESTEP;
	//	player[k].velocity.y = player[k].lerp(player[k].velocity.y, 0.0, FIXED_TIMESTEP * player[k].friction.y);
	//	//player[k].velocityY = player[k].lerp(player[k].velocityY, 0.0, FIXED_TIMESTEP * player[k].frictionY);

	//	
	//	//player[k].velocityY += player[k].accelerationY * FIXED_TIMESTEP;

	//	//player[k].incrementAccelerationX(FIXED_TIMESTEP);
	//}
	//for (int indx = 1; indx < 5; indx++) {
		//if (player[indx].alive) {
			
		//}
	//}



	//sprites[1].textureID = spriteSheetTexture;
	///*shots[shotIndex].setOrthoProjection();
	//shots[shotIndex].setMatrices(program);
	//shots[shotIndex].identityMatrix();*/
	//player[0].shots[player[0].ammoIndex].setOrthoProjection();
	//player[0].shots[player[0].ammoIndex].setMatrices(program);
	//player[0].shots[player[0].ammoIndex].identityMatrix();
	//sprites[1].Draw(program);

	//sprites[4].textureID = spriteSheetTexture;
	///*shots[shotIndex].setOrthoProjection();
	//shots[shotIndex].setMatrices(program);
	//shots[shotIndex].identityMatrix();*/
	//player[1].shots[player[1].ammoIndex].setOrthoProjection();
	//player[1].shots[player[1].ammoIndex].setMatrices(program);
	//player[1].shots[player[1].ammoIndex].identityMatrix();
	//sprites[4].Draw(program);
	
	/*shots[shotIndex].setMatrices(program);
	shots[shotIndex].identityMatrix();
	*/
	//shots[shotIndex].YPos = player[0].YPos;
	
	//spriteSheets[1].Draw(program);
	




	/*player[1].incrementXPos(1.0 * player[1].HDirection * player[1].velocityX * elapsed);
	player[2].incrementXPos(-1.0 * player[2].HDirection * player[2].velocityY * elapsed);
	player[3].incrementXPos(1.75 * player[3].HDirection * player[3].velocityY * elapsed);
	player[4].incrementXPos(-1.25 * player[4].HDirection * player[4].velocityY * elapsed);*/
	//shots[shotIndex].visible = false;
	/*shots[shotIndex].moveMatrix(shots[shotIndex].position.x, shots[shotIndex].position.y, 0.0);*/
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
	
	player[0].shots[player[0].ammoIndex].moveMatrix(player[0].shots[player[0].ammoIndex].position.x, player[0].shots[player[0].ammoIndex].position.y, 0.0);
	player[0].shots[player[0].ammoIndex].incrementYPos(8.0 * player[0].shots[player[0].ammoIndex].direction.y * elapsed);



	player[1].shots[player[1].ammoIndex].moveMatrix(player[1].shots[player[1].ammoIndex].position.x, player[1].shots[player[1].ammoIndex].position.y, 0.0);
	//player[1].shots[player[1].ammoIndex].direction.y = -1.0;
	player[1].shots[player[1].ammoIndex].incrementYPos(8.0 * player[1].shots[player[1].ammoIndex].direction.y * elapsed);
	//}
	// time_t timer;
	//if (time(&timer) > 5) {
	//if (events.type == SDL_KEYDOWN) {
	//	if (events.key.keysym.scancode == SDL_SCANCODE_SPACE) {
	//		//if (keys[SDL_SCANCODE_SPACE]) { //Hold to shoot. FULL AUTO
	//		shots[shotIndex].position.x = player[0].position.x;
	//		shoot(program, FIXED_TIMESTEP);
	//	}
	//}
			//shots[shotIndex].YPos = player[0].YPos;
			
			/*shoot(program, FIXED_TIMESTEP);*/
		//}
	//}


	
	player[0].model.Scale(1.0, player[0].scaleFactor, 1.0);
	player[1].shots[player[1].ammoIndex].model.Rotate(180.0 * (3.1415926 / 180.0));
	player[1].model.Scale(1.0, player[1].scaleFactor, 1.0);
	//for (int i = 1; i < 5; i++) {
		if (player[1].position.x > 2.7) {
			player[1].position.x = 2.7;
			//player[1].HDirection *= -1;
			//player[i].incrementYPos(-0.25 * player[i].velocityY);
		}
		if (player[1].position.x < -2.7) {
			player[1].position.x = -2.7;
			//player[1].HDirection *= -1;
			//player[i].incrementYPos(-0.25 * player[i].velocityY);
		}
		if (player[0].position.x > 2.7) {
			player[0].position.x = 2.7;
			//player[1].HDirection *= -1;
			//player[i].incrementYPos(-0.25 * player[i].velocityY);
		}
		if (player[0].position.x < -2.7) {
			player[0].position.x = -2.7;
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
	else if (player[0].position.y > 0.0) {
		//player[0].model.Rotate(180.0 * (3.1415926 / 180.0));
		player[0].scaleFactor = -1.0;
		player[0].shots[player[0].ammoIndex].model.Rotate(180.0 * (3.1415926 / 180.0));
		player[0].shots[player[0].ammoIndex].direction.y = -1.0;
	}
	else if (player[0].position.y < 0.0) {
		//player[0].model.Rotate(180.0 * (3.1415926 / 180.0));
		//shots[shotIndex].model.Rotate(180.0 * (3.1415926 / 180.0));
		player[0].scaleFactor = 1.0;
		player[0].shots[player[0].ammoIndex].direction.y = 1.0;
	}
	//shots[shotIndex].direction.y = 1.0;
	if (player[1].position.y < -2.5) {
		player[1].position.y = -2.5;
	}
	if (player[1].position.y > 2.5) {
		player[1].position.y = 2.5;
	}
	else if (player[1].position.y < 0.0) {
		//player[1].model.Rotate(180.0 * (3.1415926 / 180.0));
		player[1].scaleFactor = 1.0;
		player[1].shots[player[1].ammoIndex].model.Rotate(180.0 * (3.1415926 / 180.0));
		player[1].shots[player[1].ammoIndex].direction.y = 1.0;
	}
	else if (player[1].position.y > 0.0) {
		//player[1].model.Rotate(180.0 * (3.1415926 / 180.0));
		player[1].scaleFactor = -1.0;
		player[1].shots[player[1].ammoIndex].direction.y = -1.0;
	}
	float ydist = abs(player[0].position.y - player[1].position.y);
	float xdist = 20;

	if (player[0].shots[player[0].ammoIndex].position.y > 2.4 || player[0].shots[player[0].ammoIndex].position.y < - 2.4) {
		player[0].shots[player[0].ammoIndex].visible = false;
		player[0].shots[player[0].ammoIndex].scaleMatrix(0.0, 0.0, 0.0);
	}
	/*bool collisiontwofromone, collissiononefromtwo;
	collisiontwofromone = raySegmentIntersect(player[0].shots[player[0].ammoIndex].position, player[0].shots[player[0].ammoIndex].direction,
		player[1].segment1, player[1].segment2, xdist);
	if (collisiontwofromone) {
		shots[player[0].ammoIndex].visible = false;
		score += 5;
		player[1].health -= 10;
	}*/
	
	//shots.erase(remove_if(shots.begin(), shots.end(), shouldRemoveBullet), shots.end());
	//for (int i = 0; i < 2; i++) { //Bullet collision with enemy. Incresed hitbox so that hitting any part of ship deals damage
		if (player[0].shots[player[0].ammoIndex].position.y < player[1].position.y + player[1].height * 0.5
			&& player[0].shots[player[0].ammoIndex].position.y > player[1].position.y - player[1].height * 0.5
			&& player[0].shots[player[0].ammoIndex].position.x < player[1].position.x + player[1].width * 3.25
			&& player[0].shots[player[0].ammoIndex].position.x > player[1].position.x - player[1].width * 1.75) {
			if (player[0].shots[player[0].ammoIndex].visible) {

				player[0].shots[player[0].ammoIndex].position.x = 20.5;
				player[0].shots[player[0].ammoIndex].direction.y = 1.0;//still gotta remove bullet, just a temp fix
				player[0].shots[player[0].ammoIndex].visible = false;
				if (player[1].shields > 0) {
					player[1].shields -= 6;
					player[1].health -= 4;
				}
				else {
					player[1].shields = 0;
					player[0].shots[player[0].ammoIndex].position.y = 2.5;
					player[1].health -= 10;
					
					
				}
				//score += 5;
			}
		}
		if (player[1].shots[player[1].ammoIndex].position.y < player[0].position.y + player[0].height * 0.5
			&& player[1].shots[player[1].ammoIndex].position.y > player[0].position.y - player[0].height * 0.5
			&& player[1].shots[player[1].ammoIndex].position.x < player[0].position.x + player[0].width * 3.25
			&& player[1].shots[player[1].ammoIndex].position.x > player[0].position.x - player[0].width * 1.75) {
			if (player[1].shots[player[1].ammoIndex].visible) {
				player[1].shots[player[1].ammoIndex].position.x = 20.5;
				player[1].shots[player[1].ammoIndex].direction.y = 1.0;//still gotta remove bullet, just a temp fix
				player[1].shots[player[1].ammoIndex].visible = false;
				if (player[0].shields > 0) {
					player[0].shields -= 6;
					player[0].health -= 4;
				}
				else {
					player[1].shots[player[1].ammoIndex].position.y = 2.5;
					player[0].health -= 10;
					
				}
				//score += 5;
			}
		}
		if (player[0].health < 0 || player[1].health < 0) {
			state = 2;
		}
	//}
	//		if (player[i].health <= 0) {
	//			player[i].alive = false;
	//			//player[i].setWidthAndHeight(0.0, 0.0);
	//			//player.erase(remove_if(player.begin(), player.end(), player[i].alive), player.end());
	//			score += 100;
	//			state = 2;
	//			/*shots[shotIndex].remove = true;
	//			numEnemies -= 1;
	//			Mix_PlayChannel(-1, scored, 0);*/
	//			//shots.erase(remove_if(shots.begin(), shots.end(), shouldRemoveBullet), shots.end());

	//			//numEnemies--;
	//		}
	//	}
	//	/*if (player[i].YPos < -1.5) {
	//		Mix_PlayChannel(-1, gameover, 0);
	//		state = 2;
	//	}
	//	if (numEnemies == 0) {
	//		Mix_PlayChannel(-1, victory, 0);
	//		state = 3;
	//	}*/

	//}



	//if (keys[SDL_SCANCODE_B]) {
	//	state = 2;
	//	//player.XPos = 0.0;
	//	//objects[0].XPos = 0.0;
	//	player[0].position.x = 0.0;
	//}
	//if (keys[SDL_SCANCODE_LEFT]) {
	//	//player.incrementXPos(-2.0 * elapsed);
	//	player[0].incrementXPos(-1.5 * player[0].direction.x * player[0].velocity.x * elapsed);
	//	shots[shotIndex].incrementXPos(-1.5 * player[0].direction.x * player[0].velocity.x *  elapsed);
	//	//player[0].incrementAccelerationX(-1 * FIXED_TIMESTEP);
	//	//ammo[1].incrementXPos(-2.0 * elapsed);
	//}
	//if (keys[SDL_SCANCODE_RIGHT]) {
	//	//player.incrementXPos(2.0 * elapsed);
	//	player[0].incrementXPos(1.5 * player[0].direction.x *  player[0].velocity.x * elapsed);
	//	shots[shotIndex].incrementXPos(1.5 * player[0].direction.x * player[0].velocity.x *  elapsed);
	//	//player[0].incrementAccelerationX(FIXED_TIMESTEP);
	//	//score += 100;
	//	//ammo[1].incrementXPos(2.0 * elapsed);
	//}
	//if (keys[SDL_SCANCODE_UP]) {
	//	//player.incrementXPos(-2.0 * elapsed);
	//	player[0].incrementYPos(1.5 *player[0].direction.y * player[0].velocity.y * elapsed);
	//	shots[shotIndex].incrementYPos(1.5 *player[0].direction.y * player[0].velocity.y *  elapsed);
	//	//player[0].incrementAccelerationX(-1 * FIXED_TIMESTEP);
	//	//ammo[1].incrementXPos(-2.0 * elapsed);
	//}
	//if (keys[SDL_SCANCODE_DOWN]) {
	//	//player.incrementXPos(2.0 * elapsed);
	//	player[0].incrementYPos(-1.5 *player[0].direction.y *  player[0].velocity.y * elapsed);
	//	shots[shotIndex].incrementYPos(-1.5 * player[0].direction.y * player[0].velocity.y *  elapsed);
	//	//player[0].incrementAccelerationX(FIXED_TIMESTEP);
	//	//score += 100;
	//	//ammo[1].incrementXPos(2.0 * elapsed);
	//}
	//if (keys[SDL_SCANCODE_A]) {
	//	player[1].incrementXPos(-2.0 * player[1].velocity.x * elapsed);
	//}
	//if (keys[SDL_SCANCODE_D]) {
	//	player[1].incrementXPos(2.0 * player[1].velocity.x * elapsed);
	//}
	//if (keys[SDL_SCANCODE_S]) {
	//	player[1].incrementYPos(-2.0 * player[1].velocity.y * elapsed);
	//}
	//if (keys[SDL_SCANCODE_W]) {
	//	player[1].incrementYPos(2.0 * player[1].velocity.y * elapsed);
	//}



}

void SpacialArea::endGameEvents(SDL_Event event) {
	
	

		//if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) { //Toss this entire if statement into processInput function
		//	done = true;

		//}
	//if (event.type == SDL_KEYDOWN) {
	//	if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) { //RESET
	//		player[1].alive = true;
	//		numEnemies = 1;
	//		state = 1;

	//		//player[1].YPos = 1.5;
	//		//SpacialArea();
	//	}
	//}
		//	//shots[0].incrementYPos(4.0 * elapsed);

		//}


	

}

void SpacialArea::updateThings(ShaderProgram& program, float elasped) {
	switch (state) {
	case STATE_TITLE:
		titleEvents(events, program);
		break;
	case STATE_GAME_LEVEL:
		inGameEvents(events, program, elasped);
		break;
	case STATE_GAME_OVER:
		endGameEvents(events);
		break;
	case STATE_VICTORY:
		endGameEvents(events);
		break;


	}


}

void SpacialArea::shoot(AstralEntity& player) {
	//shots[0].setMatrices(program);
	////shots[0].YPos = -1.5;
	//sprite.Draw(program);
	//shots[0].incrementYPos(7.5 * elapsed);
	//if (shotfired) {
		if (player.position.y < 0) {
			player.shots[shotIndex].position.y = player.position.y + 0.5;
		}
		else {
			player.shots[shotIndex].position.y = player.position.y - 0.5;
		}
		player.shots[shotIndex].position.x = player.position.x;

		player.shots[shotIndex].visible = true;
		//player.shots[player.ammoIndex].incrementYPos(8.0 * player.shots[player.ammoIndex].direction.y * elapsed);

		if (shots[shotIndex].position.y > 2.0) {
			shots[shotIndex].position.y = -1.5;
		}
		shotIndex++;


		if (shotIndex > maxshots - 1) {
			shotIndex = 0;

		}
	//}
	

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
bool SpacialArea::raySegmentIntersect(const Vector &rayOrigin, const Vector &rayDirection, const
	Vector &linePt1, const Vector &linePt2, float &dist)
{
	Vector seg1 = linePt1;
	Vector segD;
	segD.x = linePt2.x - seg1.x;
	segD.y = linePt2.y - seg1.y;

	float raySlope = rayDirection.y / rayDirection.x;
	float n = ((seg1.x - rayOrigin.x)*raySlope + (rayOrigin.y - seg1.y)) / (segD.y -
		segD.x*raySlope);

	if (n < 0 || n > 1)
		return false;

	float m = (seg1.x + segD.x * n - rayOrigin.x) / rayDirection.x;
	if (m < 0)
		return false;

	dist = m;
	return true;
}
bool SpacialArea::inputProcessor(ShaderProgram& program,float elapsed) {
	/*time_t timer;
	if (time(&timer) % 5 == 0) {
	score += 1;
	}*/
	time_t refireTimerOne, refireTimerTwo;
	while (SDL_PollEvent(&events)) {

		//done = environment.windowCloseChecker(event, ships, ammos, spriteSheets, program, fixedElapsed);

		if (events.type == SDL_QUIT || events.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
		if (events.type == SDL_KEYDOWN) {
			if (events.key.keysym.scancode == SDL_SCANCODE_P) { //RESET
				player[1].alive = true;
				numEnemies = 1;
				state = 1;

				//player[1].YPos = 1.5;
				//SpacialArea();
			}
			else if (events.key.keysym.scancode == SDL_SCANCODE_LEFT) {
				if (state == 4) {
					Mix_PlayChannel(-1, select, 0);
					playerIndexOne--;
					if (playerIndexOne < 0)
						playerIndexOne = 3;
					else if (playerIndexOne == 1)
						playerIndexOne = 0;
					
				}
			}
			else if (events.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				if (state == 4) {
					Mix_PlayChannel(-1, select, 0);
					playerIndexOne++;
					if (playerIndexOne > 3)
						playerIndexOne = 0;
					else if (playerIndexOne == 1)
						playerIndexOne = 2;
				}
			}
			else if (events.key.keysym.scancode == SDL_SCANCODE_A) {
				if (state == 4) {
					Mix_PlayChannel(-1, select, 0);
					playerIndexTwo--;
					if (playerIndexTwo < 0)
						playerIndexTwo = 3;
					else if (playerIndexTwo == 1)
						playerIndexTwo = 0;

				}
			}
			else if (events.key.keysym.scancode == SDL_SCANCODE_D) {
				if (state == 4) {
					Mix_PlayChannel(-1, select, 0);
					playerIndexTwo++;
					if (playerIndexTwo > 3)
						playerIndexTwo = 0;
					else if (playerIndexTwo == 1)
						playerIndexTwo = 2;
				}
			}
			else if (events.key.keysym.scancode == SDL_SCANCODE_SPACE) {

				//if (keys[SDL_SCANCODE_SPACE]) { //Hold to shoot. FULL AUTO
				//player[0].shots[player[0].ammoIndex].position.x = player[0].position.x;
				//shoot(program, FIXED_TIMESTEP);
				//player[0].shots[player[0].ammoIndex].position.y = player[0].position.y;
				//player[0].shots[player[0].ammoIndex].position.x = player[0].position.x;
				if (time(&refireTimerOne) % 2 == 0) {
					Mix_PlayChannel(-1, laserShot, 0);
					shoot(player[0]);
				}
			}
			else if (events.key.keysym.scancode == SDL_SCANCODE_R) {
				//if (keys[SDL_SCANCODE_SPACE]) { //Hold to shoot. FULL AUTO
				//player[0].shots[player[0].ammoIndex].position.x = player[0].position.x;
				//shoot(program, FIXED_TIMESTEP);
				//player[0].shots[player[0].ammoIndex].position.y = player[0].position.y;
				//player[0].shots[player[0].ammoIndex].position.x = player[0].position.x;
				if (time(&refireTimerOne) % 2 == 0) {
					Mix_PlayChannel(-1, laserShot, 0);
					shoot(player[1]);
				}
			}
			else if (events.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				if (state == 0) {
					
					state = 4;
				}
				else if (state == 4) {
					Mix_PlayChannel(-1, start, 0);
					player[0].health = 100;
					player[0].shields = 100;
					player[1].health = 100;
					player[1].shields = 100;
					state = 1;

				}
				else if (state == 2) {
					player[0].health = 100;
					player[0].shields = 100;
					player[0].position.x = 2.0;
					player[0].position.y = -2.0;
					player[1].health = 100;
					player[1].shields = 100;
					player[1].position.x = -2.0;
					player[1].position.y = 2.0;
					state = 1;
				}
			}
			//shots[0].incrementYPos(4.0 * elapsed);
			//else if (events.type == SDL_JOYAXISMOTION) {
			//	//shoot(player[0]);
			//	if (events.jaxis.which == 0) {
			//		if (events.jaxis.axis == 0) {
			//			player[0].incrementXPos(2.0 * player[0].direction.x * player[0].velocity.x * elapsed);
			//			if (events.jaxis.value < 0)
			//				player[0].direction.x = -1.0;
			//			else if (events.jaxis.value > 0)
			//				player[0].direction.x = 1.0;
			//		}
			//		else if (events.jaxis.axis == 1) {

			//			player[0].incrementYPos(2.0 * player[0].direction.y * player[0].velocity.y * elapsed);
			//			if (events.jaxis.value < 0)
			//				player[0].direction.y = -1.0;
			//			else if (events.jaxis.value > 0)
			//				player[0].direction.y = 1.0;

			//		}
			//	}
			//	
			//}
			//if (events.type = SDL_JOYBUTTONDOWN) {
			//	//cout << SDL_JoystickNameForIndex(0) << endl;
			//	//cout << events.jbutton.button << endl;
			//	shoot(player[0]);

			//}
			
		}
		else if (events.type == SDL_JOYAXISMOTION) {


			if (events.jaxis.which == 0) {
				if (events.jaxis.axis == 0)
				{
					if (events.jaxis.value < -JOYSTICK_DEADZONE)
						/* Left-right movement code goes here */
						//player[0].moveMatrix(events.jaxis.value * player[0].position.x, 0.0, 0.0);
						player[0].incrementXPos(-3.0 * elapsed);
					else if (events.jaxis.value > JOYSTICK_DEADZONE)
						player[0].incrementXPos(3.0 * elapsed);
				}

				else if (events.jaxis.axis == 1)
				{
					if (events.jaxis.value < -JOYSTICK_DEADZONE)
						/* Left-right movement code goes here */
						//player[0].moveMatrix(events.jaxis.value * player[0].position.x, 0.0, 0.0);
						player[0].incrementYPos(3.0 * elapsed);
					else if (events.jaxis.value > JOYSTICK_DEADZONE)
						player[0].incrementYPos(-3.0 * elapsed);
				}

			}
			if (events.jaxis.which == 1) {
				if (events.jaxis.axis == 0)
				{
					if (events.jaxis.value < -JOYSTICK_DEADZONE)
						/* Left-right movement code goes here */
						//player[0].moveMatrix(events.jaxis.value * player[0].position.x, 0.0, 0.0);
						player[1].incrementXPos(-3.0 * elapsed);
					else if (events.jaxis.value > JOYSTICK_DEADZONE)
						player[1].incrementXPos(3.0 * elapsed);
				}

				else if (events.jaxis.axis == 1)
				{
					if (events.jaxis.value < -JOYSTICK_DEADZONE)
						/* Left-right movement code goes here */
						//player[0].moveMatrix(events.jaxis.value * player[0].position.x, 0.0, 0.0);
						player[1].incrementYPos(3.0 * elapsed);
					else if (events.jaxis.value > JOYSTICK_DEADZONE)
						player[1].incrementYPos(-3.0 * elapsed);
				}

			}

			
		}
		else if (events.type == SDL_JOYBUTTONDOWN) {
			if (events.jbutton.which == 0)
			{
				/* code goes here */
				//Mapping FOR PS4 Controller (and third party PS3 controllers) ONLY
				//Button 0 is SQUARE, 1 is X, 2 is CIRCLE, 3 is TRANGLE, 4 IS L1, 5 is R1, 6 is L2, 7 is R2, 
				//8 is SHARE button, 9 is OPTIONS Button, 10 is L3, 11 IS R3, 12 is PS Button, 13 is touchpad
				if (events.jbutton.button == 5) 
					if (time(&refireTimerOne) % 2 == 0) {
						Mix_PlayChannel(-1, laserShot, 0);
						shoot(player[0]);
					}
			}
			if (events.jbutton.which == 1)
			{
				/* code goes here */
				//Mapping for FOR PS4 Controller ONLY
				//Button 0 is SQUARE, 1 is X, 2 is CIRCLE, 3 is TRANGLE, 4 IS L1, 5 is R1, 6 is L2, 7 is R2, 
				//8 is SHARE button, 9 is OPTIONS Button, 10 is L3, 11 IS R3, 12 is PS Button, 13 is touchpad
				//For PS3 Controller, Button 9 IS L1
				if (events.jbutton.button == 9)
					if (time(&refireTimerOne) % 2 == 0) {
						Mix_PlayChannel(-1, laserShot, 0);
						shoot(player[1]);
					}
			}
		}
		//if (events.type == SDL_KEYDOWN) {
			//if (events.key.keysym.scancode == SDL_SCANCODE_SPACE) {
			//	//if (keys[SDL_SCANCODE_SPACE]) { //Hold to shoot. FULL AUTO
			//	//shots[shotIndex].position.x = player[0].position.x;
			//	shoot(program, FIXED_TIMESTEP);
			//}
		//}

	}
	if (keys[SDL_SCANCODE_B]) {
		state = 2;
		//player.XPos = 0.0;
		//objects[0].XPos = 0.0;
		player[0].position.x = 0.0;
	}
	if (keys[SDL_SCANCODE_LEFT]) {
		//player.incrementXPos(-2.0 * elapsed);
		player[0].incrementXPos(-2.0 * player[0].direction.x * player[0].velocity.x * elapsed);
		//player[0].shots[player[0].ammoIndex].incrementXPos(-2.0 * player[0].direction.x * player[0].velocity.x * elapsed);
		//shots[shotIndex].incrementXPos(-1.5 * player[0].direction.x * player[0].velocity.x *  elapsed);
		//player[0].incrementAccelerationX(-1 * FIXED_TIMESTEP);
		//ammo[1].incrementXPos(-2.0 * elapsed);
	}

	if (keys[SDL_SCANCODE_RIGHT]) {
		//player.incrementXPos(2.0 * elapsed);
		player[0].incrementXPos(2.0 * player[0].direction.x *  player[0].velocity.x * elapsed);
		//player[0].shots[player[0].ammoIndex].incrementXPos(2.0 * player[0].direction.x * player[0].velocity.x * elapsed);
		//shots[shotIndex].incrementXPos(1.5 * player[0].direction.x * player[0].velocity.x *  elapsed);
		//player[0].incrementAccelerationX(FIXED_TIMESTEP);
		//score += 100;
		//ammo[1].incrementXPos(2.0 * elapsed);
	}
	if (keys[SDL_SCANCODE_UP]) {
		//player.incrementXPos(-2.0 * elapsed);
		player[0].incrementYPos(2.0 *player[0].direction.y * player[0].velocity.y * elapsed);
		//shots[shotIndex].incrementYPos(1.5 *player[0].direction.y * player[0].velocity.y *  elapsed);
		//player[0].incrementAccelerationX(-1 * FIXED_TIMESTEP);
		//ammo[1].incrementXPos(-2.0 * elapsed);
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		//player.incrementXPos(2.0 * elapsed);
		player[0].incrementYPos(-2.0 *player[0].direction.y *  player[0].velocity.y * elapsed);
		//shots[shotIndex].incrementYPos(-1.5 * player[0].direction.y * player[0].velocity.y *  elapsed);
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

	return false;
}
bool SpacialArea::collisionDetectionX(float x, float y) {
	//int gridX, gridY;
	//worldToTileCoordinates(x, y, &gridX, &gridY);
	////float penetration;

	//if (0 > gridY > mapHeight - 1) {
	//	penetrationY = 0.0;
	//}

	/*else if (solid[gridY][gridX]) {
	penetrationY = -1 * y - gridY * tileHeight;
	}*/
	//else if (y > -(TILE_SIZE * tileHeight) ){
	////if (solid[gridY][gridX]) {
	//		penetrationY = -1 * y - ( gridY * tileHeight);
	////}
	//}
	//else if (y < (-TILE_SIZE * tileHeight) - TILE_SIZE) {
	//	//if (solid[gridY][gridX]) {
	//		penetrationY = -1 * y - (gridY * tileHeight);
	//	//}
	//}
	//else {
	//	penetrationY = 0.0;
	//}



	return false;
}
bool SpacialArea::collisionDetectionY(float x, float y) {
	//int gridX, gridY;
	//worldToTileCoordinates(x, y, &gridX, &gridY);
	////float penetration;

	//if (0 > gridY > mapHeight - 1) {
	//	penetrationY = 0.0;
	//}
	
	/*else if (solid[gridY][gridX]) {
		penetrationY = -1 * y - gridY * tileHeight;
	}*/
	//else if (y > -(TILE_SIZE * tileHeight) ){
	////if (solid[gridY][gridX]) {
	//		penetrationY = -1 * y - ( gridY * tileHeight);
	////}
	//}
	//else if (y < (-TILE_SIZE * tileHeight) - TILE_SIZE) {
	//	//if (solid[gridY][gridX]) {
	//		penetrationY = -1 * y - (gridY * tileHeight);
	//	//}
	//}
	//else {
	//	penetrationY = 0.0;
	//}


	
	return false;
}

void SpacialArea::collisionHandler(Projectile& bullets,AstralEntity& player) {
	float penetrationXL, penetrationXR, penetrationYT, penetrationYB; //left, right, top, bottom

	penetrationXL = collisionDetectionX(player.XPos - ( 0.5  * player.width), player.YPos);
	penetrationXR = collisionDetectionX(player.XPos + (0.5  * player.width), player.YPos);
	penetrationYT = collisionDetectionY(player.XPos, player.YPos + (0.5 * player.height));
	penetrationYB = collisionDetectionY(player.XPos, player.YPos - (0.5 * player.height));


	//if (penetrationXL > 0.0) {
	//	player.incrementXPos(0.2 * penetrationXL);
	//	//player.velocity = 0;
	//	player.collideLeft = true;
	//}
	//else if (penetrationXR > 0.0) {
	//	player.incrementXPos(0.2 * ( penetrationXR - tileLength));
	//	//player.velocity = 0;
	//	player.collideRight = true;
	//}
	//else if (penetrationYT > 0.0) {
	//	player.incrementYPos(0.2 * (penetrationYT));
	//	//player.velocity = 0;
	//	player.collideTop = true;
	//}
	//else if (penetrationYB > 0.0) {
	//	player.incrementYPos(0.2 * (penetrationYB - tileHeight));
	//	//player.velocity = 0;
	//	player.collideBottom = true;
	//}
}

void SpacialArea::readFile(const char* levelFile, ShaderProgram& program) {
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
void SpacialArea::placeEntity(std::string type, float x, float y) {
	if (type == "Start") {
		player[0].position.x = x;
		player[0].position.y = y;
		//player. = x;
		//player.YPos = y;
	}
	//player.setMatrices(*program0);

}
bool SpacialArea::readEntityData(std::ifstream &stream, ShaderProgram& program) {
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



bool SpacialArea::readLayerData(std::ifstream &stream) {
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



bool SpacialArea::readHeader(std::ifstream &stream) {
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
		//solid = new bool*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
			//solid[i] = new bool[mapWidth];
		}
		return true;
	}
}
void SpacialArea::render(ShaderProgram& program) {

	/*for (int i = 0; i < 7; i++) {
	playerAnimation[i].textureID = animation;
	}*/

	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			if (levelData[y][x] != 0) {

				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;

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

	//player.setupAndRender(program, vertexData.data(), texCoordData.data(), texture);
	//worldToTileCoordinates(LEVEL_WIDTH, LEVEL_HEIGHT, &gridX, &gridY);


}
void SpacialArea::setupAndRender(ShaderProgram& program, float vertices[], float texCoords[], GLuint& texture) {
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
