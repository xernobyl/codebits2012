#include "TextSlide.h"
#include "Text.h"
#include "Shaders.h"


TextSlide::TextSlide(Font *font, const char *str, const unsigned &strlen, const float &size, const bool &bg)
{
	this->bg = bg;
	text = new Text(font, str, strlen, size);
}


TextSlide::~TextSlide()
{
	delete text;
}


void TextSlide::UpdateSlide(uint_fast64_t ns, FlatCamera *c)
{
	m = *c->P() * *text->Matrix();
}


void TextSlide::DrawSlide()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text->Texture());

	if(bg)
	{
		Shader::Solid::Use();
		Shader::Solid::c((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 0.0);
		Shader::Solid::m((float*)&m);
		
		text->DrawBox();
	}

	Shader::Text::Use();
	Shader::Text::S(0);
	Shader::Text::c(1.0f, 1.0f, 1.0f, 1.0f);
	Shader::Text::m((float*)&m);

	text->Draw();

	glDisable(GL_BLEND);
}
