#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyApp.h"

class MyApp : public QMainWindow
{
    Q_OBJECT

public:
    MyApp(QWidget *parent = Q_NULLPTR);

private:
    Ui::MyAppClass ui;
    void HideAll();
    qint64 GetSum();

private slots:
    void EvaluateClicked();
    void CheckKv1();
    void CheckKv2();
    void CheckKv3();
    void CheckKv4();
};
