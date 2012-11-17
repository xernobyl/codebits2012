precision lowp float;

uniform sampler2D A, B;
uniform float f;

varying vec2 t;

void main()
{
	gl_FragColor = mix(texture2D(A, t), texture2D(B, t), f);
}