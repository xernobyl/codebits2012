attribute vec4 A0;
varying vec2 t;

void main()
{
	gl_Position = A0;
	t = A0.xy * 0.5 + 0.5;
}
