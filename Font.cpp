#include "Font.h"

#define MAX(V, M) (V > M ? V : M)
#define MIN(V, M) (V < M ? V : M)
#define CLAMP(V, min, max) MIN(MAX(V, min), max)

#include <iostream>

Font::Font()
{
}


Font::Font(const unsigned &texture, const unsigned &texturesize, const int *fontinfo, const unsigned &infosize)
{
	Init(texture, texturesize, fontinfo, infosize);
}


void Font::Init(const unsigned &texture, const unsigned &texturesize, const int *fontinfo, const unsigned &infosize)
{
	this->texture = texture;

	float txsize = 65536.0f / texturesize;
	float fontsize = 1.0f / fontinfo[0];

	spaceadvance = 0.001f * fontinfo[1] * fontsize;
	
	float descender = 0.0f;
	float ascender = 0.0f;

	for(unsigned i = 2; i < infosize; i += 8)
	{
		unsigned idx = fontinfo[i + 0] - 33;

		if(idx <= 126)
		{
			glyph[idx].width = (float)fontinfo[i + 3] * fontsize;
			glyph[idx].height = (float)fontinfo[i + 4] * fontsize;
			glyph[idx].offsetx = 0.001f * fontinfo[i + 5] * fontsize;
			glyph[idx].offsety = 0.001f * fontinfo[i + 6] * fontsize;
			glyph[idx].advance = 0.001f * fontinfo[i + 7] * fontsize;

			glyph[idx].u0 = CLAMP((0.5f + fontinfo[i + 1]) * txsize, 0.0f, 65535.0f);
			glyph[idx].v0 = CLAMP((0.5f + fontinfo[i + 2]) * txsize, 0.0f, 65535.0f);
			glyph[idx].u1 = CLAMP((0.5f + fontinfo[i + 1] + fontinfo[i + 3]) * txsize, 0.0f, 65535.0f);
			glyph[idx].v1 = CLAMP((0.5f + fontinfo[i + 2] + fontinfo[i + 4]) * txsize, 0.0f, 65535.0f);

			if(glyph[idx].offsety > ascender)
				ascender = glyph[idx].offsety;

			if(glyph[idx].offsety - glyph[idx].height < descender)
				descender = glyph[idx].offsety - glyph[idx].height;
		}
	}

	border = glyph[87].offsety - glyph[87].height;

	descender += border;
	ascender -= border;

	lineheight = ascender - descender;
}
