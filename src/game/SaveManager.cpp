#include "../../include/game/SaveManager.hpp"
#include "../../include/characters/Character.hpp"
#include "../../include/game/Game.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

enum class tokens { Name, Level, Wave, maxHealth, Health, Attack, Defence, Unknown };

tokens parseTokens(const std::string key) {
  if (key == "Name")
    return tokens::Name;
  if (key == "Level")
    return tokens::Level;
  if (key == "Wave")
    return tokens::Wave;
  if (key == "maxHealth")
    return tokens::maxHealth;
  if (key == "Health")
    return tokens::Health;
  if (key == "Attack")
    return tokens::Attack;
  if (key == "Defence")
    return tokens::Defence;
  return tokens::Unknown;
}

bool SaveManager::saveGame(Game &game, const std::filesystem::path &path) {
  std::ofstream file(path);

  if (!file.is_open())
    return false;

  const Player &player{game.getPlayer()};

  file << "Name: " << player.name << "\n";
  file << "Level: " << player.getLevel() << "\n";
  file << "Wave: " << game.getWave() << "\n";
  file << "maxHealth: " << player.stats.maxHealth << "\n";
  file << "Health: " << player.stats.health << "\n";
  file << "Attack: " << player.stats.attack << "\n";
  file << "Defence: " << player.stats.defence << "\n";

  return true;
}

bool SaveManager::loadGame(Game &game, const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file.is_open()) {
    return false;
  }

  Player &player(game.getPlayer());

  std::string key, value{};
  while (std::getline(file, key, ':')) {
    std::getline(file, value);

    if (!value.empty() && value[0] == ' ') {
      value.erase(0,1);
    }

    switch (parseTokens(key)) {
      case tokens::Name:
        player.name = value;
        break;

      case tokens::maxHealth:
        player.stats.maxHealth = std::stoi(value);
        break;

      case tokens::Health:
        player.stats.health = std::stoi(value);
        break;

      case tokens::Attack:
        player.stats.attack = std::stoi(value);
        break;

      case tokens::Defence:
        player.stats.defence = std::stoi(value);
        break;

      case tokens::Wave:
        game.setWave(std::stoi(value));
        break;

      case tokens::Level:
        player.getLevel() = std::stoi(value);
        break;

      default:
        return false;
        break;
    }
  }

  return true;
}

void SaveManager::eraseGame(Game &game, const std::filesystem::path &path) {
  std::filesystem::path file = path / "save.txt";
  if(!std::filesystem::remove(file)) {
    std::cerr << "Error occured when deleting file";
  }
  exit(1);
}
