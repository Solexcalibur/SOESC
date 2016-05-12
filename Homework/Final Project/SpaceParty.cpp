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
#define JOYSTICK_DEADZONE 5000
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
	//joystickPlayer0 = SDL_JoystickGetAxis(nullptr, 0);
	playerOne = nullptr;
	playerTwo = nullptr;
	for (int i = 0; i < 5; i++) {
		player.push_back(AstralEntity());
		text.push_back(AstralEntity());
		levels.push_back(AstralEntity());
	}
	for (int i = 0; i < 10; i++) {
		shots.push_back(Projectile());
	}
	for (int i = 0; i < 2; i++) {
		party.push_back(ParticleEmitter(10));
	}
	//wordTexture = LoadTexture(fontSheetPath);
	//spriteSheetTexture = LoadTexture(spritepath);
	r_filter = 0.0;
	g_filter = 0.0;
	b_filter = 0.0;
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
	player[0].width = sprites[2].width;
	player[0].height = sprites[2].height;
	//<SubTexture name="shield1.png" x="0" y="412" width="133" height="108"/>
	player[0].shots[shotIndex].width = sprites[1].width;
	player[0].shots[shotIndex].height = sprites[1].height;
	player[0].shots[shotIndex].position.y = player[0].position.y;
	player[0].shots[shotIndex].position.x = player[0].position.x;
	player[0].shots[shotIndex].direction.x = player[0].direction.x;
	player[0].shots[shotIndex].direction.y = 1.0;
	player[1].shots[shotIndex].width = sprites[1].width;
	player[1].shots[shotIndex].height = sprites[1].height;
	player[1].shots[shotIndex].position.y = player[1].position.y;
	player[1].shots[shotIndex].position.x = player[1].position.x;
	player[1].shots[shotIndex].direction.x = player[1].direction.x;
	player[1].shots[shotIndex].direction.y = 1.0;
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
	animationTime = 0;
	levelIndex = 0;
	previousState = 0;
	joystickPlayer0x = 0;
	joystickPlayer0y = 0;
	joystickPlayer1x = 0;
	joystickPlayer1y = 0;
	animationValue = 0;
	
}

SpacialArea::~SpacialArea()
{
	SDL_JoystickClose(playerOne);
	playerOne = nullptr;
	SDL_JoystickClose(playerTwo);
	playerTwo = nullptr;
	Mix_FreeChunk(laserShot);
	Mix_FreeChunk(select);
	Mix_FreeChunk(scored);
	Mix_FreeChunk(gameover);
	Mix_FreeChunk(victory);
	Mix_FreeChunk(start);
	Mix_FreeMusic(mysteriousSound);
	SDL_Quit();
}


