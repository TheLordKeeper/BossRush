#include "../include/Character.hpp"
#include "../include/Game.hpp"
#include <ncurses.h>
#include <string>

void Character::takeDamage(int damage) {
  Character::stats.health -= damage;
  if (Character::stats.health < 0)
    Character::stats.health = 0;
}

void Character::basicAttack(Character &target) {
  int damage{Character::stats.attack - target.stats.defence};
  if (damage <= 0)
    damage = 1;

  target.takeDamage(damage);

  std::string text = "\n" + this->name + " dealt " + std::to_string(damage) +
                     " damage to " + target.name + "!";
  Game::actionLog.push_back(text);
}

Stats Enemy::getEnemyStats(Stats &baseStats, int wave) {
  int maxHealth = baseStats.maxHealth + wave * 10;
  int health = maxHealth;
  int attack = baseStats.attack + wave * 5;
  int defence = baseStats.defence + wave * 3;
  return {health, maxHealth, attack, defence};
};

std::string Enemy::typeToName(EnemyType type) {
  switch (type) {
  case EnemyType::Goblin:
    return "Goblin";
  case EnemyType::Orc:
    return "Orc";
  case EnemyType::Troll:
    return "Troll";
  default:
    return "Enemy";
  }
}
