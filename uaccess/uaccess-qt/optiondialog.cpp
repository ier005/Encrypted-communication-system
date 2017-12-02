#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog),
    re(new QRegExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b"))
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
            key_len = 16;
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

    if (!re->exactMatch(ip)) {
        QMessageBox::critical(this, "Error", "Please check the format of IP address!");
        return;
    }

    if (key.length() != key_len) {
        QMessageBox::critical(this, "Error", "Please check the length of the key");
        return;
    }

    switch (operation) {
        case 0:
            cmt_option(1, 1, id, alg, ip.toLatin1().data(), key_len, key.toLatin1().data());
            break;
        case 1:
            cmt_option(1, 0, id, alg, ip.toLatin1().data(), key_len, key.toLatin1().data());
            break;
        case 2:
            cmt_option(2, 1, id, alg, ip.toLatin1().data(), key_len, key.toLatin1().data());
            break;
        case 3:
            cmt_option(2, 0, id, alg, ip.toLatin1().data(), key_len, key.toLatin1().data());
            break;
        default:
            break;
    }
    
    emit sig_option(operation, id, alg, ip, key);
    this->accept();
}

void OptionDialog::on_pushButton_2_clicked()
{
    this->close();
}

void OptionDialog::option_info(int operation, int id, int alg, QString ip, QString key, int fd)
{
    this->fd = fd;
    this->operation = operation;
    this->id = id;
    this->ui->comboBox->setCurrentIndex(alg);
    this->ui->lineEdit->setText(ip);
    this->ui->lineEdit_2->setText(key);
}

void OptionDialog::cmt_option(int oper, int io, int id, int type, char *ip, int key_len, char *key)
{
    char opt[MAX_OPT_LEN];
    int len = OPT_HEAD_LEN + key_len;
    unsigned int ipn = inet_addr(ip);

    memcpy(opt, &oper, 1);
    memcpy(opt + 1, &io, 1);
    memcpy(opt + 2, &id, 4);
    memcpy(opt + 6, &type, 1);
    memcpy(opt + 7, &ipn, 4);
    memcpy(opt + 11, &key_len, 1);
    memcpy(opt + 12, key, key_len);

    write(fd, opt, len);
}
