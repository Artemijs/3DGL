#include "MaterialButton.h"
#include "../Renderer.h"
MaterialButton::MaterialButton():
	Material(Renderer::instance()->GetShader(1), "Assets/Textures/btn.png"),
	_texScale(Vec2(1, 0.25f)),
	_uvOffset(Vec2(0, 3)){
	//_shader = Renderer::instance()->GetShader(0);
}
MaterialButton::MaterialButton(const char* texturePath): 
	Material(Renderer::instance()->GetShader(1), texturePath),
	_texScale(Vec2(1, 0.25f)),
	_uvOffset( Vec2(0, 3)){

}
MaterialButton::~MaterialButton() {
	printf("deleting default material class\n");
}
void MaterialButton::Bind(const Matrix4x4* model)const  {
	_shader->Activate();
	glUniform2f(glGetUniformLocation(_shader->ID, "uvOff"), _uvOffset.x, _uvOffset.y);
	glUniform2f(glGetUniformLocation(_shader->ID, "uv_scale"), _texScale.x, _texScale.y);
	glUniformMatrix4fv(glGetUniformLocation(_shader->ID, "model"), 1, GL_TRUE, &model->buff[0]);
	Renderer::instance()->SetShaderVariables(_shader->ID);
	_texture->Bind();
	_texture->texUni(_shader, "tex0", 0);
	Renderer::instance()->GetVAO()->Bind();
}

void MaterialButton::Unbind()const {
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}