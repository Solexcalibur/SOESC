#include "ShaderProgram.h"
#include "Matrix.h"
#include "Projectile.h"
#include "Astral.h"
#include <vector>
#include "SpriteSheet.h"
#include "Vector.h"
#include <SDL.h>
#include <SDL_image.h>


AstralEntity::AstralEntity()
{
	position.x = 0.0;
	position.y = -1.5;
	position.z = 0.0;
	direction.x = 1.0;
	direction.y = 1.0;
	direction.z = 1.0;
	acceleration.x = 0.5;
	acceleration.y = 0.5;
	acceleration.z = 0.0;
	velocity.x = 1.0;
	velocity.y = 1.0;
	velocity.z = 0.0;
	friction.x = 1.577;
	friction.y = 1.577;
	XPos = 0.0;
	YPos = 0.0;
	HDirection = 1.0;
	VDirection = 1.0;
	health = 100.0;
	score = 0;
	ammoIndex = 0;
	maxshots = 10;
	alive = true;
	//velocity = 1;
	velocityX = 1.0;
	velocityY = 1.0;
	accelerationX = 1.75;
	accelerationY = 1.75;
	frictionX = 1.577;
	frictionY = 1.577;
	isStatic = false;
	collideBottom = false;
	collideLeft = false;
	collideRight = false;
	collideTop = false;
	//width = 1.0;
	//height = 1.0;
}

AstralEntity::AstralEntity(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix) {
	XPos = 0.0;
	YPos = 0.0;
	HDirection = 1.0;
	VDirection = 1.0;
	health = 100.0;
	model = modelMatrix;
	projection = projectionMatrix;
	//view = viewMatrix;
	score = 0;
	ammoIndex = 0;
	maxshots = 10;
	alive = true;
	//velocity = 1;
	//sprite = spritez;


}
AstralEntity::AstralEntity(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix, SpriteSheet& spritez) {
	XPos = 0.0;
	YPos = 0.0;
	HDirection = 1.0;
	VDirection = 1.0;
	health = 100.0;
	model = modelMatrix;
	projection = projectionMatrix;
	//view = viewMatrix;
	score = 0;
	ammoIndex = 0;
	maxshots = 10;
	texID = spritez.textureID;
	//velocity = 1;
	//sprite = spritez;


}
void AstralEntity::setWidthAndHeight(float widthv, float heightv)
{
	width = widthv;
	height = heightv;
}
void AstralEntity::DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing){
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

bool AstralEntity::lifeChecker(){
	return alive;

}

void AstralEntity::setMatrices(ShaderProgram& program){
	program.setModelMatrix(model); 
	program.setProjectionMatrix(projection); 
	program.setViewMatrix(view);
}

void AstralEntity::setOrthoProjection(){
	projection.setOrthoProjection(-3.0f, 3.0f, -3.0f, 3.0f, -1.0f, 1.0f);
	//view.setOrthoProjection(-3.0f, 3.0f, -3.0f, 3.0f, -1.0f, 1.0f);
}

void AstralEntity::identityMatrix(){

	model.identity();
	//view.identity();
}

void AstralEntity::moveMatrix(float x_value, float y_value, float z_value){
			model.Translate(x_value, y_value, z_value);
		}
void AstralEntity::moveViewMatrix(float x_value, float y_value, float z_value) {
	view.Translate(x_value, y_value, z_value);
}

void AstralEntity::BlendSprite(GLuint& texture) {
		
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture); //Disabling this will result in the most recent image loaded to take over everything
		
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisable(GL_TEXTURE_2D);
		
		}
	
void AstralEntity::setupAndRender(ShaderProgram& program, float vertices[], float texCoords[], GLuint& texture){
			BlendSprite(texture); //Blend first? Why?
			glUseProgram(program.programID);
		
		
		
			glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
			glEnableVertexAttribArray(program.positionAttribute);
		
			glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
			glEnableVertexAttribArray(program.texCoordAttribute);
		
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisableVertexAttribArray(program.positionAttribute);
			glDisableVertexAttribArray(program.texCoordAttribute);
		
		}

void AstralEntity::incrementYPos(float value){
			//YPos += value;
			position.y += value;
		}
void AstralEntity::incrementAccelerationX(float value)
{
	//accelerationX += value;
	acceleration.x += value;
}
void AstralEntity::incrementAccelerationY(float value)
{
	//accelerationY += value;
	acceleration.y += value;
}
float AstralEntity::lerp(float start, float end, float time)
{
	return (1.0 - time) * start + time * end;
}
void AstralEntity::incrementXPos(float value){
			//XPos += value;
			position.x += value;
		}

void AstralEntity::shoot(ShaderProgram& program, Projectile ammo[], std::vector<SpriteSheet>& spriteSheets, float elapsed){
	
	//bullet.identityMatrix();
	//bullet.renderWithNoTexture(program, vertices_ball);
	//bullet.incrementXPos(-0.75 * elapsed);
	//bullet.incrementYPos(0.75 * elapsed);
	//bullet.moveMatrix(0.0, bullet.YPos, 0.0);
	//ammo[ammoIndex].renderWithNoTexture(program);
	//ammo[0].setMatrices(program);
	/*ammo[0].setMatrices(program);
	spriteSheets[1].Draw(program);*/
	//ammo[0].YPos = -1.5;
	int ammoIndex = 0;
	
	ammo[ammoIndex].setMatrices(program);
	ammo[ammoIndex].identityMatrix();
	ammo[ammoIndex].moveMatrix(ammo[ammoIndex].XPos, ammo[ammoIndex].YPos, 0.0);
	
	spriteSheets[1].Draw(program); 
	ammo[ammoIndex].incrementYPos(6.5 * elapsed);
	
	if (ammo[ammoIndex].YPos > 2.0){
		ammoIndex++;
	}
	
	
	/*for (int i = 1; i < 3; i++){
		ammo[i].setMatrices(program);
		ammo[i].incrementYPos(-5.75 * elapsed);
		spriteSheets[3].Draw(program); 
	}*/
	/*ammo[1].setMatrices(program);
	ammo[1].incrementYPos(-5.75 * elapsed);
	spriteSheets[3].Draw(program);
	ammo[2].setMatrices(program);
	ammo[2].incrementYPos(-5.75 * elapsed);
	spriteSheets[3].Draw(program);*/
	//ammo[0].scaleMatrix(20.0, 1.0, 1.0);
	
	if (ammoIndex > maxshots - 1){
		ammoIndex = 0;

	}
	/*ammo[ammoIndex]->renderWithNoTexture(program, ammo[ammoIndex]->vertices);
	ammo[ammoIndex]->identityMatrix();
	ammo[ammoIndex]->incrementYPos(2.0 * elapsed);
	ammo[ammoIndex]->moveMatrix(0.0, ammo[ammoIndex]->YPos, 0.0);*/


}

GLuint AstralEntity::LoadTexture(const char* image_path) {

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


