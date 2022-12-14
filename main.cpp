#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include "menu.h"
#include "gameOver.h"

using namespace sf;

const int N = 25, M = 19;
const int size = 32;
const int h = size * M;
bool gameWithWalls;

std::ifstream is("leaders.txt");
std::istream_iterator<double> start(is), end;
std::vector<int> leaderboardResults(start, end);

class Snake {
public:
    int x = 1, y = 2;
    int lenght = 2;
    enum directions {
        down, up, right, left
    };
    int dir = directions::right;
    bool isAlive = true;
} snake[500];

bool click = true;

class Fruit {
public:
    int x, y;
} apple;

Sprite snakeHeadSprite;
Sound appleTake;

void Update() {
    // змеиный хвостик :3
    for (int i = snake->lenght; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    // перемещение головы по полю
    if (snake->dir == Snake::directions::down) snake[0].y += 1;
    if (snake->dir == Snake::directions::up) snake[0].x -= 1;
    if (snake->dir == Snake::directions::right) snake[0].x += 1;
    if (snake->dir == Snake::directions::left) snake[0].y -= 1;

    // кушаем яблоко
    if ((snake[0].x == apple.x) && (snake[0].y == apple.y)) {
        appleTake.play();
        snake->lenght++;

        if (gameWithWalls) {
            apple.x = rand() % (N - 2) + 1;
            apple.y = rand() % (M - 2) + 1;
        } else {
            apple.x = rand() % N;
            apple.y = rand() % M;
        }
        for (int i = 0; i < snake->lenght; i++) {
            if ((snake[i].x == apple.x) && (snake[i].y == apple.y)) {
                if (gameWithWalls) {
                    apple.x = rand() % (N - 2) + 1;
                    apple.y = rand() % (M - 2) + 1;
                } else {
                    apple.x = rand() % N;
                    apple.y = rand() % M;
                }
            }
        }
    }

    // выход за пределы экрана
    if (snake[0].x >= N) snake[0].x = 0;
    if (snake[0].x < 0) snake[0].x = N - 1;
    if (snake[0].y >= M) snake[0].y = 0;
    if (snake[0].y < 0) snake[0].y = M - 1;

    // смерть если врезаешься в себя
    for (int i = 1; i < snake->lenght; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            snake->isAlive = false;
        }
    }
    if (gameWithWalls) { //смерть если в стену
        if ((snake[0].x == 0) || (snake[0].y == 0) || (snake[0].x == N - 1) || (snake[0].y == M - 1)) {
            snake->isAlive = false;
        }
    }
}

