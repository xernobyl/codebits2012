precision lowp float;

attribute vec4 A0;
attribute vec3 A1;

uniform mat4 m;
uniform float zs;
uniform vec3 c;

varying vec3 col;


void main()
{
	gl_Position = m * A0;
	gl_PointSize = zs / gl_Position.w;

	col = fract(A1 + c);
}
