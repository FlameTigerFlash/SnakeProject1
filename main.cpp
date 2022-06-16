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

const int field_size_x = 35;
const int field_size_y = 25;
const int cell_size = 32;
const int window_width = field_size_x * cell_size;
const int window_height = field_size_y * cell_size;

int field[field_size_x][field_size_y];
int snake_position_x = field_size_x / 2;
int snake_position_y = field_size_y / 2;
int snake_length = 4;
int snake_direction = SNAKE_DIRECTION_RIGHT;
int score = 0;
bool game_paused = false;
bool game_over = false;


sf::Texture snake_picture;
sf::Sprite snake;

sf::Texture none_picture;
sf::Sprite none;

sf::Texture apple_picture;
sf::Sprite apple;

sf::Texture wall_picture;
sf::Sprite wall;

sf::Texture snake_head_picture;
sf::Sprite snake_head;

sf::SoundBuffer sb_ate_apple;
sf::Sound sound_ate_apple;

sf::SoundBuffer sb_snake_kill_yourself;
sf::Sound sound_snake_kill_yourself;

sf::SoundBuffer sb_pause_game;
sf::Sound sound_pause_game;

sf::SoundBuffer sb_snake_dead_from_wall;
sf::Sound sound_snake_dead_from_wall;

sf::SoundBuffer sb_score10;
sf::Sound sound_score10;

sf::Font font_score;
sf::Text text_score;

sf::Font font_game_over;
sf::Text text_game_over;


auto init_game()
{
    std::srand(time(NULL));

    snake_picture.loadFromFile("images/snake.png");
    snake.setTexture(snake_picture);

    none_picture.loadFromFile("images/none.png");
    none.setTexture(none_picture);

    apple_picture.loadFromFile("images/apple.png");
    apple.setTexture(apple_picture);

    wall_picture.loadFromFile("images/wall.png");
    wall.setTexture(wall_picture);

    snake_head_picture.loadFromFile("images/head.png");
    snake_head.setTexture(snake_head_picture);

    sb_ate_apple.loadFromFile("sounds/get_apple2.wav");
    sound_ate_apple.setBuffer(sb_ate_apple);
    sound_ate_apple.setVolume(50);

    sb_snake_kill_yourself.loadFromFile("sounds/died.wav");
    sound_snake_kill_yourself.setBuffer(sb_snake_kill_yourself);
    sound_snake_kill_yourself.setVolume(50);

    sb_snake_dead_from_wall.loadFromFile("sounds/dead.wav");
    sound_snake_dead_from_wall.setBuffer(sb_snake_dead_from_wall);
    sound_snake_dead_from_wall.setVolume(70);

    sb_pause_game.loadFromFile("sounds/pause_game.wav");
    sound_pause_game.setBuffer(sb_pause_game);
    sound_pause_game.setVolume(20);

    sb_score10.loadFromFile("sounds/score10.wav");
    sound_score10.setBuffer(sb_score10);
    sound_score10.setVolume(45);

    font_score.loadFromFile("fonts/BigfatScript-2OvA8.otf");
    text_score.setFont(font_score);

    font_game_over.loadFromFile("fonts/BigOldBoldy-dEjR.ttf");
    text_game_over.setFont(font_game_over);

    text_game_over.setString("GAME OVER");
    text_game_over.setCharacterSize(86);
    text_game_over.setFillColor(sf::Color::Blue);
    text_game_over.setPosition((window_width - text_game_over.getLocalBounds().width) / 2, (window_height - text_game_over.getLocalBounds().height) / 2 - 30);
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
                    if(field[i][j] == snake_length){
                        float offset_x = snake_head.getLocalBounds().width / 2;
                        float offset_y = snake_head.getLocalBounds().height / 2;
                        snake_head.setPosition(float(i * cell_size + offset_x), float(j * cell_size) + offset_y);
                        snake_head.setOrigin(offset_x,offset_y);
                        switch(snake_direction){
                            case SNAKE_DIRECTION_UP:
                                snake_head.setRotation(90);
                                break;
                            case SNAKE_DIRECTION_RIGHT:
                                snake_head.setRotation(-180);
                                break;
                            case SNAKE_DIRECTION_LEFT:
                                snake_head.setRotation(0);
                                break;
                            case SNAKE_DIRECTION_DOWN:
                                snake_head.setRotation(-90);
                                break;
                        }
                        window.draw(snake_head);
                    }
                    else{
                        snake.setPosition(float(i * cell_size), float(j * cell_size));
                        window.draw(snake);
                    }
            }
        }
    }
    // Draw score_bar//
    text_score.setString("Score: " + std::to_string(score));
    text_score.setCharacterSize(28);
    if(score > 50){
        text_score.setFillColor(sf::Color::Red);
        text_score.setPosition(window_width / 2 - 65, 0);
        window.draw(text_score);
    }
    else{
        text_score.setFillColor(sf::Color::Black);
        text_score.setPosition(window_width / 2 - 65, 0);
        window.draw(text_score);
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
        switch(field[snake_position_x][snake_position_y]) {
            case FIELD_CELL_TYPE_APPLE:
                sound_ate_apple.play();
                snake_length += 1;
                score += 1;
                if(score % 10 == 0){
                    sound_score10.play();
                }
                increaseSnake();
                apple_add();
                break;
            case FIELD_CELL_TYPE_WALL:
                sound_snake_dead_from_wall.play();
                game_over = true;
                break;
            default:
                if (field[snake_position_x][snake_position_y] > 1) {
                    sound_snake_kill_yourself.play();
                    game_over = true;
                }
        }
    }
    if(!game_over) {
        for (int i = 0; i < field_size_x; i++) {
            for (int j = 0; j < field_size_y; j++) {
                if (field[i][j] > 0) {
                    field[i][j] -= 1;
                }
            }
        }
        field[snake_position_x][snake_position_y] = snake_length;
    }
}


