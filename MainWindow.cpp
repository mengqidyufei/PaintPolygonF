#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMatrix4x4>

void CalcTransform_SRT(const QPointF& _Trans
        ,  const QSizeF& _Scale
        ,  qreal _RotateZ
        ,  const QPointF& _Anchor
        ,  const QSizeF& _Size
        , QMatrix4x4& _Mat)
{
    _Mat.setToIdentity();

    // S * R * T: _matrix = t * toOrginal * r * toAnchor * s;
    auto len = QPointF(_Anchor.x() * _Size.width(), _Anchor.y() * _Size.height());
    _Mat.translate(_Trans.x(), _Trans.y(), 0);
    _Mat.translate(len.x(), len.y(), 0);
    _Mat.rotate(_RotateZ, 0.0f, 0.0f, 1.0f);
    _Mat.scale(_Scale.width(), _Scale.height());
    _Mat.translate(-len.x(), -len.y(), 0);
    _Mat.scale(_Size.width(), _Size.height(), 1.0);
}

QPointF AnchorAbsolute(const QPointF& anchorNormalized, const QRectF& boundingRect)
{
    QPointF topLeft = boundingRect.topLeft();
    return QPointF(anchorNormalized.x() * boundingRect.width() + topLeft.x(),
        anchorNormalized.y() * boundingRect.height() + topLeft.y());
}

QPolygonF PolygonRotation(const QPolygonF& poly, qreal angle, const QPointF& anchorNormalized)
{
    QPointF anchorPos = AnchorAbsolute(anchorNormalized, poly.boundingRect());
    QTransform trans;
    trans.translate(anchorPos.x(), anchorPos.y());
    trans.rotate(angle);
    trans.translate(-anchorPos.x(), -anchorPos.y());
    return trans.map(poly);
}

QPolygonF PolygonRotation(const QPolygonF &poly, qreal angle, const QPointF &anchorNormalized, const QRectF anchorRect)
{
    QPointF anchorPos = AnchorAbsolute(anchorNormalized, anchorRect);
    QTransform trans;
    trans.translate(anchorPos.x(), anchorPos.y());
    trans.rotate(angle);
    trans.translate(-anchorPos.x(), -anchorPos.y());
    return trans.map(poly);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QMatrix4x4 matrix;
    //CalcTransform_SRT(QPointF(-1001, -562), QSizeF(2.0339, 2.0339), 110, QPointF(0.5, 0.5), QSizeF(1888, 1062), matrix);
    //QPolygonF p;
    //p.append(matrix.map(QPointF(0, 0)));
    //p.append(matrix.map(QPointF(1, 0)));
    //p.append(matrix.map(QPointF(1, 1)));
    //p.append(matrix.map(QPointF(0, 1)));
    //p.append(matrix.map(QPointF(0, 0)));
    //qDebug() << "p" << p;

    ui->spinBoxScale->setValue(10);
    ui->lineEditPolygon1->setText("QPointF(0,0),QPointF(1920,0),QPointF(1920,1080),QPointF(0,1080),QPointF(0,0)");
    ui->lineEditPolygon2->setText("QPointF(1614.55,-1465.83),QPointF(301.19,2142.59),QPointF(-1728.55,1403.83),QPointF(-415.19,-2204.59),QPointF(1614.55,-1465.83)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    updatePaint();
}

void MainWindow::on_spinBoxScale_valueChanged(int arg1)
{
    updatePaint();
}

void MainWindow::updatePaint()
{
    std::function<QPolygonF(const QString &, int)>toPolygonF = [](const QString& text, int scale)
    {
        QString input = text;
        qDebug() << input;

        // 正则表达式模式
        QRegularExpression regExp("QPointF\\(([-+]?\\d+(\\.\\d+)?),([-+]?\\d+(\\.\\d+)?)\\)");
        //QRegularExpression regExp(R"(\QPointF\((-?\d+\.\d+),(-?\d+\.\d+)\))");
        QRegularExpressionMatchIterator i = regExp.globalMatch(input);

        // 创建 QPolygonF 对象
        QPolygonF polygon;

        // 解析并添加点到 QPolygonF
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            //for (int i = 0; i <= match.lastCapturedIndex(); ++i) {
            //    QString captured = match.captured(i);
            //    qDebug() << "Match last capture index:" << captured;
            //}
            QPointF point(match.captured(1).toDouble(), match.captured(3).toDouble());
            //qDebug() << point;
            polygon << point;
        }

        // 输出多边形的点
        for (QPointF &point : polygon) {
            point /= scale;
        }
        return polygon;
    };
    QString text1 = ui->lineEditPolygon1->text();
    if (text1.isEmpty())
        text1 = ui->lineEditPolygon1->placeholderText();
    QPolygonF p1 = toPolygonF(text1, ui->spinBoxScale->value());
    qreal angle1 = ui->angle1->value();
    QPointF anchor1 = QPointF(ui->anchorX1->value(), ui->anchorY1->value());
    QPolygonF p1Rotation1 = PolygonRotation(p1, angle1, anchor1);
    ui->paintWidget->setPolygonF1(p1Rotation1);

    QString text2 = ui->lineEditPolygon2->text();
    if (text2.isEmpty())
        text2 = ui->lineEditPolygon2->placeholderText();
    QPolygonF p2 = toPolygonF(text2, ui->spinBoxScale->value());
    qreal angle2 = ui->angle2->value();
    QPointF anchor2 = QPointF(ui->anchorX2->value(), ui->anchorY2->value());
    QPolygonF p2Rotation = PolygonRotation(p2, angle2, anchor2);
    qDebug() << "Rotation poly:" << p2Rotation;
    ui->paintWidget->setPolygonF2(p2Rotation);
}

