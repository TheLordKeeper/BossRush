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

  Game::addToActionLog(text);
}

bool Character::checkAlive() {
  if (this->stats.health > 0) {
    return true;
  } else
    return false;
}

Stats Enemy::getEnemyStats(Stats &baseStats, int wave) {
  int maxHealth = baseStats.maxHealth + wave * 3;
  int health = maxHealth;
  int attack = baseStats.attack + wave * 2;
  int defence = baseStats.defence + wave * 1;
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

void Player::levelUp() {
  xp -= xpUntilLevel;
  xpUntilLevel *= 1.25;

  this->stats.attack += 2;
  this->stats.defence += 1;
  this->stats.maxHealth += 10;
  this->stats.health = this->stats.maxHealth;

  return;
}

void Player::addXP(int amount) {
  xp += amount;
  if (xp >= xpUntilLevel) {
    levelUp();
  }
}
