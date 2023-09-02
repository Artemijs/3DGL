#include "Renderer.h"
#include <iostream>
#include "SDL_mixer.h"
//#include "../UI/RenderNode.h"
#include "../Graphics/Sprite.h"
#include "../Game/Game.h"

Renderer* Renderer::_instance = 0x0;
Renderer* Renderer::instance() {
	if (_instance == 0x0) {
		_instance = new Renderer();
	}
	return _instance;
}


Renderer::Renderer() {

	_windowSize = Vec2(800, 800);
	CreateWindow();
	CreateGeometry();
	_projection = Matrix4x4::Ortho(0.0f, _windowSize.x, 0.0f, _windowSize.y, 0.1f, 1000.0f);
	_all_graphics = new std::vector<Graphic*>();
	_all_shaders = new std::vector<Shader*>();
	_all_shaders->push_back(new Shader("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"));
	_all_shaders->push_back(new Shader("Assets/Shaders/animated_sprite.vert", "Assets/Shaders/animated_sprite.frag"));
	_all_shaders->push_back(new Shader("Assets/Shaders/outlineText.vert", "Assets/Shaders/outlineText.frag"));
	_all_shaders->push_back(new Shader("Assets/Shaders/final_rect.vert", "Assets/Shaders/final_rect.frag"));
	//_all_shaders->push_back();

	_all_textures = new std::vector<std::pair<const char*, Texture*>>();

	_fbo = new FBO(_windowSize.x, _windowSize.y);

	_fRect = new FinalRect();


}
Renderer::~Renderer() {

	for (int i = 0; i < _all_shaders->size(); ++i) {
		_all_shaders->at(i)->Delete();
		delete _all_shaders->at(i);
	}
	delete _all_shaders;

	for (int i = 0; i < _all_textures->size(); ++i) {
		_all_textures->at(i).second->Delete();
		delete _all_textures->at(i).second;
	}
	delete _all_textures;

	_vao->Delete();
	_vbo->Delete();
	_ebo->Delete();
	delete _vao;
	delete _vbo;
	delete _ebo;
	delete _fbo;
	delete _all_graphics;
	Mix_CloseAudio();
	glfwDestroyWindow(_window);
	glfwTerminate();
	delete _fRect;

}

