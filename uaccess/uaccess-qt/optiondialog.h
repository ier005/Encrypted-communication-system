#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

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
    void option_info(int operation, int id, int alg, QString ip, QString key);

signals:
    void add_option(int operation, int id, int alg, QString ip, QString key);
    void mod_option(int operation, int id, int alg, QString ip, QString key);

private:
    Ui::OptionDialog *ui;
    int operation;
    int id;
};

#endif // OPTIONDIALOG_H
