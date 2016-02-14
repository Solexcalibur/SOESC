/*CS3113 HW2: Pong
//By: Stefan N. Cherubin
//Filename: HW2 SCherubin
//Simple game of Pong.
Player1 is leftPaddle (aka RedTeam) Player1 is controlled by W and S keys
Player2 is rightPaddle (aka BlueTeam) Player2 is controlled by UP and DOWN arrow keys (DO NOT USE ARROW KEYS ON NUMPAD)
Game is Best of 7 rounds. (first one to 4 wins)
The screen starts out as purple. A purple screen means that both players have the same score.
The screen will slowly transition to a more red blue color depending on who is leading.
Once a player reaches 4, the ball will disappear and the game is over.
It's more fun not knowing who has Match Point! :)
//*/
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Entity.h"
#include <math.h>
#include <vector>
using namespace std;

 
#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

float redFilter = 0.5f;
float blueFilter = 0.5f;
float greenFilter = 0.0f;
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

	float xPos, yPos;

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

	float HDirection, VDirection, width, height;
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
	

	void setWidthAndHeight(float vertices[]){
		if (vertices[0] != vertices[2]){
			//width = abs()
			width = abs(vertices[0] - vertices[2]);
		}
		else
		{
			width = abs(vertices[0] - vertices[4]);
		}
		if (vertices[1] != vertices[3]){
			//width = abs()
			height = abs(vertices[1] - vertices[3]);
		}
		else
		{
			height = abs(vertices[1] - vertices[5]);
		}
		
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

//Checks to see if somebody presses the close button
bool windowCloseChecker(SDL_Event* events){
	bool done = false;
	if (events->type == SDL_QUIT || events->type == SDL_WINDOWEVENT_CLOSE) { //Toss this entire if statement into processInput function
		done = true;

	}
	
	return done;
}

//Updates Ball Movement
//KNOWN ISSUE: BALL MAY FAIL TO CHANGE EITHER X OR Y DIRECTION DEPENDING ON WHETHER IT HITS VERTICAL OR HORIZONTAL BARRIERS
void processBallMovement(Element* ball, float elapsed){
	
	ball->incrementYPos(1.75f * ball->VDirection * elapsed);
	ball->incrementXPos(1.75f * ball->HDirection * elapsed);
	ball->identityMatrix();
	float y = ball->getYPos();
	ball->moveMatrix(ball->getXPos(),  ball->getYPos(), 0.0);
    
	

}

void collisionDetection(Element* ball, Element* leftPaddle, Element* rightPaddle, float elapsed){
	//Top Barrier Collision
	if (ball->getYPos() > 0.79){
		ball->VDirection *= -1;
		
	}
	//Bottom Barrier Collision
	if (ball->getYPos() < -0.79){
		ball->VDirection *= -1;
		
	}
	
	//Left Paddle Collision
	if (((ball->getXPos() - ball->width * 0.5) < leftPaddle->getXPos()) && ((ball->getYPos() + ball->height * 0.5) > (leftPaddle->getYPos() - leftPaddle->height * 0.5))
		&& ((ball->getYPos() - ball->height * 0.5) < (leftPaddle->getYPos() + leftPaddle->height * 0.5))){
		ball->HDirection *= -1;
	}
	//Right Paddle Collision 
	if (((ball->getXPos() + ball->width * 0.5) > rightPaddle->getXPos() - rightPaddle->width * 0.2) && ((ball->getYPos() + ball->height * 0.5) > (rightPaddle->getYPos() - rightPaddle->height * 0.5))
		&& ((ball->getYPos() - ball->height * 0.5) < (rightPaddle->getYPos() + rightPaddle->height * 0.5))){
		ball->HDirection *= -1;
	}
	//Ball moves past left paddle
	if ((ball->getXPos() + ball->width) < leftPaddle->getXPos()){
		ball->setXPos(0.0);
		ball->setYPos(0.0);
		ball->HDirection *= -1;
		rightPaddle->updateScore();
	}
	//Ball moves past right paddle
	if ((ball->getXPos() + ball->width) > rightPaddle->getXPos()){
		ball->setXPos(0.0);
		ball->setYPos(0.0);
		ball->HDirection *= -1;
		leftPaddle->updateScore();
	}

}

//Keeps track of score. Equal score will result in purple background.
//Screen changes colors to indicate who is leading.
void scoreBoard(Element* leftPaddle, Element* rightPaddle, Element* ball){
	if (leftPaddle->getScore() == rightPaddle->getScore()){
		redFilter = 0.5f;
		blueFilter = 0.5f;
	}
	if (abs(leftPaddle->getScore() - rightPaddle->getScore()) == 1){
		if (leftPaddle->getScore() > rightPaddle->getScore()){
			redFilter = 0.625f;
			blueFilter = 0.375f;
		}
		else {
			redFilter = 0.375f;
			blueFilter = 0.625f;
		}
	}
	if (abs(leftPaddle->getScore() - rightPaddle->getScore()) == 2){
		if (leftPaddle->getScore() > rightPaddle->getScore()){
			redFilter = 0.75f;
			blueFilter = 0.25f;
		}
		else {
			redFilter = 0.25f;
			blueFilter = 0.75f;
		}
	}
	if (abs(leftPaddle->getScore() - rightPaddle->getScore()) == 3){
		if (leftPaddle->getScore() > rightPaddle->getScore()){
			redFilter = 0.875f;
			blueFilter = 0.125f;
			//greenFilter = 0.2;
		}
		else {
			redFilter = 0.125f;
			blueFilter = 0.875f;
			//greenFilter = 0.2;

		}
	}
	
	if (leftPaddle->getScore() == 4 || rightPaddle->getScore() == 4){ //GAME OVER	
		ball->identityMatrix();
		ball->scaling(0.0, 0.0, 0.0); //Ball disappears
		if (leftPaddle->getScore() == 4){
			///PLAYER 1 WINS
			redFilter = 1.0f;
			blueFilter = 0.0f;
			
		}
		else {
			//PLAYER 2 WINS
			redFilter = 0.0f;
			blueFilter = 1.0f;
		}
	}
	
}

//Processes User Input and Updates screen
void processInput(Element* element, Element* element2, float elapsed){

	        const Uint8 *keys = SDL_GetKeyboardState(nullptr);
			const Uint8 *keys2 = SDL_GetKeyboardState(nullptr);
			float yPos = element->getYPos();
			float yPos2 = element2->getYPos();
	
			if (keys[SDL_SCANCODE_W]){ 
				element->incrementYPos(elapsed * 2.0f);
			}
			if (keys[SDL_SCANCODE_S]){
				
				element->incrementYPos(elapsed * -2.0f);
			}
			if (keys2[SDL_SCANCODE_UP]){
				
				element2->incrementYPos(elapsed * 2.0f);
			}
			if (keys2[SDL_SCANCODE_DOWN]){
				
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


			
}

//Creates view window
void setup(){
	SDL_Init(SDL_INIT_VIDEO);
	int x_resolution = 800;
	int y_resolution = 600;
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_resolution, y_resolution, SDL_WINDOW_OPENGL);// <-OUTER BOUND
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, x_resolution, y_resolution);//<- INNER BOUND
}

