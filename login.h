#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QString>
#include<QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QTextCursor>
#include <QMessageBox>
#include<iostream>

#include "data.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:

    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_regNameText_textChanged(const QString /*&arg1*/);
    void on_regSurnameText_textChanged(const QString /*&arg1*/);
    void on_regUserText_textChanged(const QString /*&arg1*/);
    void on_regPswText_textChanged(const QString /*&arg1*/);
    void on_logUserText_textChanged(const QString /*&arg1*/);
    void on_recoveryPswButton_clicked();

private:

    Ui::Login *ui;

    // dati di accesso admin
    QString admin_user = "admin@pas.com";
    QString admin_psw = "admin";

    // percorso salvataggio database
    QString path;

    // check esistenza user
    bool user_exist(QList<data::user> users, QString input_user);

    // check correttezza psw
    bool check_psw(QList<data::user> users, QString input_user, QString input_psw);

    // controllo compilazione campi registrazione
    void check_line_edits_reg();

    // pulitura dei campi
    void clean_line_edits();

    // controllo campo email-cellulare
    bool check_user_text();

    // controllo campo psw
    bool check_psw_text();

    // gestione message box
    void info_msg(QString msg);
};
#endif // LOGIN_H
