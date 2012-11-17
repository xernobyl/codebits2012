#include "BGPlanes.h"
#include "Shaders.h"
#include "Quad.h"
#include "Resources.h"
#include "GLWindow.h"


BGPlanes::BGPlanes()
{
	BGRestart();
}


BGPlanes::~BGPlanes()
{
	
}


void BGPlanes::BGUpdate(const float &time)
{
	z = s * time;
}


void BGPlanes::BGRender()
{
	StartRender();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Resources::texture[TX_SAPOS]);

	Shader::Planes::Use();
	Shader::Planes::m((float*)flatcamera.P());
	Shader::Planes::z(z);
	Shader::Planes::S(0);
	Quad::Draw();
}


void BGPlanes::BGRestart()
{
	if(rand() & 1)
		s = -0.25f;
	else
		s = 0.25f;
}
