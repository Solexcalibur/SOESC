#include "Astral.h"
#include "SpaceParty.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Projectile.h"
#include "SpriteSheet.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <math.h>
using namespace std;

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

SpacialArea::SpacialArea(){
	//score = 0;
	state = 0;
	keys = SDL_GetKeyboardState(nullptr);
	wordTexture = LoadTexture(fontSheetPath);
	r_filter = 0.5;
	g_filter = 0.0;
	b_filter = 0.5;
	player[0].YPos = -1.5;
	player[0].XPos = 0.0;
	shotIndex = 0;
	shots[shotIndex].YPos = -1.5;
	//player = new AstralEntity();

}


void SpacialArea::setup(){
	SDL_Init(SDL_INIT_VIDEO);
	int x_resolution = 800;
	int y_resolution = 600;
	displayWindow = SDL_CreateWindow("Astral Horizon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_resolution, y_resolution, SDL_WINDOW_OPENGL);// <-OUTER BOUND
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, x_resolution, y_resolution);//<- INNER BOUND
}

bool SpacialArea::windowCloseChecker(SDL_Event event, vector<AstralEntity>& objects, Projectile ammo[], vector<SpriteSheet>& spriteSheets, ShaderProgram& program, float elasped){
	bool done = false;
	
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) { //Toss this entire if statement into processInput function
			done = true;

		}
		//else if (event.type == SDL_KEYDOWN){
		else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
		objects[0].shoot(program, ammo, spriteSheets, elasped);

		}

		//}
	

	return done;
}

