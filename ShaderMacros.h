#pragma once


#define L(X) (sizeof(X)/sizeof(void*))

#define START_SHADER(NAME)\
namespace NAME\
{\
	extern int uniformloc[];\
	inline void Use(){ glUseProgram(shaders[SP_##NAME]); }


#define DEF_SHADER(NAME, ...)\
namespace NAME\
{\
	static const char *uniforms[] = { __VA_ARGS__ };\
	int uniformloc[L(uniforms)];\
	inline void InitUniforms()\
	{\
		for(unsigned i=0; i<L(uniforms); ++i)\
			uniformloc[i] = glGetUniformLocation(shaders[SP_##NAME], uniforms[i]);\
	}\
}

#define END_SHADER()\
}

#define SAMPLER(NAME, ID)\
inline void NAME(const int &a){ glUniform1i(uniformloc[ID], a); }

#define FLOAT(NAME, ID)\
inline void NAME(const float &a){ glUniform1f(uniformloc[ID], a); }

#define VEC2(NAME, ID)\
inline void NAME(const float &a, const float &b){ glUniform2f(uniformloc[ID], a, b); }\
inline void NAME(const float *a){ glUniform2fv(uniformloc[ID], 1, a); }

#define VEC3(NAME, ID)\
inline void NAME(const float &a, const float &b, const float &c){ glUniform3f(uniformloc[ID], a, b, c); }\
inline void NAME(const float *a){ glUniform3fv(uniformloc[ID], 1, a); }

#define VEC4(NAME, ID)\
inline void NAME(const float &a, const float &b, const float &c, const float &d){ glUniform4f(uniformloc[ID], a, b, c, d); }\
inline void NAME(const float *a){ glUniform4fv(uniformloc[ID], 1, a); }

#define MAT4(NAME, ID)\
inline void NAME(const float *a){ glUniformMatrix4fv(uniformloc[ID], 1, 0, a); }
