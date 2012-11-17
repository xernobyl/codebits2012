#include "Cube.h"
#include "GLWindow.h"


namespace Cube
{
	static unsigned cubevbo, cubeabo;

	static const signed char cube[] =
	{
		-1, -1, -1, 0, 0, -128,
		1, -1, -1, 0, 0, -128,
		1, 1, -1, 0, 0, -128,

		1, -1, -1, 0, 0, -128,
		1, 1, -1, 0, 0, -128,
		-1, 1, -1, 0, 0, -128,
	};

	void Init()
	{
		glGenBuffers(1, &quadvbo);

		glBindBuffer(GL_ARRAY_BUFFER, cubevbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(char) * 8 * 3, quad, GL_STATIC_DRAW);
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
