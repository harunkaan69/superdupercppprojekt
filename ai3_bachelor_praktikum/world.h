#pragma once
#include <vector>
#include <memory>
#include <time.h>
#include <algorithm>
#include "creature.h"
#include "exception.h"

/*
	Class to manage general terrains
	This class can only be accessed by its friend class 'world'
*/
class tile
{
	friend class world;
public:
	enum terrain { earth, sand, rocks, snow, deepWater, shallowWater , END };
	terrain getTerrain() const;
	const std::vector<std::shared_ptr<creature>>& getCreatureList() const;
	void removeCreature(std::shared_ptr<creature>& creature);

	int getX() const;
	int getY() const;

private:
	int x, y;
	terrain terrainType;
	std::vector<std::shared_ptr<creature>> creatureList;
	tile(terrain pTerrain, int pX, int pY) : terrainType(pTerrain) , x(pX), y(pY){ }
};

/*
	Class to manage the world map consisting of tiles
*/
class world
{
private:
	// const values for perlinNoise
	static const int MAXWIDTH = 2000;
	static const int MAXHEIGHT = 2000;
	static const int OCTAVES = 32;
	static const int FREQUENCY = 32;

	// map width, height, tiles for each coord
	int width;
	int height;

	// noise + helper func
	static tile::terrain noiseToTerrain(float pValue);
	static std::vector<float> perlinNoise(int pWidth, int pHeight);

public:
	world(int pWidth, int pHeight);
	tile& tileAt(int pX, int pY);
	bool existsTileAt(int pX, int pY) const;

	std::vector<tile> tiles;

	// get map size
	int getWidth() const;
	int getHeight() const;

	// creature relevant operations
	void moveCreature(const std::shared_ptr<creature>& pCreature, int pX, int pY);
	void placeCreature(const std::shared_ptr<creature>& pCreature, int pX, int pY);
	void removeCreature(const std::shared_ptr<creature>& pCreature);
};


