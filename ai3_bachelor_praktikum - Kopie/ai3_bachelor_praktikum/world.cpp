#include "world.h"

world::world(int pWidth, int pHeight)
{
	width = pWidth;
	height = pHeight;

	if (width <= 0 || height <= 0 || width > MAXWIDTH || height > MAXHEIGHT)
	{
		throw parameterError("Map too big");
	}

	tiles.reserve(pWidth * pHeight);

	std::vector<float> noise = perlinNoise(pWidth, pHeight);

	for (int y = 0; y < pHeight; ++y) {
		for (int x = 0; x < pWidth; ++x) {
			tiles.push_back(tile(noiseToTerrain(noise[y * pWidth + x]), x, y));
		}
	}
}

tile& world::tileAt(int pX, int pY)
{
	if (pX < 0 || pY < 0 || pX > width || pY > height)
	{
		throw parameterError("");
	}

	return tiles[pY * width + pX];
}

tile::terrain world::noiseToTerrain(float pValue)
{
	/*
		assing what float intervals shoudld correspond to what tiles
		[ 0.00f - 0.15f ] = deepWater			[ 0.45f - 0.60f ] = earth
		[ 0.15f - 0.30f ] = shallowWater		[ 0.60f - 0.75f ] = rocks
		[ 0.30f - 0.45f ] = sand				[	> 0.75f		] = snow
	*/
	float tileIntervals[tile::END - 1] =
	{ 0.15f, 0.30f, 0.45f, 0.60f, 0.75f };
	// set up the tiles
	tile::terrain indexedTiles[tile::END - 1] =
	{ tile::deepWater, tile::shallowWater, tile::sand, tile::earth, tile::rocks };
	/*
		loop through the number of tiles and check in what interval
		the current float value is and assign the corresponding tile
	*/
	for (int i = 0; i < tile::END - 1; i++) {
		if (pValue < tileIntervals[i]) {
			return indexedTiles[i];
		}
	}
	return tile::snow;
}

int world::getWidth() const
{
	return width;
}

int world::getHeight() const
{
	return height;
}

tile::terrain tile::getTerrain() const
{
	return terrainType;
}

const std::vector<std::shared_ptr<creature>>& tile::getCreatureList() const
{
	return creatureList;
}

int tile::getX() const
{
	return x;
}

int tile::getY() const
{
	return y;
}

void world::moveCreature(const std::shared_ptr<creature>& pCreature, int pX, int pY)
{
	// check if the creature we're moving is valid
	if (!pCreature) {
		throw parameterError("NULL can not be moved");
	}

	// return if the position to move to is the same as the current position
	if ((pCreature->getX() == pX) && (pCreature->getY() == pY)) {
		return;
	}

	// place creatre on new spot, remove creature from old spot
	placeCreature(pCreature, pX, pY);
	removeCreature(pCreature);

	// update creatures position values
	pCreature->updatePosX(pX);
	pCreature->updatePosY(pY);
}

void world::placeCreature(const std::shared_ptr<creature>& pCreature, int pX, int pY)
{
	// return if demanded position to place the creature at is out of bounds (map)
	if (pX < 0 || pY < 0 || pX > width || pY > height) {
		return;
	}

	// add creature to list at correct index
	std::vector<std::shared_ptr<creature>>& tempCreaturesOnTiles = tiles[(pY * width) + pX].creatureList;
	tempCreaturesOnTiles.push_back(pCreature);
}

void world::removeCreature(const std::shared_ptr<creature>& pCreature)
{
	// return of the creature is out of the map
	if (pCreature->getX() < 0 || pCreature->getY() < 0 || pCreature->getX() >= width || pCreature->getY() >= height) {
		return;
	}

	// loop throug the creates on the tile and erase it
	std::vector<std::shared_ptr<creature>>& tempCreaturesOnTiles = tiles[(pCreature->getY() * width) + pCreature->getX()].creatureList;

	for (int i = 0; i < tempCreaturesOnTiles.size(); i++) {
		if (tempCreaturesOnTiles[i] == pCreature) {
			tempCreaturesOnTiles.erase(tempCreaturesOnTiles.begin() + i);
		}
	}
}

std::vector<float> world::perlinNoise(int pWidth, int pHeight)
{
	srand((unsigned int)time(NULL));

	std::vector<float> res;
	res.resize(pWidth * pHeight);

	for (int octave = OCTAVES; octave > 0; octave /= 2) {
		int rSizeX = pWidth / octave + 2;
		int rSizeY = pHeight / octave + 2;

		std::vector<float> ran;
		ran.resize(rSizeX * rSizeY);

		for (int ry = 0; ry < rSizeY; ++ry) {
			for (int rx = 0; rx < rSizeX; ++rx) {
				ran[ry * rSizeX + rx] =
					(static_cast<float>(rand()) * 2 / RAND_MAX - 1) * std::sqrt(static_cast<float>(octave) / OCTAVES);
			}
		}

		for (int ry = 0; ry < rSizeY - 1; ++ry) {
			for (int rx = 0; rx < rSizeX - 1; ++rx) {
				float values[4] = {
					ran[ry * rSizeX + rx],
					ran[ry * rSizeX + rx + 1],
					ran[(ry + 1) * rSizeX + rx],
					ran[(ry + 1) * rSizeX + rx + 1]
				};

				for (int y = 0; y < octave; ++y) {
					for (int x = 0; x < octave; ++x)
					{
						int tx = x + rx * octave;
						int ty = y + ry * octave;

						if (tx < 0 || tx >= pWidth || ty < 0 || ty >= pHeight) continue;

						float fx = static_cast<float>(x) / octave;
						float fy = static_cast<float>(y) / octave;

						res[ty * pWidth + tx] +=
							(1 - fx) * (1 - fy) * values[0]
							+ fx * (1 - fy) * values[1]
							+ (1 - fx) * fy * values[2]
							+ fx * fy * values[3];
					}
				}
			}
		}
	}

	float max = 0;
	for (int i = 0; i < pWidth * pHeight; ++i) {
		max = std::max(max, std::fabs(res[i]));
	}
	for (int i = 0; i < pWidth * pHeight; ++i) {
		res[i] = std::fabs(res[i]) / max;
	}

	return res;
}