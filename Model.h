#pragma once

#include "GLWindow.h"


class Model
{
public:

	~Model();
	bool Load(const char *filepath);

	void Draw();

private:

	unsigned vertexbuffersize, indexbuffersize;
	//void *vertexbuffer, *indexbuffer;
	GLenum indextype, vertextype;
	unsigned stride, normaloffset, uvoffset;
	unsigned vbo, ibo;
	bool hastextures, hasnormals;
};
