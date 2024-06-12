#include <GL\glew.h>
#include <GL\glu.h>

typedef struct gl_context
{
	GLuint g_program_id;
	GLint gVertexPos2DLocation;
	GLuint gVBO;
	GLuint gIBO;
} gl_context;
