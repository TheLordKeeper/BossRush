#include "../../include/Game.hpp"
#include "../../include/Character.hpp"
#include "../../include/ncurses.hpp"
#include <cmath>
#include <memory>
#include <ncurses.h>
#include <random>
#include <string>
#include <vector>

//  TODO: Implement EXP mechanic

std::vector<std::string> Game::actionLog{};

void Game::boldPrint(std::string phrase) {
  attron(A_REVERSE);
  printw(phrase.c_str());
  attroff(A_REVERSE);
}

int Game::getWave() const { return wave; }

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

  Stats baseStats{20, 20, 3, 1};
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

void Game::printHealthBar(Character &character) {
  int maxHealth{character.stats.maxHealth};
  int health{character.stats.health};
  int units{10};
  int full{(health * units) / maxHealth};
  int empty{units - full};

  int color;
  if (health > maxHealth * 0.6)
    color = 1;
  else if (health > maxHealth * 0.3)
    color = 2;
  else
    color = 3;

  printw("%s\nHP: {", character.name.c_str());

  attron(COLOR_PAIR(color));
  for (int i{}; i < full; i++) {
    addch(ACS_BLOCK);
  }
  attroff(COLOR_PAIR(color));

  for (int i{}; i < empty; i++) {
    addch(' ');
  }
  printw("} %i/%i\n", health, maxHealth);
}

void Game::printActionLog() {
  for (auto log : actionLog) {
    printw(log.c_str());
    printw("\n");
  }
}

int Game::getGameSelection(Enemy &enemy, std::vector<std::string> &choices) {
  bool run{true};
  int selected{0};

  while (run) {
    clear();
    printw("Wave: %i\n", wave);
    printHealthBar(*player);
    printw("\n");
    printHealthBar(enemy);
    printw("\n");

    displayChoice(choices, selected);
    printActionLog();

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

void Game::addToActionLog(std::string &text) {
  if (actionLog.size() >= 3) {
    actionLog.erase(actionLog.begin());
  }
  actionLog.push_back(text);
}

void Game::enemyTurn(Enemy &enemy) {
  enemy.basicAttack(*player);
  return;
}

bool Game::generateStage(Enemy &enemy) {
  bool run{true};
  std::vector<std::string> attackChoices{"Attack", "Skills", "Items"};
  actionLog = {};

  const int Attack{0};
  const int Skills{1};
  const int Items{2};

  while (run) {
    if (!player->checkAlive()) {
      return false;
    } else if (!enemy.checkAlive()) {
      return true;
    }

    int selected{getGameSelection(enemy, attackChoices)};

    switch (selected) {
    case Attack:
      player->basicAttack(enemy);
      break;

    default:
      return false;
    }
    enemyTurn(enemy);
  }
  return false;
}

bool Game::playAgain() {
  clear();
  std::string phrase = "You have died! You lasted until wave " +
                       std::to_string(wave) + "!\nDo you want to play again?";

  std::vector<std::string> choices{"Yes", "No"};
  int selected{getMenuSelection(choices, phrase)};

  switch (selected) {
  case 0:
    return true;
  case 1:
    return false;
  }

  return false;
}

int Game::calculateWaveXP() {
  return static_cast<int>(20 * std::pow(wave, 1.15));
}

void Game::run() {
  bool gameOver{false};
  NcursesSession n;
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);

  startMenu();
  if (!player) {
    return;
  }

  while (!gameOver) {
    auto enemy{generateEnemy()};
    if (generateStage(*enemy)) {
      actionLog.clear();
      int xp{calculateWaveXP()};
      player->addXP(calculateWaveXP());
      wave++;
    } else {
      if (playAgain()) {
        player.reset();
        newGame();
        wave = 1;
      } else {
        gameOver = true;
      }
    }
  }

  return;
}
