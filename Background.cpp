#include "Background.h"
#include "Shaders.h"
#include "Quad.h"
#include "Resources.h"
#include <vector>


FlatCamera Background::flatcamera;

std::vector<Background*> Background::vector;
unsigned Background::width, Background::height;
unsigned Background::framebuffertexture[3], Background::framebuffer[3], Background::depthrenderbuffer;
unsigned Background::currentbackground;
unsigned Background::currentbuffer;

uint_fast64_t Background::duration;
uint_fast32_t Background::fadelength;
float Background::fadetime;
unsigned Background::fadefx;
bool Background::fading;


unsigned Background::Width()
{
	return width;
}


unsigned Background::Height()
{
	return height;
}


bool Background::Init(const unsigned &w, const unsigned &h)
{
	currentbuffer = 0;
	currentbackground = 0xffffffff;
	fadefx = 0;

	fadelength =	4000000000u;
	duration =		40000000000ull;

	width = w;
	height = h;

	glGenTextures(3, framebuffertexture);

	glBindTexture(GL_TEXTURE_2D, framebuffertexture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, framebuffertexture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, framebuffertexture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);	// DEPTH_COMPONENT24_OES

	glGenFramebuffers(3, framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffertexture[0], 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffertexture[1], 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[2]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffertexture[2], 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	flatcamera.AspectRatio((float)width / height);

	return true;
}


void Background::Kill()
{
	glDeleteFramebuffers(3, framebuffer);
	glDeleteRenderbuffers(1, &depthrenderbuffer);
	glDeleteTextures(3, framebuffertexture);

	for(unsigned i = 0; i < vector.size(); ++i)
		delete vector[i];

	vector.clear();
}


void Background::Add(Background *bg)
{
	vector.push_back(bg);

	if(vector.size() == 1)
		currentbackground = 0;
}


void Background::StartRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[currentbuffer]);
	glViewport(0, 0, width, height);
}


float pos[2], rotation;


void Background::Update(uint_fast64_t ns)
{
	if(currentbackground == 0xffffffff)
		return;

	static unsigned lastt = 0xffffffff;
	unsigned t = ns / duration;
	ns = ns % duration;

	if(t != lastt)
	{
		rotation = 6.283185307179586476925286766559f / RAND_MAX * rand();
		pos[0] = 2.0f / RAND_MAX * rand() - 1.0f;
		pos[1] = 2.0f / RAND_MAX * rand() - 1.0f;
		
		lastt = t;
		fadefx = rand() % 2;
		currentbackground = rand() % vector.size();
		currentbuffer = !currentbuffer;
		vector[currentbackground]->BGRestart();
	}

	fading = (ns <= fadelength);
	fadetime = (float) ns / fadelength;

	flatcamera.Position(pos[0], pos[1]);
	flatcamera.Rotation(rotation);
	flatcamera.UpdateProjection();

	vector[currentbackground]->BGUpdate(1e-9f * ns);
}


void Background::Render()
{
	if(currentbackground == 0xffffffff)
		return;

	vector[currentbackground]->BGRender();
	
	Shader::Blur::Use();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebuffertexture[currentbuffer]);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[2]);
	Shader::Blur::S(0);
	Shader::Blur::texel(1.2f / width, 0.0f);
	Quad::Draw();

	glBindTexture(GL_TEXTURE_2D, framebuffertexture[2]);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[currentbuffer]);
	Shader::Blur::S(0);
	Shader::Blur::texel(0.0f, 1.2f / height);
	Quad::Draw();
}


void Background::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebuffertexture[currentbuffer]);
	
	if(fading)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, framebuffertexture[!currentbuffer]);
		
		switch(fadefx)
		{
		case 0:
			Shader::Fade::Use();
			Shader::Fade::A(1);
			Shader::Fade::B(0);
			Shader::Fade::f(sqrt(fadetime));
			break;

		case 1:
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, Resources::texture[TX_PUF]);

			Shader::FadeTexture::Use();
			Shader::FadeTexture::N(2);
			Shader::FadeTexture::A(1);
			Shader::FadeTexture::B(0);
			Shader::FadeTexture::s(1.0f, 1.0f);
			Shader::FadeTexture::b(0.0f, 0.0f);
			Shader::FadeTexture::f(fadetime);
		}
	}
	else
	{
		Shader::Texture::Use();
		Shader::Texture::S(0);		
	}

	Quad::Draw();
}
