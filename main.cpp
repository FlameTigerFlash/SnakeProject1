#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>


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



auto get_random_empty_cell()
{
    int emptyCellCount = 0;
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (field[j][i] == FIELD_CELL_TYPE_NONE) {
                emptyCellCount++;
            }
        }
    }
    int target_empty_cell_index = std::rand() % emptyCellCount;
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
    apple_add();
}

auto draw_field(sf::RenderWindow &window)
{
sf::Texture snake_picture;
snake_picture.loadFromFile("images/snake.png");
sf::Sprite snake;
snake.setTexture(snake_picture);

sf::Texture none_picture;
none_picture.loadFromFile("images/none.png");
sf::Sprite none;
none.setTexture(none_picture);

sf::Texture apple_picture;
apple_picture.loadFromFile("images/apple.png");
sf::Sprite apple;
apple.setTexture(apple_picture);

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


auto read_keyboard()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        if(snake_direction != SNAKE_DIRECTION_DOWN) {
            snake_direction = SNAKE_DIRECTION_UP;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        if(snake_direction != SNAKE_DIRECTION_LEFT) {
            snake_direction = SNAKE_DIRECTION_RIGHT;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        if(snake_direction != SNAKE_DIRECTION_UP) {
            snake_direction = SNAKE_DIRECTION_DOWN;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        if(snake_direction != SNAKE_DIRECTION_RIGHT) {
            snake_direction = SNAKE_DIRECTION_LEFT;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        if(snake_direction != SNAKE_DIRECTION_DOWN) {
            snake_direction = SNAKE_DIRECTION_UP;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        if(snake_direction != SNAKE_DIRECTION_LEFT) {
            snake_direction = SNAKE_DIRECTION_RIGHT;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        if(snake_direction != SNAKE_DIRECTION_UP) {
            snake_direction = SNAKE_DIRECTION_DOWN;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        if(snake_direction != SNAKE_DIRECTION_RIGHT) {
            snake_direction = SNAKE_DIRECTION_LEFT;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
        gameOver = true;
    }
}



int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake", sf::Style::Close);
    clear_field();
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
         if(event.type == sf::Event::Closed)
             window.close();
        }
        movement();

        if(gameOver){
            window.close();
        }
        window.clear(sf::Color(150, 212, 140));

        draw_field(window);

        read_keyboard();

        window.display();

        sf::sleep(sf::milliseconds(100));
    }
    return 0;
}

