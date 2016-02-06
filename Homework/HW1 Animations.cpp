/*CS3113 HW1: Animated Stuff
By: Stefan N. Cherubin 
Filename: HW1 Animations
Animated Elements: emoji, ghost
Note: Use arrow keys or WASD keys to move emoji 
*/


#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"

using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
//GLuint ghost;
float angle = 0.0f;
float smileyX = 0.0;
float smileyY = 0.0;
float ghostX = 0.0;
float elapsed;

GLuint LoadTexture(const char *image_path) {

	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	SDL_FreeSurface(surface);
	return textureID;
}
float vertices[] = { -0.5, -0.5,
0.5, -0.5,
0.5, 0.5,
-0.5, -0.5,
0.5, 0.5,
-0.5, 0.5 };
float texCoords[] = { 0.0, 1.0,
1.0, 1.0,
1.0, 0.0,
0.0, 1.0,
1.0, 0.0,
0.0, 0.0 };
float vertices2[] = { -0.5, -0.5,
0.5, -0.5,
0.5, 0.5,
-0.5, -0.5,
0.5, 0.5,
-0.5, 0.5 };
float texCoords2[] = { 0.0, 1.0,
1.0, 1.0,
1.0, 0.0,
0.0, 1.0,
1.0, 0.0,
0.0, 0.0 };

void BlendSprite(GLuint& texture) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glBindTexture(GL_TEXTURE_2D, texture2);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);

}
void matrixStuff(ShaderProgram& program, Matrix& projectionMatrix, Matrix& modelMatrix, Matrix& viewMatrix){
	program.setModelMatrix(modelMatrix); program.setProjectionMatrix(projectionMatrix); program.setViewMatrix(viewMatrix);
	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	modelMatrix.identity();
	modelMatrix.Translate(smileyX, smileyY, 0.0);
	modelMatrix.Translate(0.5 * elapsed, 0.0, 0.0);
}


void setupAndRender(ShaderProgram& program, Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix, float vertices[], float texCoords[], GLuint& texture){
	BlendSprite(texture); //Blend first? Why?
	glUseProgram(program.programID);
	//Matrix projectionMatrix; Matrix modelMatrix; Matrix viewMatrix;
	program.setModelMatrix(modelMatrix); program.setProjectionMatrix(projectionMatrix); program.setViewMatrix(viewMatrix);
	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    //modelMatrix.identity();
	//modelMatrix.Translate(smileyX, smileyY, 0.0);
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);
	
}
void processInput();

int main(int argc, char *argv[])
{
	//cout << "nope" << endl;
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("SCIENCE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	
#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, 800, 600);
	GLuint emojiTexture = LoadTexture("emoji.png");
	GLuint ghost = LoadTexture("ghost.png");
	GLuint pac = LoadTexture("PacMan.png");
	
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    //ShaderProgram program2(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	//setup(program2);
	
	//ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	
	Matrix projectionMatrix, projectionMatrix2, proj3; Matrix modelMatrix,modelMatrix2, mod3; Matrix viewMatrix,viewMatrix2, view3;
	//projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	//projectionMatrix2.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	
	//GLuint textureID;     glGenTextures(1, &textureID); 
	
	
	
	
	
	SDL_Event event;
	bool done = false;
	float lastFrameTicks = 0.0f;
	/*float angle = 0.0f;
	
	float smileyX = 0.0;
	float smileyY = 0.0;
	float ghostX = 0.0;*/
	//glUseProgram(program.programID);
	//glUseProgram(program2.programID);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	while (!done) {
		//projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
		
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			//Input Events go here! SDL_KEYDOWN means when any key is pressed SDL_KEYUP means when key is released
			//if key.keysym.scancode == SDL_SCANCODE_SPACE ... 
		}

		
		glClearColor(0.5f, 0.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//program.setModelMatrix(modelMatrix); program.setProjectionMatrix(projectionMatrix); program.setViewMatrix(viewMatrix);
		//program.setModelMatrix(modelMatrix2); program.setProjectionMatrix(projectionMatrix2); program.setViewMatrix(viewMatrix2);
		modelMatrix.identity();
		//modelMatrix2.identity();

		modelMatrix.Translate(smileyX, smileyY, 0.0);
	    modelMatrix2.Translate(ghostX, 0.0, 0.0);
		//glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		////glVertexAttribPointer(program2.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
		//glEnableVertexAttribArray(program.positionAttribute);
		////glEnableVertexAttribArray(program2.positionAttribute);


		//glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		////glVertexAttribPointer(program2.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
		//glEnableVertexAttribArray(program.texCoordAttribute);
		////glEnableVertexAttribArray(program2.texCoordAttribute);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		////glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDisableVertexAttribArray(program.positionAttribute);
		////glDisableVertexAttribArray(program2.positionAttribute);
		//glDisableVertexAttribArray(program.texCoordAttribute);
		//glDisableVertexAttribArray(program2.texCoordAttribute);
		setupAndRender(program, modelMatrix, projectionMatrix, viewMatrix, vertices, texCoords, emojiTexture);
		setupAndRender(program, modelMatrix2, projectionMatrix2, viewMatrix2, vertices2, texCoords2, ghost);
		setupAndRender(program, mod3, proj3, view3, vertices2, texCoords2, pac);
		

		float ticks = (float)SDL_GetTicks() / 1000.0f; float elapsed = ticks - lastFrameTicks; lastFrameTicks = ticks;
		angle += elapsed; 
		modelMatrix.Translate(0.5 * elapsed, 0.0, 0.0);
		modelMatrix2.Translate(0.5 * elapsed, 0.0, 0.0);
		//matrixStuff(program, projectionMatrix, modelMatrix, viewMatrix);
	
		
		const Uint8 *keys = SDL_GetKeyboardState(nullptr);
		 
		if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]){
			smileyX -= elapsed * 2.0;
			//ghostX -= elapsed * 1.0;
		}
		if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]){
			smileyX += elapsed * 2.0;
		}
		if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]){
			smileyY += elapsed * 2.0;
		}
		if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]){
			smileyY -= elapsed * 2.0;
		}
		//if (smileyX > 2.0){
		//	//modelMatrix.Translate(-0.5 * elapsed, 0.0, 0.0);
		//	smileyX += -1 * elapsed * 0.5;
		//	
		//}
		/*else if(smileyX < -4.0){
			smileyX += elapsed * 0.5;
		}*/
		//BlendSprite(ghost);
		//BlendSprite(emojiTexture);
		
		//DrawSprite(ghost, 0.0, 0.0, angle);
		//BlendSprite(emojiTexture);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, vertices);
		//glDisable(GL_BLEND);

		SDL_GL_SwapWindow(displayWindow);
	}
	
	//BlendSprite(ghost);
	SDL_Quit();
	return 0;
}
