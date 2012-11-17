#include "BGTunnel.h"
#include "Shaders.h"
#include "Quad.h"
#include "Resources.h"
#include "GLWindow.h"


BGTunnel::BGTunnel()
{
	BGRestart();
}


BGTunnel::~BGTunnel()
{
	
}


void BGTunnel::BGUpdate(const float &time)
{
	z = s * time;
}


void BGTunnel::BGRender()
{
	StartRender();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Resources::texture[TX_SAPOS]);

	Shader::Tunnel::Use();
	Shader::Tunnel::m((float*)flatcamera.P());
	Shader::Tunnel::z(z);
	Shader::Tunnel::S(0);
	Quad::Draw();
}


void BGTunnel::BGRestart()
{
	if(rand() & 1)
		s = -0.25f;
	else
		s = 0.25f;
}