int main()
{
    init_game();

    std::srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Snake", sf::Style::Close);

    clear_field();

    std::vector<int> snake_direction_buffer;


    while(window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (game_paused) {
                    switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            game_paused = false;
                            sound_pause_game.play();
                            break;
                        case sf::Keyboard::G:
                            game_over = true;
                            sound_snake_dead_from_wall.play();
                            break;
                    }

                } else {
                    int snake_direction_last = snake_direction_buffer.empty() ? snake_direction : snake_direction_buffer.at(0);
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                            if (snake_direction_last != SNAKE_DIRECTION_DOWN) {
                                if (snake_direction_buffer.size() < 2) {
                                    snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_UP);
                                }
                            }
                            break;
                        case sf::Keyboard::Down:
                            if (snake_direction_last != SNAKE_DIRECTION_UP) {
                                if (snake_direction_buffer.size() < 2) {
                                    snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_DOWN);
                                }
                            }
                            break;
                        case sf::Keyboard::Left:
                            if (snake_direction_last != SNAKE_DIRECTION_RIGHT) {
                                if (snake_direction_buffer.size() < 2) {
                                    snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_LEFT);
                                }
                            }
                            break;
                        case sf::Keyboard::Right:
                            if (snake_direction_last != SNAKE_DIRECTION_LEFT) {
                                if (snake_direction_buffer.size() < 2) {
                                    snake_direction_buffer.insert(snake_direction_buffer.begin(),SNAKE_DIRECTION_RIGHT);
                                }

                            }
                            break;
                        case sf::Keyboard::W:
                            if (snake_direction_last != SNAKE_DIRECTION_DOWN) {
                                if (snake_direction_buffer.size() < 2) {
                                    snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_UP);
                                }
                            }
                            break;
                        case sf::Keyboard::S:
                            if (snake_direction_last != SNAKE_DIRECTION_UP) {
                                if (snake_direction_buffer.size() < 2) {
                                    snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_DOWN);
                                }
                            }
                            break;
                        case sf::Keyboard::A:
                            if (snake_direction_last != SNAKE_DIRECTION_RIGHT) {
                                if (snake_direction_buffer.size() < 2) {
                                    snake_direction_buffer.insert(snake_direction_buffer.begin(), SNAKE_DIRECTION_LEFT);
                                }
                            }
                            break;
                        case sf::Keyboard::D:
                            if (snake_direction_last != SNAKE_DIRECTION_LEFT) {
                                if (snake_direction_buffer.size() < 2) {
                                    snake_direction_buffer.insert(snake_direction_buffer.begin(),SNAKE_DIRECTION_RIGHT);
                                }
                            }
                            break;
                        case sf::Keyboard::Escape:
                            game_paused = true;
                            sound_pause_game.play();
                            break;
                        case sf::Keyboard::G:
                            game_over = true;
                            sound_snake_dead_from_wall.play();
                            break;
                    }
                    if (!snake_direction_buffer.empty()) {
                        snake_direction = snake_direction_buffer.back(); // Возвращает последний элемент вектора //
                        snake_direction_buffer.pop_back();

                    }
                }
            }
        }

        if (!game_paused) {
        movement();
        }

        window.clear(sf::Color(150, 212, 140));

        draw_field(window);

        if(game_over){
            window.draw(text_game_over);
            window.display();
            sf::sleep(sf::seconds(2));
            window.close();
        }

        window.display();

        sf::sleep(sf::milliseconds(100));
    }
    return 0;
}

