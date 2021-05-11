#include "admin_console.h"
#include "ui_admin_console.h"

Admin_console::Admin_console(QWidget *parent, QList<data::user> users) :
    QMainWindow(parent),
    ui(new Ui::Admin_console)
{
    ui->setupUi(this);
    fill_table(users);
    fill_age_chart(users);
    fill_gender_chart(users);
}

/*
 * la funzione riempie il widget QTable con i dati anagrafici
 * degli iscritti
*/
void Admin_console::fill_table(QList<data::user> users){
    QList<data::user>::const_iterator i;
    int cols = ui->usersTable->columnCount();

    for (i = users.constBegin(); i != users.constEnd(); ++i){

        QString anag[]{(*i).name,(*i).surname,(*i).birth_date,(*i).gender,(*i).username};
        ui->usersTable->insertRow(ui->usersTable->rowCount());
        int this_row = ui->usersTable->rowCount()-1;

        for(int c = 0; c < cols; c++){
            QString string = anag[c];
            ui->usersTable->setItem(this_row,c,new QTableWidgetItem(string));
        }
    }
}

/*
 * la funzione aggiunge alla console di admin il grafico a torta relativo
 * alle fasce di genere
 */
void Admin_console::fill_gender_chart(QList<data::user> users){
    int maschi = 0;
    int femmine = 0;
    int non_binari = 0;
    QList<data::user>::const_iterator i;

    //setto i valori per ogni fascia
    for (i = users.constBegin(); i != users.constEnd(); ++i){
        int x = QString::compare((*i).gender, "maschio", Qt::CaseInsensitive);
        int y = QString::compare((*i).gender, "femmina", Qt::CaseInsensitive);
        int z = QString::compare((*i).gender, "non binario", Qt::CaseInsensitive);

        if(x == 0){
            maschi++;
        }
        if(y == 0){
            femmine++;
        }
        if(z == 0){
            non_binari++;
        }
    }

    //setto le fasce del grafico
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
    series->append("maschio", maschi);
    series->append("femmina", femmine);
    series->append("non binario", non_binari);

    //creo il grafico
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("grafico per genere");
    chart->legend()->show();

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //aggiungo il grafico alla console
    ui->gridLayout_2->addWidget(chartView);

}

/*
 * la funzione aggiunge alla console di admin il grafico a torta relativo
 * alle fasce di età
 */
void Admin_console::fill_age_chart(QList<data::user> users){
    int first_serie = 0;
    int second_serie = 0;
    int third_serie = 0;
    int fourth_serie = 0;
    int fifth_serie = 0;
    QList<data::user>::const_iterator i;
    QDate today = QDate::currentDate();

    //setto i valori per ogni fascia
    for (i = users.constBegin(); i != users.constEnd(); ++i){
        QString date_str = (*i).birth_date;
        QDate date = QDate::fromString(date_str,"dd/MM/yyyy");
        qint64 diff = date.daysTo(today) / 365;

        if(diff > 18 && diff <= 26){
            first_serie++;
        } else if(diff > 26 && diff <= 35){
            second_serie++;
        } else if(diff > 35 && diff <= 44){
            third_serie++;
        } else if(diff > 44 && diff <= 53){
            fourth_serie++;
        } else if (diff > 53){
            fifth_serie++;
        }
    }

    //setto le fasce del grafico
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
    series->append("18-26", first_serie);
    series->append("27-35", second_serie);
    series->append("36-44", third_serie);
    series->append("45-53", fourth_serie);
    series->append("54+", fifth_serie);

    //creo il grafico
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("grafico per età");
    chart->legend()->show();

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //aggiungo il grafico alla console
    ui->gridLayout_2->addWidget(chartView);

}

Admin_console::~Admin_console()
{
    delete ui;
}
