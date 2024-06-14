#include <string>
#include <sstream>
#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::render(GLfloat vertex_data[], std::vector<GLuint> program_ids)
{
	//get vertex attribute location
	GLuint gVBO = 0;
	GLuint gVAO = 0;
	//create VAO
	glGenVertexArrays(1, &gVAO);
	CHECK_GL_ERR();
	//create VBO
	glGenBuffers(1, &gVBO);
	CHECK_GL_ERR();
	glBindVertexArray(gVAO);
	CHECK_GL_ERR();

	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	CHECK_GL_ERR();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	CHECK_GL_ERR();

	//set vertex data 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)0);
	CHECK_GL_ERR();
	//enable vertex position 
	glEnableVertexAttribArray(0);
	CHECK_GL_ERR();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECK_GL_ERR();
	glBindVertexArray(0);
	CHECK_GL_ERR();
	//bind program
//	this->alive ? glUseProgram(alive_program_id) : glUseProgram(dead_program_id);
//    int dead_program_id = program_ids[0];
	int alive_program_id = program_ids[1];
	glUseProgram(alive_program_id);
	CHECK_GL_ERR();
	glBindVertexArray(gVAO);
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
