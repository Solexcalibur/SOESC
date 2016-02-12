#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Entity.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;


float vertices[] = { -0.5, -0.5,
0.05, -0.5,
0.05, 0.5,
-0.5, -0.5,
0.05, 0.5,
-0.5, 0.5 };
float texCoords[] = { 0.0, 1.0,
1.0, 1.0,
1.0, 0.0,
0.0, 1.0,
1.0, 0.0,
0.0, 0.0 };
float vertices2[] = { -0.04, -0.04,
0.04, -0.04,
0.04, 0.04,
-0.04, -0.04,
0.04, 0.04,
-0.04, 0.04 };

float texCoords2[] = { 0.0, 1.0,
1.0, 1.0,
1.0, 0.0,
0.0, 1.0,
1.0, 0.0,
0.0, 0.0 };
class Element{

	float xPos, yPos, angle, width, height, speed, speedCap ;

	bool victory = false;

	int score = 0;

	Matrix model, projection, view;




public:
	Element(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix): model(modelMatrix), projection(projectionMatrix), view(viewMatrix) {
		yPos = 0.0;
		xPos = 0.0;
		VDirection = 1.0;
		HDirection = 1.0;
	
	}

	float HDirection, VDirection;
	void render(ShaderProgram* program, float vertices[], float texCoords[]) {
		glUseProgram(program->programID);

		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program->positionAttribute);

		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program->texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program->positionAttribute);
		glDisableVertexAttribArray(program->texCoordAttribute);


	}

	void setMatrices(ShaderProgram* program){
		program->setModelMatrix(model); program->setProjectionMatrix(projection); program->setViewMatrix(view);
	}
	
	void setOrthoProjection(){
		projection.setOrthoProjection(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
	}
	
	void scaling(float x_value, float y_value, float z_value){
		model.Scale(x_value, y_value, z_value);
	}

	void identityMatrix(){
		model.identity();
	}

	void moveMatrix(float x_value, float y_value, float z_value){
		model.Translate(x_value, y_value, z_value);
	}

	float getXPos()  {
		return xPos;
	}
	float getYPos()  {
		return yPos;
	}
	void setXPos(float value){
		xPos = value;
	}
	void setYPos(float value){
		yPos = value;
	}
	
	void incrementXPos(float value){
		xPos += value;
	}
	void incrementYPos(float value){
		yPos += value;
	}
	

	void setWidthAndHeight(float widthvar, float heightvar){
		width = widthvar;
		height = heightvar;
	}

	bool victoryCondition(){
		victory = true;
		return victory;
	}

	void updateScore(){
		score += 1;
	}
	int getScore(){
		return score;
	}
};

bool processEvents(SDL_Event* events){
	bool done = false;
	if (events->type == SDL_QUIT || events->type == SDL_WINDOWEVENT_CLOSE) { //Toss this entire if statement into processInput function
		done = true;

	}
	
	return done;
}
//Update Ball Movement
//KNOWN ISSUE: BALL MAY FAIL TO CHANGE EITHER X OR Y DIRECTION DEPENDING ON WHETHER IT HITS VERTICAL OR HORIZONTAL BARRIERS
void processBallMovement(Element* ball, float elapsed){
	
	ball->incrementYPos(0.95f * ball->VDirection * elapsed);
	ball->incrementXPos(0.95f * ball->HDirection * elapsed);
	ball->identityMatrix();
	float y = ball->getYPos();
	ball->moveMatrix(ball->getXPos(),  ball->getYPos(), 0.0);
    
	if (ball->getYPos() > 0.79){
		ball->VDirection *= -1;
		//ball->incrementYPos(-200.75f * elapsed);
		//ball->moveMatrix(0.0, ball->getYPos() * elapsed, 0.0);
	}
	if (ball->getYPos() < -0.79){
		ball->VDirection *= -1;
		//ball->incrementYPos(-200.75f * elapsed);
		//ball->moveMatrix(0.0, ball->getYPos() * elapsed, 0.0);
	}
	if (ball->getXPos() > 1.5){
		ball->HDirection *= -1;
	}
	if (ball->getXPos() < -1.6){
		ball->HDirection *= -1;
	}

}

void processInput(Element* element, Element* element2, float elapsed){

	        const Uint8 *keys = SDL_GetKeyboardState(nullptr);
			const Uint8 *keys2 = SDL_GetKeyboardState(nullptr);
			float yPos = element->getYPos();
			float yPos2 = element2->getYPos();
			//float yPos3 = element3->getYPos(); 
			//element3->setYPos(0.3 * elapsed);
			//if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]){
			//	smileyX -= elapsed * 2.0f;
			//	//ghostX -= elapsed * 1.0f;
			//}
			//if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]){
			//	smileyX += elapsed * 2.0f;
			//}
			if (keys[SDL_SCANCODE_W]){
				element->incrementYPos(elapsed * 2.0f);
			}
			if (keys[SDL_SCANCODE_S]){
				//smileyY -= elapsed * 2.0f;
				element->incrementYPos(elapsed * -2.0f);
			}
			if (keys2[SDL_SCANCODE_UP]){
				//ghostY += elapsed * 2.0f;
				element2->incrementYPos(elapsed * 2.0f);
			}
			if (keys2[SDL_SCANCODE_DOWN]){
				//ghostY -= elapsed * 2.80f;
				element2->incrementYPos(elapsed * -2.0f);
			}
			if (yPos > 0.79){
				element->setYPos(0.78);
			}
			if (yPos < -0.79){
				element->setYPos(-0.78);
			}
			if (yPos2 > 0.79){
				element2->setYPos(0.78);
			}
			if (yPos2 < -0.79){
				element2->setYPos(-0.78);
			}
			/*if (yPos3 > 0.79){
				element3->setYPos(0.78);
			}*/
						/*if (ghostY > 0.79){
				ghostY = 0.79;
			}
			if (ghostY < -0.79){
				ghostY = -0.79;
			}*/
			//float units_x = (pixel_x / x_resolution) * ortho_width ) - ortho_width / 2.0;
}

