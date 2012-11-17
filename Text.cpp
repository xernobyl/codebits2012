#include "Text.h"
#include <vector>
#include <string.h>
#include <glm/gtx/transform.hpp>


Text::Text(const Font *font, const char *string, const unsigned &len, const float &size)
{
	float minx, maxx, miny, maxy;
	maxx = minx = 0.5f;
	miny = maxy = -0.5f;

	this->font = font;

	char *vertexbuffer = new char[(sizeof(float) * 2 + sizeof(unsigned short) * 2) * 4 * len];

	union
	{
		char *u8;
		unsigned short *u16;
		float *f32;
	} vertexp;

	vertexp.u8 = vertexbuffer;

	unsigned short *indexbuffer = new unsigned short[6 * len];
	unsigned short *indexp = indexbuffer;
	unsigned vertexsize = 0;
	indexsize = 0;

	maxlength = 0.0f;
	height = font->LineHeight();
	float cursor = 0.0f;
	
	for(unsigned i = 0; i < len; ++i)
	{
		if(string[i] == '\0')
			break;
		else if(string[i] == '\n')	// new line
		{
			if(cursor > maxlength)
				maxlength = cursor;

			cursor = 0.0f;
			height += font->LineHeight();
		}
		else if(string[i] == '\t')	// add 4 spaces
			cursor += 4.0f * font->spaceadvance;
		else if(string[i] == ' ')	// add a space
			cursor += font->spaceadvance;
		else if(string[i] >= 33 && string[i] <= 126)	// add a glyph
		{
			unsigned idx = string[i] - 33;
			float x0 = font->glyph[idx].offsetx + cursor;
			float y0 = font->glyph[idx].offsety - height;
			float x1 = x0 + font->glyph[idx].width;
			float y1 = y0 - font->glyph[idx].height;

			unsigned short u0 = font->glyph[idx].u0;
			unsigned short v0 = font->glyph[idx].v0;
			unsigned short u1 = font->glyph[idx].u1;
			unsigned short v1 = font->glyph[idx].v1;

			if(x1 > maxx)
				maxx = x1;
			if(x0 < minx)
				minx = x0;
			if(y0 > maxy)
				maxy = y0;
			if(y1 < miny)
				miny = y1;

			cursor += font->glyph[idx].advance;

			*indexp++ = vertexsize + 0;
			*indexp++ = vertexsize + 1;
			*indexp++ = vertexsize + 2;

			*indexp++ = vertexsize + 2;
			*indexp++ = vertexsize + 1;
			*indexp++ = vertexsize + 3;

			*vertexp.f32++ = x0;
			*vertexp.f32++ = y0;
			*vertexp.u16++ = u0;
			*vertexp.u16++ = v0;

			*vertexp.f32++ = x0;
			*vertexp.f32++ = y1;
			*vertexp.u16++ = u0;
			*vertexp.u16++ = v1;

			*vertexp.f32++ = x1;
			*vertexp.f32++ = y0;
			*vertexp.u16++ = u1;
			*vertexp.u16++ = v0;

			*vertexp.f32++ = x1;
			*vertexp.f32++ = y1;
			*vertexp.u16++ = u1;
			*vertexp.u16++ = v1;

			indexsize += 6;
			vertexsize += 4;
		}
	}

	if(cursor > maxlength)
		maxlength = cursor;

	// add bounding box

	*indexp++ = vertexsize + 0;
	*indexp++ = vertexsize + 1;
	*indexp++ = vertexsize + 2;

	*indexp++ = vertexsize + 2;
	*indexp++ = vertexsize + 1;
	*indexp++ = vertexsize + 3;

	*vertexp.f32++ = minx + ((float)rand() / RAND_MAX - 0.5f) + font->Border();
	*vertexp.f32++ = maxy + ((float)rand() / RAND_MAX - 0.5f) - font->Border();
	*vertexp.u16++ = 0;
	*vertexp.u16++ = 0;

	*vertexp.f32++ = minx + ((float)rand() / RAND_MAX - 0.5f) + font->Border();
	*vertexp.f32++ = miny + ((float)rand() / RAND_MAX - 0.5f) + font->Border();
	*vertexp.u16++ = 0;
	*vertexp.u16++ = 0;

	*vertexp.f32++ = maxx + ((float)rand() / RAND_MAX - 0.5f) - font->Border();
	*vertexp.f32++ = maxy + ((float)rand() / RAND_MAX - 0.5f) - font->Border();
	*vertexp.u16++ = 0;
	*vertexp.u16++ = 0;

	*vertexp.f32++ = maxx + ((float)rand() / RAND_MAX - 0.5f) - font->Border();
	*vertexp.f32++ = miny + ((float)rand() / RAND_MAX - 0.5f) + font->Border();
	*vertexp.u16++ = 0;
	*vertexp.u16++ = 0;

	vertexsize += 4;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexsize * (sizeof(float) * 2 + sizeof(unsigned short) * 2), vertexbuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indexsize + 6) * sizeof(unsigned short), indexbuffer, GL_STATIC_DRAW);

	delete [] indexbuffer;
	delete [] vertexbuffer;

	position[0] = -0.5f * maxlength;
	position[1] = 0.5f * height;
	this->size = size;

	Update();
	
	if(len != strlen(string))	// DELETE!
		puts(string);
}


Text::~Text()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}


void Text::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 12, (void*)0);	// vertex
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, 12, (void*)8);	// texture coordinate
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, indexsize, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(1);
}


void Text::DrawBox()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 12, (void*)0);	// vertex
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, 12, (void*)8);	// texture coordinate
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(indexsize * 2));

	glDisableVertexAttribArray(1);
}


void Text::Update()
{
	m = scale(size, size, 1.0f);
	m = translate(m, position[0], position[1], 0.0f);
}