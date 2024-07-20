#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBoxScale->setValue(10);
    ui->lineEditPolygon1->setPlaceholderText("QPointF(0,0),QPointF(1920,0),QPointF(1920,1080),QPointF(0,1080),QPointF(0,0)");
    ui->lineEditPolygon2->setPlaceholderText("QPointF(100,100),QPointF(1820,100),QPointF(1820,980),QPointF(100,980),QPointF(100,100)");
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

        // 正则表达式模式
        QRegularExpression regExp("QPointF\\((\\d+),(\\d+)\\)");
        QRegularExpressionMatchIterator i = regExp.globalMatch(input);

        // 创建 QPolygonF 对象
        QPolygonF polygon;

        // 解析并添加点到 QPolygonF
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QPointF point(match.captured(1).toDouble(), match.captured(2).toDouble());
            polygon << point;
        }

        // 输出多边形的点
        for (QPointF &point : polygon) {
            point /= scale;
            //qDebug() << point;
        }
        return polygon;
    };
    QString text1 = ui->lineEditPolygon1->text();
    if (text1.isEmpty())
        text1 = ui->lineEditPolygon1->placeholderText();
    QPolygonF p1 = toPolygonF(text1, ui->spinBoxScale->value());
    ui->paintWidget->setPolygonF1(p1);
    QString text2 = ui->lineEditPolygon2->text();
    if (text2.isEmpty())
        text2 = ui->lineEditPolygon2->placeholderText();
    QPolygonF p2 = toPolygonF(text2, ui->spinBoxScale->value());
    ui->paintWidget->setPolygonF2(p2);
}

