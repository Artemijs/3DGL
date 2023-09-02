#include "Text.h"
#include <stb/stb_image.h>
#include <fstream>
#include "Renderer.h"

 std::vector<CharData>* Text::_allData = new std::vector<CharData>();
Text::Text(Vec3 position, float angle, int fontSize, std::string txt, int maxCharPerLine):
	Graphic("Assets/Fonts/arial.png", position, Vec3 (1,1,1), angle), _text(txt), _maxCharsPerLine(maxCharPerLine), _fontSize(fontSize) {
	if (txt.length() == 0) {
		_text = " ";
	}
	if (_maxCharsPerLine == 0) {
		_maxCharsPerLine = txt.length();
	}

	
	SetFontSize(_fontSize);
	MakeMesh();
	_shader = Renderer::instance()->GetShader(2);
	Text::GetStandardText(this);
}
Text::~Text() {
	_vao->Delete();
	_ebo->Delete();
	_vbo->Delete();
	delete[] _verts;
	delete[] _indices;
	delete _vao;
	delete _vbo;
	delete _ebo;
	if (_allData != NULL) {
		delete _allData;
		_allData = NULL;
	}
}
void Text::CenterText(Vec2 size, int vArrSize) {
	size.x *= 0.5f;
	size.y *= 0.5f;
	for (int i = 0; i < vArrSize; i+=5) {//5
		_verts[i] += -size.x;//x
		_verts[i + 1] -= size.y;//x
	}
}
void Text::SetVert(int charCount, Vec3 pos, char c, GLfloat* verts, Vec2 uvs) {

	verts[charCount] = pos.x;
	charCount++;
	verts[charCount] = pos.y;
	charCount++;
	verts[charCount] = pos.z;
	charCount++;
	//uvs
	verts[charCount] = uvs.x;
	charCount++;
	verts[charCount] = uvs.y;

}
void Text::SetIndices(int topRight, GLuint* indices, int indexCount) {
	//{2, 1, 0}
	indices[indexCount] = topRight + 2;
	indexCount++;
	indices[indexCount] = topRight + 1;
	indexCount++;
	indices[indexCount] = topRight;
	indexCount++;
	//{2, 3, 1}
	indices[indexCount] = topRight + 2;
	indexCount++;
	indices[indexCount] = topRight + 3;
	indexCount++;
	indices[indexCount] = topRight + 1;
}
void Text::GetCd(char c, CharData* cd) {
	CharData space;
	for (int i = 0; i < Text::_allData->size(); ++i) {
		(*cd) = Text::_allData->at(i);
		if ((*cd).id == c) {
			return;
		}
		else if (cd->id == ' ') {
			space = (*cd);
		}
	}
	(*cd) = space;

}

Texture* Text::GetTexture() {
	return Renderer::instance()->GetTexture(_texId);
}
void Text::Init() {
	std::ifstream input("Assets/Fonts/arial.fnt");
	int count = -1;
	float texW = 512;
	float texH = 512;
	for (std::string line; getline(input, line); )
	{
		count++;
		if (count < 4) continue;
		int spos = 0;
		int lastspos = 0;
		int spaceCount = 0;
		CharData cd;
		for (int i = 0; i < line.length(); i++) {
			char c = line[i];
			
			if (c == ' ') {
				spaceCount++;
				lastspos = spos;
				spos = i;
				int start = 0;
				if (spaceCount == 2) {// char id
					for (int j = i; j >= 0; j--) {
						if (line[j] == '=') {
							j += 1;
							cd.id = stoi(line.substr(j, i - j));
							break;
						}
					}
					
				}
				else if (spaceCount == 3) {//x
					for (int j = i; j >= 0; j--) {

						if (line[j] == '=') {
							j += 1;
							cd.x = stoi(line.substr(j, i - j));
							break;
						}
					}
				}
				else if (spaceCount == 4) {//y
					for (int j = i; j >= 0; j--) {

						if (line[j] == '=') {
							j += 1;
							cd.y = stoi(line.substr(j, i - j));
							break;
						}
					}
				}
				else if (spaceCount == 5) {//width
					for (int j = i; j >= 0; j--) {

						if (line[j] == '=') {
							j += 1;
							cd.width = stoi(line.substr(j, i - j));
							break;
						}
					}
				}
				else if (spaceCount == 6) {//height
					for (int j = i; j >= 0; j--) {

						if (line[j] == '=') {
							j += 1;
							cd.height = stoi(line.substr(j, i - j));
							break;
						}
					}
				}
				else if (spaceCount == 7) {//xoff
					for (int j = i; j >= 0; j--) {

						if (line[j] == '=') {
							j += 1;
							cd.xoffset = stoi(line.substr(j, i - j));
							break;
						}
					}
				}
				else if (spaceCount == 8) {//yoff
					for (int j = i; j >= 0; j--) {
						if (line[j] == '=') {
							j += 1;
							cd.yoffset = stoi(line.substr(j, i - j));
							break;
						}
					}
				}
				else if (spaceCount == 9) {//xadvance
					for (int j = i; j >= 0; j--) {
						if (line[j] == '=') {
							j += 1;
							cd.xadvance = stoi(line.substr(j, i - j));
							break;
						}
					}
				}
			}
		}
		//std::cout << " id " << ", " << (char)(cd.id) << ", x: " << cd.x <<"( " <<(cd.x/texW) <<" ), y: " << cd.y << "( "<<(cd.y/texH) <<")\n";
		cd.x /= texW;
		cd.y /= texH;
		cd.width /= texW;
		cd.height /= texH;
		cd.xoffset /= texW;
		cd.yoffset /= texH;
		cd.xadvance /= texW;
		
		Text::_allData->push_back(cd);
	}
}

