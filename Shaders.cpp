#include "Shaders.h"

#include <stdio.h>


namespace Shader
{
	enum
	{
		FS_normal = 0x0000,
		FS_point,
		FS_text,
		FS_solid,
		FS_blur,
		FS_texture,
		FS_circle,
		FS_planes,
		FS_tunnel,
		FS_fade,
		FS_fadetexture,

		N_FRAGMENT_SHADERS
	};

	static const char *fragmentshaders[N_FRAGMENT_SHADERS] =
	{
		"normal",
		"point",
		"text",
		"solid",
		"blur",
		"texture",
		"circle",
		"planes",
		"tunnel",
		"fade",
		"fadetexture"
	};

	enum
	{
		VS_normal = 0x4000,
		VS_point,
		VS_text,
		VS_flat,
		VS_circle,
		VS_planes,

		N_VERTEX_SHADERS
	};

	static const char *vertexshaders[N_VERTEX_SHADERS - 0x4000] =
	{
		"normal",
		"point",
		"text",
		"flat",
		"circle",
		"planes",
	};


	static const unsigned shadertable[] =
	{
		2, VS_normal, FS_normal,
		2, VS_point, FS_point,
		2, VS_text, FS_text,
		2, VS_text, FS_solid,
		2, VS_flat, FS_blur,
		2, VS_flat, FS_texture,
		2, VS_circle, FS_circle,
		2, VS_planes, FS_planes,
		2, VS_planes, FS_tunnel,
		2, VS_flat, FS_fade,
		2, VS_flat, FS_fadetexture,

		0
	};

	unsigned shaders[N_SHADER_PROGRAMS];


	DEF_SHADER(Normal, "m");
	DEF_SHADER(Point, "S", "nm", "m", "zs");
	DEF_SHADER(Text, "S", "m", "c");
	DEF_SHADER(Solid, "m", "c");
	DEF_SHADER(Blur, "S", "texel");
	DEF_SHADER(Texture, "S");
	DEF_SHADER(Circle, "m", "zs", "c");
	DEF_SHADER(Planes, "m", "z", "S");
	DEF_SHADER(Tunnel, "m", "z", "S");
	DEF_SHADER(Fade, "A", "B", "f");
	DEF_SHADER(FadeTexture, "A", "B", "N", "f", "s", "b");

	bool InitAll()
	{
		if(!GLWindow::InitShaders(shaders, shadertable, fragmentshaders, L(fragmentshaders), vertexshaders, L(vertexshaders)))
		{
			puts("Could not compile shaders or something like that.");
			return false;
		}

		Shader::Normal::InitUniforms();
		Shader::Point::InitUniforms();
		Shader::Text::InitUniforms();
		Shader::Solid::InitUniforms();
		Shader::Blur::InitUniforms();
		Shader::Texture::InitUniforms();
		Shader::Circle::InitUniforms();
		Shader::Planes::InitUniforms();
		Shader::Tunnel::InitUniforms();
		Shader::Fade::InitUniforms();
		Shader::FadeTexture::InitUniforms();

		return true;
	}
}
