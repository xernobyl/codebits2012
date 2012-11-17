precision lowp float;

const vec3 hl2_basis0 = vec3(-1.0 / sqrt(2.0), -1.0 / sqrt(3.0), 1.0 / sqrt(6.0));
const vec3 hl2_basis1 = vec3(1.0 / sqrt(2.0), -1.0 / sqrt(3.0), 1.0 / sqrt(6.0));
const vec3 hl2_basis2 = vec3(0.0, -1.0 / sqrt(3.0), -sqrt(2.0 / 3.0));

uniform sampler2D S;
varying mat2 rm;
varying vec2 rt;
varying vec3 pos;

varying vec3 basis_col0, basis_col1, basis_col2;

// puf middle == 0.50196077(9)

void main()
{
	vec2 t = rm * gl_PointCoord + rt;	// rm * (gl_PointCoord - 0.5) + 0.5;

	vec3 n = texture2D(S, t).rgb;
	n.xy = n.xy * 2.0078739983565937 - 1.0078739983565937;
	float a = length(n);

	if(a < 1.5/255.0)
		discard;

	n /= a;
	n.xy = rm * n.xy;

	vec3 w = clamp(vec3(dot(n, hl2_basis0), dot(n, hl2_basis1), dot(n, hl2_basis2)), 0.0, 1.0);
	vec3 diffuse_light = basis_col0 * w.x + basis_col1 * w.y + basis_col2 * w.z;

	gl_FragColor = vec4(a * diffuse_light, a);
}