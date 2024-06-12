#include <stdlib.h>
#include <vector>
#include <GL\glew.h>
#include <GL\glu.h>

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

	typedef struct
	{
		GLuint dead_program_id;
		GLuint alive_program_id;
		GLint gVertexPos2DLocation;
		GLuint gVBO;
		GLuint gIBO;
	} gl_context;

	class Cell
	{
		public:
			Cell();
			Cell(Pos pos, bool alive);
			virtual ~Cell();
			int nearby_cells(Grid* grid);
			void update_cell(Grid* grid);
			//TODO: put this into a render class!
			void render_cell(int width, int height, gl_context* ctx);
			Pos pos;
			bool alive;
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
			void render_grid(gl_context* ctx);
			int width;
			int height;
			std::vector<std::vector<Cell>> cells;
	};
}