int main(int argc, char *argv[]){
	//cout << "nope" << endl;
	SDL_Init(SDL_INIT_VIDEO);
	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	//SDL_JoystickOpen is passed the joystick index
	//SDL_Joystick * playerOneController = SDL_JoystickOpen(0);
	//In while loop, clean up for each open joystick
	//SDL_JoystickClose(playerOneController);
	int x_resolution = 800;
	int y_resolution = 600;
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_resolution, y_resolution, SDL_WINDOW_OPENGL);// <-OUTER BOUND
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, x_resolution, y_resolution);//<- INNER BOUND
	ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

	Matrix projectionMatrix, leftProjectionMatrix;
	Matrix modelMatrix, leftModelMatrix;
	Matrix viewMatrix, leftViewMatrix;

	Element ball(modelMatrix, projectionMatrix, viewMatrix);
	//Element leftPaddle(leftModelMatrix, leftProjectionMatrix, leftViewMatrix);
	Element leftPaddle(modelMatrix, projectionMatrix, viewMatrix);
	Element rightPaddle(modelMatrix, projectionMatrix, viewMatrix);
	
	SDL_Event event;
	bool done = false;
	float lastFrameTicks = 0.0f;
	float angle = 0.0f;
	while (!done) {
		float ticks = (float)SDL_GetTicks() / 1000.0f; float elapsed = ticks - lastFrameTicks; lastFrameTicks = ticks;

		while (SDL_PollEvent(&event)) {
			done = processEvents(&event);
		}
		

		glClearColor(0.5f, 0.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//ball
		ball.setMatrices(&program);
		ball.setOrthoProjection();
		ball.render(&program, vertices2, texCoords);
		//ball.setYPos(0.25 * elapsed);
		//ball.incrementYPos(0.25 * elapsed);
		//ball.setYPos(0.25);
		//ball.setYPos(0.15);
		//ball.setYPos(0.25);
		//ball.moveMatrix(0.0 , ball.getYPos() * elapsed, 0.0);
		processBallMovement(&ball, elapsed);
		//leftPaddle, Player 1, controlled by "W" and "S" keys
		leftPaddle.setMatrices(&program);
		leftPaddle.setOrthoProjection();
		leftPaddle.render(&program, vertices, texCoords);
		leftPaddle.identityMatrix();
		leftPaddle.setXPos(-1.67);
		leftPaddle.moveMatrix(leftPaddle.getXPos(), 0.0, 0.0);
		leftPaddle.moveMatrix(0.0, leftPaddle.getYPos(), 0.0);
		leftPaddle.scaling(0.2, 0.4, 1.0);
		//rightPaddle, Player 2, controlled by UP and DOWN arrow keys. Don't use arrows on numpad, won't work
		rightPaddle.setMatrices(&program);
		rightPaddle.setOrthoProjection();
		rightPaddle.render(&program, vertices, texCoords);
		rightPaddle.identityMatrix();
		rightPaddle.setXPos(1.77);
		rightPaddle.moveMatrix(rightPaddle.getXPos() , 0.0, 0.0);
		rightPaddle.moveMatrix(0.0, rightPaddle.getYPos(), 0.0);
		rightPaddle.scaling(0.2, 0.4, 1.0);

		processInput(&leftPaddle, &rightPaddle, elapsed);
		
		
		//ball.identityMatrix();
		//ball.scaling(0.1, 0.5, 1.0);
		//modelMatrix2.identity();


		//modelMatrix.Translate(-3.0, 0.0, 0.0);
		//modelMatrix2.Translate(ghostX, 0.0f, 0.0f);
		//moveY += elapsed;
		
		//glBindTexture(GL_TEXTURE_2D, emojiTexture);
		//smileyX += elapsed * 0.5f;

		angle += elapsed;

		//mod3.Translate(0.0f, moveY, 0.0f);


		//modelMatrix.Translate(0.5f * elapsed, 0.0f, 0.0f);
		//modelMatrix2.Translate(0.5f, 0.0, 0.0f);
		//modelMatrix2.Rotate(45.0 * (3.1415926 / 180.0));
		//if (smileyX > 2.0 || smileyX < -2.0 || smileyY > 1.0 || smileyY < -1.0) {
		//	//modelMatrix2.Rotate(25.0 * (3.1415926 / 180.0));
		//	//modelMatrix2.Rotate(25.0 * (3.1415926 / 180.0));
		//	//mod3.Rotate(25.0 * (3.1415926 / 180.0));
		//	
		//}

		
		//todo: collision detection, keeping score, organize code
		//SDL_JoystickClose(playerOneController);

		SDL_GL_SwapWindow(displayWindow);
	}
	//SDL_JoystickClose(playerOneController);

	SDL_Quit();
	return 0;



}
