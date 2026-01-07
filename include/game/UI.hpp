#pragma once
#include "../characters/Character.hpp"
#include <string>
#include <vector>

class UI {
public:
  void displayChoice(std::vector<std::string> choices, int &selected);
  void boldPrint(std::string phrase);
  int getMenuSelection(const std::vector<std::string> &choices,
                       const std::string &prompt);
  void printHealthBar(Character &character);
  void printActionLog(std::vector<std::string> &actionLog);
};
