#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/login_icon.png"));
    Login w;
    w.show();
    return a.exec();
}
