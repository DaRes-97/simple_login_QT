#ifndef DATA_H
#define DATA_H

#include<QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class data
{
public:

    //contenitore dei dati dell'utente
    typedef struct user_struct{
        QString username;
        QString psw;
        QString name;
        QString surname;
        QString birth_date;
        QString gender;
    } user;

    // salva i dati nel file database.csv
    static void save_user(user u,QString path);

    // carica i dati dal file database.csv
    static QList<user> load_users(QString path);
};

#endif // DATA_H
