#ifndef SPACEPARTY_H
#define SPACEPARTY_H
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Astral.h"
#include "Projectile.h"
#include "SpriteSheet.h"
#include <vector>
#include <SDL.h>

#define MAX_HUMAN_SHOTS 15
#define MAX_BAD_GUYS 20
#define MAX_BADGUY_SHOTS 20

	class SpacialArea {
	public:
		SpacialArea();
		~SpacialArea();
		void render();
		void update();
		void updatePlusRender();
		void TitleScreen(ShaderProgram* program);
		void scoreBoard(ShaderProgram* program);
		void gameOverScreen(ShaderProgram* program);
		void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing);
		GLuint LoadTexture(const char* image_path);
		void blendSprite(GLuint& texture);
	    bool windowCloseChecker(SDL_Event event);
		void screenSelector(ShaderProgram* program);
		void updateThings(ShaderProgram* program, std::vector<AstralEntity>& objects, Projectile ammo[],
			SDL_Event event, std::vector<SpriteSheet>& spriteSheets, float elasped);
		void titleEvents(SDL_Event event);
		void inGameEvents(SDL_Event event, ShaderProgram* program, std::vector<AstralEntity>& objects,  Projectile ammo[],
			std::vector<SpriteSheet>& spriteSheets, float elasped);
		void endGameEvents(SDL_Event event);
		void shoot(AstralEntity& bullet, ShaderProgram* program, float elapsed);
		void setup();
		void clearScreen();
		void windowSwapping();


	private:
		SDL_Window* displayWindow;
		enum gameState { STATE_TITLE, STATE_GAME_LEVEL, STATE_GAME_OVER };
		int state, numEnemies, shotIndex;
		const Uint8* keys;
		const char* fontSheetPath = "font2.png";
		bool done = false;
		SDL_Event event;
		GLuint wordTexture, spriteSheetTexture;
		//AstralEntity player;
		//AstralEntity* enemies = new AstralEntity[MAX_BAD_GUYS];
		//AstralEntity* playerGun = new AstralEntity[MAX_HUMAN_SHOTS];
		float r_filter, g_filter, b_filter;


	};










#endif