#include "ai3_bachelor_praktikum.h"

ai3_bachelor_praktikum::ai3_bachelor_praktikum(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);
}

void ai3_bachelor_praktikum::setPrototypeList(const QStringList& prototypeList)
{
    modelCreaturePrototypeList.setStringList(prototypeList);
    ui.cbCreature->setModel(&modelCreaturePrototypeList);
}

simAreaWidget& ai3_bachelor_praktikum::getAreaWidget()
{
    return *ui.simulationArea;
}

void ai3_bachelor_praktikum::paintEvent(QPaintEvent* pEvent) {
    emit paintOccured();
}