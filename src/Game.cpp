#include "../include/Game.hpp"
#include "../include/Character.hpp"
#include "../include/ncurses.hpp"
#include <memory>
#include <ncurses.h>
#include <random>
#include <string>
#include <vector>

void Game::boldPrint(std::string phrase) {
  attron(A_REVERSE);
  printw(phrase.c_str());
  attroff(A_REVERSE);
}

int Game::getWave() { return wave; }

EnemyType Game::getRandomEnemyType() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  int numTypes = static_cast<int>(EnemyType::Count);

  std::uniform_real_distribution<> dist(0, numTypes);

  auto value{dist(gen)};
  return static_cast<EnemyType>(value);
}

std::unique_ptr<Enemy> Game::generateEnemy() {
  EnemyType type{getRandomEnemyType()};

  Stats baseStats{50, 50, 5, 3};
  Stats stats{Enemy::getEnemyStats(baseStats, getWave())};

  return std::make_unique<Enemy>(type, stats);
}

void Game::displayChoice(std::vector<std::string> choices, int &selected) {
  for (auto i{0}; i < choices.size(); i++) {
    if (i == selected) {
      boldPrint(choices[i]);
    } else {
      printw(choices[i].c_str());
    }
    printw("\n");
  }
}

int Game::getMenuSelection(const std::vector<std::string> &choices,
                           const std::string &prompt) {
  int selected{0};

  while (true) {
    clear();
    printw("%s\n", prompt.c_str());
    displayChoice(choices, selected);
    refresh();

    int ch{getch()};

    switch (ch) {
    case KEY_UP:
    case 'w':
      selected--;
      break;

    case KEY_DOWN:
    case 's':
      selected++;
      break;

    case '\n':
      return selected;
    }

    if (selected < 0)
      selected = choices.size() - 1;
    if (selected > choices.size() - 1)
      selected = 0;
  }
  return -1;
}

void Game::startMenu() {
  if (!NcursesSession::active) {
    return;
  };

  std::string prompt{"Welcome to my game! Please selected an option"};
  std::vector<std::string> choices{"New Game", "Load Game", "Exit"};

  int selected{Game::getMenuSelection(choices, prompt)};

  switch (selected) {
  case 0:
    Game::newGame();
    break;

  case 1:
    break;

  case 2:
    return;
  }
}

void Game::newGame() {
  echo();
  nocbreak();
  keypad(stdscr, FALSE);
  clear();

  printw("Please enter your name: ");
  refresh();
  char buf[512];
  getnstr(buf, sizeof(buf) - 1);
  std::string name(buf);

  Stats stats{100, 100, 10, 5};
  player = std::make_unique<Player>(name, stats);

  noecho();
  cbreak();
  keypad(stdscr, TRUE);
}

void Game::run() {
  bool running{true};
  NcursesSession n;

  startMenu();
  if (player) {
    printw(player->name.c_str());
    refresh();
  }
  getch();

  return;
}
