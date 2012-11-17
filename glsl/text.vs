attribute vec4 A0;
attribute vec2 A1;

varying vec2 t;

uniform mat4 m;

void main()
{
	gl_Position = m * A0;
	t = A1;
}