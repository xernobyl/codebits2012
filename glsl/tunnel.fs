precision lowp float;

varying vec2 p;
uniform float z;
uniform sampler2D S;

void main()
{
    float a = atan(p.y, p.x);
	float l = 1.0 / length(p);
	vec2 uv = vec2(a * 1.2732395447351626861510701069801, l - z);
    gl_FragColor = texture2D(S, uv);
}