#include "MyApp.h"
#include <QVector>
#include <QString>
#include <QDate>
#include <random>

MyApp::MyApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //ui.Table->verticalHeader()->setVisible(false);
    ui.EmptyNum->setStyleSheet("color: rgb(255, 0, 0)");
    ui.EmptyKvart->setStyleSheet("color: rgb(255, 0, 0)");
    ui.Incorrect->setStyleSheet("color: rgb(255, 0, 0)");
    ui.Table->setEditTriggers(0);
    ui.Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.Table->verticalHeader()->hide();
    HideAll();
    connect(ui.Evaluate, SIGNAL(clicked()), this, SLOT(EvaluateClicked()));
    connect(ui.kvartal1, SIGNAL(clicked()), this, SLOT(CheckKv1()));
    connect(ui.kvartal2, SIGNAL(clicked()), this, SLOT(CheckKv2()));
    connect(ui.kvartal3, SIGNAL(clicked()), this, SLOT(CheckKv3()));
    connect(ui.kvartal4, SIGNAL(clicked()), this, SLOT(CheckKv4()));
}

void MyApp::HideAll()
{
    ui.EmptyKvart->setVisible(false);
    ui.EmptyNum->setVisible(false);
    ui.Incorrect->setVisible(false);
}

qint64 MyApp::GetSum()
{
    qint64 res = 0;
    for (std::size_t i{ 0 }; i < 12; ++i)
        res += ui.Table->item(i, 1)->text().toInt();

    return res;
}

static bool kv1 = false, kv2 = false, kv3 = false, kv4 = false;

static QVector<std::size_t> GetMonths()
{
    if (kv1)
        return QVector<std::size_t>({ 1/*"January"*/, 2/*"February"*/, 3/*"March"*/ });

    if (kv2)
        return QVector<std::size_t>({ 4/*"April"*/, 5/*"May"*/, 6/*"June"*/ });

    if (kv3)
        return QVector<std::size_t>({ 7/*"July"*/, 8/*"August"*/, 9/*"September"*/ });

    if (kv4)
        return QVector<std::size_t>({ 10/*"October"*/, 11/*"November"*/, 12/*"December"*/ });
}

static
QDate FindFirstMonday(QDate obj)
{
    for (std::size_t i{ 0 }; i < obj.daysInMonth(); ++i)
        if (obj.dayOfWeek() == 1)
            return obj;
        else
            obj=obj.addDays(1);
}

void MyApp::EvaluateClicked()
{
    HideAll();
    bool correct = true;
    if (!(kv1 || kv2 || kv3 || kv4))
    {
        ui.EmptyKvart->setVisible(true);
        correct = false;
    }
        
    if (!ui.Enter->text().size())
    {
        ui.EmptyNum->setVisible(true);
        correct = false;
    }

    bool ok;
    qint64 rub = ui.Enter->text().toInt(&ok);
    if (!ok)
    {
        ui.Incorrect->setVisible(true);
        correct = false;
    }

    if (correct)
    {
        std::random_device rd;
        std::mt19937 mersenne(rd());

        // Заполнение дат
        int year = ui.Year->currentText().toInt();
        QVector<std::size_t> curKvartal = GetMonths();

        QVector<QDate> dates = { QDate(year, curKvartal[0], 1), QDate(year, curKvartal[1], 1), QDate(year, curKvartal[2], 1) };

        std::size_t j{ 0 };
        for (std::size_t i{ 0 }; i < 3; ++i)
        {
            QDate day1 = FindFirstMonday(dates[i]), day2, day3, day4;
            day2 = day1.addDays(7ULL + mersenne() % 5);
            day3 = day1.addDays(14ULL + mersenne() % 5);
            day4 = day1.addDays(21ULL + mersenne() % 5);

            if (day4 > QDate(day1.year(), day1.month(), day1.daysInMonth()))
                day4 = QDate(day1.year(), day1.month(), day1.daysInMonth());

            ui.Table->setItem(j++, 0, new QTableWidgetItem(day1.toString("MM/dd/yyyy")));
            ui.Table->setItem(j++, 0, new QTableWidgetItem(day2.toString("MM/dd/yyyy")));
            ui.Table->setItem(j++, 0, new QTableWidgetItem(day3.toString("MM/dd/yyyy")));
            ui.Table->setItem(j++, 0, new QTableWidgetItem(day4.toString("MM/dd/yyyy")));
        }

        // Заполнение сумм
        qint64 SumInMon = rub / 3 + 1,
               SumInWeek = SumInMon / 4 + 1,
               CurSum = 0;
               
        for (int i{ 0 }; i < 12; ++i)
        {
            if (i % 4 != 3)
            {
                qint64 diff = static_cast<qint64>(mersenne()) % 20001 - 10000,
                          SumInCurWeek = SumInWeek + diff;
                CurSum += SumInCurWeek;
                QString str;
                str.setNum(SumInCurWeek);
                ui.Table->setItem(i, 1, new QTableWidgetItem(str));
            }
            else
            {
                qint64 rest = SumInMon - CurSum;
                QString str;
                str.setNum(rest);
                ui.Table->setItem(i, 1, new QTableWidgetItem(str));
                CurSum = 0;
            }
        }
    }

    QString res = ui.Result->text().left(7);
    qint64 sum = GetSum();
    QString num; num.setNum(sum);
    ui.Result->setText(res+num);
}

void MyApp::CheckKv1()
{
    if (ui.kvartal1->isChecked())
    {
        kv1 = true;

        ui.kvartal2->setEnabled(false);
        ui.kvartal3->setEnabled(false);
        ui.kvartal4->setEnabled(false);
    }
    else
    {
        kv1 = false;

        ui.kvartal2->setEnabled(true);
        ui.kvartal3->setEnabled(true);
        ui.kvartal4->setEnabled(true);
    }

    kv2 = false;
    kv3 = false;
    kv4 = false;
}

void MyApp::CheckKv2()
{
    if (ui.kvartal2->isChecked())
    {
        kv2 = true;

        ui.kvartal1->setEnabled(false);
        ui.kvartal3->setEnabled(false);
        ui.kvartal4->setEnabled(false);
    }
    else
    {
        kv2 = false;

        ui.kvartal1->setEnabled(true);
        ui.kvartal3->setEnabled(true);
        ui.kvartal4->setEnabled(true);
    }

    kv1 = false;
    kv3 = false;
    kv4 = false;
}

void MyApp::CheckKv3()
{
    if (ui.kvartal3->isChecked())
    {
        kv3 = true;

        ui.kvartal2->setEnabled(false);
        ui.kvartal1->setEnabled(false);
        ui.kvartal4->setEnabled(false);
    }
    else
    {
        kv3 = false;

        ui.kvartal2->setEnabled(true);
        ui.kvartal1->setEnabled(true);
        ui.kvartal4->setEnabled(true);
    }

    kv2 = false;
    kv1 = false;
    kv4 = false;
}

void MyApp::CheckKv4()
{
    if (ui.kvartal4->isChecked())
    {
        kv4 = true;

        ui.kvartal2->setEnabled(false);
        ui.kvartal3->setEnabled(false);
        ui.kvartal1->setEnabled(false);
    }
    else
    {
        kv4 = false;

        ui.kvartal2->setEnabled(true);
        ui.kvartal3->setEnabled(true);
        ui.kvartal1->setEnabled(true);
    }

    kv2 = false;
    kv3 = false;
    kv1 = false;
}