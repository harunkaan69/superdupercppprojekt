#include "ai.h"

std::mt19937 ai::gen = std::mt19937(std::random_device());

void plant::performStep()
{
	if (creatureInstance->getLifetime() == 0) 
	{
		// remove creature and form this hell
	} else {
		int temp = (creatureInstance->getLifetime() / 100);
		if ((temp > stepsSinceGrowth) && (checkSpaceToGrow())) 
		{
			plantGrow();
			state = GROW;
		}
	}
}

bool plant::checkSpaceToGrow()
{
	int exists = 0;

	for (int i = -5; i < 5; i++) {
		for (int j = -5; j < 5; j++) {
			if ((i == 0) && (j == 0)) continue; // middle

			auto &tempTileList = map->tileAt(creatureInstance->getX()+ i, creatureInstance->getY() + j).getCreatureList();
			if (std::find(tempTileList.begin(), tempTileList.end(), creatureInstance) != tempTileList.end()) {
				exists++;

				if (exists >= 10) return false;
			}
		}
	}

	return (exists > 2);
}



void plant::plantGrow()
{
	auto tempList = getSpaceToGrow();
	std::uniform_int_distribution<> intDist(tempList.size());
	
	for (int i = 0; (i < 2) && (i < tempList.size()); i++) {
		int randX = distr(gen);
		int randY = distr(gen);

	}
}

std::list<std::pair<int, int>> plant::getSpaceToGrow()
{
	std::list<std::pair<int, int>> tempList;
	for (int i = -5; i < 5; i++) {
		for (int j = -5; j < 5; j++) {
			bool hasPlant = false;
			auto& tempTileList = map->tileAt(creatureInstance->getX() + i, creatureInstance->getY() + j).getCreatureList();
			for (const auto &temp : tempTileList) {
				if ((temp->getCreaturePrototype().getSustentation() == creaturePrototype::sustentation::plant))
					hasPlant = true;
					break;
			}
			
			if (!hasPlant) {
				tempList.push_back(std::pair<int, int>(i, j));
			}
		}
	}

	return tempList;
}
