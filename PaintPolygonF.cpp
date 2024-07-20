#include <QPainter>
#include "PaintPolygonF.h"

PaintPolygonF::PaintPolygonF(QWidget *widget)
    :QWidget()
{

}

void PaintPolygonF::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter p(this);
    p.setPen(Qt::black);
    p.drawPolygon(mPoly1);
    p.drawPolygon(mPoly2);
}

void PaintPolygonF::setPolygonF1(const QPolygonF &poly1)
{
    mPoly1 = poly1;
    repaint();
}

void PaintPolygonF::setPolygonF2(const QPolygonF &poly2)
{
    mPoly2 = poly2;
    repaint();
}
