#include "login.h"
#include "ui_login.h"
#include "admin_console.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
    , path("")
{
    ui->setupUi(this);

    //percorso di salvataggio database
    path = qApp->applicationDirPath();

    // registrazione solo per maggiorenni
    QDate today = QDate::currentDate();
    QDate default_date = QDate(2000,1,1);
    QDate max_valid_date = today.addYears(-18);
    ui->regBirthDate->setMaximumDate(max_valid_date);
    ui->regBirthDate->setDisplayFormat("dd/MM/yyyy");
    ui->regBirthDate->setDate(default_date);
}

Login::~Login()
{
    delete ui;
}

/*
 * una volta cliccato su login, il programma verifica
 * la correttezza dei dati inseriti, e verifica anche
 * se siano stati inseriti o meno i dati dell'account
 * di amministrazione
 */
void Login::on_loginButton_clicked()
{
    QString input_user = ui->logUserText->text().trimmed();
    QString input_psw = ui->logPswText->text().trimmed();
    QList<data::user> users = data::load_users(path);

    QString msg = "";

    int usr = QString::compare(input_user, admin_user, Qt::CaseInsensitive);
    int psw = QString::compare(input_psw, admin_psw, Qt::CaseSensitive);

    if(usr == 0){

        if(psw == 0){

            // accesso admin
            Admin_console *console = new Admin_console(this,users);
            (*console).setWindowIcon(QIcon(":/resources/console_icon.png"));
            console->setAttribute(Qt::WA_DeleteOnClose);
            console->show();

            clean_line_edits();
            return;
        } else {

            msg = "psw admin non corretta";
        }
    } else if(user_exist(users,input_user)){

        // accesso user
        msg = (check_psw(users,input_user,input_psw)) ? "login effettuato" : "psw errata";

    } else{

        msg = "user non trovato";
    }

    info_msg(msg);
    clean_line_edits();
}

/*
 * la funzione verifica l'esistenza di un dato user tra quelli
 * salvati nel dayabase
 */
bool Login::user_exist(QList<data::user> users, QString input_user)
{
    QList<data::user>::const_iterator i;

    for (i = users.constBegin(); i != users.constEnd(); ++i){

        int x = QString::compare((*i).username, input_user, Qt::CaseInsensitive);

        if(x == 0)
            return true;
    }

    return false;
}

/*
 * la funzione prende in ingresso un user e una psw e verifica
 * che la psw sia effettivamente quella corrispondente al dato
 * user
 */
bool Login::check_psw(QList<data::user> users, QString input_user, QString input_psw)
{
    QList<data::user>::const_iterator i;

    for (i = users.constBegin(); i != users.constEnd(); ++i){

        int x = QString::compare((*i).username, input_user, Qt::CaseInsensitive);
        int y = QString::compare((*i).psw, input_psw, Qt::CaseSensitive);

        if(x == 0){
            return (y == 0);
        }

    }

    return false;
}

/*
 * una volta premuto il tasto di registrazione,
 * la funzione verifica che l'user non sia già
 * stato inserito nel database, e procede al
 * salvataggio dei dati
 */
void Login::on_registerButton_clicked()
{
    QString username = ui->regUserText->text().trimmed();
    QString psw = ui->regPswText->text().trimmed();
    QString name = ui->regNameText->text().trimmed();
    QString surname = ui->regSurnameText->text().trimmed();
    QString birth_date = ui->regBirthDate->text().trimmed();
    QString gender = ui->regGender->currentText().trimmed();

    QString msg = "";
    int equals_admin = QString::compare(username, admin_user, Qt::CaseInsensitive);
    QList<data::user> users = data::load_users(path);

    if(!user_exist(users,username) && equals_admin != 0){

        data::user u{username,psw,name,surname,birth_date,gender};
        data::save_user(u,path);
        msg = "utente registrato correttamente";

    } else{

        msg = "utente già registrato";
    }

    info_msg(msg);
    clean_line_edits();
}

/*
 * la funzione abilita il tasto di registrazione se
 * tutti i campi sono correttamente compilati
 */
void Login::check_line_edits_reg()
{
    bool ok(true);
    ok &= !ui->regUserText->text().isEmpty();
    ok &= !ui->regPswText->text().isEmpty();
    ok &= !ui->regNameText->text().isEmpty();
    ok &= !ui->regSurnameText->text().isEmpty();
    ok &= check_user_text();
    ok &= check_psw_text();

    ui->registerButton->setEnabled(ok);
}

