#include "cxledbugdialog.h"
#include "ui_cxledbugdialog.h"

CxledBugDialog::CxledBugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CxledBugDialog)
{
    ui->setupUi(this);
}

CxledBugDialog::~CxledBugDialog()
{
    delete ui;
}

void CxledBugDialog::on_buttonBox_accepted()
{
   x=ui->spinBox->value();
   y=ui->spinBox_2->value();
}
