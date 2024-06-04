#include "GameLogic.h"

//allocate memory for the grid
void init_grid(Grid* grid, int width, int height)
{
	grid->width = width;
	grid->height = height;
	grid->cells = new Cell*[width];
	for (int i = 0; i < width; i++)
	{
		grid->cells[i] = new Cell[height];
	}
}

void update_grid(Grid* grid)
{
	for (int i = 0; i < grid->width; ++i)
	{
		for (int j = 0; j < grid->height; ++j)
		{
			update_cell(grid, &grid->cells[i][j]);
		}
	}
}

//draw it in openGL
void render_grid(Grid* grid)
{

}

//main loop logic
//conways game of life follows the following rules:
// 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation
// 2. Any live cell with two or three live neighbours lives on to the next generation
// 3. Any live cell with more than three live neighbours dies, as if by overpopulation
// 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
void update_cell(Cell* cell)
{
	switch (nearby_cells(cell))
	{
		case 0:
		case 1:
			cell->alive = false;
			break;
		case 2:
			break;
		case 3:
			cell->alive = true;
			break;
		default: 
			cell->alive = false;
	}
}

int nearby_cells(Grid* grid, Cell* cell)
{
	int ret = 0;
	// down left
	if (grid->cells[cell->pos.x - 1][cell->pos.y - 1].alive) 
	{
		++ret;
	}
	// up left
	if (grid->cells[cell->pos.x - 1][cell->pos.y + 1].alive)
	{
		++ret;
	}
	// up right
	if (grid->cells[cell->pos.x + 1][cell->pos.y + 1].alive)
	{
		++ret;
	}
	// down right 
	if (grid->cells[cell->pos.x + 1][cell->pos.y - 1].alive) 
	{
		++ret;
	}
	//down
	if (grid->cells[cell->pos.x][cell->pos.y - 1].alive) 
	{
		++ret;
	}
	//up
	if (grid->cells[cell->pos.x][cell->pos.y + 1].alive) 
	{
		++ret;
	}
	//left
	if (grid->cells[cell->pos.x - 1][cell->pos.y].alive) 
	{
		++ret;
	}
	//right
	if (grid->cells[cell->pos.x + 1][cell->pos.y].alive) 
	{
		++ret;
	}

	return ret;
}

