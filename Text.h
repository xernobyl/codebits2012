#pragma once

#include "Font.h"


class Text
{
public:

	Text(const Font *font, const char *string, const unsigned &strlen, const float &size);
	~Text();

	void Draw();
	void DrawBox();

	unsigned Width() const{ return maxlength; };
	unsigned Height() const{ return height; };
	
	unsigned Texture() const{ return font->Texture(); };
	mat4 *Matrix(){ return &m; };
	
	void Size(const float &s){ size = s; };

	void Update();

private:

	unsigned vbo, ibo;
	float maxlength;
	float height;
	unsigned indexsize;
	const Font *font;
	
	float position[2];
	float size;
	mat4 m;
};
