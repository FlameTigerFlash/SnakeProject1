#include <string>
#include "Source.h"

const int field_cell_type_none = 0;
const int field_cell_type_apple = -1;
const int field_cell_type_wall = -2;
const int snake_direction_up = 0;
const int snake_direction_right = 1;
const int snake_direction_down = 2;
const int snake_direction_left = 3;

const int field_size_x = 35;
const int field_size_y = 25;
const int cell_size = 32;

using namespace std;

// Игровая функция по выбору случайной пустой клетки
int get_random_empty_cell(int field[field_size_x][field_size_y])
{
    int empty_cell_count = 0;
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (field[j][i] == field_cell_type_none) {
                empty_cell_count++;
            }
        }
    }
    int target_empty_cell_index = std::rand() % empty_cell_count;
    int empty_cell_index = 0;
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (field[j][i] == field_cell_type_none) {
                if (empty_cell_index == target_empty_cell_index) {
                    return j * field_size_x + i;
                }
                empty_cell_index++;
            }
        }
    }
    return -1;
}

//Игровая функция по добавлению яблока в случайную клетку
auto apple_add(int field[field_size_x][field_size_y])
{
    int applePosition = get_random_empty_cell(field);
    if (applePosition != -1) {
        field[applePosition / field_size_x][applePosition % field_size_x] = field_cell_type_apple;
    }
}

//Игровая функция по очищению всего игрового поля
auto clear_field(int field[field_size_x][field_size_y], int snake_length, int snake_position_x, int snake_position_y)
{
    for (int i = 0; i < field_size_x; i++) {
        for (int j = 0; j < field_size_y; j++) {
            field[i][j] = 0;
        }
    }
    for (int i = 0; i < snake_length; i++) {
        field[snake_position_x - i][snake_position_y] = snake_length - i;
    }
    for (int i = 0; i < field_size_x; i += 1) {
        if (i < 6 || (field_size_x - i - 1) < 6) {
            field[i][0] = field_cell_type_wall;
            field[i][field_size_y - 1] = field_cell_type_wall;
        }
    }
    for (int i = 0; i < field_size_y; i += 1) {
        if (i < 6 || (field_size_y - i - 1) < 6) {
            field[0][i] = field_cell_type_wall;
            field[field_size_x - 1][i] = field_cell_type_wall;
        }
    }
    apple_add(field);
}

//Игровая функция по увеличению длины змейки
auto increaseSnake(int field[field_size_x][field_size_y])
{
    for (int i = 0; i < field_size_x; i++) {
        for (int j = 0; j < field_size_y; j++) {
            if (field[i][j] > field_cell_type_none) {
                field[i][j] += 1;
            }
        }
    }
}

//Игровая функция по передвижению змейки
auto movement(int snake_length, int snake_direction, int snake_position_y, int snake_position_x, int field[][field_size_y]) {
    switch (snake_direction) {
        case snake_direction_up:
            snake_position_y -= 1;
            if (snake_position_y < 0) {
                snake_position_y = field_size_y - 1;
            }
            break;
        case snake_direction_right:
            snake_position_x += 1;
            if (snake_position_x > field_size_x - 1) {
                snake_position_x = 0;
            }
            break;
        case snake_direction_down:
            snake_position_y += 1;
            if (snake_position_y > field_size_y - 1) {
                snake_position_y = 0;
            }
            break;
        case snake_direction_left:
            snake_position_x -= 1;
            if (snake_position_x < 0) {
                snake_position_x = field_size_x - 1;
            }
            break;
    }

    if ((field[snake_position_x][snake_position_y]) != field_cell_type_none) {
        switch (field[snake_position_x][snake_position_y]) {
            case field_cell_type_apple:
                increaseSnake(field);
                break;
            case field_cell_type_wall:
                for (int i = 0; i < field_size_x; i++) {
                    for (int j = 0; j < field_size_y; j++) {
                        if (field[i][j] > 0) {
                            field[i][j] = 0;
                        }
                    }
                }
                return;
            default:
                if (field[snake_position_x][snake_position_y] > 1) {
                    for (int i = 0; i < field_size_x; i++) {
                        for (int j = 0; j < field_size_y; j++) {
                            if (field[i][j] > 0) {
                                field[i][j] = 0;
                            }
                        }
                    }
                    return;
                }
        }
    }
    for (int i = 0; i < field_size_x; i++) {
        for (int j = 0; j < field_size_y; j++) {
            if (field[i][j] > 0) {
                field[i][j] -= 1;
            }
        }
    }
    field[snake_position_x][snake_position_y] = snake_length;
}
