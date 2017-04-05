#include "cxsetnetdialog.h"
#include "ui_cxsetnetdialog.h"

CxSetNetDialog::CxSetNetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CxSetNetDialog)
{
    ui->setupUi(this);
}

CxSetNetDialog::~CxSetNetDialog()
{
    delete ui;
}
