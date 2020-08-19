#pragma once
#include <QObject>
#include <QMouseEvent>
#include <QWidget>
#include <QPainter>

/*
    Class to manage the simulation area (left most widget)
*/
class simAreaWidget : public QWidget
{
	Q_OBJECT

public:
	simAreaWidget(QWidget* parent = Q_NULLPTR);
	~simAreaWidget();

    // get values
    int getScrollY() const;
    int getScrollX() const;

    // set values
    void setScrollX(int pVal);
    void setScrollY(int pVal);

private:
    
    // current scrollbar values
    int xScroll;
    int yScroll;

    // events
    void paintEvent(QPaintEvent* pEvent);
    void mousePressEvent(QMouseEvent* pEvent);

    QImage canvas;

    void resizeEvent(QResizeEvent* pEvent);

signals:
    void paintRequest();
    void activeTileChanged(QPoint pPoint);
    void resizeOccured();

public slots:
    void handleImage(const QImage& image);

};
