#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_ai3_bachelor_praktikum.h"
#include "qstringlistmodel.h"
#include "qmessagebox.h"
#include "creatureFile.h"
#include "simAreaWidget.h"
#include "exception.h"
/*
    Class to handle main gui form
*/
class ai3_bachelor_praktikum : public QMainWindow
{
    Q_OBJECT

public:
    ai3_bachelor_praktikum(std::vector<creaturePrototype>&& pCreatureList, QWidget *parent = Q_NULLPTR);
private:
    Ui::ai3_bachelor_praktikum ui;

    // models
    QStringListModel* modelCreaturePrototypeList;

    // creature/prototype list to manage
    std::vector<creaturePrototype> creaturePrototypeList;
    std::vector<std::shared_ptr<creature>> creatureList;

    // events
    void resizeEvent(QResizeEvent* pEvent);
    void paintEvent(QPaintEvent* pEvent);

    // utility
    void updateScrollbars();

private slots:

    // buttons
    void onBtnStartPressed();
    void onBtnStopPressed();
    void onBtnPausePressed();
    void onBtnPlacePressed();

    // combobox
    void onCbCreatureCurrentIndexChanged(int pIndex);

    // sim-area sliders
    void onVSliderValueChanged(int pVal);
    void onHSliderValueChanged(int pVal);

    // others
    std::weak_ptr<creature> createCreature(const creaturePrototype& pPrototype, int pX, int pY);
    void printCreatureStatistics();
};
