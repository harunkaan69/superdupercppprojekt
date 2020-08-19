#pragma once
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include "world.h"
#include "creature.h"

/*
	Class for path finding
*/
class astar
{
private:

	/*
		Weight tables for the algorithm.
		------------------------------------------------------------------------------
		If the tile has a corresponding value of '-1', the creature is unable to move
		through that specific tile and has to avoid it (tile gets set as an obstacle).
		If the value is anything but '< 0', the value is getting added to the F-Cost
		of the node (smaller weight ergo smaller F-Cost is preferred)
	*/
	std::map<tile::terrain, int> tblWaterCreature = {
		{ tile::terrain::deepWater,		 3 },
		{ tile::terrain::shallowWater,	 1 },
		{ tile::terrain::sand,			-1 },
		{ tile::terrain::earth,			-1 },
		{ tile::terrain::rocks,			-1 },
		{ tile::terrain::snow,			-1 } };

	std::map<tile::terrain, int> tblLandCreature = {
		{ tile::terrain::deepWater,		-1 },
		{ tile::terrain::shallowWater,	-1 },
		{ tile::terrain::sand,			 1 },
		{ tile::terrain::earth,			 1 },
		{ tile::terrain::rocks,			 4 },
		{ tile::terrain::snow,			 2 } };

	std::map<creaturePrototype::habitate, std::map<tile::terrain, int>> tblHabitateToTable = {
		{ creaturePrototype::habitate::land,  tblLandCreature  },
		{ creaturePrototype::habitate::water, tblWaterCreature } };

	std::map<tile::terrain, creaturePrototype::habitate> tblTerrainToHabitate = {
		{ tile::terrain::deepWater,		 creaturePrototype::habitate::water },
		{ tile::terrain::shallowWater,	 creaturePrototype::habitate::water },
		{ tile::terrain::sand,			 creaturePrototype::habitate::land },
		{ tile::terrain::earth,			 creaturePrototype::habitate::land },
		{ tile::terrain::rocks,			 creaturePrototype::habitate::land },
		{ tile::terrain::snow,			 creaturePrototype::habitate::land } };

	std::vector<std::pair<int, int>> directions = {
		{0,  1}, // North
		{1,  0}, // East
		{0, -1}, // South
		{-1, 0}  // West
	};

	/*
		Struct to manage Nodes themselves
		-----------------------------------------------------------------------------------
		A node can be either be an obstacle or not (has a weight added to its global cost)
		Nodes also store their parent and their neighbors (in 4 directions: North, East,
		South and West to avoid diagonal movement), their position on the grid, their local
		and flobal cost values and whether it has been visited already or not
	*/
	struct Node
	{
		bool isObstacle;
		bool visited;

		float g, h;
		int x, y;

		Node* parent;
		Node(int pX, int pY, Node* pParent = nullptr);
		float getScore();
	};

	std::shared_ptr<world> map;

	float heuristic(int pX1, int pY1, int pX2, int pY2);
	bool isObstacle(int pX1, int pY1, int pX2, int pY2);
	Node* findNode(std::set<Node*>& pList, int pX, int pY);
public:
	astar(std::shared_ptr<world> pMap) : map(pMap) { };
	std::vector<std::pair<int, int>> getPath(const std::shared_ptr<creature>& pCreature, tile* pStart, const tile& pTile);
};

