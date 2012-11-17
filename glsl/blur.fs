precision lowp float;

varying vec2 t;

uniform sampler2D S;
uniform vec2 texel;		// 1.2 / width (or height)

void main()
{
	gl_FragColor = 0.375 * texture2D(S, t);
	gl_FragColor += 0.3125 * texture2D(S, t + texel);
	gl_FragColor += 0.3125 * texture2D(S, t - texel);
}