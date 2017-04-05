#include "cxserialsetdialog.h"
#include "ui_cxserialsetdialog.h"

using std::cout;
using std::endl;

CxSerialSetDialog::CxSerialSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CxSerialSetDialog)
{
    ui->setupUi(this);

    baud_rate   =ui->comboBox_Baudrate->currentText().toLong();
    parity      =ui->comboBox_Parity->currentText().toInt();
    charsize    =ui->comboBox_Charsize->currentText().toInt();
    stopbits    =ui->comboBox_Stopbits->currentIndex();
}

CxSerialSetDialog::~CxSerialSetDialog()
{
    delete ui;
}

void CxSerialSetDialog::on_buttonBox_accepted()
{
    baud_rate   =ui->comboBox_Baudrate->currentText().toLong();
    parity      =ui->comboBox_Parity->currentText().toInt();
    charsize    =ui->comboBox_Charsize->currentText().toInt();
    stopbits    =ui->comboBox_Stopbits->currentIndex();
}
