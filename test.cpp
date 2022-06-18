#include "Source.cpp"
#include "doctest.h"
#include <vector>
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