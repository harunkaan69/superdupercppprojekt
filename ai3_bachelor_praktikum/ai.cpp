#include "ai.h"

std::mt19937 ai::gen = std::mt19937(std::random_device()());

void plant::performStep()
{
	if (state == DEAD)
	{
		creatureInstance->decayStep();
		return;
	}

	switch (state)
	{
	case GROW:
		plantGrow();
		break;
	case IDLE:
		int temp = (creatureInstance->getLifetime() / 100);

		if ((temp > stepsSinceGrowth) && (checkSpaceToGrow()))
			state = GROW;
		else
		{
			auto terrain = map->tileAt(creatureInstance->getX(), creatureInstance->getY()).getTerrain();
			if (terrain == tile::sand || terrain == tile::earth || terrain == tile::shallowWater)
				creatureInstance->updateLifetime(creatureInstance->getLifetime() - 10);
			else if (terrain == tile::rocks || terrain == tile::snow || terrain == tile::deepWater)
				creatureInstance->updateLifetime(creatureInstance->getLifetime() - 25);

			if (creatureInstance->isDead())
				state = DEAD;
		}
		break;
	}
}

bool plant::checkSpaceToGrow()
{
	int exists = 0;

	for (int i = -5; i < 5; i++) {
		for (int j = -5; j < 5; j++) {
			
			if (!map->existsTileAt(creatureInstance->getX() + i, creatureInstance->getY() + j))
				continue;

			//There may be more plants on the same tile
			if ((i == 0) && (j == 0))
				--exists;

			auto &tempTileList = map->tileAt(creatureInstance->getX() + i, creatureInstance->getY() + j).getCreatureList();
			for (const auto& temp : tempTileList) {
				if ((temp->getCreaturePrototype().getName() == creatureInstance->getCreaturePrototype().getName()) && 
					(!temp->isDead())) {
					if (++exists >= 10)
						return false;
				}
			}
		}
	}
	return (exists > 2);
}

void plant::plantGrow()
{
	stepsSinceGrowth = 0;
	state = IDLE;

	if (!checkSpaceToGrow())
		return;

	auto tempVec = getSpaceToGrow();

	if (tempVec.size() == 0)
		return;

	std::uniform_int_distribution<> intDist(0, tempVec.size() - 1);

	int i = 0;
	if (tempVec.size() > 0)
	{
		i = intDist(gen);
		createCreature(creatureInstance->getCreaturePrototype(), tempVec[i].first, tempVec[i].second);
	}

	if (tempVec.size() > 1)
	{
		int j;
		do
		{
			j = intDist(gen);
		} while (j == i);
		createCreature(creatureInstance->getCreaturePrototype(), tempVec[j].first, tempVec[j].second);
	}	
}

std::vector<std::pair<int, int>> plant::getSpaceToGrow()
{
	std::vector<std::pair<int, int>> tempVec;
	for (int i = -5; i < 5; i++) {
		for (int j = -5; j < 5; j++) {
			bool hasAlivePlant = false;
			if (!map->existsTileAt(creatureInstance->getX() + i, creatureInstance->getY() + j))
				continue;

			auto currentTile = map->tileAt(creatureInstance->getX() + i, creatureInstance->getY() + j);
			auto& tempTileList = currentTile.getCreatureList();
			for (const auto &temp : tempTileList) {
				if ((temp->getCreaturePrototype().getSustentation() == creaturePrototype::sustentation::plant) &&
					!temp->isDead())
					hasAlivePlant = true;
					break;
			}
			if (hasAlivePlant)
				continue;

			if ((creatureInstance->getCreaturePrototype().getHabitate() == creaturePrototype::habitate::land) &&
				((currentTile.getTerrain() != tile::terrain::deepWater) && (currentTile.getTerrain() != tile::terrain::shallowWater))) {
				tempVec.push_back(std::pair<int, int>(creatureInstance->getX() + i, creatureInstance->getY() + j));
			}
			else if ((creatureInstance->getCreaturePrototype().getHabitate() == creaturePrototype::habitate::water) &&
				((currentTile.getTerrain() == tile::terrain::deepWater) || (currentTile.getTerrain() == tile::terrain::shallowWater))) {
				tempVec.push_back(std::pair<int, int>(creatureInstance->getX() + i, creatureInstance->getY() + j));
			}
		}
	}
	return tempVec;
}

ai::ai(std::shared_ptr<creature> pCreatureInstance, std::shared_ptr<world> pMap, creatureCreationFunction pCreateCreature)
	: creatureInstance(pCreatureInstance), map(pMap), createCreature(pCreateCreature)
{
}

ai::~ai() noexcept
{
	map->tileAt(creatureInstance->getX(), creatureInstance->getY()).removeCreature(creatureInstance);
}

const std::weak_ptr<creature> ai::getCreatureInstance() const
{
	return creatureInstance;
}

plant::plant(std::shared_ptr<creature> pCreatureInstance, std::shared_ptr<world> pMap, creatureCreationFunction pCreateCreature) :
	ai(pCreatureInstance, pMap, pCreateCreature)
{
}

void animal::rest()
{
}

void animal::walk()
{
}

void animal::mate()
{
}

void animal::hunt()
{
}

void animal::attack()
{
}

void animal::performStep()
{
	if (state == DEAD)
	{
		creatureInstance->decayStep();
		return;
	}
}

animal::animal(std::shared_ptr<creature> pCreatureInstance, std::shared_ptr<world> pMap, creatureCreationFunction pCreateCreature) :
	ai(pCreatureInstance, pMap, pCreateCreature)
{
}
