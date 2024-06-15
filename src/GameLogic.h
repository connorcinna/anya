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

namespace GameLogic
{
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
			void render_cell(int width, int height, GLuint dead_program_id, GLuint alive_program_id);
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
			Grid(Config* config);
			virtual ~Grid();
	     	void update_grid();
			//TODO: put this into a render class!
			void render_grid(GLuint dead_program_id, GLuint alive_program_id);
			int width;
			int height;
			std::vector<std::vector<Cell>> cells;
			Renderer* renderer;
	};
}

