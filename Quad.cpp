#include "Quad.h"
#include "GLWindow.h"


namespace Quad
{
	static unsigned quadvbo;

	static const signed char quad[] =
	{
		-128, 2,
		0, -128,
		127, 2
	};

	void Init()
	{
		glGenBuffers(1, &quadvbo);

		glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(char) * 2 * 3, quad, GL_STATIC_DRAW);
	}

	void Draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
		glVertexAttribPointer(0, 2, GL_BYTE, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void Kill()
	{
		glDeleteBuffers(1, &quadvbo);
	}
}
