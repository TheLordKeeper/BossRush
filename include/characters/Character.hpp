#pragma once
#include <string>

enum class EnemyType { Goblin, Orc, Troll, Count };

struct Stats {
  int health;
  int maxHealth;
  int attack;
  int defence;
};

class Character {
public:
  Character(const std::string &n, const Stats &s = {100, 100, 10, 5})
      : name(n), stats(s) {};

  virtual ~Character() = default;

  virtual void basicAttack(Character &target);
  virtual void takeDamage(int damage);
  bool checkAlive();

  std::string name;
  Stats stats;

private:
};

class Player : public Character {
public:
  Player(const std::string &n, const Stats &s) : Character(n, s) {};
  void addXP(int amount);
  void levelUp();

private:
  int level{1};
  int xp{0};
  int xpUntilLevel{100};
};

class Enemy : public Character {
public:
  Enemy(EnemyType type, const Stats &s) : Character(typeToName(type), s) {};

  static Stats getEnemyStats(Stats &baseStats, int wave);

private:
  static std::string typeToName(EnemyType type);
  EnemyType type;
};
