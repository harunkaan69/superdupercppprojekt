#include "areaPresenter.h"

areaPresenter::areaPresenter(model& pModel, ai3_bachelor_praktikum& pWidgetMain) :
    modelInstance(pModel), widget(pWidgetMain.getAreaWidget()), widgetMain(pWidgetMain)
{
    timer = new QTimer(this);
    timer->setInterval(1000);

    // connect sim-control widget elements
    connect(widgetMain.ui.btnStart, SIGNAL(clicked()), this, SLOT(onBtnStartPressed()));
    connect(widgetMain.ui.btnStop, SIGNAL(clicked()), this, SLOT(onBtnStopPressed()));
    connect(widgetMain.ui.btnPause, SIGNAL(clicked()), this, SLOT(onBtnPausePressed()));
    connect(widgetMain.ui.btnPlace, SIGNAL(clicked()), this, SLOT(onBtnPlacePressed()));
    connect(widgetMain.ui.cbCreature, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbCreatureCurrentIndexChanged(int)));
    // connect sim-area widget sliders
    connect(widgetMain.ui.hScrollbar, SIGNAL(valueChanged(int)), this, SLOT(onHSliderValueChanged(int)));
    connect(widgetMain.ui.vScrollbar, SIGNAL(valueChanged(int)), this, SLOT(onVSliderValueChanged(int)));

    connect(&widget, SIGNAL(resizeOccured()), this, SLOT(onResizeOccured()));
    connect(&widgetMain, SIGNAL(paintOccured()), this, SLOT(onPaintOccured()));

    connect(this, SIGNAL(imageCreated(const QImage&)), &widget, SLOT(handleImage(const QImage&)));

    connect(&widget, SIGNAL(paintRequest()), this, SLOT(onPaintRequest()));
    connect(&widget, SIGNAL(activeTileChanged(QPoint)), &modelInstance, SLOT(onActiveTileChanged(QPoint)));
    
    connect(timer, &QTimer::timeout, this, &areaPresenter::onTimerTick);

    QStringList list;

    // loop through each creature in the list and append its name to the combobox
    for (auto const& i : modelInstance.getCreaturePrototypeList())
        list.push_back(QString::fromStdString(i.getName()));

    widgetMain.setPrototypeList(list);
}

QImage areaPresenter::drawMap()
{
    QImage canvas(widget.width(), widget.height(), QImage::Format::Format_RGBA8888);
    canvas.fill(Qt::white);
    QPainter painter(&canvas);

    int xScroll = widget.getScrollX();
    int yScroll = widget.getScrollY();

    // check how many tiles the current scroll value has exceeded
    int xScrollPixel = xScroll % modelInstance.TILEWIDTH;
    int yScrollPixel = yScroll % modelInstance.TILEHEIGHT;

    // set borders of what tiles to to render
    int xTileStart = (xScroll - xScrollPixel) / modelInstance.TILEWIDTH;
    int xTileEnd = (xScroll + widget.width() - xScrollPixel) / modelInstance.TILEWIDTH + 1;
    int yTileStart = (yScroll - yScrollPixel) / modelInstance.TILEWIDTH;
    int yTileEnd = (yScroll + widget.height() - yScrollPixel) / modelInstance.TILEHEIGHT + 1;

    // loop and draw tiles in the visible frame
    for (int y = yTileStart; y <= yTileEnd; y++) {
        for (int x = xTileStart; x <= xTileEnd; x++) {

            // skip if tiles are left & top most or right & bottom most 
            auto& map = modelInstance.getMap();
            if (x < 0 || y < 0 || x >= map.getWidth() || y >= map.getHeight())
                continue;

            int xTilePos = (x - xTileStart) * modelInstance.TILEWIDTH - xScrollPixel;
            int yTilePos = (y - yTileStart) * modelInstance.TILEHEIGHT - yScrollPixel;

            /*
                Get the current tile that is being rendered and check,
                whether it has any entries in its creature list. If so,
                loop through the list and add a pair of creature prototypes
                and their corresponding texture to the std::map containing
                creature prototype textures if it i not already inside. Only
                draw if there are entries in the creature list. (ensures only
                creatures that have been played are stored and rendered)
            */
            tile& currTile = map.tileAt(x, y);

            // draw tiles
            auto& envImage = modelInstance.getImage(currTile);
            painter.drawImage(QPoint(xTilePos, yTilePos), envImage);

            for (auto const& entry : currTile.getCreatureList()) 
            {
                if (entry->isDead())
                {
                    auto& deathImage = modelInstance.getDeadTexture();
                    painter.drawImage(QPoint(xTilePos, yTilePos), deathImage);
                }
                else
                {
                    auto& creatureImage = modelInstance.getImage(entry->prototype);
                    painter.drawImage(QPoint(xTilePos, yTilePos), creatureImage);
                }
            }

            auto activeTile = modelInstance.getActiveTile();
            // check if a tile has been marked as active and draw it
            if (x == activeTile.x() && y == activeTile.y()) {
                painter.drawImage(QPoint(xTilePos, yTilePos), modelInstance.getCursorImage());
            }

            // demonstration purpose: draw path
            for (const auto& dude : modelInstance.getDemoPath()) 
            {
                if (x == dude.first && y == dude.second)
                    painter.drawImage(QPoint(xTilePos, yTilePos), modelInstance.getPathTexture());
            }
        }
    }
    return canvas;
}

void areaPresenter::onBtnStartPressed() {
    //QMessageBox::about(this, "'Start' Button Message Box", "'Start' Button Clicked");
    timer->start();
}

