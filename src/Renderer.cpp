#include <sstream>
#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

//TODO:change this in the future, copying over the whole vector for every cell every time
//seems really inefficient
void Renderer::render(std::vector<GLuint> program_ids, GLuint VAO, bool alive)
{
	//bind program
	alive ? glUseProgram(program_ids[1]) : glUseProgram(program_ids[0]);
	CHECK_GL_ERR();
	glBindVertexArray(VAO);
	CHECK_GL_ERR();
	//draw
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	CHECK_GL_ERR();

	glDisableVertexAttribArray(0);
	CHECK_GL_ERR();
	glUseProgram(NULL);
	CHECK_GL_ERR();
}

bool Renderer::check_gl_error(const char* file, const char* function, int line)
{
	GLenum error = glGetError();
	std::stringstream err_metadata_ss;
	err_metadata_ss << "[ " << file << " " << function << " " << line << " ]";
	const char* err_metadata = err_metadata_ss.str().c_str();
	switch(error)
	{
		case GL_INVALID_ENUM: SDL_Log(" %s  GL_INVALID_ENUM", err_metadata); break;
		case GL_INVALID_VALUE: SDL_Log(" %s GL_INVALID_VALUE", err_metadata); break;
		case GL_INVALID_OPERATION: SDL_Log(" %s GL_INVALID_OPERATION", err_metadata); break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: SDL_Log(" %s GL_INVALID_FRAMEBUFFER_OPERATION", err_metadata); break;
		case GL_OUT_OF_MEMORY: SDL_Log(" %s GL_OUT_OF_MEMORY", err_metadata); break;
		case GL_STACK_UNDERFLOW: SDL_Log(" %s GL_STACK_UNDERFLOW", err_metadata); break;
		case GL_STACK_OVERFLOW: SDL_Log(" %s GL_STACK_OVERFLOW", err_metadata); break;
		case GL_TABLE_TOO_LARGE: SDL_Log(" %s GL_TABLE_TOO_LARGE", err_metadata); break;
		default: break;
	}
	return error != GL_NO_ERROR;
}
