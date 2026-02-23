#pragma once
#include <csignal>
#include <cstdlib>
#include <ncurses.h>

class NcursesSession {

public:
  inline static bool active{false};

  NcursesSession() {
    if (!active) {
      initscr();
      cbreak();
      noecho();
      keypad(stdscr, TRUE);

      std::signal(SIGINT, handleSignal);

      active = true;
    }
  std::atexit([]() {
      if (active) {
        endwin();
        active = false;
      }
    });
  }

  ~NcursesSession() {
    if (active) {
      endwin();
      active = false;
    }
  }

  static void handleSignal(int sig);
};

inline void NcursesSession::handleSignal(int sig) {
  if (NcursesSession::active) {
    endwin();
    NcursesSession::active = false;
  }
  exit(sig);
}
