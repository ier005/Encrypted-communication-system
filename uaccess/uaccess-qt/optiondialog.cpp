#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    QStringList slist;
    slist.append(tr("AES-ECB(128)"));
    ui->comboBox->addItems(slist);

}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
        case 0:
            ui->label_3->setText(tr("Key(16 Bytes)"));
            break;
        default:
            break;
    }
}

void OptionDialog::on_pushButton_clicked()
{
    int alg = this->ui->comboBox->currentIndex();
    QString ip = this->ui->lineEdit->text();
    QString key = this->ui->lineEdit_2->text();
    
    emit add_option(operation, id, alg, ip, key);
    this->accept();
}

void OptionDialog::on_pushButton_2_clicked()
{
    this->close();
}

void OptionDialog::option_info(int operation, int id, int alg, QString ip, QString key)
{
    this->operation = operation;
    this->id = id;
    this->ui->comboBox->setCurrentIndex(alg);
    this->ui->lineEdit->setText(ip);
    this->ui->lineEdit_2->setText(key);
}
