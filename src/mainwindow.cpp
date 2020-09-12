#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "./Utils/parse.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);

     opendialog = new OpenDialog();
     connect(opendialog, SIGNAL(pathChanged(QString, QString)), this, SLOT(openImage(QString, QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    opendialog->setModal(true);
    opendialog->exec();
}

void MainWindow::openImage(QString hdrPath, QString datPath)
{
    Utils::raster *r = new Utils::raster(hdrPath.toStdString(), datPath.toStdString());

    QMessageBox msg;
       msg.setText(r->get_metadata("brands"));
}
