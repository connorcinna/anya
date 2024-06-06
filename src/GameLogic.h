#define MAP_SZ 100
#include <stdlib.h>
#include <vector>

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
			void render_cell(int width, int height);
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
			void render_grid();
			int width;
			int height;
			std::vector<std::vector<Cell>> cells;
	};
}

