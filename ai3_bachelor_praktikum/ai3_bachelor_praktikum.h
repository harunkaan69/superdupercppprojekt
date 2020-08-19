#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_ai3_bachelor_praktikum.h"
#include "qstringlistmodel.h"
#include "qmessagebox.h"
#include "creatureFile.h"
#include "exception.h"

/*
    Class to handle main gui form
*/
class ai3_bachelor_praktikum : public QMainWindow
{
    friend class areaPresenter;
    Q_OBJECT

public:
    ai3_bachelor_praktikum(QWidget *parent = Q_NULLPTR);

    void setPrototypeList(const QStringList& prototypeList);
    simAreaWidget& getAreaWidget();

private:

    Ui::ai3_bachelor_praktikum ui;

    QStringListModel modelCreaturePrototypeList;

    void paintEvent(QPaintEvent* pEvent);

signals:
    void paintOccured();
};
