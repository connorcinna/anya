#include <fstream>
#include <sstream>
#include <string>
#include <GL\GLU.h>
#include "GameLogic.h"

using namespace GameLogic;

Grid::Grid(Config* config)
{
	this->width = config->width;
	this->height = config->height;
	this->cells = std::vector<std::vector<Cell>>();
	//TODO: improve this - O(N^3) is nasty
	for (int i = 0; i < config->width; i++)
	{
		this->cells.push_back(std::vector<Cell>());
		for (int j = 0; j < config->height; ++j)
		{
			this->cells[i].push_back(Cell({i, j}, false));
			for (auto c : config->cells)
			{
				if (i == c.pos.x && j == c.pos.y)
				{
					this->cells[i][j].alive = true;
				}
			}
		}
	}
}

Grid::~Grid()
{
}

//call update_cell for each cell in the grid
void Grid::update_grid()
{
	for (int i = 0; i < this->width; ++i)
	{
		for (int j = 0; j < this->height; ++j)
		{
			this->cells[i][j].update_cell(this);
		}
	}
}

//draw the grid in opengl -- call render_cell for each cell in the grid
void Grid::render_grid()
{
	for (int i = 0; i < this->width; ++i)
	{
		for (int j = 0; j < this->height; ++j)
		{
			this->cells[i][j].render_cell();
		}
	}
}

Cell::Cell()
{
	pos = {0, 0};
	alive = false;
}
Cell::Cell(Pos pos, bool alive)
{
	this->pos = pos;
	this->alive = alive;
}
Cell::~Cell()
{

}

//draw a single cell
void Cell::render_cell()
{
	if (this->alive)
	{
		glBegin(GL_QUADS);
		{
			glVertex2f(this->pos.x, this->pos.y);
			glVertex2f(this->pos.x, this->pos.y + 1);
			glVertex2f(this->pos.x + 1, this->pos.y + 1);
			glVertex2f(this->pos.x + 1, this->pos.y);
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
void Cell::update_cell(Grid* grid)
{
	switch (nearby_cells(grid))
	{
		case 0:
		case 1:
			this->alive = false;
			break;
		case 2:
			break;
		case 3:
			this->alive = true;
			break;
		default: 
			this->alive = false;
	}
}

//count the number of nearby cells that are alive
int Cell::nearby_cells(Grid* grid)
{
	int ret = 0;
	// down left
	if (grid->cells[this->pos.x - 1][this->pos.y - 1].alive) 
	{
		++ret;
	}
	// up left
	if (grid->cells[this->pos.x - 1][this->pos.y + 1].alive)
	{
		++ret;
	}
	// up right
	if (grid->cells[this->pos.x + 1][this->pos.y + 1].alive)
	{
		++ret;
	}
	// down right 
	if (grid->cells[this->pos.x + 1][this->pos.y - 1].alive) 
	{
		++ret;
	}
	//down
	if (grid->cells[this->pos.x][this->pos.y - 1].alive) 
	{
		++ret;
	}
	//up
	if (grid->cells[this->pos.x][this->pos.y + 1].alive) 
	{
		++ret;
	}
	//left
	if (grid->cells[this->pos.x - 1][this->pos.y].alive) 
	{
		++ret;
	}
	//right
	if (grid->cells[this->pos.x + 1][this->pos.y].alive) 
	{
		++ret;
	}

	return ret;
}

Config::Config()
{
	std::ifstream ifs("resources/game.config");
	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.find("Map Size") != std::string::npos)
		{
			std::getline(ifs, line);
			this->width = std::stoi(line);
			this->height = std::stoi(line);
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
				this->cells.push_back(cell);
			}
		}
	}
}

Config::~Config()
{
}
