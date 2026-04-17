#include <SFML/Graphics.hpp>
#include <deque>
#include <optional>
#include <cstdlib>
#include <string>

const int CELL_SIZE = 20;
const int WIDTH = 30;
const int HEIGHT = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

void resetGame(std::deque<sf::Vector2i>& snake, Direction& dir, sf::Vector2i& food, int& score) {
    snake = { {10, 10}, {9, 10}, {8, 10} };
    dir = RIGHT;
    food = sf::Vector2i(rand() % WIDTH, rand() % HEIGHT);
    score = 0;
}

int main() {
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE)),
        "Snake"
    );

    std::deque<sf::Vector2i> snake;
    Direction dir;
    sf::Vector2i food;
    int score = 0;

    bool gameOver = false;

    resetGame(snake, dir, food, score);

    sf::Clock clock;
    float delay = 0.15f;

    while (window.isOpen()) {
        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>();

                if (!gameOver) {
                    if (key->code == sf::Keyboard::Key::W && dir != DOWN) dir = UP;
                    if (key->code == sf::Keyboard::Key::S && dir != UP) dir = DOWN;
                    if (key->code == sf::Keyboard::Key::A && dir != RIGHT) dir = LEFT;
                    if (key->code == sf::Keyboard::Key::D && dir != LEFT) dir = RIGHT;
                }

                // Restart game
                if (gameOver && key->code == sf::Keyboard::Key::R) {
                    resetGame(snake, dir, food, score);
                    gameOver = false;
                }
            }
        }

        // Game update
        if (!gameOver && clock.getElapsedTime().asSeconds() > delay) {
            clock.restart();

            sf::Vector2i head = snake.front();

            if (dir == UP) head.y--;
            if (dir == DOWN) head.y++;
            if (dir == LEFT) head.x--;
            if (dir == RIGHT) head.x++;

            // Wall collision
            if (head.x < 0 || head.y < 0 || head.x >= WIDTH || head.y >= HEIGHT) {
                gameOver = true;
            }

            // Self collision
            for (auto& s : snake) {
                if (head == s) {
                    gameOver = true;
                }
            }

            snake.push_front(head);

            // Eat food
            if (head == food) {
                score++;
                food = sf::Vector2i(rand() % WIDTH, rand() % HEIGHT);
            } else {
                snake.pop_back();
            }
        }

        // Update window title with score
        std::string title = "Snake | Score: " + std::to_string(score);
        if (gameOver) title += " | GAME OVER (Press R to Restart)";
        window.setTitle(title);

        // Draw
        window.clear();

        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

        // Snake
        rect.setFillColor(sf::Color::Green);
        for (auto& s : snake) {
            rect.setPosition(sf::Vector2f(s.x * CELL_SIZE, s.y * CELL_SIZE));
            window.draw(rect);
        }

        // Food
        rect.setFillColor(sf::Color::Red);
        rect.setPosition(sf::Vector2f(food.x * CELL_SIZE, food.y * CELL_SIZE));
        window.draw(rect);

        window.display();
    }

    return 0;
}
