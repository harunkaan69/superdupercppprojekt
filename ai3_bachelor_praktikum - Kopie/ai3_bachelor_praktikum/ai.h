#pragma once
#include <math.h>
#include <random>
#include "world.h"
#include "creature.h"
#include "astar.h"

class ai {
protected:
	static std::mt19937 gen;

	enum states { IDLE, GROW, WALK, MAITING, HUNT, ATTACK, DEAD };
	int steps = 0;
	states state = IDLE;
	std::shared_ptr<creature> creatureInstance;
	std::shared_ptr<world> map;

public:
	virtual void performStep();
};

class plant : public ai
{
private:
	int stepsSinceGrowth = 0;
	bool checkSpaceToGrow();
	void plantGrow();
	std::list<std::pair<int, int>> getSpaceToGrow();
public:
	void performStep() override;
};

class animal : public ai
{
private:
	void rest();
	void walk();
	void mate();
	void hunt();
	void attack();
public:
	void performStep() override;
};

