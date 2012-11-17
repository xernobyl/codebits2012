uniform sampler2D S;
varying vec2 t;

uniform vec4 c;

void main()
{
	float a = clamp(0.0, 1.0, texture2D(S, t).r * 16.0 - 8.0);
	gl_FragColor = c * vec4(a);
}
