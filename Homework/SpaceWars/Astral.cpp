#include "ShaderProgram.h"
#include "Matrix.h"
#include "Projectile.h"
#include "Astral.h"
#include <vector>
#include "SpriteSheet.h"


AstralEntity::AstralEntity(Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix) {
	XPos = 0.0;
	YPos = 0.0;
	HDirection = 1.0;
	VDirection = 1.0;
	health = 100.0;
	model = modelMatrix;
	projection = projectionMatrix;
	view = viewMatrix;
	score = 0;
	ammoIndex = 0;
	maxshots = 10;
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
	view = viewMatrix;
	score = 0;
	ammoIndex = 0;
	maxshots = 10;
	texID = spritez.textureID;
	//sprite = spritez;


}
void AstralEntity::DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing){
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

bool AstralEntity::lifeChecker(){
	return health > 0.0;

}

void AstralEntity::setMatrices(ShaderProgram* program){
	program->setModelMatrix(model); program->setProjectionMatrix(projection); program->setViewMatrix(view);
}

void AstralEntity::setOrthoProjection(){
	projection.setOrthoProjection(-2.777f, 2.777f, -2.0f, 2.0f, -1.0f, 1.0f);
}

void AstralEntity::identityMatrix(){

	model.identity();
}

void AstralEntity::moveMatrix(float x_value, float y_value, float z_value){
			model.Translate(x_value, y_value, z_value);
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
			YPos += value;
		}
void AstralEntity::incrementXPos(float value){
			XPos += value;
		}

void AstralEntity::renderWithNoTexture(ShaderProgram* program, float vertices[]){

		    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
			glEnableVertexAttribArray(program->positionAttribute);
	
	
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisableVertexAttribArray(program->positionAttribute);
}
void AstralEntity::shoot(ShaderProgram* program, Projectile ammo[], float elapsed){
	
	//bullet.identityMatrix();
	//bullet.renderWithNoTexture(program, vertices_ball);
	//bullet.incrementXPos(-0.75 * elapsed);
	//bullet.incrementYPos(0.75 * elapsed);
	//bullet.moveMatrix(0.0, bullet.YPos, 0.0);
	//ammo[ammoIndex].renderWithNoTexture(program);
	ammo[0].incrementYPos(8.5 * elapsed);
	ammo[1].incrementYPos(-9.5 * elapsed);
	//ammo[0].scaleMatrix(20.0, 1.0, 1.0);
	ammoIndex++;
	if (ammoIndex > maxshots - 1){
		ammoIndex = 0;

	}
	/*ammo[ammoIndex]->renderWithNoTexture(program, ammo[ammoIndex]->vertices);
	ammo[ammoIndex]->identityMatrix();
	ammo[ammoIndex]->incrementYPos(2.0 * elapsed);
	ammo[ammoIndex]->moveMatrix(0.0, ammo[ammoIndex]->YPos, 0.0);*/


}
