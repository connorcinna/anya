#include <fstream>
#include <sstream>
#include <string>
#include <GL\GLU.h>
#include "GameLogic.h"

//allocate memory for the grid
void init_grid(Config* config, Grid* grid)
{
	grid->width = config->width;
	grid->height = config->height;
	grid->cells = std::vector<std::vector<Cell>>();
	//TODO: improve this - O(N^3) is nasty
	for (int i = 0; i < config->width; i++)
	{
		grid->cells.push_back(std::vector<Cell>());
		for (int j = 0; j < config->height; ++j)
		{
			for (auto c : config->cells)
			{
				if (i == c.pos.x && j == c.pos.y)
				{
					grid->cells[i][j].alive = true;
				}
			}
			grid->cells[i][j].pos = {i, j};
		}
	}
}

//call update_cell for each cell in the grid
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

//draw the grid in opengl -- call render_cell for each cell in the grid
void render_grid(Grid* grid)
{
	for (int i = 0; i < grid->width; ++i)
	{
		for (int j = 0; j < grid->height; ++j)
		{
			render_cell(&grid->cells[i][j]);
		}
	}
}

//draw a single cell
void render_cell(Cell* cell)
{

	if (cell->alive)
	{
		glBegin(GL_QUADS);
		{
			glVertex2f(cell->pos.x, cell->pos.y);
			glVertex2f(cell->pos.x, cell->pos.y + 1);
			glVertex2f(cell->pos.x + 1, cell->pos.y + 1);
			glVertex2f(cell->pos.x + 1, cell->pos.y);
		}
		glEnd();
	}
}

//main loop logic
//conways game of life follows the following rules:
// 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation
// 2. Any live cell with two or three live neighbours lives on to the next generation
// 3. Any live cell with more than three live neighbours dies, as if by overpopulation
// 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
void update_cell(Grid* grid, Cell* cell)
{
	switch (nearby_cells(grid, cell))
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

//count the number of nearby cells that are alive
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

//read in resources/game.config to set up the map
Config read_config(void)
{
	Config config;

	std::ifstream ifs("resources/game.config");
	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.find("Map Size") != std::string::npos)
		{
			std::getline(ifs, line);
			config.width = std::stoi(line);
			config.height = std::stoi(line);
			continue;
		}
		if (line.find("Cells") != std::string::npos)
		{
			while(std::getline(ifs, line))
			{
				Cell cell;
				std::istringstream iss(line);
				std::string token;
				int i = 0;
				while (std::getline(iss, token, ','))
				{
					if (i == 0)
					{
						cell.pos.x = std::stoi(token);
					}
					else
					{
						cell.pos.y = std::stoi(token);
					}
					++i;
				}
				config.cells.push_back(cell);
			}
		}
	}
	return config;
}

