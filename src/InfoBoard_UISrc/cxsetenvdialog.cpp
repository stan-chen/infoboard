#include "cxsetenvdialog.h"
#include "ui_cxsetenvdialog.h"

#ifndef Q_MOC_RUN
    #include "CxGlobalVariable.h"
#endif

CxSetEnvDialog::CxSetEnvDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CxSetEnvDialog)
{
    ui->setupUi(this);
}

CxSetEnvDialog::~CxSetEnvDialog()
{
    delete ui;
}

void CxSetEnvDialog::on_buttonBox_accepted()
{
    MSG_SIGN::CxGlobalVariable    gl;
    gl.setTmp(ui->spinBox->value());
    gl.setSmoke(ui->spinBox_2->value());
}
