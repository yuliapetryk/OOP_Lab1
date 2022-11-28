#include <vector>
#include <string>
#include "Tetromino.h"
#include <SFML/Graphics.hpp>
#include <random>


Tetromino::Tetromino(int my_shape, const vector<vector<int>>& i_matrix) :
	rotation(0),
	shape(my_shape),
	blocks(get_Tetromino(my_shape, COLUMNS / 2, 1))
{}

int random_shape;
int Tetromino_number() {
    random_device number ;
	mt19937 gen(number());
	uniform_int_distribution<>dis(0, 6);
    random_shape = dis(gen);
		
	return random_shape;
};

vector<Position> get_Tetromino(int shape, int x, int y)
{
	vector<Position> output(4);
	switch (shape)
	{
	case 0:
	{
		output[0] = { 1, -1 };
		output[1] = { 0, -1 };
		output[2] = { -1, -1 };
		output[3] = { -2, -1 };

		break;
	}
	case 1:
	{
		output[0] = { 0, 0 };
		output[1] = { 1, 0 };
		output[2] = { -1, -1 };
		output[3] = { -1, 0 };

		break;
	}
	case 2:
	{
		output[0] = { 0, 0 };
		output[1] = { 1, 0 };
		output[2] = { 1, -1 };
		output[3] = { -1, 0 };

		break;
	}
	case 3:
	{
		output[0] = { 0, 0 };
		output[1] = { 0, -1 };
		output[2] = { -1, -1 };
		output[3] = { -1, 0 };

		break;
	}
	case 4:
	{
		output[0] = { 0, 0 };
		output[1] = { 1, -1 };
		output[2] = { 0, -1 };
		output[3] = { -1, 0 };

		break;
	}
	case 5:
	{
		output[0] = { 0, 0 };
		output[1] = { 1, 0 };
		output[2] = { 0, -1 };
		output[3] = { -1, 0 };

		break;
	}
	case 6:
	{
		output[0] = { 0, 0 };
		output[1] = { 1, 0 };
		output[2] = { 0, -1 };
		output[3] = { -1, -1 };
	}
	}

	for (Position& block : output)
	{
		block.x += x;
		block.y += y;
	}
	return output;
}

int Tetromino::get_shape()
{
	return shape;
}

bool Tetromino::move_down(const vector<vector<int>>& matrix)
{
	for (Position& block : blocks)
	{
		if (ROWS == 1 + block.y)
		{
			return 0;
		}

		if (0 < matrix[block.x][1 + block.y])
		{
			return 0;
		}
	}

	for (Position& block : blocks)
	{
		block.y++;
	}

	return 1;
}

bool Tetromino::reset(int my_shape, const vector<vector<int>>& matrix)
{
	rotation = 0;
	shape = my_shape;

	blocks = get_Tetromino(shape, COLUMNS / 2, 1);

	for (Position& block : blocks)
	{
		if (0 < matrix[block.x][block.y])
		{
			return 0;
		}
	}
	return 1;
}



void Tetromino::go_to_bottom(const vector<vector<int>>& matrix)
{
	blocks = shadow_Tetromino(matrix);
}

void Tetromino::move_left(const vector<vector<int>>& matrix)
{
	for (Position& block : blocks)
	{
		if (block.x - 1<0)
		{
			return;
		}

		if (block.y<0)
		{
			continue;
		}
		else if (matrix[block.x - 1][block.y]>0)
		{
			return;
		}
	}

	for (Position& block : blocks)
	{
		block.x--;
	}
}

void Tetromino::move_right(const vector<vector<int>>& matrix)
{
	for (Position& block : blocks)
	{
		if (COLUMNS == 1 + block.x)
		{
			return;
		}

		if ( block.y<1)
		{
			continue;
		}
		else if (matrix[1 + block.x][block.y]>0)
		{
			return;
		}
	}

	for (Position& block : blocks)
	{
		block.x++;
	}
}