bool startGame() {
    if ((snake->dir = Snake::directions::up)) snakeHeadSprite.setTextureRect(IntRect(64, 32, -32, -32));
    if ((snake->dir = Snake::directions::right)) snakeHeadSprite.setTextureRect(IntRect(32, 0, 32, 32));
    if ((snake->dir = Snake::directions::left)) snakeHeadSprite.setTextureRect(IntRect(32, 32, -32, -32));
    if ((snake->dir = Snake::directions::down)) snakeHeadSprite.setTextureRect(IntRect(0, 0, 32, 32));

    int playerScore = 0;

    snake->lenght = 2;
    snake->isAlive = true;
    click = true;

    srand(time(nullptr));

    RenderWindow window(VideoMode(800, 608), "Snake Game!");
    menu(window, leaderboardResults);
    std::ifstream fileMode("wallsMode.txt");
    fileMode >> gameWithWalls;

    Texture grassTexture, snakeTexture, snakeHeadTexture, appleTexture, wallTexture;
    grassTexture.loadFromFile("images/grass.png");
    snakeTexture.loadFromFile("images/snake.png");
    snakeHeadTexture.loadFromFile("images/snakeHead.png");
    appleTexture.loadFromFile("images/apple.png");
    wallTexture.loadFromFile("images/wall.png");

    Sprite grassSprite(grassTexture);
    Sprite snakeSprite(snakeTexture);
    snakeHeadSprite.setTexture(snakeHeadTexture);
    Sprite appleSprite(appleTexture);
    Sprite wallSprite(wallTexture);

    Font font;
    font.loadFromFile("Roboto-Bold.ttf");
    Text text("", font, 16);
    text.setFillColor(Color::White);

    SoundBuffer appleBuffer;
    appleBuffer.loadFromFile("audio/apple1.ogg");
    appleTake.setBuffer(appleBuffer);

    SoundBuffer gameOverBuffer;
    gameOverBuffer.loadFromFile("audio/gameover.ogg");
    Sound gameOverSound(gameOverBuffer);

    SoundBuffer chooseBuffer;
    chooseBuffer.loadFromFile("audio/choose.ogg");
    Sound chooseSound(chooseBuffer);
    chooseSound.play();

    Music music;//создаем объект музыки
    music.openFromFile("audio/music.ogg");//загружаем файл
    music.play();//воспроизводим музыку
    music.setLoop(true);

    Clock clock;
    float timer = 0, delay = 0.1;

    Clock gameTimeClock;
    int gameTime = 0;

    apple.x = 3;
    apple.y = 5;
    snake[0].x = 1;
    snake[0].y = 2;

    std::vector<std::vector<int> > wall(M, std::vector<int>(N, 1));

    for (int i = 1; i < M - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            wall[i][j] = 0;
        }
    }

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        if (snake->isAlive) gameTime = int(gameTimeClock.getElapsedTime().asSeconds());
        clock.restart();
        timer += time;

        Event e{};
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) { return true; }//если Esc, то перезагружаем игру

        if (timer > delay) {
            timer = 0;
            click = true;
            Update();
        }

        if (snake->isAlive) {
            if (click) {
                if ((Keyboard::isKeyPressed(Keyboard::Left) && (snake->dir != 2))) {
                    snake->dir = Snake::directions::up;
                    snakeHeadSprite.setTextureRect(IntRect(32, 0, 32, 32));
                    click = false;
                }
                if ((Keyboard::isKeyPressed(Keyboard::Right)) && (snake->dir != 1)) {
                    snake->dir = Snake::directions::right;
                    snakeHeadSprite.setTextureRect(IntRect(64, 32, -32, -32));
                    click = false;
                }
                if ((Keyboard::isKeyPressed(Keyboard::Up)) && (snake->dir != 0)) {
                    snake->dir = Snake::directions::left;
                    snakeHeadSprite.setTextureRect(IntRect(0, 0, 32, 32));
                    click = false;
                }
                if ((Keyboard::isKeyPressed(Keyboard::Down)) && (snake->dir != 3)) {
                    snake->dir = Snake::directions::down;
                    snakeHeadSprite.setTextureRect(IntRect(32, 32, -32, -32));
                    click = false;
                }
            }
            //draw
            window.clear();

            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    grassSprite.setPosition(float(i * size), float(j * size));
                    window.draw(grassSprite);
                }
            }

            for (int i = 0; i < snake->lenght; i++) {
                snakeSprite.setPosition(float(snake[i].x * size), float(snake[i].y * size));
                window.draw(snakeSprite);
                snakeHeadSprite.setPosition(float(snake[0].x * size), float(snake[0].y * size));
                window.draw(snakeHeadSprite);
            }

            appleSprite.setPosition(float(apple.x * size), float(apple.y * size));
            window.draw(appleSprite);

            if (gameWithWalls) {
                for (int i = 0; i < M; i++) {
                    for (int j = 0; j < N; j++) {
                        if (wall[i][j] == 1) {
                            wallSprite.setPosition(float(j * size), float(i * size));
                            window.draw(wallSprite);
                        }
                    }
                }
            }
            std::ostringstream gameTimeSeconds, gameTimeMinutes;
            gameTimeSeconds << gameTime % 60;
            gameTimeMinutes << gameTime / 60;
            text.setString("Playtime\n" + gameTimeMinutes.str() + ':' + gameTimeSeconds.str());
            text.setPosition(6, 6);
            window.draw(text);

            playerScore = snake->lenght - 2;

            std::ostringstream playerScoreString;
            playerScoreString << playerScore;
            text.setString("Collected food: " + playerScoreString.str());
            text.setPosition(650, 6);
            window.draw(text);

            text.setString("Click Esc to Exit");
            text.setPosition(6, 585);
            window.draw(text);
        } else {
            gameOverSound.play();
            music.stop();
            leaderboardResults[5] = playerScore;
            std::sort(leaderboardResults.rbegin(), leaderboardResults.rend());

            std::ofstream iss("leaders.txt");
            for (const auto &q: leaderboardResults) iss << q << "\n";

            if (gameOver(window)) {
                return true;
            }
            return false;
        }
        window.display();
    }
    return false;
}

void gameRunning() {//ф-ция перезагружает игру , если это необходимо
    if (startGame()) { gameRunning(); }
}

int main() {
    gameRunning();
    return 0;
}