//Clears screen. Setting background color is optional.
void clearScreen(){
	glClearColor(redFilter, greenFilter, blueFilter, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Initalizing entities, setting things up, etc.
int main(int argc, char *argv[]){
	setup();
	ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	Element ball(modelMatrix, projectionMatrix, viewMatrix);
	Element leftPaddle(modelMatrix, projectionMatrix, viewMatrix);//Player1
	Element rightPaddle(modelMatrix, projectionMatrix, viewMatrix);//Player2
	
	SDL_Event event;
	bool done = false;
	float lastFrameTicks = 0.0f;
	float angle = 0.0f;
	while (!done) {
		float ticks = (float)SDL_GetTicks() / 1000.0f; float elapsed = ticks - lastFrameTicks; lastFrameTicks = ticks;

		while (SDL_PollEvent(&event)) {
			done = windowCloseChecker(&event);
		}
		
		clearScreen();
		
		//ball
		ball.setMatrices(&program);
		ball.setOrthoProjection();
		ball.setWidthAndHeight(vertices2);
		ball.render(&program, vertices2, texCoords);
		
		//leftPaddle, Player 1, controlled by "W" and "S" keys
		leftPaddle.setMatrices(&program);
		leftPaddle.setOrthoProjection();
		leftPaddle.setWidthAndHeight(vertices);
		leftPaddle.render(&program, vertices, texCoords);
		leftPaddle.identityMatrix();
		leftPaddle.setXPos(-1.67); //left paddle has advantage???
		leftPaddle.moveMatrix(leftPaddle.getXPos(), 0.0, 0.0);
		leftPaddle.moveMatrix(0.0, leftPaddle.getYPos(), 0.0);
		leftPaddle.scaling(0.2, 0.4, 1.0);

		//rightPaddle, Player 2, controlled by UP and DOWN arrow keys. Don't use arrows on numpad, won't work
		rightPaddle.setMatrices(&program);
		rightPaddle.setOrthoProjection();
		rightPaddle.setWidthAndHeight(vertices);
		rightPaddle.render(&program, vertices, texCoords);
		rightPaddle.identityMatrix();
		rightPaddle.setXPos(1.76); //Note, right paddle is slightly farther away from center than left paddle, possible due to window size
		rightPaddle.moveMatrix(rightPaddle.getXPos() , 0.0, 0.0);
		rightPaddle.moveMatrix(0.0, rightPaddle.getYPos(), 0.0);
		rightPaddle.scaling(0.2, 0.4, 1.0);


		//Processing movement
		processInput(&leftPaddle, &rightPaddle, elapsed);
		processBallMovement(&ball, elapsed);

		//Collision Detection
		collisionDetection(&ball, &leftPaddle, &rightPaddle, elapsed);

		//Scoring
		scoreBoard(&leftPaddle, &rightPaddle, &ball);
		

		angle += elapsed;

	
		SDL_GL_SwapWindow(displayWindow);
	}
	

	SDL_Quit();
	return 0;



}
