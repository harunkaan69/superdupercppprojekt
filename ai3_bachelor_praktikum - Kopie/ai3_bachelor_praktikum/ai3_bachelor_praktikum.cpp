#include "ai3_bachelor_praktikum.h"

ai3_bachelor_praktikum::ai3_bachelor_praktikum(std::vector<creaturePrototype>&& pCreatureList, QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);
    
    // connect sim-control widget elements
    connect(ui.btnStart, SIGNAL(clicked()), this, SLOT(onBtnStartPressed()));
    connect(ui.btnStop, SIGNAL(clicked()), this, SLOT(onBtnStopPressed()));
    connect(ui.btnPause, SIGNAL(clicked()), this, SLOT(onBtnPausePressed()));
    connect(ui.btnPlace, SIGNAL(clicked()), this, SLOT(onBtnPlacePressed()));
    connect(ui.cbCreature, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbCreatureCurrentIndexChanged(int)));
    // connect sim-area widget sliders
    connect(ui.hScrollbar, SIGNAL(valueChanged(int)), this, SLOT(onHSliderValueChanged(int)));
    connect(ui.vScrollbar, SIGNAL(valueChanged(int)), this, SLOT(onVSliderValueChanged(int)));
    
    // setup creatureprototype list for the combobox
    creaturePrototypeList = pCreatureList;
    modelCreaturePrototypeList = new QStringListModel(this);
    QStringList list;

    // loop through each creature in the list and append its name to the combobox
    for (auto const& i : creaturePrototypeList) {
        list.push_back(QString::fromStdString(i.getName()));    
    }

    // link model
    modelCreaturePrototypeList->setStringList(list);
    ui.cbCreature->setModel(modelCreaturePrototypeList);
}

void ai3_bachelor_praktikum::onBtnStartPressed() {
    QMessageBox::about(this, "'Start' Button Message Box", "'Start' Button Clicked");
}

void ai3_bachelor_praktikum::onBtnStopPressed() {
    QMessageBox::about(this, "'Stop' Button Message Box", "'Stop' Button Clicked");
}

void ai3_bachelor_praktikum::onBtnPausePressed() {
    QMessageBox::about(this, "'Pause' Button Message Box", "'Pause' Button Clicked");
}

void ai3_bachelor_praktikum::onBtnPlacePressed() {
    // get current index of the combobox
    int index = ui.cbCreature->currentIndex();

    // return if no item is selected (empty at index -1) or index is out of bounds
    if (index < 0 || index >= ui.cbCreature->count()) {
        return;
    }

    creaturePrototype::habitate currentHabitate = creaturePrototypeList[index].getHabitate();
    tile::terrain               currentTile     = ui.simulationArea->getMap().tileAt(
                                                    ui.simulationArea->getActiveTile().x(), ui.simulationArea->getActiveTile().y()).getTerrain();




    if (currentHabitate == creaturePrototype::habitate::land) {
        if ((currentTile != tile::terrain::deepWater) && (currentTile != tile::terrain::shallowWater)) {
            createCreature(creaturePrototypeList[index], ui.simulationArea->getActiveTile().x(), ui.simulationArea->getActiveTile().y());
        } else {
            QMessageBox::about(this, "Fehler beim Plazieren der Kreatur",
                "Zulaessiger Lebensraum der Kreatur stimmt nicht mit dem Terrain Typen der Umgebung ein.");
            return;
        }
    } else if (currentHabitate == creaturePrototype::habitate::water) {
        if ((currentTile == tile::terrain::deepWater) || (currentTile == tile::terrain::shallowWater)) {
            createCreature(creaturePrototypeList[index], ui.simulationArea->getActiveTile().x(), ui.simulationArea->getActiveTile().y());
        } else {
            QMessageBox::about(this, "Fehler beim Plazieren der Kreatur",
                "Zulaessiger Lebensraum der Kreatur stimmt nicht mit dem Terrain Typen der Umgebung ein.");
            return;
        }
    }

    // update frame
    ui.simulationArea->update();
    printCreatureStatistics();
}

void ai3_bachelor_praktikum::onCbCreatureCurrentIndexChanged(int pIndex) {

    QString tempAttributesString = "";
    std::vector<std::string> tempAttributes = creaturePrototypeList[pIndex].getAttributes();

    for (int k = 0; k < tempAttributes.size(); k++) {
        tempAttributesString += QString::fromStdString(tempAttributes[k]);
        tempAttributesString += " ";
    }

    ui.tbAttributes->setText(tempAttributesString);
    ui.tbStrength->setText(QString::number(creaturePrototypeList[pIndex].getStrength()));
    ui.tbSpeed->setText(QString::number(creaturePrototypeList[pIndex].getSpeed()));
    ui.tbLife->setText(QString::number(creaturePrototypeList[pIndex].getLifetime()));
}

void ai3_bachelor_praktikum::resizeEvent(QResizeEvent* pEvent) {
    updateScrollbars();
}

void ai3_bachelor_praktikum::paintEvent(QPaintEvent* pEvent) {
    updateScrollbars();
    printCreatureStatistics();
}

void ai3_bachelor_praktikum::onVSliderValueChanged(int pVal) {
    /*
        update current vertical slider value for the sim-area
        widget and draw the new frame
    */
    ui.simulationArea->setScrollY(pVal);
    ui.simulationArea->update();
}

void ai3_bachelor_praktikum::onHSliderValueChanged(int pVal) {
    /* 
        update current horizontal slider value for the sim-area
        widget and draw the new frame
    */
    ui.simulationArea->setScrollX(pVal);
    ui.simulationArea->update();
}

void ai3_bachelor_praktikum::updateScrollbars() {
    // get current width and height of the sim-area
    QSize widgetSize(ui.simulationArea->width(), ui.simulationArea->height());
    // get map width and height in pixels
    int mapWith = ui.simulationArea->getMap().getWidth() * ui.simulationArea->getTileWidth();
    int mapHeight = ui.simulationArea->getMap().getHeight() * ui.simulationArea->getTileHeight();

    // set sliders max value and size (page step)
    ui.hScrollbar->setMaximum(mapWith - widgetSize.width());
    ui.vScrollbar->setMaximum(mapHeight - widgetSize.height());
    ui.hScrollbar->setPageStep(widgetSize.width());
    ui.vScrollbar->setPageStep(widgetSize.height());
}

std::weak_ptr<creature> ai3_bachelor_praktikum::createCreature(const creaturePrototype& pPrototype, int pX, int pY) {
    std::shared_ptr<creature> tempCreature(new creature(pPrototype, pY, pY));

    if (pX < 0 || pY < 0 || pX >= ui.simulationArea->getMap().getWidth() || pY >= ui.simulationArea->getMap().getHeight())
    {
        throw parameterError("");
    }

    creatureList.push_back(tempCreature);

    try {
        ui.simulationArea->getMap().placeCreature(tempCreature, ui.simulationArea->getActiveTile().x(), ui.simulationArea->getActiveTile().y());
    }
    catch (...) {
        creatureList.pop_back();
        throw;
    }

    return tempCreature;
}

void ai3_bachelor_praktikum::printCreatureStatistics() {
    ui.statusBar->showMessage("GT: " + QString::number(creatureList.size()) +
        "| LT: " + QString::number((ui.simulationArea->getMap().tileAt(
            ui.simulationArea->getActiveTile().x(),
            ui.simulationArea->getActiveTile().y()).getCreatureList().size())));
}