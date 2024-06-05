#define MAP_SZ 100
#include <stdlib.h>
#include <vector>

typedef struct
{
	int x;
	int y;
} Pos;

typedef struct
{
	Pos pos;	
	bool alive;
} Cell;

typedef struct
{
	int width;
	int height;
	std::vector<std::vector<Cell>> cells;
} Grid;

//this is kind of stupid, because we already have Grid, but it didn't make sense
//to return a whole Grid just to create another grid
typedef struct 
{
	int width;
	int height;
	std::vector<Cell> cells;
} Config;

void init_grid(Config* config, Grid* grid);
void update_grid(Grid* grid);
void render_grid(Grid* grid);
void update_cell(Grid* grid, Cell* cell);
void render_cell(Cell* cell);
int nearby_cells(Grid* grid, Cell* cell);
Config read_config(void);
