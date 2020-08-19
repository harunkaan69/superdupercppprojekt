#pragma once

#include <QObject>
#include <qstringlistmodel.h>
#include <qimage.h>
#include "qtimer.h"

#include "creature.h"
#include "world.h"
#include "astar.h"
#include "ai.h"


class model : public QObject
{
	Q_OBJECT

public:
	model(std::vector<creaturePrototype>&& pCreatureList);

	// const values
	const int WORLDWIDTH = 200;
	const int WORLDHEIGHT = 200;
	const int TILEWIDTH = 32;
	const int TILEHEIGHT = 32;

	//map
	world& getMap() const;
	const QImage& getImage(tile pTile) const;
	const QImage& getImage(const creaturePrototype& pPrototyp) const;
	const QImage& getCursorImage() const;
	const QImage& getPathTexture() const;
	const QImage& getDeadTexture() const;

	//get tile
	QPoint getActiveTile();

	//demo
	std::vector<std::pair<int, int>> const& getDemoPath() const;

	//
	//std::vector<std::shared_ptr<creature>>& getCreatureList();
	std::vector<creaturePrototype>const& getCreaturePrototypeList() const;

	//other(s)
	std::weak_ptr<creature> createCreature(const creaturePrototype& pPrototype, int pX, int pY);
	std::list<std::unique_ptr<ai>>& getAiList();

	int getCreatureCnt() const;

private:

	//demonstration purpose
	std::vector<std::pair<int, int>> demoPath;
	std::shared_ptr<astar> as;

	// creature/prototype list to manage
	std::vector<creaturePrototype> creaturePrototypeList;

	//ai
	std::list<std::unique_ptr<ai>> aiList;

	// map to generate
	//world* map = new world(WORLDWIDTH, WORLDHEIGHT);
	std::shared_ptr<world> map = std::make_shared<world>(WORLDWIDTH, WORLDHEIGHT);

	//textures to manage
	QImage environmentTexture[tile::terrain::END];
	QImage cursorTexture;
	QImage deadTexture;
	QImage pathTexture;
	std::map<const creaturePrototype*, QImage> creatureTextures;

	// helper function to convert raw image data to QImage objects
	QImage toTexture(const image& pImg);

	// selected tile
	QPoint activeTile;

public slots:
	void onActiveTileChanged(QPoint pPoint);
};