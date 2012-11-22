#include "Slide.h"

class Text;
class Font;


class TextSlide: public Slide
{
public:

	TextSlide(Font *font, const char *str, const unsigned &strlen, const float &size, const bool &bg = false);
	~TextSlide();

private:

	void UpdateSlide(uint_fast64_t ns, FlatCamera *c);
	void DrawSlide();

	Text *text;
	mat4 m;
	bool bg;
	float col[4];
};
