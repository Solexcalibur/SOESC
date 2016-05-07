#ifndef SPACEPARTY_H
#define SPACEPARTY_H
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Astral.h"
#include "Projectile.h"
#include "SpriteSheet.h"
#include "ParticleEmitter.h"
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_audio.h>

#define LEVEL_HEIGHT 32
#define LEVEL_WIDTH 32
#define TILE_SIZE 0.4f
#define SPRITE_COUNT_X 32
#define SPRITE_COUNT_Y 32

	class SpacialArea {
	public:
		SpacialArea();
		~SpacialArea();
		void render();
		void update();
		void updatePlusRender();
		void TitleScreen(ShaderProgram& program);
		void scoreBoard(ShaderProgram& program);
		void gameOverScreen(ShaderProgram& program);
		void VictoryScreen(ShaderProgram & program);
		//void DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing);
		GLuint LoadTexture(const char* image_path);
		void blendSprite(GLuint& texture);
		//bool windowCloseChecker(SDL_Event event, std::vector<AstralEntity>& objects, Projectile ammo[], std::vector<SpriteSheet>& spriteSheets, ShaderProgram& program, float elasped);
		void screenSelector(ShaderProgram& program);
		void SelectCharacter();
		void SelectCharacter(ShaderProgram & program);
		//void updateThings(ShaderProgram& program, std::vector<AstralEntity>& objects, Projectile ammo[],
		//	SDL_Event event, std::vector<SpriteSheet>& spriteSheets, float elasped);
		void titleEvents(SDL_Event event, ShaderProgram& program);
		void setupThings();
		void setupThings(ShaderProgram & program);
		void inGameEvents(SDL_Event event, ShaderProgram& program, float elasped);
		void endGameEvents(SDL_Event event);
		void updateThings(ShaderProgram & program, float elasped);
		void shoot(AstralEntity & player, float elapsed);
		void shoot(AstralEntity& player);
		//void updateThings(ShaderProgram & program, SDL_Event event, float elasped);
		void shoot(float elapsed);
		bool shouldRemoveBullet(Projectile & bullet);
		bool inputProcessor(ShaderProgram & program, float elapsed);
		bool collisionDetectionX(float x, float y);
		bool collisionDetectionY(float x, float y);
		void collisionHandler(Projectile & bullets, AstralEntity & player);
		bool raySegmentIntersect(const Vector & rayOrigin, const Vector & rayDirection, const Vector & linePt1, const Vector & linePt2, float & dist);
		void setup();
		bool windowCloseChecker(SDL_Event event);
		void clearScreen();
		void windowSwapping();
		void DrawText(ShaderProgram & program, int fontTexture, std::string text, float size, float spacing, float x, float y);
		void sweep(int index);
		void readFile(const char * levelFile, ShaderProgram & program);
		bool readEntityData(std::ifstream & stream, ShaderProgram & program);

		bool readLayerData(std::ifstream & stream);
		void placeEntity(std::string type, float x, float y);


		bool readHeader(std::ifstream & stream);

		void render(ShaderProgram & program);

		void setupAndRender(ShaderProgram & program, float vertices[], float texCoords[], GLuint & texture);

		float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);

		float easeIn(float from, float to, float time);

		void DrawTextz(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);

		
		Mix_Chunk *scored, *gameover, *victory, *start, *laserShot, *select;
	private:
		SDL_Window* displayWindow;
		
		enum gameState { STATE_TITLE, STATE_GAME_LEVEL, STATE_GAME_OVER, STATE_VICTORY, STATE_CHARACTER_SELECT, STATE_LEVEL_SELECT };
		int state, numEnemies, shotIndex, maxshots, score, index;
		GLuint wordTexture, spriteSheetTexture, particletex, metalTex;
		const Uint8* keys;
		const char* fontSheetPath = "font2.png";
		const char* spritepath = "SpaceStuff.png";
		bool done = false;
		SDL_Event events;
		bool shotfired = false;
		//Projectile* shots = new Projectile[4];
		//AstralEntity player[5];
		//Projectile shots[10];
		SpriteSheet sprites[6];
		std::vector<AstralEntity> player;
		std::vector<AstralEntity> levels;
		std::vector<AstralEntity> text;
		std::vector<Projectile> shots;
		std::vector<float> vertexData;
		std::vector<float> texCoordData;
		std::vector<ParticleEmitter> party;
		AstralEntity particle;
		//ParticleEmitter party;
		//ParticleEmitter party;
		int mapHeight, mapWidth, playerIndexOne, playerIndexTwo, laserIndexOne, laserIndexTwo;
		unsigned char** levelData;
		//std::vector<Projectile> enemyshots;
		//std::vector<AstralEntity> player[5];
		//AstralEntity player;
		//AstralEntity* enemies = new AstralEntity[MAX_BAD_GUYS];
		//AstralEntity* playerGun = new AstralEntity[MAX_HUMAN_SHOTS];
		float r_filter, g_filter, b_filter ,spriteWidth, spriteHeight, tileLength, tileHeight, animationTime;
		SDL_Joystick * playerOne;
		SDL_Joystick * playerTwo;

	};










#endif