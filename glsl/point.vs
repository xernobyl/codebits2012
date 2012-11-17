precision lowp float;

const vec3 hl2_basis0 = vec3(-1.0 / sqrt(2.0), -1.0 / sqrt(3.0), 1.0 / sqrt(6.0));
const vec3 hl2_basis1 = vec3(1.0 / sqrt(2.0), -1.0 / sqrt(3.0), 1.0 / sqrt(6.0));
const vec3 hl2_basis2 = vec3(0.0, -1.0 / sqrt(3.0), -sqrt(2.0 / 3.0));

attribute vec4 A0;

uniform float time;
uniform mat4 m;
uniform mat4 nm;
uniform vec2 zs;

varying mat2 rm;
varying vec2 rt;
varying vec3 pos;

varying vec3 basis_col0, basis_col1, basis_col2;


const vec3 lightpos0 = vec3(5.0, 0.0, 0.0);
const vec3 lightcol0 = vec3(1.0, 0.0, 0.0);
const vec3 lightpos1 = vec3(0.0, 5.0, 0.0);
const vec3 lightcol1 = vec3(0.0, 1.0, 0.0);
const vec3 lightpos2 = vec3(0.0, 0.0, 5.0);
const vec3 lightcol2 = vec3(0.0, 0.0, 1.0);

void accumulate_lighting(vec3 light_dir, vec3 light_col)
{
	vec3 weights = clamp(vec3(dot(light_dir, hl2_basis0), dot(light_dir, hl2_basis1), dot(light_dir, hl2_basis2)), 0.0, 1.0);

	basis_col0 += light_col * weights.x;
	basis_col1 += light_col * weights.y;
	basis_col2 += light_col * weights.z;
}

void main()
{
	gl_Position = m * vec4(A0.xyz, 1.0);
	gl_PointSize = zs.x / gl_Position.w;

	float ct = cos(A0.w);
	float st = sin(A0.w);
	rm = mat2(ct, st, -st, ct);

	rt = - rm * vec2(0.5) + 0.5;

	vec3 ld0 = mat3(nm) * (A0.xyz - lightpos0);
	accumulate_lighting(ld0, lightcol0);
	vec3 ld1 = mat3(nm) * (A0.xyz - lightpos1);
	accumulate_lighting(ld1, lightcol1);
	vec3 ld2 = mat3(nm) * (A0.xyz - lightpos2);
	accumulate_lighting(ld2, lightcol2);
}
