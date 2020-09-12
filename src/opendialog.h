#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class OpenDialog;
}

class OpenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenDialog(QWidget *parent = nullptr);
    ~OpenDialog();

private:
    Ui::OpenDialog *ui;


private slots:
    void openHdr();
    void openDat();
    void submitReturns();

signals:
    void pathChanged(const QString& hdrPath, const QString& datPath);
};

#endif // OPENDIALOG_H
