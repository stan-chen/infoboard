#ifndef CXSERIALSETDIALOG_H
#define CXSERIALSETDIALOG_H

#include <QDialog>

#ifndef Q_MOC_RUN

    #include <boost/asio.hpp>

#endif


namespace Ui {
class CxSerialSetDialog;
}

class CxSerialSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CxSerialSetDialog(QWidget *parent = 0);
    ~CxSerialSetDialog();
    
public:
    size_t          baud_rate;
    size_t          parity;
    size_t          charsize;
    size_t          stopbits;

private slots:
    void on_buttonBox_accepted();

private:



    Ui::CxSerialSetDialog *ui;
};

#endif // CXSERIALSETDIALOG_H
