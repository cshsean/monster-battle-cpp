#include <iostream>
#include <string>
#include "random.h"

class Creature {
protected:
	std::string_view name{};
	char symbol{};
	int health{};
	int damage{};
	int gold{};
public:
	Creature(std::string_view name, char sym, int health, int damage, int gold)
		: name{name}
		, symbol{sym}
		, health{health}
		, damage{damage}
		, gold{gold}
	{}

	std::string_view getName() const { return name; }
	char getSymbol() const { return symbol; }
	int getHealth() const { return health; }
	int getDamage() const { return damage; }
	int getGold() const { return gold; }

	void addGold(int x) { gold += x; }
	void reduceHealth(int x) { health -= x; }
	bool isDead() { return health <= 0; }
};

class Monster : public Creature {
public:
	enum Type {
		dragon,
		orc,
		slime,
		max_types
	};
private:
	static inline Creature monsterData[]{ 
		{"dragon", 'D', 20, 4, 100},
		{"orc", 'o', 4, 2, 25},
		{"slime", 's', 1, 1, 10}
	};
public:
	Monster(Type type)
		: Creature(monsterData[type])
	{}

	static Monster::Type getRandomMonster() {
		int random{ Random::get(0, max_types - 1) };
		return static_cast<Monster::Type>(random);
	}
};

class Player : public Creature {
private:
	int level{ 1 };
public:
	Player(std::string_view name = "", char symbol = '@', int health = 10, int damage = 1, int gold = 0)
		: Creature{ name, symbol, health, damage, gold }
	{}

	void levelUp() {
		++level;
		++damage;
	}

	int getLevel() const { return level; }
	bool hasWon() const { return level >= 20; }
};

std::string enterInitialName() {
	std::string name{};
	std::cout << "Enter your name: ";
	std::cin >> name;
	std::cout << "Welcome, " << name << ".\n";
	return name;
}

void fightMonster(Player& p, Monster& m) {
	m.reduceHealth(p.getDamage());
	std::cout << "You hit the " << m.getName() << " for " << p.getDamage() << " damage.\n";
}

void attackPlayer(Player& p, Monster& m) {
	p.reduceHealth(m.getDamage());
	std::cout << "The " << m.getName() << " hit you for " << m.getDamage() << ".\n";
}

int main() {
	Player player{ enterInitialName() };
	while (player.getHealth() > 0 || player.getLevel() == 20) {
		Monster m{ Monster::getRandomMonster() };
		std::cout << "You have encountered a " << m.getName() << " (" << m.getSymbol() << ")\n";
		while (true) {

			if (player.isDead()) {
				break;
			}

			char choice{};
			while (true) {
				std::cout << "(R)un or (F)ight: ";
				std::cin >> choice;
				if (!(choice == 'f' || choice == 'r')) {
					std::cin.clear();
					std::cout << "That wasn't a valid input.  Try again.\n";
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					continue;
				}
				break;
			}

			if (choice == 'f')
				fightMonster(player, m);
			else {
				if ( Random::get(0, 1) ? true : false ) {
					std::cout << "You successfully fled.\n";
					break;
				}
				else {
					std::cout << "You failed to flee.\n";
				}
			}

			if (m.isDead()) {
				std::cout << "You killed the " << m.getName() << ".\n";
				player.addGold(m.getGold());
				player.levelUp();
				break;
			}

			attackPlayer(player, m);
		}
	}

	if (player.getHealth() <= 0) {
		std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold.\n";
	}

	if (player.hasWon()) {
		std::cout << "You Won!";
	}

	return 0;
}
