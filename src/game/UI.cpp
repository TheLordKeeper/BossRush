#include "../../include/game/UI.hpp"
#include <ncurses.h>

void UI::boldPrint(std::string phrase) {
  attron(A_REVERSE);
  printw(phrase.c_str());
  attroff(A_REVERSE);
}

void UI::displayChoice(std::vector<std::string> choices, int &selected) {
  for (auto i{0}; i < choices.size(); i++) {
    if (i == selected) {
      boldPrint(choices[i]);
    } else {
      printw(choices[i].c_str());
    }
    printw("\n");
  }
}

int UI::getMenuSelection(const std::vector<std::string> &choices,
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

void UI::printHealthBar(Character &character) {
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

  printw("HP: {", character.name.c_str());

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

void UI::printActionLog(std::vector<std::string> &actionLog) {
  for (auto log : actionLog) {
    printw("\n");
    printw(log.c_str());
    printw("\n");
  }
}
