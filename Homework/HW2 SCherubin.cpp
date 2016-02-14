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
	
	ball->incrementYPos(1.75f * ball->VDirection * elapsed);
	ball->incrementXPos(1.75f * ball->HDirection * elapsed);
	ball->identityMatrix();
	float y = ball->getYPos();
	ball->moveMatrix(ball->getXPos(),  ball->getYPos(), 0.0);
    
	

}

void collisionDetection(Element* ball, Element* leftPaddle, Element* rightPaddle, float elapsed){
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
	/*if (ball->getXPos() > 1.6){
	ball->HDirection *= -1;
	}*/

	/*if (ball->getXPos() <- 1.6){
	ball->HDirection *= -1;
	}*/
	//Left Paddle Collison
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
		//SDL_GL_SwapWindow(displayWindow);
	}

}
//Keeps track of score.
void scoreBoard(Element* leftPaddle, Element* rightPaddle, Element* ball){
	if (leftPaddle->getScore() == rightPaddle->getScore()){
		//glClearColor(redFilter, 0.0f, blueFilter, 1.0f);
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
	
	if (leftPaddle->getScore() == 4 || rightPaddle->getScore() == 4){
		//ball->moveMatrix(0.0, 0.0, 0.0);
		ball->identityMatrix();
		ball->scaling(0.0, 0.0, 0.0);
		if (leftPaddle->getScore() == 4){
			///PLAYER 1 WINS
			redFilter = 1.0f;
			blueFilter = 0.0f;
			//glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			//glClear(GL_COLOR_BUFFER_BIT);
		}
		else {
			//PLAYER 2 WINS
			redFilter = 0.0f;
			blueFilter = 1.0f;
			//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			//glClear(GL_COLOR_BUFFER_BIT);
			//SDL_GL_SwapWindow(displayWindow);
		}
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

void DrawText(ShaderProgram *program, int fontTexture, string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;

	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
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
		

		glClearColor(redFilter, greenFilter, blueFilter, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//ball
		ball.setMatrices(&program);
		ball.setOrthoProjection();
		ball.setWidthAndHeight(vertices2);
		ball.render(&program, vertices2, texCoords);
		//ball.setYPos(0.25 * elapsed);
		//ball.incrementYPos(0.25 * elapsed);
		//ball.setYPos(0.25);
		//ball.setYPos(0.15);
		//ball.setYPos(0.25);
		//ball.moveMatrix(0.0 , ball.getYPos() * elapsed, 0.0);
		
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
		
		//DrawText(&program, 2, "?", 2, 1);
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
