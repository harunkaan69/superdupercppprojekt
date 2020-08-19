#pragma once
#include <QObject>
#include <qimage.h>

#include "simAreaWidget.h"
#include "ai3_bachelor_praktikum.h"
#include "model.h"

class areaPresenter : public QObject
{
	Q_OBJECT

public:
	areaPresenter(model& pModel, ai3_bachelor_praktikum& pWidgetMain);

	QImage drawMap();

private:
	
	model& modelInstance;
	simAreaWidget& widget;
	ai3_bachelor_praktikum& widgetMain;

    QTimer* timer;

    // utility
    void updateScrollbars();

signals:
    void imageCreated(const QImage& image);

public slots:

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

    // other(s)
    void printCreatureStatistics();

    void onResizeOccured();
    void onPaintOccured();

    void onPaintRequest();

    void onTimerTick();
};

