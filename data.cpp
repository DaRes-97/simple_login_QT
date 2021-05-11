#include "data.h"

/*
 * la funzione crea un file database.csv nella directory
 * radice dell'eseguibile, e se non trova tale file ne crea
 * uno nuovo. dopodichè aggiunge al file i dati dell'user
 * passato in ingresso
 */
void data::save_user(data::user u, QString path)
{
    path.append("/database.csv"); // percorso di salvataggio
    QFile outFile(path);

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << outFile.errorString();
        return;
    }

    QTextStream out(&outFile);
    out << u.username << ",";
    out << u.psw << ",";
    out << u.name << ",";
    out << u.surname << ",";
    out << u.birth_date << ",";
    out << u.gender << "\n";

    outFile.close();
}

/*
 * la funzione legge (se presente) il file csv
 * relativo al dabase e ritorna una Qlist di utenti
 */
QList<data::user> data::load_users(QString path)
{
    QList<data::user> users;
    path.append("/database.csv");
    QFile outFile(path);

    if (!outFile.open(QIODevice::ReadOnly)) {
        qDebug() << outFile.errorString();
        return users;
    }

    QTextStream in(&outFile);
    QStringList wordList;
    QRegExp rx("\\,");
    QString line;

    do{
        line = in.readLine();

        if(!line.isNull()){
            wordList = line.split(rx);

            QString username = wordList[0];
            QString psw = wordList[1];
            QString name = wordList[2];
            QString surname = wordList[3];
            QString birth_date = wordList[4];
            QString gender = wordList[5];

            user u{username,psw,name,surname,birth_date,gender};
            users.append(u);
        }
    }while(!line.isNull());

    outFile.close();

    return users;
}
