#include "model.h"

model::model(std::vector<creaturePrototype>&& pCreatureList)
	: QObject()
{
    // setup textures for each environment type, cursor and death
    environmentTexture[tile::deepWater] = toTexture(*image::load("terrain/deep_sea.tga"));
    environmentTexture[tile::shallowWater] = toTexture(*image::load("terrain/shallow_water.tga"));
    environmentTexture[tile::sand] = toTexture(*image::load("terrain/sand.tga"));
    environmentTexture[tile::earth] = toTexture(*image::load("terrain/earth.tga"));
    environmentTexture[tile::rocks] = toTexture(*image::load("terrain/rocks.tga"));
    environmentTexture[tile::snow] = toTexture(*image::load("terrain/snow.tga"));
    cursorTexture = toTexture(*image::load("cursor/cursor.tga"));
    deadTexture = toTexture(*image::load("dead/dead.tga"));
    pathTexture = toTexture(*image::load("path/path.tga"));

    as = std::make_unique<astar>(astar(map));

    // setup creatureprototype list for the combobox
    creaturePrototypeList = pCreatureList;
    
    for (const auto& prototyp : creaturePrototypeList)
        creatureTextures[&prototyp] = toTexture(prototyp.getTexture());

}

world& model::getMap() const
{
	return *map;
}

const QImage & model::getImage(tile pTile) const
{
    return environmentTexture[pTile.getTerrain()];
}

const QImage& model::getImage(const creaturePrototype& pPrototyp) const
{
    return creatureTextures.at(&pPrototyp);
}

const QImage& model::getCursorImage() const
{
    return cursorTexture;
}

const QImage& model::getPathTexture() const
{
    return pathTexture;
}

const QImage& model::getDeadTexture() const
{
    return deadTexture;
}

QPoint model::getActiveTile()
{
    return activeTile;
}

std::vector<std::pair<int, int>> const& model::getDemoPath() const
{
    return demoPath;
}

std::vector<creaturePrototype> const& model::getCreaturePrototypeList() const
{
    return creaturePrototypeList;
}

QImage model::toTexture(const image& pImg)
{
    return QImage(&pImg.getPixelData().front(), (int)pImg.getHeader().imageWidth, (int)pImg.getHeader().imageHeight, QImage::Format_RGBA8888).rgbSwapped();
}

std::weak_ptr<creature> model::createCreature(const creaturePrototype& pPrototype, int pX, int pY) {
    std::shared_ptr<creature> tempCreature(std::make_shared<creature>(pPrototype, pX, pY));

    if (pX < 0 || pY < 0 || pX >= map->getWidth() || pY >= map->getHeight())
        throw parameterError("");

    if (tempCreature->getCreaturePrototype().getSustentation() == creaturePrototype::sustentation::plant)
        aiList.push_back(std::make_unique<plant>(tempCreature, map, 
            [this](const creaturePrototype& pPrototype, int pX, int pY) {return this->createCreature(pPrototype, pX, pY); }));
    else
        aiList.push_back(std::make_unique<animal>(tempCreature, map, 
            [this](const creaturePrototype& pPrototype, int pX, int pY) {return this->createCreature(pPrototype, pX, pY); }));

    try {
       map->placeCreature(tempCreature, pX, pY);
    }
    catch (...) {
        aiList.pop_back();
        throw;
    }

    return tempCreature;
}

std::list<std::unique_ptr<ai>>& model::getAiList()
{
    return aiList;
}

int model::getCreatureCnt() const
{
    return aiList.size();
}

void model::onActiveTileChanged(QPoint pPoint)
{
    QPoint oldTile = activeTile;

    // update active tile pos
    activeTile = QPoint(pPoint.x() / TILEWIDTH, pPoint.y() / TILEHEIGHT);

    // a-star path finding demo
    demoPath = as->getPath(nullptr,
        &map->tileAt(oldTile.x(), oldTile.y()),
        map->tileAt(activeTile.x(), activeTile.y()));
}
