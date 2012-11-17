precision lowp float;

uniform sampler2D A, B, N;
uniform float f;
uniform vec2 s, b;

varying vec2 t;

void main()
{
	float q = clamp(2.0 * f - 1.0 + texture2D(N, t * s + b).r, 0.0, 1.0);
	gl_FragColor = mix(texture2D(A, t), texture2D(B, t), q);
}