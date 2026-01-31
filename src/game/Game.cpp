#include "../../include/game/Game.hpp"
#include "../../include/characters/Character.hpp"
#include "../../include/utils/ncurses.hpp"
#include <cmath>
#include <filesystem>
#include <memory>
#include <ncurses.h>
#include <random>
#include <string>
#include <vector>

Game::Game(std::filesystem::path dir) : saveDir(dir) {
  std::filesystem::create_directories(saveDir);
};

std::vector<std::string> Game::actionLog{};

int Game::getWave() const { return wave; }

void Game::setWave(int value) { wave = value; }

Player &Game::getPlayer() const { return *player; }

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

int Game::getGameSelection(Enemy &enemy, std::vector<std::string> &choices) {
  bool run{true};
  int selected{0};

  while (run) {
    clear();
    printw("Wave: %i\n", wave);
    printw("%s (Level %i)\n", player->name.c_str(), player->getLevel());
    ui.printHealthBar(*player);
    printw("\n");
    printw("%s\n", enemy.name.c_str());
    ui.printHealthBar(enemy);
    printw("\n");

    ui.displayChoice(choices, selected);
    ui.printActionLog(actionLog);

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
  int selected{ui.getMenuSelection(choices, phrase)};

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

void Game::startMenu() {
  std::string prompt{"Welcome to my game! Please selected an option"};
  std::vector<std::string> choices{"New Game", "Load Game", "Exit"};

  int selected{ui.getMenuSelection(choices, prompt)};

  switch (selected) {
  case 0:
    newGame();
    break;

  case 1:
    if (!player) {
      player = std::make_unique<Player>("Default", Stats{100, 100, 10, 5});
    }
    saveManager.loadGame(*this, saveDir / "save.txt");
    break;

  case 2:
    return;
    break;
  }
}

void Game::run() {
  bool gameOver{false};
  NcursesSession n;
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);

  Game::startMenu();

  if (!player) {
    return;
  }

  while (!gameOver) {
    auto enemy{generateEnemy()};

    if (generateStage(*enemy)) { // If the player wins
      actionLog.clear();
      int xp{calculateWaveXP()};
      player->addXP(calculateWaveXP());
      wave++;
      saveManager.saveGame(*this, saveDir / "save.txt");

    } else if (playAgain()) {
      player.reset();
      newGame();
      wave = 1;
    } else {
      gameOver = true;
    }
  }

  return;
}
