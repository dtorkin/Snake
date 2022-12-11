#ifndef MAIN_CPP_MENU_H
#define MAIN_CPP_MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

using namespace sf;

void menu(RenderWindow &window, std::vector<int> &vect) {
    Texture menuTexture1, menuTexture2, menuTexture3, menuBackground, lbBackground;
    menuTexture1.loadFromFile("images/menu/menuStart.png");
    menuTexture2.loadFromFile("images/menu/menuLeaders.png");
    menuTexture3.loadFromFile("images/menu/menuExit.png");
    menuBackground.loadFromFile("images/menu/menuWallpaper.png");
    lbBackground.loadFromFile("images/menu/leaderboard.png");
    Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menuBg(menuBackground);
    Sprite lbBg(lbBackground);
    bool isMenu = true;
    int menuNum;
    menu1.setPosition(312, 19);
    menu2.setPosition(356, 225);
    menu3.setPosition(342, 424);
    menuBg.setPosition(0, 0);

    Font font;
    font.loadFromFile("Roboto-Bold.ttf");
    Text    place1(std::to_string(vect[0]), font, 50),
            place2(std::to_string(vect[1]), font, 50),
            place3(std::to_string(vect[2]), font, 50),
            place4(std::to_string(vect[3]), font, 50),
            place5(std::to_string(vect[4]), font, 50);
    place1.setFillColor(Color::Black);
    place2.setFillColor(Color::Black);
    place3.setFillColor(Color::Black);
    place4.setFillColor(Color::Black);
    place5.setFillColor(Color::Black);

    place1.setPosition(108, 135);
    place2.setPosition(108, 195);
    place3.setPosition(108, 255);
    place4.setPosition(108, 315);
    place5.setPosition(108, 375);
    lbBg.setPosition(0, 0);

// МЕНЮ
    while (isMenu) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        SoundBuffer chooseBuffer;
        chooseBuffer.loadFromFile("audio/choose.ogg");
        Sound chooseSound(chooseBuffer);

        menu1.setColor(Color::White);
        menu2.setColor(Color::White);
        menu3.setColor(Color::White);
        menuNum = 0;
        window.clear(Color(129, 181, 221));
        if (IntRect(312, 19, 208, 186).contains(Mouse::getPosition(window))) {
            menu1.setColor(Color{31, 70, 31});
            menuNum = 1;
        }
        if (IntRect(356, 225, 161, 170).contains(Mouse::getPosition(window))) {
            menu2.setColor(Color{31, 70, 31});
            menuNum = 2;
        }
        if (IntRect(342, 424, 107, 92).contains(Mouse::getPosition(window))) {
            menu3.setColor(Color{31, 70, 31});
            menuNum = 3;
        }
        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню
            if (menuNum == 2) {
                chooseSound.play();
                window.draw(lbBg);
                window.draw(place1);
                window.draw(place2);
                window.draw(place3);
                window.draw(place4);
                window.draw(place5);
                window.display();
                while (!Keyboard::isKeyPressed(Keyboard::Escape));
            }
            if (menuNum == 3) {
                window.close();
                isMenu = false;
            }
        }
        window.draw(menuBg);
        window.draw(menu1);
        window.draw(menu2);
        window.draw(menu3);
        window.display();
    }
////////////////////////////////////////////////////
}

#endif //MAIN_CPP_MENU_H
