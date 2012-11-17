#pragma once

#include "GLWindow.h"
#include "ShaderMacros.h"


namespace Shader
{
	enum
	{
		SP_Normal,
		SP_Point,
		SP_Text,
		SP_Solid,
		SP_Blur,
		SP_Texture,
		SP_Circle,
		SP_Planes,
		SP_Tunnel,
		SP_Fade,
		SP_FadeTexture,

		N_SHADER_PROGRAMS
	};

	extern unsigned shaders[N_SHADER_PROGRAMS];

	START_SHADER(Normal);
		MAT4(m,		0);
	END_SHADER();

	START_SHADER(Point);
		SAMPLER(S, 0);
		MAT4(nm, 1);
		MAT4(m, 2);
		VEC2(zs, 3);
	END_SHADER();

	START_SHADER(Text);
		SAMPLER(S,	0);
		MAT4(m,		1);
		VEC4(c,		2);
	END_SHADER();

	START_SHADER(Solid);
		MAT4(m,		0);
		VEC4(c,		1);
	END_SHADER();

	START_SHADER(Blur);
		SAMPLER(S,	0);
		VEC2(texel,	1);
	END_SHADER();
	
	START_SHADER(Texture);
		SAMPLER(S,	0);
	END_SHADER();
	
	START_SHADER(Circle);
		MAT4(m,		0);
		FLOAT(zs,	1);
		VEC3(c,		2);
	END_SHADER();
	
	START_SHADER(Planes);
		MAT4(m,		0);
		FLOAT(z,	1);
		SAMPLER(S,	2);
	END_SHADER();

	START_SHADER(Tunnel);
		MAT4(m,		0);
		FLOAT(z,	1);
		SAMPLER(S,	2);
	END_SHADER();

	START_SHADER(Fade);
		SAMPLER(A,	0);
		SAMPLER(B,	1);
		FLOAT(f,	2);
	END_SHADER();
	
	START_SHADER(FadeTexture);
		SAMPLER(A,	0);
		SAMPLER(B,	1);
		SAMPLER(N,	2);
		FLOAT(f,	3);
		VEC2(s,		4);
		VEC2(b,		5);
	END_SHADER();

	bool InitAll();
}