//0 top left 1 top right 2 bot left 3 bot right
Vec2 Text::GetUV(CharData cd, int corner){
	
	Vec2 v = Vec2(cd.x, cd.y);//top left
	if (corner == 0) {//top left
		
	}
	else if (corner == 1) {//top right
		v.x += cd.width;
	}
	else if (corner == 2) {//bot left
		v.y += cd.height;
	}
	else if (corner == 3) {//bot right
	
		v.x += cd.width;
		v.y += cd.height;
	}
	return v;
}

void Text::BeforeDraw() {
	_shader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(_shader->ID, "transform"), 1, GL_TRUE, &_model.buff[0]);
	
	Renderer::instance()->SetShaderVariables(_shader);
	Renderer::instance()->GetTexture(_texId)->Bind();
	Renderer::instance()->GetTexture(_texId)->texUni(_shader, "tex0", 0);
	glUniform1f(glGetUniformLocation(_shader->ID, "borderWidth"), _borderWidth);
	glUniform2f(glGetUniformLocation(_shader->ID, "offset"), _borderDirection.x, _borderDirection.y);
	glUniform3f(glGetUniformLocation(_shader->ID, "color"), _color.x, _color.y, _color.z);
	glUniform3f(glGetUniformLocation(_shader->ID, "outlineColor"), _borderColor.x, _borderColor.y, _borderColor.z);
	_vao->Bind();
}
void Text::Draw() {
	BeforeDraw();
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
	AfterDraw();
}
void Text::MakeMesh() {
	int linesTotal = std::ceil(_text.length() / (float)_maxCharsPerLine);

	//set up geometry
	int rowCount = linesTotal * 2;
	int colCount = _maxCharsPerLine * 2;
	int vCount = colCount * rowCount;
	//pos + color + uv
	int vSize = (3 + 2);
	const int vArrSize = vCount * vSize;
	_verts = new GLfloat[vArrSize];

	const int indTotal = 6 * (_maxCharsPerLine * linesTotal);
	_indexCount = indTotal;
	_indices = new GLuint[indTotal];///b5 : 1


	float ySpace = 0.17f;
	Vec3 cursor = Vec3(0, 0, -1);
	Vec2 size = Vec2(-9999, 9999);
	for (int j = 0; j < linesTotal; j++) {
		for (int i = 0; i < _maxCharsPerLine; i++) {
			int charId = i + (j * _maxCharsPerLine);
			char c = ' ';
			if (charId < _text.length()) {
				c = _text[charId];
			}
			//vertex id 
			CharData cd;
			GetCd(c, &cd);

			Vec3 pos = Vec3(cursor.x + cd.xoffset, cursor.y - cd.yoffset, -1);
			SetVert(charId * 4 * vSize, pos, c, _verts, GetUV(cd, 0));//top left			0

			pos.x += cd.width;
			SetVert((charId * 4 + 1) * vSize, pos, c, _verts, GetUV(cd, 1));//top right		1

			pos.x -= cd.width;
			pos.y -= cd.height;
			SetVert((charId * 4 + 2) * vSize, pos, c, _verts, GetUV(cd, 2));//bot left		2

			pos.x += cd.width;
			SetVert((charId * 4 + 3) * vSize, pos, c, _verts, GetUV(cd, 3));//bot right		3


			SetIndices(charId * 4, _indices, charId * 6);
										
			cursor.x += (cd.xadvance - (11.0f/512.0f));//10 is the spacing in the png file and 512 is its size
			//cursor.x += cd.xadvance;//10 is the spacing in the png file and 512 is its size

			if (pos.x > size.x)
				size.x = pos.x;
			if (pos.y < size.y) {
				size.y = pos.y;
			}
		}
		cursor.y -= ySpace;
		cursor.x = 0;
	}
	//size.y = linesTotal * ySpace;
	CenterText(size, vArrSize);
	_vao = new VAO();
	_vao->Bind();

	_vbo = new VBO(_verts, sizeof(_verts) * vArrSize);
	_ebo = new EBO(_indices, sizeof(_indices) * indTotal);
	_vao->LinkAttrib(_vbo, 0, 3, GL_FLOAT, vSize * sizeof(float), (void*)0);//positions
	_vao->LinkAttrib(_vbo, 1, 2, GL_FLOAT, vSize * sizeof(float), (void*)(3 * sizeof(float)));//uvs 
	_vao->Unbind();
	_vbo->Unbind();
	_ebo->Unbind();
}
void Text::SetFontSize(int size) {
	float baseSize = 50;
	float scaler = 20;
	_fontSize = size;
	_transform._scale = Vec3(baseSize + size * scaler, baseSize + size * scaler, 1);
}
void Text::SetColor(Vec3 col) { _color = col; }
Vec3 Text::GetColor() { return _color; }
void Text::SetOutlineColor(Vec3 col) { _borderColor = col; }
Vec3 Text::GetOutlineColor() { return _borderColor; }
void Text::SetOutlineWidth(float w) { _borderWidth = w; }
float Text::GetOutlineWidth() { return _borderWidth; }
void Text::SetBorderDirection(Vec2 dir) { _borderDirection = dir; }
Vec2 Text::GetBorderDirection() { return _borderDirection; }

void Text::GetStandardText(Text* t) {
	t->SetColor(Vec3(1, 1, 1));
	t->SetOutlineColor(Vec3(0, 0, 0));
	t->SetOutlineWidth(0.6f);
	t->SetBorderDirection(Vec2(0, 0));
}
void Text::GetDarkText(Text* t) {
	t->SetColor(Vec3(0.1f, 0.1f, 0.1f));
	t->SetOutlineColor(Vec3(1, 1, 1));
	t->SetOutlineWidth(0.6f);
	t->SetBorderDirection(Vec2(0, 0));
}