void Renderer::CreateGeometry() {
	_vao = new VAO();
	_vao->Bind();

	_vbo = new VBO(_vertices, sizeof(_vertices));
	_ebo = new EBO(_indices, sizeof(_indices));

	_vao->LinkAttrib(_vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	_vao->LinkAttrib(_vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	_vao->LinkAttrib(_vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	_vao->Unbind();
	_vbo->Unbind();
	_ebo->Unbind();
}
void Renderer::CreateWindow() {
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	_window = glfwCreateWindow(_windowSize.x, _windowSize.y, "SOON_TM", NULL, NULL);
	if (_window == NULL) {

		std::cout << "failed to create window \n";
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(_window);
	gladLoadGL();
	glViewport(0, 0, _windowSize.x, _windowSize.y);
	glEnable(GL_DEPTH_TEST);


}

VAO* Renderer::GetVAO() {
	return _vao;
}
GLFWwindow* Renderer::GetWindow() { return _window; }
Vec2 Renderer::GetWindowSize() { return _windowSize; }
const Vec3 Renderer::WindowSizeVec3() { return Vec3(_windowSize.x, _windowSize.y, 1.0f); }
/// <summary>
/// set projection and camera view matrix
/// </summary>
/// <param name="s"></param>
void Renderer::SetShaderVariables(const unsigned int shaderID) {
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "proj"), 1, GL_TRUE, &_projection.buff[0]);

}
Shader* Renderer::GetShader(int id) {
	return _all_shaders->at(id);
}
Texture* Renderer::GetTexture(int id) {
	return _all_textures->at(id).second;
}
const Texture* Renderer::LoadTexture(const char* path) {
	//check if exissts
	for (int i = 0; i < _all_textures->size(); ++i) {
		const char* p = _all_textures->at(i).first;

		for (int j = 0; p[j] == path[j]; j++) {
			//if strings are identical
			if (p[j] == '\0' && path[j] == '\0') {
				//dont need to add it because it already exists
				return _all_textures->at(i).second;
			}
		}
	}
	//add new if not exists
	Texture* t = new Texture(path, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	_all_textures->push_back(std::pair<const char*, Texture*>{path, t});
	return t;
}



/*
	root{
			sprite
			rn1{
				sprite
				sprtie
			}
			rn2{
				sprite
				sprite

				rn3{
					sprite
				}
			}
		}

*/

#include "../Util/Utility.h"
#include "../Game/FBOComponent.h"
void Renderer::DrawNodes(BaseNode* node, BaseNode* parent) {

	bool renderNode = Utility::IsRenderNode(node);
	bool isRoot = (node->GetParent() == NULL);
	//RenderNode* rn = NULL;
	const FBOComponent* fbo = NULL;


	if (renderNode) {
		//unbind prev fbo
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//bind node fbo
		//rn = node->GetComponent<RenderNode>();
		fbo = node->GetComponent<FBOComponent>(FBOComponent::_component_id);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->_fbo);
		if (isRoot) {
			glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glClearColor(0.2f, 0.13f, 0.17f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
		}
	}

	const std::vector<BaseNode*>* children = node->GetAllChildren();
	if (children != NULL) {
		for (int i = 0; i < children->size(); ++i) {
			//WHAT IF A CHILD IS A RENDER NODE ?????????????????????????
			//children->at(i)->GetComponent<Graphic>()->TryDraw();
			DrawNodes(children->at(i), node);
		}
	}

	if (isRoot) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//DRAW ROOT TO FINLal RECT

		_fRect->Bind();
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, fbo->_fboTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		_fRect->Unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		
		//check if it is a drawable type 
		//check if node has a graphic component 
		//if (node->GetNodeType() == 1) {
		if(!renderNode){
			auto  comps = node->Components();
			for (int i = 0; i < comps->size(); ++i) {
				if (comps->at(i)->second->IsGraphic()) {
					dynamic_cast<Graphic*>(comps->at(i)->second)->Draw(node->GetModelMatrix());
				}
			}
			/*
			const Graphic* g = node->GetGraphic();

			//if it does, cast it to graphic and draw it
			if (g != NULL) {
				g->Draw(node->GetModelMatrix());
			}*/
		}
		else if (renderNode) {
			//unbind prev FBO
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//ifi t is a render node but not the root node bind the FBO of the last FBO node
			glBindFramebuffer(GL_FRAMEBUFFER, parent->GetComponent<FBOComponent>(FBOComponent::_component_id)->_fbo);
			//bind vao 
			_vao->Bind();
			//activate shader	USING DEFAULT FOR NOW
			Shader* s = _all_shaders->at(0);
			s->Activate();
			//set shader vars
				//model projecttion texture
			Renderer::SetShaderVariables(s->ID);
			glUniformMatrix4fv(glGetUniformLocation(s->ID, "model"), 1, GL_TRUE, node->GetModelMatrix()->buff);
			//bind texture from FBO
			unsigned int tId = node->GetComponent<FBOComponent>(FBOComponent::_component_id)->_fboTexture;
			//unsigned int tId = Game::_testG->GetTexture()->ID;
			glBindTexture(GL_TEXTURE_2D, tId);
			//maybe this part is optioNAL because in the example above he doesnt use this
			//glUniform1i(glGetUniformLocation(s->ID, "tex0"), rn->GetFBO()->_fboTex); //maybe this part is optioNAL
			//glUniform1i(glGetUniformLocation(s->ID, "tex0"), tId); //maybe this part is optioNAL

			//draw triangles
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			//unbind vao
			_vao->Unbind();
			//deactivate shader
			glUseProgram(0);
			//unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}
