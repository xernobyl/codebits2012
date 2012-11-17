precision lowp float;

varying vec3 n;

void main()
{
	gl_FragColor = vec4(normalize(n) * 0.5 + 0.5, 0.0);
}