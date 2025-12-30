#include "../include/Character.hpp"

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
