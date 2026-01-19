#include "../../include/game/SaveManager.hpp"
#include "../../include/characters/Character.hpp"
#include "../../include/game/Game.hpp"
#include <fstream>

bool SaveManager::saveGame(Game &game, const std::filesystem::path &path) {
  std::ofstream file(path);

  if (!file)
    return false;

  const Player &player{game.getPlayer()};

  file << player.name << player.stats.attack;

  return true;
}
