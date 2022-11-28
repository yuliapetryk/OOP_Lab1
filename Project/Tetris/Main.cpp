#include <SFML\Graphics.hpp>
#include "Tetromino.h"
#include <chrono>
#include <random>
#include <iostream>

using namespace std;
using namespace chrono;
using namespace sf;
int main()

{
	
	bool game_over = 0;
	bool go_to_bottom_pressed = 0;
	bool rotate_pressed = 0;
	unsigned delay = 0;
	unsigned lines_cleared = 0;
	int scores=0;
	int lines = 0;
	int clear_timer = 0;
	int current_speed = START_SPEED;
	int fall_timer = 0;
	int move_timer = 0;
	int next_Tetromino;
	int one_cell_down = 0;
	time_point<steady_clock> previous_time;
	vector<bool> clear_lines(ROWS, 0);

	vector<Color> colors_of_cell = {
		Color(60, 60, 60),
		Color::Cyan,      
		Color::Blue,    
		Color(255, 146, 0),//Orange
		Color::Yellow,
		Color::Green,
		Color::Magenta,
		Color::Red, 
		Color(30, 30, 30)
	};
	vector<vector<int>> matrix(COLUMNS, vector<int>(ROWS));
	Event event;
	RenderWindow window(VideoMode(2 * CELL_SIZE * COLUMNS * SCREEN_RESIZE+2, CELL_SIZE * ROWS * SCREEN_RESIZE), "Tetris", Style::Close);
	window.setView(View(FloatRect(0, 0, 2 * CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));
	Tetromino Tetromino((Tetromino_number()), matrix);
	next_Tetromino = (Tetromino_number());
	previous_time = steady_clock::now();

	while (window.isOpen())
	{
		unsigned time_difference = duration_cast<chrono::microseconds>(steady_clock::now() - previous_time).count();
		delay += time_difference;
		previous_time += chrono::microseconds(time_difference);
		while (FRAME_DURATION <= delay)
		{
			delay -= FRAME_DURATION;
			while ( window.pollEvent(event))
			{
				switch (event.type)
				{
				case Event::Closed:
				{
					window.close();
					break;
				}
				case Event::KeyReleased:
				{
					switch (event.key.code)
					{
					case Keyboard::C:
					case Keyboard::Z:
					{
						rotate_pressed = 0;
						break;
					}
					case Keyboard::Down:
					{
						one_cell_down = 0;
						break;
					}
					case Keyboard::Left:
					case Keyboard::Right:
					{
						move_timer = 0;
						break;
					}
					case Keyboard::Space:
					{
						go_to_bottom_pressed = 0;
					}
					}
				}
				}
			}
			lines = 0;
			if (clear_timer==0)
			{
				if (game_over==0)
				{
					if ( rotate_pressed==0)
					{
						if ( Keyboard::isKeyPressed(Keyboard::C))
						{
							rotate_pressed = 1;
							Tetromino.rotate(1, matrix);
						}
						else if ( Keyboard::isKeyPressed(Keyboard::Z))
						{
							rotate_pressed = 1;
							Tetromino.rotate(0, matrix);
						}
					}

					if ( move_timer==0)
					{
						if (1 == Keyboard::isKeyPressed(Keyboard::Left))
						{
							move_timer = 1;
							Tetromino.move_left(matrix);
						}
						else if (1 == Keyboard::isKeyPressed(Keyboard::Right))
						{
							move_timer = 1;
							Tetromino.move_right(matrix);
						}
					}
					else
					{
						move_timer = (1 + move_timer) % MOVE_SPEED;
					}

					if ( go_to_bottom_pressed==0)
					{
						if (Keyboard::isKeyPressed(Keyboard::Space))
						{
							go_to_bottom_pressed = 1;
							fall_timer = current_speed;
							Tetromino.go_to_bottom(matrix);
						}
					}
					if ( one_cell_down==0)
					{
						if (1 == Keyboard::isKeyPressed(Keyboard::Down))
						{
							if (1 == Tetromino.move_down(matrix))
							{
								fall_timer = 0;
								one_cell_down = 1;
							}
						}
					}
					else
					{
						one_cell_down = (1 + one_cell_down) % ONE_CELL_DOWN_SPEED;
					}
					if (current_speed == fall_timer)
					{
						if (Tetromino.move_down(matrix)==0)
						{
							Tetromino.update_matrix(matrix);
							for (int a = 0; a < ROWS; a++)
							{
								bool clear_line = 1;
								for (int b = 0; b < COLUMNS; b++)
								{
									if ( matrix[b][a]==0)
									{
										clear_line = 0;
										break;
									}
								}
								if ( clear_line==1)
								{
									lines_cleared++;
									lines += 1;
									clear_timer = CLEAR_EFFECT_DURATION;
									clear_lines[a] = 1;
									if (lines_cleared % LINES_TO_INCREASE_SPEED==0)
									{
										current_speed = max<int>(ONE_CELL_DOWN_SPEED, current_speed - 1);
									}
								}
							}
							if (clear_timer==0)
							{
								game_over = 0 == Tetromino.reset(next_Tetromino, matrix);
								next_Tetromino =(Tetromino_number());
							}
						}
						fall_timer = 0;
					}
					else
					{
						fall_timer++;
					}
				} 
				else if ( Keyboard::isKeyPressed(Keyboard::Enter))
				{
					game_over = 0;
					go_to_bottom_pressed = 0;
					rotate_pressed = 0;
					lines_cleared = 0;
					scores = 0;
					current_speed = START_SPEED;
					fall_timer = 0;
					move_timer = 0;
					one_cell_down = 0;
					for (vector<int>& a : matrix)
					{
						fill(a.begin(), a.end(), 0);
					}
				}
			}
			else
			{
				clear_timer--;
				if ( clear_timer==0)
				{
					for (int a = 0; a < ROWS; a++)
					{
						if (clear_lines[a]==1)
						{
							for (int b = 0; b < COLUMNS; b++)
							{
								matrix[b][a] = 0;
								for (int c = a; 0 < c; c--)
								{
									matrix[b][c] = matrix[b][c - 1];
									matrix[b][c - 1] = 0;
								}
							}
						}
					}
					game_over = 0 == Tetromino.reset(next_Tetromino, matrix);
					next_Tetromino = (Tetromino_number());
					fill(clear_lines.begin(), clear_lines.end(), 0);
				}
			}
			if (FRAME_DURATION > delay)
			{
				int clear_cell_size = static_cast<int>(2 * round(0.5f * CELL_SIZE * (clear_timer / static_cast<float>(CLEAR_EFFECT_DURATION))));
				RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
				RectangleShape preview_border(Vector2f(5 * CELL_SIZE, 5 * CELL_SIZE));
				preview_border.setFillColor(Color(0, 0, 0));
				preview_border.setOutlineThickness(-1);
				preview_border.setPosition(CELL_SIZE * (1.5f * COLUMNS - 2.5f), CELL_SIZE * (0.25f * ROWS - 2.5f));
				window.clear();
				for (int a = 0; a < COLUMNS; a++)
				{
					for (int b = 0; b < ROWS; b++)
					{
						if (clear_lines[b]==0)
						{
							cell.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));

							if (1 == game_over && 0 < matrix[a][b])
							{
								cell.setFillColor(colors_of_cell[8]);
							}
							else
							{
								cell.setFillColor(colors_of_cell[matrix[a][b]]);
							}

							window.draw(cell);
						}
					}
				}
				cell.setFillColor(colors_of_cell[8]);
				if ( game_over==0)
				{
					for (Position& block : Tetromino.shadow_Tetromino(matrix))
					{
						cell.setPosition(static_cast<float>(CELL_SIZE * block.x), static_cast<float>(CELL_SIZE * block.y));
						window.draw(cell);
					}
					cell.setFillColor(colors_of_cell[1 + Tetromino.get_shape()]);
				}
				for (Position& block : Tetromino.get_blocks())
				{
					cell.setPosition(static_cast<float>(CELL_SIZE * block.x), static_cast<float>(CELL_SIZE * block.y));
					window.draw(cell);
				}
				for (int a = 0; a < COLUMNS; a++)
				{
					for (int b = 0; b < ROWS; b++)
					{
						if ( clear_lines[b]==1)
						{
							cell.setFillColor(colors_of_cell[0]);
							cell.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
							cell.setSize(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
							window.draw(cell);
							cell.setFillColor(Color(255, 255, 255));
							cell.setPosition(floor(CELL_SIZE * (0.5f + a) - 0.5f * clear_cell_size), floor(CELL_SIZE * (0.5f + b) - 0.5f * clear_cell_size));
							cell.setSize(Vector2f(clear_cell_size, clear_cell_size));
							window.draw(cell);
						}
					}
				}
				cell.setFillColor(colors_of_cell[1 + next_Tetromino]);
				cell.setSize(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
				window.draw(preview_border);
				for (Position& block : get_Tetromino(next_Tetromino, static_cast<int>(1.5f * COLUMNS), static_cast<int>(0.25f * ROWS)))
				{
					unsigned short next_Tetromino_x = CELL_SIZE * block.x;
					unsigned short next_Tetromino_y = CELL_SIZE * block.y;
					if ( next_Tetromino==0)
					{
						next_Tetromino_y += static_cast<int>(round(0.5f * CELL_SIZE));
					}
					else if (next_Tetromino!=3)
					{
						next_Tetromino_x -= static_cast<int>(round(0.5f * CELL_SIZE));
					}
					cell.setPosition(next_Tetromino_x, next_Tetromino_y);
					window.draw(cell);
				}
				short character_x = (CELL_SIZE * (0.5f + COLUMNS));
				short character_y = (0.5f * CELL_SIZE * ROWS);
				
				switch (lines) {
				case 0: 
				{
					scores = scores + 0;
					break; }
				case 1:
				{
					scores += 100;
					break; }
				case 2:
				case 3: 
				case 4:
				{scores += pow(lines, 2)*100 - 100;
					break; }
				}			
				string i_text ="Scores:\n" +to_string(scores) + "\nLines:" + to_string(lines_cleared) + "\nSpeed:" + to_string(START_SPEED / current_speed) + 'x';
				int character_width;
				Sprite character_sprite;
				Texture font_texture;
				font_texture.loadFromFile("Font_for_text.png");	
				character_width = font_texture.getSize().x /92;
				character_sprite.setTexture(font_texture);
				for (const char a : i_text)
				{
					if ('\n' == a)
					{
						character_x = (CELL_SIZE * (0.5f + COLUMNS));
						character_y += font_texture.getSize().y;
						continue;
					}
					character_sprite.setPosition(character_x, character_y);
					character_sprite.setTextureRect(IntRect(character_width * (a -32), 0, character_width, font_texture.getSize().y));
					character_x += character_width;
					window.draw(character_sprite);
				}
				window.display();
			}
		}
	}
}