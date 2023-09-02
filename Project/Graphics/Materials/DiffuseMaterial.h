#ifndef DIFFUSE_MATERIAL_H
#define DIFFUSE_MATERIAL_H
#include "../../Math/Matrix4x4.h"
#include "../ShaderClass.h"
#include "../Texture.h"
class DiffuseMaterial{

protected:
	const Shader* _shader;
	const Texture* _texture;
public :
	DiffuseMaterial();
	DiffuseMaterial(const Shader* s, const char* texturePath);
	virtual ~DiffuseMaterial();
	void Bind(const Matrix4x4* model) const override;
	void Unbind() const override;
	const Texture* GetTexture() const;
};

#endif
