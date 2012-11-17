precision lowp float;

varying vec3 col;

void main()
{
	float a = 1.0 - 2.0 * distance(gl_PointCoord, vec2(0.5, 0.5));
	if(a <= 0.0)
		discard;

	gl_FragColor = vec4(col, 1.0) * a;
}