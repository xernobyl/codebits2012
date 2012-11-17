precision lowp float;

varying vec2 p;
uniform float z;
uniform sampler2D S;

void main()
{
    vec2 uv = vec2(p.x / p.y, 1.0 / -abs(p.y) - z);
    gl_FragColor = texture2D(S, uv);
}