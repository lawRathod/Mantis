#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./Utils/parse.cpp"
#include <sstream>
#include<algorithm>

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
      qWarning()<<r->get_metadata("bands").c_str();
      int bands, samples, lines;
      std::stringstream* ss= new std::stringstream(r->get_metadata("bands"));
      *ss>>bands;
      ss= new std::stringstream(r->get_metadata("samples"));
      *ss>>samples;
      ss= new std::stringstream(r->get_metadata("lines"));
      *ss>>lines;

      void* data = r->read_bin();
      qDebug()<<bands<<" "<<samples<<" "<<lines<<"\n";
      if(r->get_metadata("data type").compare("2\r") == 0){
          MainWindow::drawAggregateChart<int16_t>(data, bands, samples, lines);
      } else if(r->get_metadata("data type").compare("1\r") == 0){
          MainWindow::drawAggregateChart<uint8_t>(data, bands, samples, lines);
      }


}

template <typename T>
void MainWindow::drawAggregateChart(void* data, int bands, int samples, int lines) {
    if (imageOption == 1) {
        int r=21, g=18, b=7;
//        int r=21, g=2, b=100;
        //        *(static_cast<T*>(data) + i*sample_num*lines + j*lines + k)

        T rr = *(static_cast<T*>(data) + r*samples*lines + 1*lines + 1);
        T gg = *(static_cast<T*>(data) + g*samples*lines + 1*lines + 1);
        T bb = *(static_cast<T*>(data) + b*samples*lines + 1*lines + 1);
        T amin =rr ,amax=rr;
        for (int _x = 0; _x < samples; ++_x) {
          for (int _y = 0; _y < lines; ++_y) {
              rr = *(static_cast<T*>(data) + r*samples*lines + _x*lines + _y);
              gg = *(static_cast<T*>(data) + g*samples*lines + _x*lines + _y);
              bb = *(static_cast<T*>(data) + b*samples*lines + _x*lines + _y);
              if(rr >= gg && rr >= bb)
                  if (rr >= amax) amax = rr;
              if(gg>= rr && gg >= bb)
                   if (rr >= amax) amax = gg;
              if(bb >= rr && bb >= gg)
                   if (rr >= amax) amax = bb;
              if(rr <= gg && rr <= bb)
                  if (rr <= amin) amin = rr;
              if(gg<= rr && gg <= bb)
                  if (rr <= amin) amin = gg;
              if(bb <= rr && bb <= gg)
                  if (rr <= amin) amin = bb;

          }
        }
        qDebug()<<amax << amin;
        QImage img(samples, lines, QImage::Format_RGB888);
          img.fill(QColor(Qt::white).rgb());
          for (int _x = 0; _x < samples; ++_x) {
            for (int _y = 0; _y < lines; ++_y) {
                rr = (((*(static_cast<T*>(data) + r*samples*lines + _x*lines + _y)) - amin)/amax)*255;
                bb = (((*(static_cast<T*>(data) + g*samples*lines + _x*lines + _y)) - amin)/amax)*255;
                gg = (((*(static_cast<T*>(data) + b*samples*lines + _x*lines + _y)) - amin)/amax)*255;
//                qDebug()<<rr<< gg<< bb;
                img.setPixel(_x, _y, qRgb(rr,gg,bb));
            }
          }

          graphics->addPixmap(QPixmap::fromImage(img));
          ui->graphicsView->setScene(graphics);
    }
}


