#pragma once
#include <math.h>
#include <random>
#include <functional>

#include "world.h"
#include "creature.h"
#include "astar.h"


class ai {
	
public:
	typedef std::function<std::weak_ptr<creature>(const creaturePrototype&, int, int)> creatureCreationFunction;

	ai(std::shared_ptr<creature> pCreatureInstance,
		std::shared_ptr<world> pMap,
		creatureCreationFunction pCreateCreature);
	~ai() noexcept;

	const std::weak_ptr<creature> getCreatureInstance() const;

protected:
	static std::mt19937 gen;

	enum states { IDLE, GROW, WALK, MAITING, HUNT, ATTACK, DEAD };
	
	int steps = 0;
	states state = IDLE;
	std::shared_ptr<creature> creatureInstance;
	std::shared_ptr<world> map;

	
	creatureCreationFunction createCreature;

public:
	virtual void performStep() = 0;
};

class plant : public ai
{
private:
	int stepsSinceGrowth = 0;
	bool checkSpaceToGrow();
	void plantGrow();
	std::vector<std::pair<int, int>> getSpaceToGrow();
public:
	void performStep() override;

	plant(std::shared_ptr<creature> pCreatureInstance,
		std::shared_ptr<world> pMap,
		creatureCreationFunction pCreateCreature);
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

	animal(std::shared_ptr<creature> pCreatureInstance,
		std::shared_ptr<world> pMap,
		creatureCreationFunction pCreateCreature);
};
