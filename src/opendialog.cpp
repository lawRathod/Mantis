#include "opendialog.h"
#include "ui_opendialog.h"

OpenDialog::OpenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDialog)
{
    ui->setupUi(this);
}

OpenDialog::~OpenDialog()
{
    delete ui;
}

void OpenDialog::openHdr() {
    QString fileName = QFileDialog::getOpenFileName(
           this,
           tr("Open File"),
           "",
           "header file (*.hdr)"
           );
        if (!fileName.isEmpty()) {
            this->ui->lineEdit_2->setText(fileName);
        }
}

void OpenDialog::openDat() {
    QString fileName = QFileDialog::getOpenFileName(
           this,
           tr("Open File"),
           "",
           "data file (*.dat)"
           );
        if (!fileName.isEmpty()) {
            this->ui->lineEdit->setText(fileName);
        }
}

void OpenDialog::submitReturns() {
    emit pathChanged(this->ui->lineEdit_2->text(), this->ui->lineEdit->text());
}
