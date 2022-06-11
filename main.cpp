#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <vector>
#include <string>

const int FIELD_CELL_TYPE_NONE = 0;
const int FIELD_CELL_TYPE_APPLE =  -1;
const int FIELD_CELL_TYPE_WALL = -2;
const int SNAKE_DIRECTION_UP = 0;
const int SNAKE_DIRECTION_RIGHT = 1;
const int SNAKE_DIRECTION_DOWN = 2;
const int SNAKE_DIRECTION_LEFT = 3;
const int LIVES = 3;

const int field_size_x = 35;
const int field_size_y = 25;
const int cell_size = 32;
const int score_bar_height = 60;
const int window_width = field_size_x * cell_size;
const int window_height = field_size_y * cell_size;

int field[field_size_x][field_size_y];
int snake_position_x = field_size_x / 2;
int snake_position_y = field_size_y / 2;
int snake_length = 4;
int snake_direction = SNAKE_DIRECTION_RIGHT;
bool gameOver = false;

sf::Texture snake_picture;
sf::Sprite snake;

sf::Texture none_picture;
sf::Sprite none;

sf::Texture apple_picture;
sf::Sprite apple;

sf::Texture wall_picture;
sf::Sprite wall;


auto init_game()
{
    srand(time(NULL));

    snake_picture.loadFromFile("images/snake.png");
    snake.setTexture(snake_picture);

    none_picture.loadFromFile("images/none.png");
    none.setTexture(none_picture);

    apple_picture.loadFromFile("images/apple.png");
    apple.setTexture(apple_picture);

    wall_picture.loadFromFile("images/wall.png");
    wall.setTexture(wall_picture);

}


auto get_random_empty_cell()
{
    int empty_cell_count = 0;
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (field[j][i] == FIELD_CELL_TYPE_NONE) {
                empty_cell_count++;
            }
        }
    }
    int target_empty_cell_index = std::rand() % empty_cell_count;
    int empty_cell_index = 0;
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (field[j][i] == FIELD_CELL_TYPE_NONE) {
                if (empty_cell_index == target_empty_cell_index) {
                    return j * field_size_x + i;
                }
                empty_cell_index++;
            }
        }
    }
    return -1;
}

auto apple_add()
{
    int applePosition = get_random_empty_cell();
    if (applePosition != -1) {
        field[applePosition / field_size_x][applePosition % field_size_x] = FIELD_CELL_TYPE_APPLE;
    }
}


auto clear_field()
{
    for(int i = 0; i < field_size_x; i++){
        for (int j = 0; j < field_size_y; j++){
            field[i][j] = 0;
        }
    }
    for(int i = 0; i < snake_length; i++){
       field[snake_position_x - i][snake_position_y] = snake_length - i;
    }
    for(int i = 0; i < field_size_x; i += 1){
        if(i < 6 || (field_size_x - i - 1) < 6) {
            field[i][0] = FIELD_CELL_TYPE_WALL;
            field[i][field_size_y - 1] = FIELD_CELL_TYPE_WALL;
        }
    }
    for(int i = 0; i < field_size_y; i += 1){
        if(i < 6 || (field_size_y -i - 1) < 6) {
            field[0][i] = FIELD_CELL_TYPE_WALL;
            field[field_size_x - 1][i] = FIELD_CELL_TYPE_WALL;
        }
    }
    apple_add();
}

auto draw_field(sf::RenderWindow &window)
{

    for(int i = 0; i < field_size_x; i++){
        for (int j = 0; j < field_size_y; j++){
            switch(field[i][j]) {
                case FIELD_CELL_TYPE_NONE:
                    none.setPosition(float(i * cell_size), float(j * cell_size));
                    window.draw(none);
                    break;
                case FIELD_CELL_TYPE_APPLE:
                    apple.setPosition(float(i * cell_size), float(j * cell_size));
                    window.draw(apple);
                    break;
                case FIELD_CELL_TYPE_WALL:
                    wall.setPosition(float(i * cell_size), float(j * cell_size));
                    window.draw(wall);
                    break;
                default:
                    snake.setPosition(float(i * cell_size), float(j * cell_size));
                    window.draw(snake);
            }


        }
    }
}

auto increaseSnake()
{
    for(int i = 0; i < field_size_x; i++) {
        for (int j = 0; j < field_size_y; j++) {
            if (field[i][j] > FIELD_CELL_TYPE_NONE) {
                field[i][j] += 1;
            }
        }
    }
}


