precision lowp float;

varying vec2 t;
uniform sampler2D S;

void main()
{
	gl_FragColor = texture2D(S, t);
}