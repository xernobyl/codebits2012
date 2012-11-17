#pragma once

class Model;
class Font;

enum
{
	TX_PUF,
	TX_FUTURA,
	TX_ANONYMOUSPRO,
	TX_SAPOS,
	N_TEXTURES
};

enum
{
	MD_CUBE,
	N_MODELS
};

enum
{
	F_FUTURA,
	F_ANONYMOUSPRO,
	N_FONTS
};

namespace Resources
{
	extern unsigned texture[N_TEXTURES];
	extern Model model[N_MODELS];
	extern Font font[N_FONTS];

	bool Init();
};