void SpacialArea::clearScreen(){
	glClearColor(r_filter, g_filter, b_filter, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void SpacialArea::windowSwapping(){
	SDL_GL_SwapWindow(displayWindow);
}

void SpacialArea::DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing){
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



void SpacialArea::blendSprite(GLuint& texture){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture); //Disabling this will result in the most recent image loaded to take over everything

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}


void SpacialArea::TitleScreen(ShaderProgram& program){
	//GLuint fontTex = wordTexture;
	//wordTexture = LoadTexture(fontSheetPath);
	DrawText(program, wordTexture, "Astral Horizon! You are outnumbered, think you can win?", 0.075, 0);
	//DrawText(program, fontTex, "2 v 1", 0.25, 0);
	//DrawText(program, fontTex, "Can you win?", 0.25, 0);
	

	blendSprite(wordTexture);

}

void SpacialArea::scoreBoard(ShaderProgram& program){
	//GLuint fontTex = wordTexture;

	int score = 0;
	DrawText(program, wordTexture,"SCORE:"+ to_string(score), 0.1, 0);
	blendSprite(wordTexture);

}

void SpacialArea::gameOverScreen(ShaderProgram& program){
	//GLuint fontTex = wordTexture;
	//glClearColor(1.0, 0.5, 0.0, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//r_filter = 1.0;
	DrawText(program, wordTexture, "GAME OVER! DARE TO PLAY AGAIN?", 0.1, 0);
	//DrawText(program, fontTex, "DARE TO PLAY AGAIN?", 0.1, 0);
	blendSprite(wordTexture);

}

void SpacialArea::screenSelector(ShaderProgram& program){
	
	switch (state){
			case STATE_TITLE:
				TitleScreen(program);
				break;
			case STATE_GAME_LEVEL:
				scoreBoard(program);
				break;
			case STATE_GAME_OVER:
				gameOverScreen(program);
				break;
			}
}

void SpacialArea::titleEvents(SDL_Event event, ShaderProgram& program){
	
	if (keys[SDL_SCANCODE_W]){
		state = 1;
	}
	
		
	}


void SpacialArea::inGameEvents(SDL_Event event, ShaderProgram& program, vector<AstralEntity>& objects,
	Projectile ammo[], vector<SpriteSheet>& spriteSheets, float elapsed){


	

	
	int ammoIndex = 0;
	//int maxshots = 10;
	
	if (shotIndex > maxshots - 1) {
		shotIndex = 0;

	}
	
	//objects[0].setMatrices(program);
	player[0].setMatrices(program);
	player[0].setOrthoProjection();
	player[0].identityMatrix();
	player[0].moveMatrix(player[0].XPos, player[0].YPos, 0.0);
	spriteSheets[0].Draw(program);


	objects[1].setMatrices(program);
	spriteSheets[2].Draw(program);


	objects[2].setMatrices(program);
	spriteSheets[2].Draw(program);

	//for (int i = 1; i < 3; i++){
	//	objects[i].setMatrices(program);
	//	//objects[i].YPos = 1.5;
	//	spriteSheets[2].Draw(program);
	//}
	shots[shotIndex].setOrthoProjection();
	shots[shotIndex].setMatrices(program);
	shots[shotIndex].identityMatrix();
	shots[shotIndex].XPos = player[0].XPos;
	shots[shotIndex].incrementYPos(6.0 * elapsed);
	spriteSheets[1].Draw(program);


	
	//objects[0].YPos = -1.5;
	//shots[0].moveMatrix(0.0, shots[0].YPos, 0.0);
	objects[1].incrementXPos(2.0 * objects[1].HDirection * elapsed);
	objects[2].incrementXPos(-2.0 * objects[2].HDirection * elapsed);
	//shoot(program, objects[0], spriteSheets[1], elapsed);
	//ammo[1].XPos = objects[1].XPos;
	//ammo[2].XPos = objects[2].XPos;
	
	
	shots[shotIndex].moveMatrix(shots[shotIndex].XPos, shots[shotIndex].YPos, 0.0);
	while (SDL_PollEvent(&events)) {
		
		if (events.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) { //Toss this entire if statement into processInput function
			done = true;

		}
		else if (events.type == SDL_KEYDOWN) {
			if (events.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				//player[0].shoot(program, ammo, spriteSheets, elapsed);
				//shoot(elapsed);
				//shotIndex++;
				//shots[0].incrementYPos(4.0 * elapsed);
				//shots[0].incrementYPos(4.0 * elapsed);
				//shots[0].moveMatrix(0.0, shots[0].YPos, 0.0);
				//DrawText(program, wordTexture, "Astral Horizon! You are outnumbered, think you can win?", 0.075, 0);

			}
			//shots[0].incrementYPos(4.0 * elapsed);

		}
		

	}

	if (keys[SDL_SCANCODE_SPACE]) {
		shoot(elapsed);
	}
	//shotIndex++;
	//ammoIndex++;
	//shots[0].incrementYPos(4.0 * elapsed);
	//shots[0].incrementYPos(4.0 * elapsed);
	/*if (keys[SDL_SCANCODE_SPACE]) {
		shots[0].incrementYPos(elapsed);

	}*/


	
	for (int i = 1; i < 3; i++){
		if (objects[i].XPos > 2.0){
			objects[i].XPos = 2.0;
			objects[i].HDirection *= -1;
			objects[i].incrementYPos(-0.05);
		}
		if (objects[i].XPos < -2.0){
			objects[i].XPos = -2.0;
			objects[i].HDirection *= -1;
			objects[i].incrementYPos(-0.05);
		}
		//if (objects[i].YPos == -1.5){
		//	//state = 2;
		//	objects[i].YPos = 1.5;
		//}

	}



	//ammo[1].YPos = objects[2].YPos;
	//if (objects[1].XPos > 2.0){
	//	objects[1].HDirection *= -1;
	//	//objects[2].incrementYPos(-0.25);
	//	//objects[2].shoot(program, ammo, elapsed);

	//}
	//if (objects[1].XPos < -2.0){
	//	objects[1].HDirection *= -1;
	//}
	//if (objects[2].XPos > 2.0){
	//	objects[2].HDirection *= -1;
	//objects[2].incrementYPos(-0.25);
	//objects[2].shoot(program, ammo, elapsed);




	//for (int i = 0; i < 10; i++){

	//	ammo[i].setMatrices(program);
	//	//ammo[i].renderWithNoTexture(program2, vertices_ball);
	//	//ammo[i].setOrthoProjection();
	//	ammo[i].setupAndRender(*program, vertices_ball, texCoords, ammo[i].texID);
	//}

	//ammo[0].renderWithNoTexture(program, vertices_ball);
	//ammo[i].setOrthoProjection();
	//ammo[0].setupAndRender(*program, vertices_paddle, texCoords, sheetx.textureID);


	if (keys[SDL_SCANCODE_A]){
		state = 2;
		//player.XPos = 0.0;
		objects[0].XPos = 0.0;
	}
	if (keys[SDL_SCANCODE_LEFT]){
		//player.incrementXPos(-2.0 * elapsed);
		player[0].incrementXPos(-2.0 * elapsed);
		ammo[0].incrementXPos(-2.0 * elapsed);
		//ammo[1].incrementXPos(-2.0 * elapsed);
	}
	if (keys[SDL_SCANCODE_RIGHT]){
		//player.incrementXPos(2.0 * elapsed);
		player[0].incrementXPos(2.0 * elapsed);
		ammo[0].incrementXPos(2.0 * elapsed);
		//ammo[1].incrementXPos(2.0 * elapsed);
	}


	//ammo[0].YPos = objects[1].YPos;
	//ammo[1].YPos = objects[2].YPos;
	/*else if (event.type == SDL_KEYDOWN){
		if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
		objects[0].shoot(program, ammo, spriteSheets, elapsed);
		}
		}*/


	//ammo[0].YPos = -1.5;
	//if (keys[SDL_SCANCODE_SPACE]){
		
//while (SDL_PollEvent(&event)) {

		
  		
		//}
	//}	//ammo[0].YPos = -1.5;
	

	//spriteSheets[1].Draw(program);
	//ammo[0].YPos = -1.5;

	//		//if (!keys[SDL_SCANCODE_SPACE]){
	//		//ammo[0].setMatrices(program);
	//		//ammo[0].identityMatrix();
	//ammo[0].incrementYPos(2.5 * elapsed);
//objects[0].shoot(program, ammo, spriteSheets, elapsed);
	//ammo[0].incrementYPos(2.5 * elapsed);


	/*if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
		objects[0].shoot(program, ammo, spriteSheets, elapsed);

	}*/
	/*if (event.type == SDL_KEYDOWN){
	
		
		objects[0].shoot(program, ammo, spriteSheets, elapsed);

	}*/
	//objects[0].shoot(program, ammo, spriteSheets, elapsed);
	
	/*if (keys[SDL_SCANCODE_SPACE]){
		objects[0].shoot(program, ammo, spriteSheets, elapsed);
	}*/
	//if (keys[SDL_SCANCODE_SPACE]){
	//	
	//	if (keys[SDL_KEYUP]){
	//		ammo[0].setMatrices(program);
	//		
	//		spriteSheets[1].Draw(program);
	//		//ammo[0].YPos = -1.5;

	//		//if (!keys[SDL_SCANCODE_SPACE]){
	//		//ammo[0].setMatrices(program);
	//		//ammo[0].identityMatrix();
	//		ammo[0].incrementYPos(2.5 * elapsed);

	//	}
	//}
	//ammo[0].setMatrices(program);

	//spriteSheets[1].Draw(program);
	//ammo[0].YPos = -1.5;

	//if (!keys[SDL_SCANCODE_SPACE]){
	//ammo[0].setMatrices(program);
	//ammo[0].identityMatrix();
	//ammo[0].incrementYPos(2.5 * elapsed);
	//}
	// if (keys[SDL_KEYUP]){
		//if (keys[SDL_SCANCODE_SPACE]){
			//ammo[0].YPos = -1.5;
			
			//spriteSheets[1].Draw(program);
			//ammo[1].YPos = 1.5;
			//ammo[2].YPos = 1.5;
		//}
		//}
	//}
	

	 if (ammo[0].YPos == objects[2].XPos){
		 ammo[0].YPos = -1.5;
	 }



	/*if (ammo.YPos > 1.0){
		bullet.YPos = 0.0;
		}*/
	//for (int i = 0; i < 10; i++){
	/*if (ammo[0].YPos > 3.0){
		ammo[0].YPos = -1.5;
	}*/
	 
	 //shoot(program, objects[0], spriteSheets[1], elapsed);
	
	/* if (keys[SDL_SCANCODE_SPACE]){
		 objects[0].shoot(program, ammo, spriteSheets, elapsed);
	 }*/

	/*if (ammo[1].YPos < -2.0){
		ammo[1].YPos = objects[1].YPos + 0.5;

	}
	if (ammo[2].YPos < -2.0){
		ammo[2].YPos = objects[2].YPos + 0.5;

	}*/
	 

}

void SpacialArea::endGameEvents(SDL_Event event){
	 
	if (keys[SDL_SCANCODE_D]){
		state = 0;
	}

}

void SpacialArea::updateThings(ShaderProgram& program, vector<AstralEntity>& objects,
	Projectile ammo[], SDL_Event event, vector<SpriteSheet>& spriteSheets, float elasped){
	switch (state){
	case STATE_TITLE:
		titleEvents(event, program);
		break;
	case STATE_GAME_LEVEL:
		inGameEvents(event, program, objects, ammo, spriteSheets, elasped);
		break;
	case STATE_GAME_OVER:
		endGameEvents(event);
		break;


	}


}

void SpacialArea::shoot(float elapsed) {
	//shots[0].setMatrices(program);
	////shots[0].YPos = -1.5;
	//sprite.Draw(program);
	//shots[0].incrementYPos(7.5 * elapsed);
	if (shots[shotIndex].YPos > 2.0){
	shots[shotIndex].YPos = -1.5;
	}
	shotIndex++;
	
	if (shotIndex > maxshots - 1) {
		shotIndex = 0;

	}
	
}