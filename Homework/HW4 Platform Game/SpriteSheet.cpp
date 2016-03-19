#include "Matrix.h"
#include "ShaderProgram.h"
#include "SpriteSheet.h"





SpriteSheet::SpriteSheet()
{
	active = true;
}

SpriteSheet::SpriteSheet(unsigned int texID, float u_var, float v_var, float width_var, float height_var, float
	size_var){
	textureID = texID;
	u = u_var;
	v = v_var;
	width = width_var;
	height = height_var;
	size = size_var;
	active = true;
	//program = shade;

}

void SpriteSheet::BlendSprite(GLuint& texture) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture); //Disabling this will result in the most recent image loaded to take over everything

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);

}
void SpriteSheet::setupAndRender(ShaderProgram& program, float vertices[], float texCoords[], GLuint& texture){
	BlendSprite(texture);//Blend first? Why?
	glUseProgram(program.programID);



	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);

	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);


}

void SpriteSheet::Draw(ShaderProgram& program) {
	//glBindTexture(GL_TEXTURE_2D, textureID);
	if (active) {
		GLfloat texCoords[] = {
			u, v + height,
			u + width, v,
			u, v,
			u + width, v,
			u, v + height,
			u + width, v + height
		};
		float aspect = width / height;
		float vertices[] = {
			-0.5f * size * aspect, -0.5f * size,
			0.5f * size * aspect, 0.5f * size,
			-0.5f * size * aspect, 0.5f * size,
			0.5f * size * aspect, 0.5f * size,
			-0.5f * size * aspect, -0.5f * size,
			0.5f * size * aspect, -0.5f * size };
		// draw our arrays
		setupAndRender(program, vertices, texCoords, textureID);
	}
}


void SpriteSheet::incrementYPos(float value){
	YPos += value;
}