#include "simAreaWidget.h"

simAreaWidget::simAreaWidget(QWidget*parent) : QWidget(parent)
{
    // set scroll values to 0 (no scrolling done)
    xScroll = 0;
    yScroll = 0;
}

simAreaWidget::~simAreaWidget()
{
}

void simAreaWidget::handleImage(const QImage& image)
{
    canvas = image;
    update();
}

void simAreaWidget::paintEvent(QPaintEvent* pEvent)
{
    QPainter paint(this);

    auto margins = contentsMargins();
    paint.drawImage(margins.left(), margins.top(), canvas);
    paint.end();
}

void simAreaWidget::mousePressEvent(QMouseEvent* pEvent)
{
    /*
        Check if ther left mouse button has been clicked.
        If so, set the tile as active
    */
    if (pEvent->button() & Qt::LeftButton) {

        emit activeTileChanged(QPoint(pEvent->pos().x() + xScroll, pEvent->pos().y() + yScroll));
        // update frame
        emit paintRequest();
    }
}

void simAreaWidget::resizeEvent(QResizeEvent* pEvent)
{
    emit resizeOccured();
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