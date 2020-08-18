#include "simAreaWidget.h"

simAreaWidget::simAreaWidget(QWidget*parent) : QWidget(parent)
{
    // set scroll values to 0 (no scrolling done)
    xScroll = 0;
    yScroll = 0;

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
}

QImage simAreaWidget::toTexture(const image& pImg)
{
    return QImage(&pImg.getPixelData().front(), (int)pImg.getHeader().imageWidth, (int)pImg.getHeader().imageHeight, QImage::Format_RGBA8888).rgbSwapped();
}

void simAreaWidget::paintEvent(QPaintEvent* pEvent)
{
    // setup painter and tile/size values
    QPainter painter(this);
    QSize widgetSize(width(), height());

    // check how many tiles the current scroll value has exceeded
    int xScrollPixel    = xScroll % TILEWIDTH;
    int yScrollPixel    = yScroll % TILEHEIGHT;

    // set borders of what tiles to to render
    int xTileStart      = (xScroll - xScrollPixel) / TILEWIDTH;
    int xTileEnd        = (xScroll + widgetSize.width() - xScrollPixel) / TILEWIDTH + 1;
    int yTileStart      = (yScroll - yScrollPixel) / TILEWIDTH;
    int yTileEnd        = (yScroll + widgetSize.height() - yScrollPixel) / TILEHEIGHT + 1;

    // loop and draw tiles in the visible frame
    for (int y = yTileStart; y <= yTileEnd; y++) {
        for (int x = xTileStart; x <= xTileEnd; x++) {

            // skip if tiles are left & top most or right & bottom most 
            if (x < 0 || y < 0 || x >= map->getWidth() || y >= map->getHeight()) {
                continue;
            }

            int xTilePos = (x - xTileStart) * TILEWIDTH  - xScrollPixel;
            int yTilePos = (y - yTileStart) * TILEHEIGHT - yScrollPixel;

            // draw tiles
            painter.drawImage(QPoint(xTilePos, yTilePos), environmentTexture[map->tileAt(x, y).getTerrain()]);

            /*
                Get the current tile that is being rendered and check,
                whether it has any entries in its creature list. If so,
                loop through the list and add a pair of creature prototypes
                and their corresponding texture to the std::map containing
                creature prototype textures if it i not already inside. Only
                draw if there are entries in the creature list. (ensures only 
                creatures that have been played are stored and rendered)
            */
            tile& currTile = map->tileAt(x, y);

            for (auto const& entry : currTile.getCreatureList()) {
                const creaturePrototype* tempPrototype = &((entry)->prototype);

                std::map<const creaturePrototype*, QImage>::iterator findTexture(creatureTextures.find(tempPrototype));

                if (findTexture == creatureTextures.end()) {
                    findTexture = creatureTextures.insert(
                        std::pair<const creaturePrototype*, QImage>
                        (tempPrototype, toTexture(tempPrototype->getTexture()))).first;
                }

                painter.drawImage(QPoint(xTilePos, yTilePos), findTexture->second);
            }

            // check if a tile has been marked as active and draw it
            if (x == activeTile.x() && y == activeTile.y()) {
                painter.drawImage(QPoint(xTilePos, yTilePos), cursorTexture);
            }

            // demonstration purpose: draw path
            for (auto& dude : demoPath) {
                if (x == dude.first && y == dude.second) {
                    painter.drawImage(QPoint(xTilePos, yTilePos), pathTexture);
                }
            }
        }
    }
}

void simAreaWidget::mousePressEvent(QMouseEvent* pEvent)
{
    /*
        Check if ther left mouse button has been clicked.
        If so, set the tile as active
    */
    if (pEvent->button() & Qt::LeftButton) {

        // a-star path finding demo

        demoPath = as->getPath(nullptr,
            &map->tileAt(activeTile.x(), activeTile.y()),
            map->tileAt(((pEvent->pos().x() + xScroll) / TILEWIDTH), ((pEvent->pos().y() + yScroll) / TILEHEIGHT)));
      
        // update active tile pos
        activeTile.setX((pEvent->pos().x() + xScroll) / TILEWIDTH);
        activeTile.setY((pEvent->pos().y() + yScroll) / TILEHEIGHT);

        // update frame
        update();
    }
}

world& simAreaWidget::getMap() const
{
    return *map;
}

int simAreaWidget::getTileHeight() const
{
    return TILEHEIGHT;
}

int simAreaWidget::getTileWidth() const
{
    return TILEWIDTH;
}

void simAreaWidget::setScrollX(int pVal)
{
    xScroll = pVal;
}

void simAreaWidget::setScrollY(int pVal)
{
    yScroll = pVal;
}

int simAreaWidget::getScrollY() const
{
    return yScroll;
}

int simAreaWidget::getScrollX() const
{
    return xScroll;
}

QPoint simAreaWidget::getActiveTile() const
{
    return activeTile;
}

simAreaWidget::~simAreaWidget()
{
}
