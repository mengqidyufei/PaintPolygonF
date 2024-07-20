#ifndef PAINTPOLYGONF_H
#define PAINTPOLYGONF_H

#include <QWidget>

class PaintPolygonF : public QWidget
{
public:
    PaintPolygonF(QWidget *widget = nullptr);

    virtual void paintEvent(QPaintEvent *event) override;
    void setPolygonF1(const QPolygonF &poly1);
    void setPolygonF2(const QPolygonF &poly2);

private:
    QPolygonF mPoly1;
    QPolygonF mPoly2;
};


#endif // PAINTPOLYGONF_H
