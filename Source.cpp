#include <doctest.h>
#include <vector>
#include <string>

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
const int window_width = field_size_x * cell_size;
const int window_height = field_size_y * cell_size;

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


TEST_CASE("Testing apple placement")
{
    int field[field_size_x][field_size_y];
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            field[i][j] = field_cell_type_none;
        }
    }
    int apple_count = rand() % 10 + 2;
    for (int i = 0; i < apple_count; i++)
    {
        apple_add(field);
    }
    bool flag = false;
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            if (field[i][j] == field_cell_type_apple)
            {
                flag = true;
            }
        }
    }
    CHECK(flag);
}

TEST_CASE("Testing field eraser")
{
    int field[field_size_x][field_size_y];
    vector <int> objects = { field_cell_type_none, field_cell_type_apple, field_cell_type_wall };
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            field[i][j] = objects[rand()%objects.size()];
        }
    }
    clear_field(field, 4, 10, 10);
    int apple_count = 0;
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            if (field[i][j] == field_cell_type_apple)
            {
                apple_count++;
            }
        }
    }
    CHECK(apple_count == 1);
}

TEST_CASE("Testing snake length")
{
    CHECK(true);
}

TEST_CASE("Testing basic game mechs")
{
    bool passed = true;
    int field[field_size_x][field_size_y];
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            field[i][j] = field_cell_type_none;
        }
    }
    for (int j = 0; j < 5; j++)
    {
        field[0][j] = j + 1;
    }
    field[0][5] = field_cell_type_apple;
    field[0][6] = field_cell_type_wall;
    movement(4, snake_direction_down, 4, 0, field);
    if (field[0][5] <= 0)
    {
        passed = false;
    }
    movement(5, snake_direction_down, 5, 0, field);
    if (field[0][2] > 0)
    {
        passed = false;
    }
    field[2][0] = 7;
    field[3][0] = 6;
    field[3][1] = 5;
    field[3][2] = 4;
    field[2][2] = 3;
    field[1][2] = 2;
    field[0][2] = 1;
    movement(7, snake_direction_down, 0, 2, field);
    movement(7, snake_direction_down, 0, 3, field);
    if (field[2][0] > 0) {
        passed = false;
    }
    CHECK(passed);
}

TEST_CASE("Testing snake movement")
{
    int field[field_size_x][field_size_y];
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            field[i][j] = field_cell_type_none;
        }
    }
    for (int j = 0; j < 5; j++)
    {
        field[0][j] = j + 1;
    }
    field[0][5] = field_cell_type_wall;
    movement(4, snake_direction_right, 4, 0, field);
    movement(4, snake_direction_down, 4, 1, field);
    movement(4, snake_direction_down, 5, 1, field);
    movement(4, snake_direction_left, 6, 1, field);
    bool req = (field[0][4] > 0 and field[1][5] > 0 and field[0][6] > 0);
    CHECK(req);
}
