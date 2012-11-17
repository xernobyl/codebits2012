#include "Particles.h"
#include "Shaders.h"


Particles::Particles(const unsigned &n)
{
	maxparticles = n;
	nparticles = 0;

	particlebuffer = new struct Particle[maxparticles];
	tmpbuffer = new vec4[maxparticles];

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * maxparticles, 0, GL_DYNAMIC_DRAW);
}


Particles::~Particles()
{
	delete [] particlebuffer;
	delete [] tmpbuffer;
}


void Particles::Update(Camera &camera)
{
	if(nparticles)
	{
		particlebuffer[0].distance = distance(particlebuffer[0].position, camera.Position());

		for(unsigned i = 1; i < nparticles; ++i)
		{
			particlebuffer[i].distance = distance(particlebuffer[i].position, camera.Position());

			if(particlebuffer[i - 1].distance < particlebuffer[i].distance)
			{
				struct Particle tmp;
				tmp = particlebuffer[i];
				particlebuffer[i] = particlebuffer[i - 1];
				particlebuffer[i - 1] = tmp;
			}
		}

		// if sub data works TRY mapping buffers (should be faster!)

		for(unsigned i = 1; i < nparticles; ++i)
			tmpbuffer[i] = vec4(particlebuffer[i].position, particlebuffer[i].rotation);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, nparticles * sizeof(vec4), tmpbuffer);
	}
}


void Particles::Draw(Camera &camera)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	Shader::Point::Use();
	Shader::Point::m((float*)camera.MVP());
	Shader::Point::nm((float*)camera.MV());
	Shader::Point::zs(0.5f * (float)GLWindow::ScreenHeight() / camera.TanHalfFOV(), 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);	
	glDrawArrays(GL_POINTS, 0, nparticles);

	glDisable(GL_BLEND);
}


void Particles::Populate()
{
	nparticles = maxparticles;

	for(unsigned i = 0; i < nparticles; ++i)
	{
		float a = 6.283185307179586476925286766559f * rand() / RAND_MAX;
		float b = 2.0f * rand() / RAND_MAX - 1.0f;

		particlebuffer[i].position = vec3(sqrt(1.0f - b * b) * cos(a), sqrt(1.0f - b * b) * sin(a), b);
		particlebuffer[i].color = vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		particlebuffer[i].age = 0.0f;
		particlebuffer[i].rotation = 6.283185307179586476925286766559f * rand() / RAND_MAX;
	}	
}
