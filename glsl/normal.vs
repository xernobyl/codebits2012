attribute vec4 A0;
attribute vec3 A2;
uniform mat4 m;

varying vec3 n;

void main()
{
	n = normalize(A2 * 2.0 - 1.0);
	gl_Position = m * A0;
}