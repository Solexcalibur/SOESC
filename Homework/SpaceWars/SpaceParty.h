#ifndef SPACEPARTY_H
#define SPACEPARTY_H
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Astral.h"
#include "Projectile.h"
#include "SpriteSheet.h"
#include <vector>
#include <SDL.h>


	class SpacialArea {
	public:
		SpacialArea();
		//~SpacialArea();
		void render();
		void update();
		void updatePlusRender();
		void TitleScreen(ShaderProgram& program);
		void scoreBoard(ShaderProgram& program);
		void gameOverScreen(ShaderProgram& program);
		void DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing);
		GLuint LoadTexture(const char* image_path);
		void blendSprite(GLuint& texture);
		bool windowCloseChecker(SDL_Event event, std::vector<AstralEntity>& objects, Projectile ammo[], std::vector<SpriteSheet>& spriteSheets, ShaderProgram& program, float elasped);
		void screenSelector(ShaderProgram& program);
		void updateThings(ShaderProgram& program, std::vector<AstralEntity>& objects, Projectile ammo[],
			SDL_Event event, std::vector<SpriteSheet>& spriteSheets, float elasped);
		void titleEvents(SDL_Event event, ShaderProgram& program);
		void inGameEvents(SDL_Event event, ShaderProgram& program, std::vector<AstralEntity>& objects,  Projectile ammo[],
			std::vector<SpriteSheet>& spriteSheets, float elasped);
		void endGameEvents(SDL_Event event);
		void shoot(float elapsed);
		bool shouldRemoveBullet(Projectile & bullet);
		void setup();
		void clearScreen();
		void windowSwapping();
		void sweep(int index);

		GLuint wordTexture, spriteSheetTexture;
	private:
		SDL_Window* displayWindow;
		
		enum gameState { STATE_TITLE, STATE_GAME_LEVEL, STATE_GAME_OVER, STATE_VICTORY };
		int state, numEnemies, shotIndex, maxshots, score, index;
		const Uint8* keys;
		const char* fontSheetPath = "font2.png";
		const char* spritepath = "SpaceStuff.png";
		bool done = false;
		SDL_Event events;
		//Projectile* shots = new Projectile[4];
		//AstralEntity player[5];
		//Projectile shots[10];
		SpriteSheet sprites[4];
		std::vector<AstralEntity> player;
		std::vector<Projectile> shots;
		//std::vector<AstralEntity> player[5];
		//AstralEntity player;
		//AstralEntity* enemies = new AstralEntity[MAX_BAD_GUYS];
		//AstralEntity* playerGun = new AstralEntity[MAX_HUMAN_SHOTS];
		float r_filter, g_filter, b_filter;


	};










#endif