vector<Position> check_lateral_borders(bool shape, int  current_rotation, int next_rotation)
{
	if (shape == 0)
	{
		switch (current_rotation)
		{
		case 0:
		case 1:
		{
			return { {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} };
		}
		case 2:
		{
			switch (next_rotation)
			{
			case 1:
			{
				return { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} };
			}
			case 3:
			{
				return { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} };
			}
			}
		}

		case 3:
		{
			return { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} };
		}
		}

		return { {0, 0} };
	}
	else
	{
		switch (current_rotation)
		{
		case 0:
		{
			switch (next_rotation)
			{
			case 1:
			{
				return { {0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2} };
			}
			case 3:
			{
				return { {0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1} };
			}
			}
		}
		case 1:
		{
			switch (next_rotation)
			{
			case 0:
			{
				return { {0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2} };
			}
			case 2:
			{
				return { {0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1} };
			}
			}
		}
		case 2:
		{
			switch (next_rotation)
			{
			case 1:
			{
				return { {0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1} };
			}
			case 3:
			{
				return { {0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2} };
			}
			}
		}
		case 3:
		{
			switch (next_rotation)
			{
			case 0:
			{
				return { {0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1} };
			}
			case 2:
			{
				return { {0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2} };
			}
			}
		}
		}

		return { {0, 0} };
	}
}

void Tetromino::rotate(bool clockwise, const vector<vector<int>>& matrix)
{
	if (shape!=3)
	{
		int next_rotation;

		vector<Position> current_blocks = blocks;
		if (clockwise==0)
		{
			next_rotation = (3 + rotation) % 4;
		}
		else
		{
			next_rotation = (1 + rotation) % 4;
		}
		if (shape==0)
		{
			float center_x = 0.5f * (blocks[1].x + blocks[2].x);
			float center_y = 0.5f * (blocks[1].y + blocks[2].y);

			switch (rotation)
			{
			case 0:
			{
				center_y += 0.5f;

				break;
			}
			case 1:
			{
				center_x -= 0.5f;

				break;
			}
			case 2:
			{
				center_y -= 0.5f;

				break;
			}
			case 3:
			{
				center_x += 0.5f;
			}
			}
			for (Position& block : blocks)
			{
				float x = block.x - center_x;
				float y = block.y - center_y;

				if (clockwise==0)
				{
					block.x = static_cast<char>(center_x + y);
					block.y = static_cast<char>(center_y - x);
				}
				else
				{
					block.x = static_cast<char>(center_x - y);
					block.y = static_cast<char>(center_y + x);
				}
			}
		}
		else
		{
			for (int a = 1; a < blocks.size(); a++)
			{
				char x = blocks[a].x - blocks[0].x;
				char y = blocks[a].y - blocks[0].y;

				if (clockwise==0)
				{
					blocks[a].x = y + blocks[0].x;
					blocks[a].y = blocks[0].y - x;
				}
				else
				{
					blocks[a].x = blocks[0].x - y;
					blocks[a].y = x + blocks[0].y;
				}
			}
		}

		for (Position& wall_kick : check_lateral_borders(shape==0, rotation, next_rotation))
		{
			bool can_turn = 1;

			for (Position& block : blocks)
			{
				if ( block.x + wall_kick.x<0 || COLUMNS <= block.x + wall_kick.x || ROWS <= block.y + wall_kick.y)
				{
					can_turn = 0;

					break;
				}

				if ( block.y + wall_kick.y<0)
				{
					continue;
				}
				else if (matrix[block.x + wall_kick.x][block.y + wall_kick.y]>0)
				{
					can_turn = 0;

					break;
				}
			}

			if ( can_turn==1)
			{
				rotation = next_rotation;
				for (Position& block : blocks)
				{
					block.x += wall_kick.x;
					block.y += wall_kick.y;
				}

				return;
			}
		}
		blocks = current_blocks;
	}
}

void Tetromino::update_matrix(vector<vector<int>>& matrix)
{
	for (Position& block : blocks)
	{
		if ( block.y<0)
		{
			continue;
		}

		matrix[block.x][block.y] = 1 + shape;
	}
}

vector<Position> Tetromino::shadow_Tetromino(const vector<vector<int>>& matrix)
{
	bool keep_falling = 1;
	int total_movement = 0;
	vector<Position> shadow_blocks = blocks;
	while ( keep_falling==1)
	{
		total_movement++;
		for (Position& block : blocks)
		{
			if (ROWS == total_movement + block.y)
			{
				keep_falling = 0;
				break;
			}

			if ( total_movement + block.y<0)
			{
				continue;
			}
			else if ( matrix[block.x][total_movement + block.y]>0)
			{
				keep_falling = 0;
				break;
			}
		}
	}

	for (Position& block : shadow_blocks)
	{
		block.y += total_movement - 1;
	}

	return shadow_blocks;
}

vector<Position> Tetromino::get_blocks()
{
	return blocks;
}
