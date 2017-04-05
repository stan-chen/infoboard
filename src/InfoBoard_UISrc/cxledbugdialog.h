#ifndef CXLEDBUGDIALOG_H
#define CXLEDBUGDIALOG_H

#include <QDialog>

namespace Ui {
class CxledBugDialog;
}

class CxledBugDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CxledBugDialog(QWidget *parent = 0);
    ~CxledBugDialog();
    
private slots:
    void on_buttonBox_accepted();

public:

    int x;
    int y;
private:
    Ui::CxledBugDialog *ui;
};

#endif // CXLEDBUGDIALOG_H
