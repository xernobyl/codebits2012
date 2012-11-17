precision lowp float;

attribute vec4 A0;
varying vec2 p;
uniform mat4 m;

void main()
{
	gl_Position = A0;
	p = (m * A0).xy;
}