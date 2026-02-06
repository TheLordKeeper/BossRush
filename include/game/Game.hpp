#pragma once
#include "../../include/game/UI.hpp"
#include "../characters/Character.hpp"
#include "SaveManager.hpp"
#include <filesystem>
#include <memory>
#include <vector>

class Game {
public:
  UI ui;
  SaveManager saveManager;

  explicit Game(std::filesystem::path saveDir);

  void run();
  static void addToActionLog(std::string &text);
  int getWave() const;
  void setWave(int value);
  Player &getPlayer() const;

private:
  std::filesystem::path saveDir;
  bool startMenu();
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
  std::unique_ptr<Player> player{std::make_unique<Player>()};
};