void areaPresenter::onBtnStopPressed() {
    //QMessageBox::about(this, "'Stop' Button Message Box", "'Stop' Button Clicked");
    timer->stop();
    //reset
}

void areaPresenter::onBtnPausePressed() {
    //QMessageBox::about(this, "'Pause' Button Message Box", "'Pause' Button Clicked");
    timer->stop();
}

void areaPresenter::onBtnPlacePressed() {
    // get current index of the combobox
    int index = widgetMain.ui.cbCreature->currentIndex();

    // return if no item is selected (empty at index -1) or index is out of bounds
    if (index < 0 || index >= widgetMain.ui.cbCreature->count())
        return;

    auto activeTile = modelInstance.getActiveTile();
    auto& prototypeList = modelInstance.getCreaturePrototypeList();

    tile::terrain currentTile = modelInstance.getMap().tileAt(
            activeTile.x(), activeTile.y()).getTerrain();

    creaturePrototype::habitate currentHabitate = prototypeList[index].getHabitate();

    if (currentHabitate == creaturePrototype::habitate::land) {
        if ((currentTile != tile::terrain::deepWater) && (currentTile != tile::terrain::shallowWater)) {
            modelInstance.createCreature(prototypeList[index], activeTile.x(), activeTile.y());
        }
        else {
            //QMessageBox::about(this, "Fehler beim Plazieren der Kreatur",
             //   "Zulaessiger Lebensraum der Kreatur stimmt nicht mit dem Terrain Typen der Umgebung ein.");
            return;
        }
    }
    else if (currentHabitate == creaturePrototype::habitate::water) {
        if ((currentTile == tile::terrain::deepWater) || (currentTile == tile::terrain::shallowWater)) {
            modelInstance.createCreature(prototypeList[index], activeTile.x(), activeTile.y());
        }
        else {
            //QMessageBox::about(this, "Fehler beim Plazieren der Kreatur",
              //  "Zulaessiger Lebensraum der Kreatur stimmt nicht mit dem Terrain Typen der Umgebung ein.");
            return;
        }
    }

    // update frame
    emit imageCreated(drawMap());
    printCreatureStatistics();
}

void areaPresenter::onCbCreatureCurrentIndexChanged(int pIndex) {

    auto& prototypeList = modelInstance.getCreaturePrototypeList();

    QString tempAttributesString = "";
    std::vector<std::string> tempAttributes = prototypeList[pIndex].getAttributes();

    for (int k = 0; k < tempAttributes.size(); k++) {
        tempAttributesString += QString::fromStdString(tempAttributes[k]);
        tempAttributesString += " ";
    }

    widgetMain.ui.tbAttributes->setText(tempAttributesString);
    widgetMain.ui.tbStrength->setText(QString::number(prototypeList[pIndex].getStrength()));
    widgetMain.ui.tbSpeed->setText(QString::number(prototypeList[pIndex].getSpeed()));
    widgetMain.ui.tbLife->setText(QString::number(prototypeList[pIndex].getLifetime()));
}

void areaPresenter::onVSliderValueChanged(int pVal) {
    /*
        update current vertical slider value for the sim-area
        widget and draw the new frame
    */
    widget.setScrollY(pVal);
    emit imageCreated(drawMap());
}

void areaPresenter::onHSliderValueChanged(int pVal) 
{
    /*
        update current horizontal slider value for the sim-area
        widget and draw the new frame
    */
    widget.setScrollX(pVal);
    emit imageCreated(drawMap());
}

void areaPresenter::printCreatureStatistics()
{
    widgetMain.ui.statusBar->showMessage("GT: " + QString::number(modelInstance.getCreatureCnt()) +
        "| LT: " + QString::number((modelInstance.getMap().tileAt(
            modelInstance.getActiveTile().x(),
            modelInstance.getActiveTile().y()).getCreatureList().size())));
}

void areaPresenter::onResizeOccured()
{
    updateScrollbars();
    emit imageCreated(drawMap());
}

void areaPresenter::onPaintOccured()
{
    updateScrollbars();
    printCreatureStatistics();
}

void areaPresenter::onPaintRequest()
{
    emit imageCreated(drawMap());
}

void areaPresenter::updateScrollbars() {
    // get current width and height of the sim-area
    QSize widgetSize(widget.width(), widget.height());
    // get map width and height in pixels
    int mapWith = modelInstance.getMap().getWidth() * modelInstance.TILEWIDTH;
    int mapHeight = modelInstance.getMap().getHeight() * modelInstance.TILEHEIGHT;

    // set sliders max value and size (page step)
    widgetMain.ui.hScrollbar->setMaximum(mapWith - widgetSize.width());
    widgetMain.ui.vScrollbar->setMaximum(mapHeight - widgetSize.height());
    widgetMain.ui.hScrollbar->setPageStep(widgetSize.width());
    widgetMain.ui.vScrollbar->setPageStep(widgetSize.height());
}

void areaPresenter::onTimerTick()
{
    auto& tempAiList = modelInstance.getAiList();

    for (auto& it = tempAiList.begin(); it != tempAiList.end(); it++)
    {
        auto instance = (*it)->getCreatureInstance().lock();
        if(instance && instance->isDecayed())
            it = tempAiList.erase(it);

        (*it)->performStep();    

        if (instance->isDecayed())
            it = tempAiList.erase(it);
    }

    emit imageCreated(drawMap());
    widget.update();
}