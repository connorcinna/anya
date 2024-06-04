#define MAP_SZ 100

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
	Cell** cells;
} Grid;

void init_grid(Grid* grid);
void update_grid(Grid* grid);
void render_grid(Grid* grid);
void update_cell(Grid* grid, Cell* cell);
int nearby_cells(Cell* cell);