void SpacialArea::setup() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
	int x_resolution = 1280;
	int y_resolution = 800;
	displayWindow = SDL_CreateWindow("Astral Horizon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_resolution, y_resolution, SDL_WINDOW_OPENGL);// <-OUTER BOUND
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	Mix_OpenAudio(34100, MIX_DEFAULT_FORMAT, 2, 4096);
	SDL_SetWindowFullscreen(displayWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_ShowCursor(0);
	SDL_JoystickEventState(SDL_ENABLE);
	playerOne = SDL_JoystickOpen(0);
	//SDL_JoystickEventState(SDL_ENABLE);
	playerTwo = SDL_JoystickOpen(1);
	metalTex = LoadTexture("MetalSheet.png");
	//scored = Mix_LoadWAV("Score.ogg");
	//party.texture = LoadTexture("fire.png");
	purple = LoadTexture("uipackSpace_sheet.png");
	//levels[0].texID = purple;
	blue = LoadTexture("blueSheet.png");
	//levels[1].texID = blue;
	particle.party = ParticleEmitter(2);
	wordTexture = LoadTexture("font2.png");
	particletex = LoadTexture("fire.png");
	party[0].texture = particletex;
	particle.texID = LoadTexture("fire.png");
	randTextures.push_back(metalTex);
	randTextures.push_back(particletex);
	randTextures.push_back(wordTexture);
	randTextures.push_back(purple);
	randTextures.push_back(blue);
	joystickPlayer0x = SDL_JoystickGetAxis(playerOne, 0);
	joystickPlayer0y = SDL_JoystickGetAxis(playerOne, 1);
	joystickPlayer1x = SDL_JoystickGetAxis(playerTwo, 0);
	joystickPlayer1y = SDL_JoystickGetAxis(playerTwo, 1);
	int hat = SDL_JoystickNumHats(playerOne);
	hatz = SDL_JoystickGetHat(playerOne, 0);
	
	for (int i = 0; i < 5; i++) {
		text[i].setOrthoProjection();
		text[i].texID = wordTexture;
	}
		for (int i = 0; i < 30; i++) {
			metaltiles.push_back(SpriteSheet(randTextures[levelIndex], 0.0f / 512.0f, 280.0f / 512.0f, 70.0f / 512.0f, 70.0f / 512.0f, 0.2));
			randomTiles.push_back(AstralEntity());
			randomTiles[i].position = Vector(rand() % 6 - 3.0, rand() % 5 - 1.5, 0.0);
			randomTiles[i].texID = randTextures[levelIndex];
			//randomTiles[i].position.x = rand() % 3;
			//randomTiles[i].position.y = rand() % 3;
			randomTiles[i].setOrthoProjection();
		}
		
		metaltiles[1].width = 20.0f / 256.0f;
		metaltiles[1].height = 20.0f / 256.0f;
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
			
	//party.SetTex("fire.png");
	//readFile("Metal Sheet Tiles.txt");
	spriteSheetTexture = LoadTexture("SpaceStuff.png");
	laserShot = Mix_LoadWAV("Laser_Shoot26.wav");
	select = Mix_LoadWAV("Blip_Select.wav");
	scored = Mix_LoadWAV("Score.ogg");
	gameover = Mix_LoadWAV("Game Over.ogg");
	victory = Mix_LoadWAV("Victory.ogg");
	start = Mix_LoadWAV("Skyjack.ogg");
	mysteriousSound = Mix_LoadMUS("UT4 Music Concept - Atrium.mp3");
	
	Mix_PlayMusic(mysteriousSound, -1);

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
	glClearColor(r_filter + 0.2, g_filter, b_filter, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SpacialArea::windowSwapping() {
	SDL_GL_SwapWindow(displayWindow);
}



void SpacialArea::DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing, float x, float y) {
	float texture_size = 1.0 / 16.0f;     std::vector<float> vertexData;     std::vector<float> texCoordData;
	
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(),
		{ ((size + spacing) * i) + (-0.5f * size) + x, 0.5f * size + y,
			((size + spacing) * i) + (-0.5f * size) + x, -0.5f * size + y,
			((size + spacing) * i) + (0.5f * size) + x, 0.5f * size + y,
			((size + spacing) * i) + (0.5f * size) + x, -0.5f * size + y,
			((size + spacing) * i) + (0.5f * size) + x, 0.5f * size + y,
			((size + spacing) * i) + (-0.5f * size) + x, -0.5f * size + y, });

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


void SpacialArea::TitleScreen(ShaderProgram& program, float elapsed) {
	
	DrawText(program, wordTexture, "ASTRAL HORIZON", 0.1, 0, -2.0, 0.0);
	DrawText(program, wordTexture, "FIGHT, KILL, SURVIVE!", 0.1, 0, -2.0, -1.0);
	
	animationTime += elapsed;
	animationValue = mapValue(animationTime, 0.0, 2.0, 0.0, 1.0);
	sprites[3].textureID = spriteSheetTexture;
	player[2].setMatrices(program);
	player[2].setOrthoProjection();
	player[2].identityMatrix();
	player[2].moveMatrix(easeOutElastic(0.0,2.5, animationValue), easeIn(0.0, 2.5, animationValue), 0.0);
	//easeIn(0.0, 2.0, animationValue)
	//player[2].model.Rotate()
	//easeIn(0.0, -2.0, animationValue)
	//spriteSheets[0].Draw(program);
	sprites[3].Draw(program);
	sprites[2].textureID = spriteSheetTexture;
	player[3].setMatrices(program);
	player[3].setOrthoProjection();
	player[3].identityMatrix();
	player[3].moveMatrix(easeOutElastic(0.0, -2.5, animationValue), easeIn(0.0, -2.5, animationValue), 0.0);

	//spriteSheets[0].Draw(program);
	sprites[2].Draw(program);
	blendSprite(particletex);

}

void SpacialArea::scoreBoard(ShaderProgram& program) {
	DrawText(program, wordTexture, "P2 HEALTH:" + to_string(player[1].health) + " SHIELDS:" + to_string(player[1].shields) + " WINS:" +
		to_string(player[1].wins), 0.12, 0, -2.8, 0.0);
	DrawText(program, wordTexture, "P1 HEALTH:" + to_string(player[0].health) + " SHIELDS:" + to_string(player[0].shields) + " WINS:" +
		to_string(player[0].wins), 0.12, 0, -2.8, -5.0);
	blendSprite(wordTexture); //Blend Particle Sprite
	

}

void SpacialArea::gameOverScreen(ShaderProgram& program) {
	
	if (player[0].health < 0 ) {
		DrawText(program, wordTexture, "Player 2 Wins the Round!", 0.11, 0, -2.0, 0.0);
		
	}
	else if (player[1].health < 0) {
		DrawText(program, wordTexture, "Player 1 Wins the Round!", 0.11, 0, -2.0, 0.0);
		
	}
	else if (player[0].health < 0 && player[1].health < 0) {
		DrawText(program, wordTexture, "Mutually Assured Death!", 0.11, 0, -2.0, 0.0);
	}
	DrawText(program, wordTexture, "Press Enter or X to play again", 0.1, 0, -2.0, -2.0);
	blendSprite(wordTexture);

}

void SpacialArea::VictoryScreen(ShaderProgram& program) {
	if (player[0].wins == 2) {
		DrawText(program, wordTexture, "Player 1 wins the match!", 0.1, 0, 0.0, 0.0);
		
	}
	else {
		DrawText(program, wordTexture, "Player 2 wins the match!", 0.1, 0, 0.0, 0.0);
		
	}
	

}

void SpacialArea::screenSelector(ShaderProgram& program, float elapsed) {

	switch (state) {
	case STATE_TITLE:
		TitleScreen(program, elapsed);
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
	case STATE_LEVEL_SELECT:
		SelectLevel(program);
		break;
	case STATE_PAUSE_GAME:
		PausedGame(program);
		break;
	}
}
void SpacialArea::PausedGame(ShaderProgram& program) {
	DrawText(program, wordTexture, "Press ESC to return to game", 0.1, 0, -2.0, 0.0);
	DrawText(program, wordTexture, "Press Q to quit game", 0.1, 0, -2.0, -1.0);
}
void SpacialArea::SelectLevel(ShaderProgram& program) {
	DrawText(program, wordTexture, "Choose a Level", 0.077, 0, -1.0, 0.0);
	float* vertices = randomTiles[0].grabVertices();
	float* texCoords = randomTiles[0].grabTexCoords();
	for (int i = 0; i < 28; i++) {
		randomTiles[i].setMatrices(program);
		randomTiles[i].identityMatrix();
		randomTiles[i].moveMatrix(randomTiles[i].position.x - 1.0, randomTiles[i].position.y - 0.5, 0.0);
		//randomTiles[i].texID = randTextures[levelIndex];
		//randomTiles[i].setupAndRender(program, vertices, texCoords, randomTiles[i].texID);
		metaltiles[i].textureID = randTextures[levelIndex];
		metaltiles[i].Draw(program);

		//entites[i].XPos = rand() % 3;
		//entites[i].YPos = rand() % 3;
		//entites[i].XPos = rand() % 6;
		//entites[i].YPos = (int)entites[i].XPos % 6;

		//entites[i].setOrthoProjection();

	}

}
void SpacialArea::SelectCharacter(ShaderProgram& program) {
	//particle.party.SetTex(particletex);
	//particle.party.Render(&program);
	DrawText(program, wordTexture, "Select your ship", 0.097, 0, 0.0, 0.0);
	DrawText(program, wordTexture, "Note: Choosing a differently colored ship will not provide any advantage", 0.077, 0, -2.7, -1.0);
	DrawText(program, wordTexture, "Choosing the same ship may cause confusion among players", 0.077, 0, -2.7, -2.0);
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
	/*party[0].texture = metalTex;
	party[0].Render(&program);
	
	party[0].Update(FIXED_TIMESTEP);*/
	//party[0].EmitXDirection(2, false);
	for (int i = 0; i < 28; i++) {
		randomTiles[i].setMatrices(program);
		randomTiles[i].identityMatrix();
		randomTiles[i].moveMatrix(randomTiles[i].position.x - 2, randomTiles[i].position.y - 0.5, 0.0);
		

		metaltiles[i].Draw(program);
	

	}
	
	
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

	sprites[playerIndexTwo].Draw(program);

	sprites[laserIndexTwo].textureID = spriteSheetTexture;
	player[1].shots[player[1].ammoIndex].setOrthoProjection();
	player[1].shots[player[1].ammoIndex].setMatrices(program);
	player[1].shots[player[1].ammoIndex].identityMatrix();
	sprites[laserIndexTwo].Draw(program);
	
	

}
void SpacialArea::inGameEvents(SDL_Event event, ShaderProgram& program, float elapsed) {
	
	setupThings(program);
	
	//render(program);
	//particle.party.Update(elapsed);
	time_t mover;
	animationTime += elapsed;
	animationValue = mapValue(animationTime, 0.0, 2.0, 0.0, 1.0);
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	if (state == 1) {
		
		//}
		//for (int i = 0; i < 28; i += 9) {
		//	//randomTiles[0].identityMatrix();
		//	//randomTiles[0].incrementXPos(0.1 * randomTiles[0].lerp(0.0, 1.0, animationValue));
		//	//if (time(&mover) > 15)
		//	//if (ticks > 2)
		//	//if (time(&mover) % 20 == 0)
		//	randomTiles[i].moveMatrix(easeIn(0.0, -1.0, animationValue), -1.0 * easeOutElastic(0.0, 1.0, animationValue), 0.0);
		//}
		//for (int i = 0; i < 27; i++) {
		//	//if (time(&mover) > 10)
		//	//if (time(&mover) % 6 == 0)
		//	//if (time(&mover) % 10 == 0)
		//	randomTiles[i].moveMatrix(0.0, -1.0 * randomTiles[i].lerp(0.0, 2.0, animationValue), 0.0);
		//}
	}

	
	int ammoIndex = 0;
	

	if (shotIndex > maxshots - 1) {
		shotIndex = 0;

	}
	
	
	player[0].shots[player[0].ammoIndex].moveMatrix(player[0].shots[player[0].ammoIndex].position.x, 
		player[0].shots[player[0].ammoIndex].position.y, 0.0);
	player[0].shots[player[0].ammoIndex].incrementYPos(8.0 * player[0].shots[player[0].ammoIndex].direction.y * elapsed);



	player[1].shots[player[1].ammoIndex].moveMatrix(player[1].shots[player[1].ammoIndex].position.x, 
		player[1].shots[player[1].ammoIndex].position.y, 0.0);
	//player[1].shots[player[1].ammoIndex].direction.y = -1.0;
	player[1].shots[player[1].ammoIndex].incrementYPos(8.0 * player[1].shots[player[1].ammoIndex].direction.y * elapsed);
	


	
	player[0].model.Scale(1.0, player[0].scaleFactor, 1.0);
	player[1].shots[player[1].ammoIndex].model.Rotate(180.0 * (3.1415926 / 180.0));
	player[1].model.Scale(1.0, player[1].scaleFactor, 1.0);
	
		if (player[1].position.x > 2.7) {
			player[1].position.x = 2.7;
			
		}
		if (player[1].position.x < -2.7) {
			player[1].position.x = -2.7;
			
		}
		if (player[0].position.x > 2.7) {
			player[0].position.x = 2.7;
			
		}
		if (player[0].position.x < -2.7) {
			player[0].position.x = -2.7;
			
		}
		
	
	if (player[0].position.y < -2.5) {
		player[0].position.y = -2.5;
	}
	if (player[0].position.y > 2.5) {
		player[0].position.y = 2.5;
	}
	else if (player[0].position.y > 0.0) {
		
		player[0].scaleFactor = -1.0;
		player[0].shots[player[0].ammoIndex].model.Rotate(180.0 * (3.1415926 / 180.0));
		player[0].shots[player[0].ammoIndex].direction.y = -1.0;
	}
	else if (player[0].position.y < 0.0) {
		for (int i = 0; i < 28; i += 3) {
			randomTiles[i].moveMatrix(easeOutElastic(0.0, 2.5, animationValue), easeIn(0.0, -0.5, animationValue), 0.0);
			//randomTiles[i].incrementYPos(easeOutElastic(0.0, 2.5, animationValue));
		}
		player[0].scaleFactor = 1.0;
		player[0].shots[player[0].ammoIndex].direction.y = 1.0;
	}

	if (player[1].position.y < -2.5) {
		player[1].position.y = -2.5;
	}
	if (player[1].position.y > 2.5) {
		player[1].position.y = 2.5;
	}
	else if (player[1].position.y < 0.0) {
		for (int i = 0; i < 28; i += 9) {
			randomTiles[i].moveMatrix(0.0, easeIn(0.0, -0.5, animationValue), 0.0);
			//randomTiles[i].incrementYPos(easeOutElastic(0.0, 2.5, animationValue));
		}
		player[1].scaleFactor = 1.0;
		player[1].shots[player[1].ammoIndex].model.Rotate(180.0 * (3.1415926 / 180.0));
		player[1].shots[player[1].ammoIndex].direction.y = 1.0;
	}
	else if (player[1].position.y > 0.0) {
		
		player[1].scaleFactor = -1.0;
		player[1].shots[player[1].ammoIndex].direction.y = -1.0;
	}
	float ydist = abs(player[0].position.y - player[1].position.y);
	float xdist = 20;

	if (player[0].shots[player[0].ammoIndex].position.y > 2.4 || player[0].shots[player[0].ammoIndex].position.y < - 2.4) {
		player[0].shots[player[0].ammoIndex].visible = false;
		player[0].shots[player[0].ammoIndex].scaleMatrix(0.0, 0.0, 0.0);
	}
	if (player[1].shots[player[1].ammoIndex].position.y > 2.4 || player[1].shots[player[1].ammoIndex].position.y < -2.4) {
		player[1].shots[player[1].ammoIndex].visible = false;
		player[1].shots[player[1].ammoIndex].scaleMatrix(0.0, 0.0, 0.0);
	}
	
	//for (int i = 0; i < 2; i++) { //Bullet collision with enemy. Incresed hitbox so that hitting any part of ship deals damage
		if (player[0].shots[player[0].ammoIndex].position.y < player[1].position.y + player[1].height * 3.45
			&& player[0].shots[player[0].ammoIndex].position.y > player[1].position.y - player[1].height * 1.5
			&& player[0].shots[player[0].ammoIndex].position.x < player[1].position.x + player[1].width * 3.45
			&& player[0].shots[player[0].ammoIndex].position.x > player[1].position.x - player[1].width * 1.5) {
			if (player[0].shots[player[0].ammoIndex].visible) {

				player[0].shots[player[0].ammoIndex].position.x = 20.5;
				player[0].shots[player[0].ammoIndex].direction.y = 1.0;//still gotta remove bullet, just a temp fix
				//player[0].shots[player[0].ammoIndex].visible = false;
				if (player[1].shields > 0) {
					player[1].shields -= 0.8 * player[0].shots[player[0].ammoIndex].damage;
					player[1].health -= 0.2 * player[0].shots[player[0].ammoIndex].damage;
				}
				else {
					
					player[1].shields = 0;
					player[0].shots[player[0].ammoIndex].position.y = 2.5;
					player[1].health -= player[0].shots[player[0].ammoIndex].damage;
					
					
				}
				
			}
		}
		if (player[1].shots[player[1].ammoIndex].position.y < player[0].position.y + player[0].height * 3.45
			&& player[1].shots[player[1].ammoIndex].position.y > player[0].position.y - player[0].height * 1.5
			&& player[1].shots[player[1].ammoIndex].position.x < player[0].position.x + player[0].width * 3.45
			&& player[1].shots[player[1].ammoIndex].position.x > player[0].position.x - player[0].width * 1.5) {
			if (player[1].shots[player[1].ammoIndex].visible) {

				player[1].shots[player[1].ammoIndex].position.x = 20.5;
				player[1].shots[player[1].ammoIndex].direction.y = 1.0;//still gotta remove bullet, just a temp fix
				//player[1].shots[player[1].ammoIndex].visible = false;
				if (player[0].shields > 0) {
					player[0].shields -= 0.8 * player[1].shots[player[1].ammoIndex].damage;
					player[0].health -= 0.2 * player[1].shots[player[1].ammoIndex].damage;
				}
				else {
					player[0].shields = 0;
					player[1].shots[player[0].ammoIndex].position.y = 2.5;
					player[0].health -= player[1].shots[player[1].ammoIndex].damage;


				}
				//score += 5;
			}
		}
		
		if (player[0].health < 0 || player[1].health < 0) {
			state = 2;
			if (player[0].health < 0) {
				player[1].wins++;
			}
			else {
				player[0].wins++;
			}
		}

		if (player[0].wins == 2 || player[1].wins == 2) {
			state = 3;
			Mix_PlayChannel(-1, gameover, 0);
		}



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
		
		if (player.position.y < 0) {
			player.shots[player.ammoIndex].position.y = player.position.y + 0.5;
		}
		else {
			player.shots[player.ammoIndex].position.y = player.position.y - 0.5;
		}
		player.shots[player.ammoIndex].position.x = player.position.x;

		player.shots[player.ammoIndex].visible = true;
		//player.shots[player.ammoIndex].incrementYPos(8.0 * player.shots[player.ammoIndex].direction.y * elapsed);
		
		if (player.shots[player.ammoIndex].position.y > 3.0) {
			player.shots[player.ammoIndex].position.x = -4.5;
		}
		

		player.ammoIndex++;
		if (player.ammoIndex > maxshots - 1) {
			player.ammoIndex = 0;

		}
		//shotIndex++;
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
			else if (events.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				if (state != 6) {
					previousState = state;
					state = 6;
				}
				else  {
					if (events.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						state = previousState;
					}
					
				}
			}
			else if (events.key.keysym.scancode == SDL_SCANCODE_Q) { //Press Q to quit game
				if (state == 6)
				return true;
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
				else if (state == 5) {
					levelIndex--;
					if (levelIndex < 0)
						levelIndex = randTextures.size() - 1;
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
				else if (state == 5) {
					levelIndex++;
					if (levelIndex > randTextures.size() - 1)
						levelIndex = 0;
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
					previousState = 0;
					state = 4;
				}
				else if (state == 5) {
					previousState = 5;
					Mix_PlayChannel(-1, start, 0);
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
				else if (state == 4) {
					previousState = 4;
					state = 5;
					

				}
				else if (state == 2) {
					/*player[0].health = 100;
					player[0].shields = 100;
					player[0].position.x = 2.0;
					player[0].position.y = -2.0;
					player[1].health = 100;
					player[1].shields = 100;
					player[1].position.x = -2.0;
					player[1].position.y = 2.0;*/
					previousState = 2;
					state = 4;
				}
			}
			
			
		}
		

		else if (events.type == SDL_JOYAXISMOTION) {


			/*	if (events.jhat.which == 0) {
					if (events.jhat.value && SDL_HAT_UP) {
						player[0].incrementYPos(9.0 * elapsed);
					}
					if (events.jhat.value &&  SDL_HAT_DOWN) {
						player[0].incrementYPos(-9.0 * elapsed);
					}
					if (events.jhat.value && SDL_HAT_LEFT) {
						player[0].incrementXPos(-9.0 * elapsed);
					}
					if (events.jhat.value && SDL_HAT_RIGHT) {
						player[0].incrementXPos(9.0 * elapsed);
					}*/
			if (events.jaxis.which == 0) {
				if (events.jaxis.axis == 0) {
					if (state == 1) 
						////if (&joystickPlayer0x[events.jaxis.axis] ) {
						if (events.jaxis.value < -JOYSTICK_DEADZONE)

							//		//player[0].moveMatrix(events.jaxis.value * player[0].position.x, 0.0, 0.0);
							player[0].incrementXPos(-0.05 * player[0].acceleration.x);
						else if (events.jaxis.value > JOYSTICK_DEADZONE)
							player[0].incrementXPos(0.05 * player[0].acceleration.x);
					}
					if (events.jaxis.axis == 1) {
						////if (&joystickPlayer0x[events.jaxis.axis] ) {
						if (events.jaxis.value < -JOYSTICK_DEADZONE)

							//		//player[0].moveMatrix(events.jaxis.value * player[0].position.x, 0.0, 0.0);
							player[0].incrementYPos(0.05 * player[0].acceleration.y);
						else if (events.jaxis.value > JOYSTICK_DEADZONE)
							player[0].incrementYPos(-0.05 * player[0].acceleration.y);
					}
				
				
			}
			if (events.jaxis.which == 1) {
				if (events.jaxis.axis == 0) {
					if (state == 1) 
						////if (&joystickPlayer0x[events.jaxis.axis] ) {
						if (events.jaxis.value < -JOYSTICK_DEADZONE)

							//		//player[0].moveMatrix(events.jaxis.value * player[0].position.x, 0.0, 0.0);
							player[1].incrementXPos(-0.05 * player[1].acceleration.x);
						else if (events.jaxis.value > JOYSTICK_DEADZONE)
							player[1].incrementXPos(0.05 * player[1].acceleration.x);
					}
					if (events.jaxis.axis == 1) {
						////if (&joystickPlayer0x[events.jaxis.axis] ) {
						if (events.jaxis.value < -JOYSTICK_DEADZONE)

							//		//player[0].moveMatrix(events.jaxis.value * player[0].position.x, 0.0, 0.0);
							player[1].incrementYPos(0.05 * player[1].acceleration.y);
						else if (events.jaxis.value > JOYSTICK_DEADZONE)
							player[1].incrementYPos(-0.05 * player[1].acceleration.y);
					}

			}
		}
		else if (events.type == SDL_JOYBUTTONDOWN) {
			if (events.jbutton.button == 10) {
				if (state == 0) {
					previousState = 0;
					state = 4;
				}
				else if (state == 4) {
					previousState = 4;
					state = 5;
					//state = 1;

				}
				else if (state == 2) {
					previousState = 2;
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
				else if (state == 5) {
					previousState = 5;
					Mix_PlayChannel(-1, start, 0);
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
			
			else if (events.jbutton.which == 0)
			{
			
				//Mapping FOR PS4 Controller (and third party PS3 controllers) ONLY
				//Button 0 is SQUARE, 1 is X, 2 is CIRCLE, 3 is TRANGLE, 4 IS L1, 5 is R1, 6 is L2, 7 is R2, 
				//8 is SHARE button, 9 is OPTIONS Button, 10 is L3, 11 IS R3, 12 is PS Button, 13 is touchpad
				if (events.jbutton.button == 9) {
					if (time(&refireTimerOne) % 2 == 0) {
						Mix_PlayChannel(-1, laserShot, 0);
						shoot(player[0]);
					}
				}
				else if (events.jbutton.button == 2) {
					if (state == 4) {
						Mix_PlayChannel(-1, select, 0);
						playerIndexOne--;
						if (playerIndexOne < 0)
							playerIndexOne = 3;
						else if (playerIndexOne == 1)
							playerIndexOne = 0;

					}

				}
				else if (events.jbutton.button == 3) {
					if (state == 4) {
						Mix_PlayChannel(-1, select, 0);
						playerIndexOne++;
						if (playerIndexOne > 3)
							playerIndexOne = 0;
						else if (playerIndexOne == 1)
							playerIndexOne = 2;
					}
					

				}
			}
			if (events.jbutton.which == 1)
			{
				
				//Mapping for FOR PS4 Controller ONLY
				//Button 0 is SQUARE, 1 is X, 2 is CIRCLE, 3 is TRANGLE, 4 IS L1, 5 is R1, 6 is L2, 7 is R2, 
				//8 is SHARE button, 9 is OPTIONS Button, 10 is L3, 11 IS R3, 12 is PS Button, 13 is touchpad
				//For PS3 Controller, Button 9 IS L1
				if (events.jbutton.button == 9) {
					if (time(&refireTimerOne) % 2 == 0) {
						Mix_PlayChannel(-1, laserShot, 0);
						shoot(player[1]);
					}
				}
					else if (events.jbutton.button == 2) {
						if (state == 4) {
							Mix_PlayChannel(-1, select, 0);
							playerIndexTwo--;
							if (playerIndexTwo < 0)
								playerIndexTwo = 3;
							else if (playerIndexTwo == 1)
								playerIndexTwo = 0;

						}

					}
					else if (events.jbutton.button == 3) {
						if (state == 4) {
							Mix_PlayChannel(-1, select, 0);
							playerIndexTwo++;
							if (playerIndexTwo > 3)
								playerIndexTwo = 0;
							else if (playerIndexTwo == 1)
								playerIndexTwo = 2;
						}

					}
			}
		}
		

	}
	if (keys[SDL_SCANCODE_B]) {
		state = 2;
		
		player[0].position.x = 0.0;
	}
	if (keys[SDL_SCANCODE_LEFT]) {
		
		player[0].incrementXPos(-2.0 * player[0].direction.x * player[0].velocity.x * elapsed);
		
	}

	if (keys[SDL_SCANCODE_RIGHT]) {
		
		player[0].incrementXPos(2.0 * player[0].direction.x *  player[0].velocity.x * elapsed);
		
	}
	if (keys[SDL_SCANCODE_UP]) {
		
		player[0].incrementYPos(2.0 *player[0].direction.y * player[0].velocity.y * elapsed);
		
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		
		player[0].incrementYPos(-2.0 *player[0].direction.y *  player[0].velocity.y * elapsed);
	
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



				
				
			}


		}

	}
	

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
float SpacialArea::mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
	float retVal = dstMin + ((value - srcMin) / (srcMax - srcMin) * (dstMax - dstMin));
	if (retVal < dstMin) {
		retVal = dstMin;
	}
	if (retVal > dstMax) {
		retVal = dstMax;
	}
	return retVal;
}
float SpacialArea::easeIn(float from, float to, float time) {
	float tVal = time*time*time*time*time;
	return (1.0f - tVal)*from + tVal*to;
}
float SpacialArea::easeOutElastic(float from, float to, float time) {
	float p = 0.3f;
	float s = p / 4.0f;
	float diff = (to - from);
	return from + diff + (diff*pow(2.0f, -10.0f*time) * sin((time - s)*(2 * 3.14) / p));
}