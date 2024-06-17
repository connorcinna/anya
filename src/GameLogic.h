#include <stdlib.h>
#include <vector>
#ifdef __linux__
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#ifdef _WIN32
#include <GL\glew.h>
#include <GL\glu.h>
#endif
#include "Renderer.h"

class Cell;
class Config;
class Grid;

typedef struct
{
	int x;
	int y;
} Pos;

class Cell
{
	public:
		Cell();
		Cell(Pos pos, bool alive);
		virtual ~Cell();
		int nearby_cells(Grid* grid);
		void update_cell(Grid* grid);
		//TODO: put this into a render class!
		void render_cell(GLuint dead_program_id, GLuint alive_program_id, GLuint VAO);
		Pos pos;
		bool alive;
		GLuint VBO;
		GLuint VAO;
};
class Config
{
	public:
		Config();
		virtual ~Config();
		int width;
		int height;
		std::vector<Cell> cells;
};
class Grid
{
	public:
		//construct a grid object from a config
		Grid(Config* config);
		virtual ~Grid();
		//initialize the VBO and VAO for each cell
		void init_cells();
		//game state logic
		void update_grid();
		//Call the renderer for the grid object
		void render_grid(GLuint dead_program_id, GLuint alive_program_id);
		int width;
		int height;
		std::vector<std::vector<Cell>> cells;
		//TODO these should maybe be part of the cell class
		std::vector<std::vector<GLuint>> VBOs;
		std::vector<std::vector<GLuint>> VAOs;
		Renderer* renderer;
};

