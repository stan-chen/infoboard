#ifndef CXSETENVDIALOG_H
#define CXSETENVDIALOG_H

#include <QDialog>

namespace Ui {
class CxSetEnvDialog;
}

class CxSetEnvDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CxSetEnvDialog(QWidget *parent = 0);
    ~CxSetEnvDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::CxSetEnvDialog *ui;
};

#endif // CXSETENVDIALOG_H
