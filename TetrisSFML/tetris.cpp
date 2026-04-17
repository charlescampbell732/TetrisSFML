#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <time.h>

#include <random>
#include <chrono>
#include <thread>

// Just map of the grid
const int l = 10;
const int g = 20;
int grid[l][g] = { 0 };


// Makes a structure that makes and x and y variable tied to a and b
struct Point { int x, y; } a[4], b[4];

static bool check()
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 or a[i].x >= l or a[i].y >= g) return 0;
        else if (grid[a[i].x][a[i].y]) return 0;
    
    return 1;
}

int main()
{

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    sf::RenderWindow window(sf::VideoMode({ 576, 672 }), "Tetris");

    // Importing assets made
    sf::Texture tiles("./TetrisImg./TetrisTiles.png");
    sf::Texture bg("./TetrisImg./tetrisbg.png");
    sf::Sprite tex(tiles);
    sf::Sprite bacg(bg);
    // Offset for where the things will be placed
    const int offset = 128;
    tex.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 }));

    // This is the size of the grid space for tetris to be played
    

    // This will hold the different shapes that are in tetris
    int shapes[7][4] = {
        // There is a grid that I'm using to create these, looks something like this
        // 0 1
        // 2 3
        // 4 5
        // 6 7
        // Then I'm just filling the grid with the correlating tile
        
        // Z SHAPE
        { 3, 5, 4, 6 },
        // I SHAPE
        { 0, 2, 4, 6 },
        // S SHAPE
        { 2, 4, 5, 7 },
        // O SHAPE
        { 4, 5, 6, 7 },
        // L SHAPE
        { 2, 4, 6, 7 }, // Flipped{ 4, 5, 7, 9 }
        // J SHAPE
        { 3, 5, 7, 6 },
        // T SHAPE
        { 4, 5, 3, 7 }, // Flipped { 6, 5, 7, 9 }
    };

    // Variables for moving / rotating
    int move = 0;
    bool rotate = 0;

    // For no errors / warnings
    a[0].x = 0;

    // Timer for the game play to continue
    float timer = 0;
    float speed = 0.5;
    sf::Clock clock;
    int color = 1;
    int n = rand() % 7;

    int happened = 0;

    // The loop to draw eveything
    while (window.isOpen())
    {
        sf::Time elapsed = clock.getElapsedTime();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // To move the piece to the left
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                // This is added so that it doesn't start flying in a direction (somehow happened once or twice)
                move = 1;
            }
            // To move the piece to the right
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                // This is added so that it doesn't start flying in a direction (somehow happened once or twice)
                move = -1;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
            {
                // Rotates piece
                rotate = 1;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                // Soft drop
                speed = 0.05f;
            }
           
        }
        
        
        // Movement code
        for (int i = 0; i < 4; i++) {
            // Storing the location before so that if it can't move there it won't
            b[i] = a[i];
            a[i].x += move;
        }
        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

        // Rotate code
        if (rotate == 1) {
            Point p = a[1];
            for (int i = 0; i < 4; i++) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
        }

        // Falling
        if (elapsed.asSeconds() > speed) {
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
                
            }
            if (!check()) {
                for (int i = 0; i < 4; i++) {
                    grid[b[i].x][b[i].y] = color;
                }
                color = 1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++) {
                    a[i].x = shapes[n][i] % 2;
                    a[i].y = shapes[n][i] / 2;
                }
            }
            
            clock.restart();
        }

        // To clear the lines (test)

        for (int i = 0; i < g; i++) {
            int count = 0;
            for (int j = 0; j < l; j++) {
                if (grid[i][j]) count++;
            }
            if (count == 9) {
                
            }
            
        }

        const float m = 32;

        // Setting them back to nothing so they can be changed again
        rotate = 0;
        move = 0;
        speed = 0.5;

        window.clear();
        window.draw(bacg);

        for (int i = 0; i < l; i++)
            for (int j = 0; j < g; j++) {
                if (grid[i][j] == 0) continue;
                tex.setTextureRect(sf::IntRect({ grid[i][j] * 32, 0 }, { 32, 32 }));
                tex.setPosition({ i*m + offset, j*m});
                window.draw(tex);
            }

        // Draws the object
        
        if (happened == 0)
        for (int i = 0; i < 4; i++) {
            a[i].x = shapes[n][i] % 2;
            a[i].y = shapes[n][i] / 2;
            happened = 1;
        }

        for (int i = 0; i < 4; i++) {
            tex.setTextureRect(sf::IntRect({ color * 32 , 0 }, { 32, 32 }));
            tex.setPosition({ a[i].x * m + offset, a[i].y * m});
            window.draw(tex);
        }


        // Display
        window.display();

       
    }
    return 0;
}