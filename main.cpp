﻿#include "MainWindow.h"

#include <QApplication>

#include <QRegularExpression>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
