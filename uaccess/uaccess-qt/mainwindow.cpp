#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    icon_start(new QAction(QIcon(":/start.png"), tr("Start the encrypted system"), this)),
    icon_end(new QAction(QIcon(":/end.png"), tr("Stop the encrypted system"), this)),
    icon_import(new QAction(QIcon(":/import.png"), tr("Import rules"), this)),
    icon_export(new QAction(QIcon(":/export.png"), tr("Export rules"), this)),
    in_rules(new QStandardItemModel),
    out_rules(new QStandardItemModel),
    optionDialog(new OptionDialog(this))
{
    ui->setupUi(this);

    algs.push_back(tr("AES-ECB(128)"));

    icon_end->setEnabled(0);
    ui->mainToolBar->addAction(icon_start);
    ui->mainToolBar->addAction(icon_end);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(icon_import);
    ui->mainToolBar->addAction(icon_export);

    in_rules->setHorizontalHeaderItem(0, new QStandardItem(tr("ID")));
    in_rules->setHorizontalHeaderItem(1, new QStandardItem(tr("Cipher alg")));
    in_rules->setHorizontalHeaderItem(2, new QStandardItem(tr("IP")));
    in_rules->setHorizontalHeaderItem(3, new QStandardItem(tr("key")));

    out_rules->setHorizontalHeaderItem(0, new QStandardItem(tr("ID")));
    out_rules->setHorizontalHeaderItem(1, new QStandardItem(tr("Cipher alg")));
    out_rules->setHorizontalHeaderItem(2, new QStandardItem(tr("IP")));
    out_rules->setHorizontalHeaderItem(3, new QStandardItem(tr("key")));

    ui->tableView->setModel(out_rules);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setColumnWidth(0, 40);
    ui->tableView->setColumnWidth(1, 160);
    ui->tableView->setColumnWidth(2, 160);
    ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView_2->setModel(in_rules);
    ui->tableView_2->verticalHeader()->hide();
    ui->tableView_2->setColumnWidth(0, 40);
    ui->tableView_2->setColumnWidth(1, 160);
    ui->tableView_2->setColumnWidth(2, 160);
    ui->tableView_2->setColumnWidth(3, 200);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->out_mod->setEnabled(0);
    ui->in_mod->setEnabled(0);
    ui->out_del->setEnabled(0);
    ui->in_del->setEnabled(0);

    connect(icon_start, &QAction::triggered, this, &MainWindow::on_icon_start_clicked);
    connect(icon_end, &QAction::triggered, this, &MainWindow::on_icon_end_clicked);
    connect(icon_import, &QAction::triggered, this, &MainWindow::on_icon_import_clicked);
    connect(icon_export, &QAction::triggered, this, &MainWindow::on_icon_export_clicked);

    connect(optionDialog, &OptionDialog::add_option, this, &MainWindow::option_handle);
    connect(this, &MainWindow::sig_option_info, optionDialog, &OptionDialog::option_info);
}

MainWindow::~MainWindow()
{
    delete in_rules;
    delete out_rules;
    delete icon_start;
    delete icon_end;
    delete icon_import;
    delete icon_export;
    delete ui;
    qDebug() << "end";
}

void MainWindow::on_icon_start_clicked()
{
    if (system("insmod /home/test/km/enccom/enccom.ko")) {
        QMessageBox::critical(this, "Error", "Please run as super user!");
    } else {
        icon_start->setEnabled(0);
        icon_end->setEnabled(1);
        ui->statusBar->showMessage(tr("The encrypted system is running!"));
    }
}

void MainWindow::on_icon_end_clicked()
{
    if (system("rmmod enccom")) {
        QMessageBox::critical(this, "Error", "Can not sotp the encrypted module!");
    } else {
        icon_start->setEnabled(1);
        icon_end->setEnabled(0);
        ui->statusBar->showMessage(tr("The encrypted system has been stopped."));
    }
}

void MainWindow::on_icon_import_clicked()
{
    QMessageBox::information(this, "Info", "to be done");
}

void MainWindow::on_icon_export_clicked()
{
    QMessageBox::information(this, "Info", "to be done");
}

void MainWindow::on_out_add_clicked()
{
    emit sig_option_info(0, out_id, 0, tr(""), tr(""));
    optionDialog->exec();
}

void MainWindow::on_in_add_clicked()
{
    emit sig_option_info(1, in_id, 0, tr(""), tr(""));
    optionDialog->exec();
}

void MainWindow::on_out_mod_clicked()
{
    QModelIndex index = this->ui->tableView->currentIndex();
    int row = index.row();

    index = out_rules->index(row, 0);
    QString sid = out_rules->data(index).toString();
    int id = sid.toInt();

    index = out_rules->index(row, 1);
    QString salg = out_rules->data(index).toString();
    int alg = 0;
    while (salg != algs.at(alg))
        alg++;

    index = out_rules->index(row, 2);
    QString ip = out_rules->data(index).toString();

    index = out_rules->index(row, 3);
    QString key = out_rules->data(index).toString();

    emit sig_option_info(2, id, alg, ip, key);
    optionDialog->exec();
}

void MainWindow::on_in_mod_clicked()
{
    QModelIndex index = this->ui->tableView_2->currentIndex();
    int row = index.row();

    index = in_rules->index(row, 0);
    QString sid = in_rules->data(index).toString();
    int id = sid.toInt();

    index = in_rules->index(row, 1);
    QString salg = in_rules->data(index).toString();
    int alg = 0;
    while (salg != algs.at(alg))
        alg++;

    index = in_rules->index(row, 2);
    QString ip = in_rules->data(index).toString();

    index = in_rules->index(row, 3);
    QString key = in_rules->data(index).toString();

    emit sig_option_info(3, id, alg, ip, key);
    optionDialog->exec();
}


void MainWindow::option_handle(int operation, int id, int alg, QString ip, QString key)
{
    QString salg = algs.at(alg);
    if (operation < 2) {
        QList<QStandardItem *> list;
        list.append(new QStandardItem(1));
        list.append(new QStandardItem(salg));
        list.append(new QStandardItem(ip));
        list.append(new QStandardItem(key));
        if (operation == 0)
            out_rules->appendRow(list);
        else
            in_rules->appendRow(list);
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    this->ui->out_mod->setEnabled(1);
    this->ui->out_del->setEnabled(1);
}

void MainWindow::on_tableView_2_clicked(const QModelIndex &index)
{
    this->ui->in_mod->setEnabled(1);
    this->ui->in_del->setEnabled(1);
}
