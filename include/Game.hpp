#pragma once
#include "Character.hpp"
#include <memory>
#include <vector>

class Game {
public:
  void displayChoice(std::vector<std::string> choices, int &selected);
  void boldPrint(std::string phrase);
  int getMenuSelection(const std::vector<std::string> &choices,
                       const std::string &prompt);
  int getWave();
  std::unique_ptr<Enemy> generateEnemy();
  EnemyType getRandomEnemyType();
  void newGame();
  void printHealthBar(Character &character);
  int getGameSelection(Enemy &enemy, std::vector<std::string> &choices);
  void generateStage(Enemy &enemy);
  void enemyTurn(Enemy &enemy);
  void printActionLog();
  static void addToActionLog(std::string &text);
  void startMenu();
  void run();

  static std::vector<std::string> actionLog;

private:
  int wave{1};
  std::unique_ptr<Player> player;
};
