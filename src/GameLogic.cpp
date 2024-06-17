#include <fstream>
#include <sstream>
#include <string>
#include "GameLogic.h"
#include "SDL_log.h"
#include "Renderer.h"

Grid::Grid(Config* config)
{
	this->width = config->width;
	this->height = config->height;
	this->cells = std::vector<std::vector<Cell>>(config->width, std::vector<Cell>(config->height));
	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; ++j)
		{
			this->cells[i][j] = Cell({i, j}, false);
		}
	}
	for (const auto &c : config->cells)
	{
		this->cells[c.pos.x][c.pos.y].alive = true;
	}
}

Grid::~Grid()
{
}

void Grid::init_cells()
{
	//get vertex attribute location
	GLfloat cell_width = 2.0f / width;
	GLfloat cell_height = 2.0f / height;
    //after printing this out, im like 99% sure this is correct, but still nothing shows up
    for (auto row : cells)
    {
    	 for (auto cell : row)
    	 {
    	    	std::vector<GLfloat> vertex_data = 
    			{
    				(-1.0f + (cell.pos.x*cell_width)), (-1.0f + (cell.pos.y*cell_height)), 0.0f,
    				((-1.0f + cell_width) + (cell.pos.x*cell_width)), (-1.0f + (cell.pos.y*cell_height)), 0.0f,
    				(-1.0f + (cell.pos.x*cell_width)), ((-1.0f + cell_height) + (cell.pos.y*cell_height)), 0.0f,
    				(-1.0f + cell_width) + (cell.pos.x*cell_width), ((-1.0f + cell_height) + (cell.pos.y*cell_height)), 0.0f
    			};
    
    			glBufferData(GL_ARRAY_BUFFER, vertex_data.size(), vertex_data.data(), GL_DYNAMIC_DRAW);
    			CHECK_GL_ERR();
    			//set vertex data 
    			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)0);
    			CHECK_GL_ERR();
    			//enable vertex position 
    			glEnableVertexAttribArray(0);
    			CHECK_GL_ERR();
    			glBindBuffer(GL_ARRAY_BUFFER, 0);
    			CHECK_GL_ERR();
    			glBindVertexArray(VAOs[cell.pos.x][cell.pos.y]);
    			CHECK_GL_ERR();
    	  }
    }
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
void Grid::render_grid(GLuint dead_program_id, GLuint alive_program_id)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < this->width; ++i)
	{
		for (int j = 0; j < this->height; ++j)
		{
			this->cells[i][j].render_cell(dead_program_id, alive_program_id, this->VAOs[i][j]);
		}
	}
	glFlush();
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
void Cell::render_cell(GLuint dead_program_id, GLuint alive_program_id, GLuint VAO)
{

	std::vector<GLuint> program_ids = 
	{
		dead_program_id,
		alive_program_id
	};

	Renderer::render(program_ids, VAO, this->alive);
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
	if (this->pos.x - 1 >= 0 &&
		this->pos.y - 1 >= 0 &&
		grid->cells.at(this->pos.x - 1).at(this->pos.y - 1).alive) 
	{
		++ret;
	}
// up left
	if (this->pos.x - 1 >= 0 &&
		this->pos.y + 1 < grid->height &&
		grid->cells.at(this->pos.x - 1).at(this->pos.y + 1).alive)
	{
		++ret;
	}
	// up right
	if (this->pos.x + 1 < grid->width &&
		this->pos.y + 1 < grid->height &&
		grid->cells.at(this->pos.x + 1).at(this->pos.y + 1).alive)
	{
		++ret;
	}
	// down right 
	if (this->pos.x + 1 < grid->width &&
		this->pos.y - 1 >= 0 &&
		grid->cells.at(this->pos.x + 1).at(this->pos.y - 1).alive) 
	{
		++ret;
	}
	//down
	if (this->pos.y - 1 >= 0 &&
		grid->cells.at(this->pos.x).at(this->pos.y - 1).alive) 
	{
		++ret;
	}
	//up
	if (this->pos.y + 1 < grid->height &&
		grid->cells.at(this->pos.x).at(this->pos.y + 1).alive) 
	{
		++ret;
	}
	//left
	if (this->pos.x - 1 >= 0 &&
		grid->cells.at(this->pos.x - 1).at(this->pos.y).alive) 
	{
		++ret;
	}
	//right
	if (this->pos.x + 1 < grid->width &&
		grid->cells.at(this->pos.x + 1).at(this->pos.y).alive) 
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
