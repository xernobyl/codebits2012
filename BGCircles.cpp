#include "BGCircles.h"
#include "Shaders.h"
#include <glm/gtx/euler_angles.hpp>


BGCircles::BGCircles()
{
	ncircles = 96;

	char *vertexbuffer = new char[ncircles * (2 * 3 + 3)];

	union
	{
		__fp16 *h;
		char *b;
	} buffer;
	
	buffer.b = vertexbuffer;

	for(unsigned i = 0; i < ncircles; ++i)
	{
		float r = sqrt(1.0f / RAND_MAX * rand());
		float t = 6.283185307179586476925286766559f / RAND_MAX * rand();
		float p = acos(2.0f / RAND_MAX * rand() - 1.0f);

		*buffer.h++ = r * cos(t) * sin(p);
		*buffer.h++ = r * sin(t) * sin(p);
		*buffer.h++ = r * cos(p);

		*buffer.b++ = rand() % 256;
		*buffer.b++ = rand() % 256;
		*buffer.b++ = rand() % 256;
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, ncircles * (2 * 3 + 3), vertexbuffer, GL_STATIC_DRAW);

	delete [] vertexbuffer;
	
	camera.AspectRatio(Width(), Height());
	camera.FocalLength(35.0f);
	camera.UpdateProjection();
	
	BGRestart();
}


BGCircles::~BGCircles()
{
	glDeleteBuffers(1, &vbo);
}


void BGCircles::BGUpdate(const float &time)
{
	float p = 0.25f * time;
	
	camera.LookAt(vec3(1.0f * cos(p), 0.0f, 1.0f * sin(p)), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	t = m * *camera.MVP();
}


void BGCircles::BGRender()
{
	StartRender();

	glClearColor(cc[0], cc[1], cc[2], cc[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	Shader::Circle::Use();
	//Shader::Circle::m((float*)camera.MVP());
	Shader::Circle::m((float*)&t);
	Shader::Circle::zs(0.125f * (float)Height() / camera.TanHalfFOV());

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_HALF_FLOAT_OES, GL_FALSE, 9, (void*)0);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 9, (void*)6);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_POINTS, 0, ncircles);

	glDisableVertexAttribArray(1);
	glDisable(GL_BLEND);
}


void BGCircles::BGRestart()
{
	cc[0] = 1.0f / RAND_MAX * rand();
	cc[1] = 1.0f / RAND_MAX * rand();
	cc[2] = 1.0f / RAND_MAX * rand();
	cc[3] = 1.0f / RAND_MAX * rand();

	m = eulerAngleZ(6.283185307179586476925286766559f / RAND_MAX * rand());

	Shader::Circle::Use();
	Shader::Circle::c(1.0f / RAND_MAX * rand(), 1.0f / RAND_MAX * rand(), 1.0f / RAND_MAX * rand());
}