auto movement() {
    switch (snake_direction) {
        case SNAKE_DIRECTION_UP:
            snake_position_y -= 1;
            if (snake_position_y < 0){
                snake_position_y = field_size_y - 1;
            }
            break;
        case SNAKE_DIRECTION_RIGHT:
            snake_position_x += 1;
            if (snake_position_x > field_size_x - 1){
                snake_position_x = 0;
            }
            break;
        case SNAKE_DIRECTION_DOWN:
            snake_position_y += 1;
            if (snake_position_y > field_size_y - 1){
                snake_position_y = 0;
            }
            break;
        case SNAKE_DIRECTION_LEFT:
            snake_position_x -= 1;
            if (snake_position_x < 0){
                snake_position_x = field_size_x - 1;
            }
            break;
    }

    if((field[snake_position_x][snake_position_y]) != FIELD_CELL_TYPE_NONE){
        switch(field[snake_position_x][snake_position_y]){
            case FIELD_CELL_TYPE_APPLE:
                snake_length += 1;
                increaseSnake();
                apple_add();
                break;
            default:
                gameOver = true;
        }
    }

    field[snake_position_x][snake_position_y] = snake_length + 1;

    for (int i = 0; i < field_size_x; i++) {
        for (int j = 0; j < field_size_y; j++) {
            if (field[i][j] > 0) {
                field[i][j] -= 1;
            }
        }
    }
}


int main()
{
    init_game();

    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake", sf::Style::Close);

    clear_field();

    std::vector<int> snake_direction_buffer;


    while(window.isOpen()){
        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
             window.close();

            if(event.type == sf::Event::KeyPressed){
             int snake_direction_last = snake_direction_buffer.empty() ? snake_direction : snake_direction_buffer.at(0);
             switch(event.key.code){
                 case sf::Keyboard::Up:
                     if(snake_direction_last != SNAKE_DIRECTION_DOWN) {
                         if(snake_direction_buffer.size() < 2){
                             snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_UP);
                         }
                     }
                     break;
                 case sf::Keyboard::Down:
                     if(snake_direction_last != SNAKE_DIRECTION_UP) {
                         if(snake_direction_buffer.size() < 2) {
                             snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_DOWN);
                         }
                     }
                     break;
                 case sf::Keyboard::Left:
                     if(snake_direction_last != SNAKE_DIRECTION_RIGHT) {
                         if(snake_direction_buffer.size() < 2) {
                             snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_LEFT);
                         }
                     }
                     break;
                 case sf::Keyboard::Right:
                     if(snake_direction_last != SNAKE_DIRECTION_LEFT) {
                         if(snake_direction_buffer.size() < 2) {
                             snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_RIGHT);
                         }

                     }
                     break;
                 case sf::Keyboard::W:
                     if(snake_direction_last != SNAKE_DIRECTION_DOWN) {
                         if(snake_direction_buffer.size() < 2) {
                             snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_UP);
                         }
                     }
                     break;
                 case sf::Keyboard::S:
                     if(snake_direction_last != SNAKE_DIRECTION_UP) {
                         if(snake_direction_buffer.size() < 2) {
                             snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_DOWN);
                         }
                     }
                     break;
                 case sf::Keyboard::A:
                     if(snake_direction_last != SNAKE_DIRECTION_RIGHT) {
                         if(snake_direction_buffer.size() < 2) {
                             snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_LEFT);
                         }
                     }
                     break;
                 case sf::Keyboard::D:
                     if(snake_direction_last != SNAKE_DIRECTION_LEFT) {
                         if(snake_direction_buffer.size() < 2) {
                             snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_RIGHT);
                         }
                     }
                     break;
                 case sf::Keyboard::Escape:
                     gameOver = true;
                     break;
             }
             if(!snake_direction_buffer.empty()){
                 snake_direction = snake_direction_buffer.back(); //Возвращает последний элемент вектора
                 snake_direction_buffer.pop_back();

             }
         }
        }

        movement();

        if(gameOver){
            window.close();
        }
        window.clear(sf::Color(150, 212, 140));

        draw_field(window);

        window.display();

        sf::sleep(sf::milliseconds(100));
    }
    return 0;
}

