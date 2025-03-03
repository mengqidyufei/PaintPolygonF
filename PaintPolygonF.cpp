#include <QPainter>
#include "PaintPolygonF.h"

PaintPolygonF::PaintPolygonF(QWidget *widget)
    :QWidget()
{
}

void PaintPolygonF::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.save();
    p.translate(300, 200);
    p.setPen(Qt::black);
    p.drawPolygon(mPoly1);
    p.drawPolygon(mPoly2);
    p.restore();
    QWidget::paintEvent(event);
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
