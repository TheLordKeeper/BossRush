#pragma once
#include "../../include/game/UI.hpp"
#include "../characters/Character.hpp"
#include <memory>
#include <vector>

class Game {
public:
  UI ui;

  void run();
  static void addToActionLog(std::string &text);

private:
  int getWave() const;
  void startMenu();
  std::unique_ptr<Enemy> generateEnemy();
  EnemyType getRandomEnemyType();
  int getGameSelection(Enemy &enemy, std::vector<std::string> &choices);
  void newGame();
  bool generateStage(Enemy &enemy);
  void enemyTurn(Enemy &enemy);
  bool playAgain();
  int calculateWaveXP();

  static std::vector<std::string> actionLog;

  int wave{1};
  std::unique_ptr<Player> player;
};
