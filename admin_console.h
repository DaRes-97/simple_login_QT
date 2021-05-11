#ifndef ADMIN_CONSOLE_H
#define ADMIN_CONSOLE_H

#include <QMainWindow>
#include <QPainter>
#include <QDate>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include "data.h"

namespace Ui {
class Admin_console;
}

class Admin_console : public QMainWindow
{
    Q_OBJECT

public:

    explicit Admin_console(QWidget *parent = nullptr,QList<data::user> users = QList<data::user> ());

    ~Admin_console();

private:

    Ui::Admin_console *ui;

    // riempie tabella anagrafica
    void fill_table(QList<data::user> users);

    // riempie grafico età
    void fill_age_chart(QList<data::user> users);

    //riempie grafico per genere
    void fill_gender_chart(QList<data::user> users);
};

#endif // ADMIN_CONSOLE_H
