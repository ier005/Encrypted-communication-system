#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    struct stat buf;
    if (stat("/dev/enccom", &buf) != 0) {
        if (system("mknod /dev/enccom c 124 0")) {
            QMessageBox::critical(NULL, "Error", "Can not mknod! Are you root?");
            return 0;
        }
    }

    if (system("insmod /home/test/km/enccom/enccom.ko")) {
        QMessageBox::critical(NULL, "Error", "Can not insmod! Are you root?");
        return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
