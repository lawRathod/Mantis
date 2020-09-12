#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "./Utils/parse.cpp"

#include <boost/numeric/ublas/tensor.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);

     opendialog = new OpenDialog();
     connect(opendialog, SIGNAL(pathChanged(QString, QString)), this, SLOT(openImage(QString, QString)));

     graphics = new QGraphicsScene(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    qWarning()<<"opening dialog";
    opendialog->setModal(true);
    opendialog->exec();
}

void MainWindow::openImage(QString hdrPath, QString datPath)
{
    qWarning()<<"opened";
      Utils::raster *r = new Utils::raster(hdrPath.toStdString(), datPath.toStdString());
      qWarning()<<r->get_metadata("bands");

}

void MainWindow::drawAggregateChart(tensor<double> data, int x, int y, int b) {
    if (imageOption == 1) {
        int r=1, g=20, b=80;
        QImage img(x, y, QImage::Format_RGB888);
          img.fill(QColor(Qt::white).rgb());
          for (int _x = 0; _x < x; ++_x) {
            for (int _y = 0; _y < y; ++_y) {
              img.setPixel(x, y, qRgb(data[r], data[g], data[b]));
            }
          }

          graphics->addPixmap(QPixmap::fromImage(img));
          ui->graphicsView->setScene(graphics);

    }
}


