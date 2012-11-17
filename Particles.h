#pragma once

#include "GLWindow.h"
#include "Camera.h"


class Particles
{
public:

	Particles(const unsigned &n);
	~Particles();

	void Update(Camera &camera);
	void Draw(Camera &camera);

	void Populate();

private:

	unsigned vbo;
	unsigned nparticles;
	unsigned maxparticles;

	struct Particle
	{
		vec3 position;
		vec3 color;
		float distance;	// for sorting
		float age;
		float rotation;
	};

	struct Particle *particlebuffer;
	vec4 *tmpbuffer;	// unneeded if using map buffer
};
