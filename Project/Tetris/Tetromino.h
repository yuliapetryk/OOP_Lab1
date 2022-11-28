#pragma once
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

 const int CELL_SIZE = 8;
 const int COLUMNS = 10;
 const int ROWS = 20;
 const int SCREEN_RESIZE = 4;
 const int LINES_TO_INCREASE_SPEED = 2;
 const int MOVE_SPEED = 4;
 const int ONE_CELL_DOWN_SPEED = 4;
 const int START_SPEED = 32;
 const int CLEAR_EFFECT_DURATION = 8;
 const int FRAME_DURATION =  16000;

struct Position
{
	char x;
	char y;
};

class Tetromino
{
	int rotation;
	int shape;

	vector<Position> blocks;
public:
	Tetromino(int Tetromino, const vector<vector<int>>& matrix);
	bool move_down(const vector<vector<int>>& matrix);
	bool reset(int Tetromino, const vector<vector<int>>& matrix);
	int get_shape();
	void go_to_bottom(const vector<vector<int>>& matrix);
	void move_left(const vector<vector<int>>& matrix);
	void move_right(const vector<vector<int>>& matrix);
	void rotate(bool clockwise, const vector<vector<int>>& matrix);
	void update_matrix(vector<vector<int>>& matrix);
	vector<Position> shadow_Tetromino(const vector<vector<int>>& matrix);
	vector<Position> get_blocks();
};

int Tetromino_number();


vector<Position> get_Tetromino(int i_Tetromino, int i_x, int i_y);

vector<Position> check_lateral_borders(bool Tetromino,int current_rotation, int next_rotation);