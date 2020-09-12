#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSignalMapper>
#include <QFileDialog>
#include <QMessageBox>

#include "opendialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    OpenDialog *opendialog;

private slots:
    void open();
    void openImage(QString, QString);
};
#endif // MAINWINDOW_H
