#ifndef MAIN_CPP_GAMEOVER_H
#define MAIN_CPP_GAMEOVER_H

#include <SFML/Graphics.hpp>

using namespace sf;

bool gameOver(RenderWindow & window) {
    Texture gameOverTexture1, gameOverTexture2, gameOverBackground;
    gameOverTexture1.loadFromFile("images/gameOver/gameOverQuit.png");
    gameOverTexture2.loadFromFile("images/gameOver/gameOverMenu.png");
    gameOverBackground.loadFromFile("images/gameOver/gameOverBackground.png");
    Sprite gameOver1(gameOverTexture1), gameOver2(gameOverTexture2), gameOverBg(gameOverBackground);
    bool isGameOver = true;
    int gameOverNum = 0;
    gameOver1.setPosition(26, 162);
    gameOver2.setPosition(411, 212);
    gameOverBg.setPosition(0, 0);

    while (isGameOver)
    {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        gameOver1.setColor(Color::White);
        gameOver2.setColor(Color::White);

        window.clear(Color(129, 181, 221));
        if (IntRect(26, 162, 324, 293).contains(Mouse::getPosition(window))) { gameOver1.setColor(Color{31, 70, 31}); gameOverNum = 1; }
        if (IntRect(411, 212, 350, 252).contains(Mouse::getPosition(window))) { gameOver2.setColor(Color{31, 70, 31}); gameOverNum = 2; }
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (gameOverNum == 1) {
                return false;
            }
            if (gameOverNum == 2){
                return true;
            }
        }
        window.draw(gameOverBg);
        window.draw(gameOver1);
        window.draw(gameOver2);
        window.display();
    }
    return false;
}

#endif //MAIN_CPP_GAMEOVER_H
