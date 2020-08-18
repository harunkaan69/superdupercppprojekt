#pragma once
#include <QObject>
#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include "astar.h"
#include "world.h"
/*
    Class to manage the simulation area (left most widget)
*/
class simAreaWidget : public QWidget
{
	Q_OBJECT

public:
	simAreaWidget(QWidget* parent = Q_NULLPTR);
	~simAreaWidget();

    // get vaues
    world& getMap() const;
    int getTileHeight() const;
    int getTileWidth() const;
    int getScrollY() const;
    int getScrollX() const;
    QPoint getActiveTile() const;

    // set values
    void setScrollX(int pVal);
    void setScrollY(int pVal);

    //demonstration purpose
    std::vector<std::pair<int, int>> demoPath;
    std::shared_ptr<astar> as; 

private:
    // const values
    const int TILEWIDTH = 32;
    const int TILEHEIGHT = 32;
    const int WORLDWIDTH = 200;
    const int WORLDHEIGHT = 200;

    // current scrollbar values
    int xScroll;
    int yScroll;

    // selected tile
    QPoint activeTile;

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

    // events
    void paintEvent(QPaintEvent* pEvent);
    void mousePressEvent(QMouseEvent* pEvent);
};
