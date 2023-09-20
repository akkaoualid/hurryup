#include <SFML/graphics.hpp>
#include <chrono>
#include <ctime>
#include <format>
#include <iostream>

int main() {
  int HEIGHT = 300;
  int WIDTH = 500;
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "HurryUp");

  auto now = std::chrono::system_clock::now();
  std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
  std::tm localTime{};
  localtime_s(&localTime, &currentTime);

  std::tm tm{};
  tm.tm_year = localTime.tm_year + 1;
  tm.tm_mday = 0;
  tm.tm_hour = 0;
  tm.tm_min = 0;

  std::time_t tmTime = std::mktime(&tm);

  auto nextYear = std::chrono::system_clock::from_time_t(tmTime);

  sf::Font font{};
  font.loadFromFile("<font location>");

  sf::Text days{};
  days.setFont(font);
  days.setCharacterSize(50);
  days.setFillColor(sf::Color(101, 67, 33));

  days.setPosition(WIDTH / 4.0f, HEIGHT / 4.0f);

  sf::Text seconds{};
  seconds.setFont(font);
  seconds.setCharacterSize(25);
  seconds.setFillColor(sf::Color(131, 97, 63));
  seconds.setPosition(WIDTH / 4.0f, HEIGHT / 4.0f);
  bool dragDays = false;
  bool dragSeconds = false;

  days.setStyle(sf::Text::Bold);
  seconds.setStyle(sf::Text::Bold);

  while (window.isOpen()) {
    auto rightNow = std::chrono::system_clock::now().time_since_epoch();
    auto duration = (nextYear.time_since_epoch() - rightNow);
    auto daysRemaining =
        std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24;
    auto hoursRemaining =
        std::chrono::duration_cast<std::chrono::hours>(duration).count() % 24;
    auto minutes =
        std::chrono::duration_cast<std::chrono::minutes>(duration).count() % 60;
    auto secondsLeft =
        std::chrono::duration_cast<std::chrono::seconds>(duration).count() % 60;
    days.setString(std::format("{} Days", daysRemaining));
    seconds.setString(std::format("{} Hours {} Minutes {} Seconds",
                                  hoursRemaining, minutes, secondsLeft));
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          auto [x, y] = sf::Mouse::getPosition(window);
          sf::Vector2f pos(x, y);
          auto s = sf::Vector2f(pos);
          dragDays = days.getGlobalBounds().contains(s);
          dragSeconds = seconds.getGlobalBounds().contains(s);
        }
      } else if (event.type == sf::Event::MouseButtonReleased) {
        dragDays = false;
        dragSeconds = false;
      }

      else if (event.type == sf::Event::MouseMoved && dragDays) {
        auto [x, y] = sf::Mouse::getPosition(window);
        auto [a, b] = days.getGlobalBounds().getSize();
        days.setPosition(x - a / 2, y - b / 2);
      } else if (event.type == sf::Event::MouseMoved && dragSeconds) {
        auto [x, y] = sf::Mouse::getPosition(window);
        auto [a, b] = days.getGlobalBounds().getSize();
        seconds.setPosition(x - a / 2, y - b / 2);
      }
    }
    window.clear(sf::Color(250, 214, 165));
    window.draw(seconds);
    window.draw(days);
    window.display();
  }
}
