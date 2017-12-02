#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <unistd.h>
#include "optiondialog.h"
#include <QtDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAction *icon_start, *icon_end;
    QAction *icon_import, *icon_export;
    QStandardItemModel *in_rules, *out_rules;
    OptionDialog *optionDialog;
    int in_id;
    int out_id;
    std::vector<QString> algs;

private slots:
    void on_icon_start_clicked();
    void on_icon_end_clicked();
    void on_icon_import_clicked();
    void on_icon_export_clicked();
    void on_out_add_clicked();
    void on_in_add_clicked();
    void on_out_mod_clicked();
    void on_in_mod_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_2_clicked(const QModelIndex &index);

public slots:
    void option_handle(int operation, int id, int alg, QString ip, QString key);

signals:
    void sig_option_info(int operation, int id, int alg, QString ip, QString key);
};

#endif // MAINWINDOW_H
