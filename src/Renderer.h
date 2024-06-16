#ifndef RENDERER_H
#define RENDERER_H
#ifdef __linux__
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#ifdef _WIN32
#include <GL\glew.h>
#include <GL\glu.h>
#endif
#include <SDL.h>
#include <vector>

#define CHECK_GL_ERR() Renderer::check_gl_error(__FILE__, __func__, __LINE__)
class Renderer
{
	public:
		//initialize openGL in here?
		Renderer();
		//destroy GL objects
		~Renderer();
		//per frame render method
		static void render(std::vector<GLfloat> vertex_data, std::vector<GLuint> program_ids);
		//check for openGL errors
		static bool check_gl_error(const char* file, const char* function, int line);
};
#endif
