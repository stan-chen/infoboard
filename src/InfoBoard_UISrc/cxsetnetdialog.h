#ifndef CXSETNETDIALOG_H
#define CXSETNETDIALOG_H

#include <QDialog>

namespace Ui {
class CxSetNetDialog;
}

class CxSetNetDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CxSetNetDialog(QWidget *parent = 0);
    ~CxSetNetDialog();
    
private:
    Ui::CxSetNetDialog *ui;
};

#endif // CXSETNETDIALOG_H
