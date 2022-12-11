#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <fstream>
#include <sstream>
#include "menu.h"
#include "gameOver.h"

using namespace sf;

const int N = 25, M = 19;
const int size = 32;
const int h = size * M;

std::ifstream is("leaders.txt");
std::istream_iterator<double> start(is), end;
std::vector<int> leaderboardResults(start, end);

enum directions {down, up, right, left};
int dir = directions::right;

int snakeLenght = 2;

class Snake {
public:
    int x, y;
} snake[500];

bool isAlive = true;
bool click = true;

class Fruit {
public:
    int x, y;
} apple;

Sprite snakeHeadSprite;
Sound appleTake;

void Update() {
    // змеиный хвостик :3
    for (int i = snakeLenght; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    // перемещение головы по полю
    if (dir == directions::down) snake[0].y += 1;
    if (dir == directions::up) snake[0].x -= 1;
    if (dir == directions::right) snake[0].x += 1;
    if (dir == directions::left) snake[0].y -= 1;

    // кушаем яблоко
    if ((snake[0].x == apple.x) && (snake[0].y == apple.y)) {
        appleTake.play();
        snakeLenght++;
        apple.x = rand() % N;
        apple.y = rand() % M;
        for (int i = 0; i < snakeLenght; i++) {
            if ((snake[i].x == apple.x) && (snake[i].y == apple.y)) {
                apple.x = rand() % N;
                apple.y = rand() % M;
                break;
            }
        }
    }

    // выход за пределы экрана
    if (snake[0].x >= N) snake[0].x = 0;
    if (snake[0].x < 0) snake[0].x = N - 1;
    if (snake[0].y >= M) snake[0].y = 0;
    if (snake[0].y < 0) snake[0].y = M - 1;

    // смерть если врезаешься в себя
    for (int i = 1; i < snakeLenght; i++)
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            isAlive = false;
        }
}
bool startGame() {

    if ((dir = directions::up)) snakeHeadSprite.setTextureRect(IntRect(64, 32, -32, -32));
    if ((dir = directions::right)) snakeHeadSprite.setTextureRect(IntRect(32, 0, 32, 32));
    if ((dir = directions::left)) snakeHeadSprite.setTextureRect(IntRect(32, 32, -32, -32));
    if ((dir = directions::down)) snakeHeadSprite.setTextureRect(IntRect(0, 0, 32, 32));

    int playerScore = 0;

    snakeLenght = 2;
    isAlive = true;
    click = true;

    srand(time(nullptr));

    RenderWindow window(VideoMode(800, 608), "Snake Game!");
    menu(window, leaderboardResults);

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

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        if (isAlive) gameTime = int(gameTimeClock.getElapsedTime().asSeconds());
        clock.restart();
        timer += time;

        Event e{};
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) { return true; }//если таб, то перезагружаем игру

        if (timer > delay) {
            timer = 0;
            click = true;
            Update();
        }

        if (isAlive) {
            if (click) {
                if ((Keyboard::isKeyPressed(Keyboard::Left) && (dir != 2))) {
                    dir = directions::up;
                    snakeHeadSprite.setTextureRect(IntRect(32, 0, 32, 32));
                    click = false;
                }
                if ((Keyboard::isKeyPressed(Keyboard::Right)) && (dir != 1)) {
                    dir = directions::right;
                    snakeHeadSprite.setTextureRect(IntRect(64, 32, -32, -32));
                    click = false;
                }
                if ((Keyboard::isKeyPressed(Keyboard::Up)) && (dir != 0)) {
                    dir = directions::left;
                    snakeHeadSprite.setTextureRect(IntRect(0, 0, 32, 32));
                    click = false;
                }
                if ((Keyboard::isKeyPressed(Keyboard::Down)) && (dir != 3)) {
                    dir = directions::down;
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

            for (int i = 0; i < snakeLenght; i++) {
                snakeSprite.setPosition(float(snake[i].x * size), float(snake[i].y * size));
                window.draw(snakeSprite);
                snakeHeadSprite.setPosition(float(snake[0].x * size), float(snake[0].y * size));
                window.draw(snakeHeadSprite);
            }

            appleSprite.setPosition(float(apple.x * size), float(apple.y * size));
            window.draw(appleSprite);

            std::ostringstream gameTimeSeconds, gameTimeMinutes;
            gameTimeSeconds << gameTime % 60;
            gameTimeMinutes << gameTime / 60;
            text.setString("Playtime\n" + gameTimeMinutes.str() + ':' + gameTimeSeconds.str());
            text.setPosition(6, 6);
            window.draw(text);

            playerScore = snakeLenght - 2;

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

