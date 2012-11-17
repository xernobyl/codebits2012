#pragma once

#include "GLWindow.h"


class Font
{
public:

	friend class Text;

	Font();
	Font(const unsigned &texture, const unsigned &texturesize, const int *fontinfo, const unsigned &infosize);
	unsigned Texture() const{ return texture; };
	
	void Init(const unsigned &texture, const unsigned &texturesize, const int *fontinfo, const unsigned &infosize);

	float LineHeight() const{ return lineheight; };
	float Border() const{ return border; };

protected:

	struct Glyph
	{
		float width;
		float height;
		float offsetx;
		float offsety;
		float advance;
	
		unsigned short u0;
		unsigned short v0;
		unsigned short u1;
		unsigned short v1;
	};

	unsigned texture;
	float spaceadvance;
	float lineheight;
	float border;

	Glyph glyph[94];
};
