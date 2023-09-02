#ifndef SPRITE_H
#define SPRITE_H

#include "Graphic.h"
#include "./Materials/MaterialDefault.h"
	class Sprite : public Graphic {
	private:
		
	public:
		static const unsigned int _component_id;
		Sprite();
		Sprite(const char* path);
		Sprite( Material* m);
		~Sprite();
		const unsigned int ID() const override;
	};



#endif // !SPRITE_H
