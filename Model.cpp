#include "Model.h"
#include <iostream>


Model::~Model()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}


bool Model::Load(const char *filepath)
{
	union
	{
		void *v;
		char *u8;
		unsigned short *u16;
		float *f;
		__fp16 *h;
	} file;
	
	unsigned filesize;
	
	char *_file = GLWindow::LoadFile(filepath, &filesize);
	file.u8 = _file;

	if(filesize < 5)
	{
		delete [] _file;
		return false;
	}

	char flags = *file.u8++;

	indexbuffersize = *file.u16++;
	vertexbuffersize = *file.u16++;

	if(vertexbuffersize <= 256)
		indextype = GL_UNSIGNED_BYTE;
	else
		indextype = GL_UNSIGNED_SHORT;

	switch(flags & 3)
	{
	case 3:		// half!
		vertextype = GL_HALF_FLOAT_OES;
		normaloffset = uvoffset = stride = 3 * 2;
		break;

	case 2:		// u16!
		vertextype = GL_UNSIGNED_SHORT;
		normaloffset = uvoffset = stride = 3 * 2;
		break;
	
	default:	// float!
		vertextype = GL_FLOAT;
		normaloffset = uvoffset = stride = 3 * 4;
	}

	if(flags & 0x80)	// no normals!
		hasnormals = false;
	else
	{
		hasnormals = true;
		stride += 3;
		uvoffset += 3;
	}

	if(flags & 0x40)	// no texture coordinates!
		hastextures = false;
	else
	{
		hastextures = true;
		stride += 2 * 2;
	}

	unsigned truelength = 5 + indexbuffersize * (indextype == GL_UNSIGNED_BYTE ? 1 : 2) + stride * vertexbuffersize;

	if(filesize != truelength)
	{
		delete [] _file;
		return false;
	}

	char *indexbuffer = file.u8;
	char *vertexbuffer = file.u8 + indexbuffersize * (indextype == GL_UNSIGNED_BYTE ? 1 : 2);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexbuffersize * stride, (void*)vertexbuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexbuffersize * (indextype == GL_UNSIGNED_BYTE ? 1 : 2), (void*)indexbuffer, GL_STATIC_DRAW);

	delete [] _file;

	return true;
}


void Model::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glVertexAttribPointer(0, 3, vertextype, GL_FALSE, stride, 0);

	if(hastextures)
	{
		glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, stride, (void*)uvoffset);
		glEnableVertexAttribArray(1);
	}

	if(hasnormals)
	{
		glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)normaloffset);
		glEnableVertexAttribArray(2);
	}

	glDrawElements(GL_TRIANGLES, indexbuffersize, indextype, 0);

	if(hasnormals)
		glDisableVertexAttribArray(2);
	if(hastextures)
		glDisableVertexAttribArray(1);
}
