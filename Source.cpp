#include <vector>
#include <string>
#include <ctime>
#include <iostream>

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

//Вспомогательная функция отрисовки поля
char get_symb(int f)
{
    switch (f)
    {
        case field_cell_type_none:
            return '.';
        case field_cell_type_apple:
            return '*';
        case field_cell_type_wall:
            return '|';
        default:
            return '+';
    }
}

//Тест размещения яблок на поле
void check_apples()
{
    int field[field_size_x][field_size_y];
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            field[i][j] = field_cell_type_none;
        }
    }
    cout << "Empty field" << endl;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }

    int apple_count = rand() % 10 + 2;
    for (int i = 0; i < apple_count; i++)
    {
        apple_add(field);
    }
    cout << "Apple field" << endl;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }

}

//Тест механизма очищения поля
void field_clearer()
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
    cout << "Then" << endl;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }

    clear_field(field, 4, 10, 10);
    cout << "Now" << endl;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }

}

//Тест увеличения длины змейки и её перемещения
void check_snake_increase()
{
    int field[field_size_x][field_size_y];
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            field[i][j] = field_cell_type_none;
        }
    }
    int y_point = rand() % field_size_x + 1;
    for (int j = 0; j < 5; j++)
    {
        field[y_point][j] = j + 1;
    }
    cout << "Then" << endl;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }

    increaseSnake(field);
    movement(4, snake_direction_down, 4, y_point, field);

    cout << "Now" << endl;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }

}

//Тест по базовым игровым механикам
void check_game_mechs()
{
    int field[field_size_x][field_size_y];
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            field[i][j] = field_cell_type_none;
        }
    }
    cout << "Increasing size by eating apples." << endl << endl;
    for (int j = 0; j < 5; j++)
    {
        field[0][j] = j + 1;
    }
    field[0][5] = field_cell_type_apple;
    field[0][6] = field_cell_type_wall;
    cout << "Starting pos" << endl;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }
    movement(4, snake_direction_down, 4, 0, field);
    cout << "The snake has increased." << endl;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }
    movement(5, snake_direction_down, 5, 0, field);
    cout << "The snake bumped into the wall." << endl;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }

    cout << "Let's collide the snake into itself" << endl << endl;
    field[field_size_x][field_size_y];
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            field[i][j] = field_cell_type_none;
        }
    }
    field[2][0] = 7;
    field[3][0] = 6;
    field[3][1] = 5;
    field[3][2] = 4;
    field[2][2] = 3;
    field[1][2] = 2;
    field[0][2] = 1;

    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }
    movement(7, snake_direction_down, 0, 2, field);
    cout << endl;
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }
    movement(7, snake_direction_down, 0, 3, field);
    cout << "Oops" << endl;
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }
}

//Тест по огибанию препятствий
void evade_obstacle()
{
    int field[field_size_x][field_size_y];
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            field[i][j] = field_cell_type_none;
        }
    }
    cout << "Evading obstacles" << endl;
    for (int j = 0; j < 5; j++)
    {
        field[0][j] = j + 1;
    }
    field[0][5] = field_cell_type_wall;

    cout << "Starting" << endl;
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }
    movement(4, snake_direction_right, 4, 0, field);
    movement(4, snake_direction_down, 4, 1, field);
    movement(4, snake_direction_down, 5, 1, field);
    movement(4, snake_direction_left, 6, 1, field);
    cout << "Final pos(4 moves)" << endl;
    for (int i = 0; i < field_size_x; i++)
    {
        for (int j = 0; j < field_size_y; j++)
        {
            cout << get_symb(field[i][j]);
        }
        cout << endl;
    }
}


void test()
{
    setlocale(LC_ALL, "ru");
    srand(time(NULL));
    cout << "Testing apple placement." << endl;
    for (int i = 0; i < 2; i++)
    {
        check_apples();
    }

    cout << "Testing field clearing." << endl;
    field_clearer();
    cout << "Testing increasing in size." << endl;
    for (int i = 0; i < 2; i++)
    {
        check_snake_increase();
    }
    cout << "Testing basic game mechanics." << endl;
    check_game_mechs();
    cout << "Testing obstacle evading." << endl;
    evade_obstacle();
}