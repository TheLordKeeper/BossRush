#pragma once
#include <filesystem>

class Game;

class SaveManager {
public:
  bool saveGame(Game &game, const std::filesystem::path &path);
  bool loadGame(Game &game, const std::filesystem::path &path);
  void eraseGame(Game &game, const std::filesystem::path &path);
};
