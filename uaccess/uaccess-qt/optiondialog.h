#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QMessageBox>
#include <QDialog>
#include <QRegExp>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define MAX_OPT_LEN 130
#define OPT_HEAD_LEN 12

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = 0);
    ~OptionDialog();

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

public slots:
    void option_info(int operation, int id, int alg, QString ip, QString key, int fd);
    void handle_original_option(int oper, int id, int type, QString ip, int key_len, QString key, int fd);

signals:
    void sig_option(int operation, int id, int alg, QString ip, QString key);

private:
    void cmt_option(int oper, int io, int id, int type, char *ip, int key_len, char *key, int fd);

    Ui::OptionDialog *ui;
    QRegExp *re;
    int operation;
    int id;
    int fd;
    int key_len;
};

#endif // OPTIONDIALOG_H