/*
 * la funzione svuota i campi del form
 * di registrazione
 */
void Login::clean_line_edits()
{
    ui->logUserText->setText("");
    ui->logPswText->setText("");
    ui->regUserText->setText("");
    ui->regPswText->setText("");
    ui->regNameText->setText("");
    ui->regSurnameText->setText("");
    ui->regGender->setCurrentIndex(0);

    QDate default_date(2000, 1, 1);
    ui->regBirthDate->setDate(default_date);

    ui->regUserText->setStyleSheet("QLineEdit { background: rgb(255, 255, 255);"
"                                   selection-background-color: rgb(0, 0, 255); }");
    ui->regPswText->setStyleSheet("QLineEdit { background: rgb(255, 255, 255);"
"                                   selection-background-color: rgb(0, 0, 255); }");
}

/*
 * la funzione verifica che il campo username sia
 * correttamente compilato (ossia contenga un indirizzo
 * mail o un numero di cellulare
 */
bool Login::check_user_text()
{
    QString str = ui->regUserText->text();
    QRegExp re_phone("^[0-9]{10}$");
    QRegExp re_mail("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    re_mail.setCaseSensitivity(Qt::CaseInsensitive);
    re_mail.setPatternSyntax(QRegExp::RegExp);

    bool check_phone = re_phone.exactMatch(str);
    bool check_mail = re_mail.exactMatch(str);

    return (check_phone || check_mail);
}

/*
 * la funzione verifica che il campo password sia
 * correttamente compilato (ossia contenga una password
 * con più di 4 caratteri
 */
bool Login::check_psw_text(){
    QString str = ui->regPswText->text();

    return(str.size() >= 5);
}

// gestione messaggi di info
void Login::info_msg(QString msg){
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setWindowTitle("Info Message");
    msgBox.exec();
}

//slot controllo nome registrazione
void Login::on_regNameText_textChanged(const QString /*&arg1*/)
{
    check_line_edits_reg();
}

//slot controllo cognome registrazione
void Login::on_regSurnameText_textChanged(const QString /*&arg1*/)
{
    check_line_edits_reg();
}

/*
 * slot controllo user registrazione
 * per segnalare la corretta compilazione del campo,
 * il colore di background passa da rosso a verde
 */
void Login::on_regUserText_textChanged(const QString /*&arg1*/)
{
    check_line_edits_reg();

    if(!check_user_text()){
        ui->regUserText->setStyleSheet("QLineEdit { background: rgb(255, 150, 150);"
"                                       selection-background-color: rgb(0, 0, 255); }");
    } else{
        ui->regUserText->setStyleSheet("QLineEdit { background: rgb(150, 255, 150);"
"                                       selection-background-color: rgb(0, 0, 255); }");
    }
}

/*
 * slot controllo psw registrazione
 * per segnalare la corretta compilazione del campo,
 * il colore di background passa da rosso a verde
 */
void Login::on_regPswText_textChanged(const QString /*&arg1*/)
{
    check_line_edits_reg();

    if(!check_psw_text()){
        ui->regPswText->setStyleSheet("QLineEdit { background: rgb(255, 150, 150);"
"                                      selection-background-color: rgb(0, 0, 255); }");
    } else{
        ui->regPswText->setStyleSheet("QLineEdit { background: rgb(150, 255, 150);"
"                                      selection-background-color: rgb(0, 0, 255); }");
    }
}

/*
 * lo slot abilita il tasto di reset psw se il campo
 * user login risulta compilato
 */
void Login::on_logUserText_textChanged(const QString /*&arg1*/)
{
    bool ok = !ui->logUserText->text().isEmpty();
    ui->recoveryPswButton->setEnabled(ok);
}

/*
 * una volta premuto il tasto di reset psw, la funzione
 * controlla se nel database sia presente l'user inserito
 * nel campo di login, e in caso affermativo manda una dialog
 * che conferma l'invio del messaggio di recupero psw
 */
void Login::on_recoveryPswButton_clicked()
{
    QString username = ui->logUserText->text().trimmed();

    QString msg = "";
    QList<data::user> users = data::load_users(path);
    if(!user_exist(users,username)){
        msg = "utente non trovato";
    } else{
        msg = "inviato link per recupero psw";
    }

    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();

    clean_line_edits();
}
