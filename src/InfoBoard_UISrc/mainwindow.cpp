#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include "cxserialsetdialog.h"
#include "cxsetenvdialog.h"
#include "cxledbugdialog.h"

#ifndef Q_MOC_RUN
    #include "CxGlobalVariable.h"
    #include <Windows.h>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectAllsignals();
    initCommCombo();
    initTempLCD();
}

void        MainWindow::initTempLCD()
{
   MSG_SIGN::CxGlobalVariable gl;
   ui->lcdNumber->display(gl.getTmp());
}

void        MainWindow::connectAllsignals()
{
    connect(this,SIGNAL(sig_pushImage(_ImageData,showMode,int)),ui->shorForm,SLOT(slot_pushImage(_ImageData,showMode,int)));
    connect(this,SIGNAL(sig_showTime(const _Time&,int)),this,SLOT(slot_shotTime(const _Time&,int)));
    connect(this,SIGNAL(sig_screenPower(bool)),ui->shorForm,SLOT(slot_setPower(bool)));
    connect(this,SIGNAL(sig_pushInfo(std::string)),this,SLOT(slot_pushInfo(std::string)));
    connect(this,SIGNAL(sig_setledBug(int,int)),this,SLOT(slot_setledBug(int,int)));
    connect(this,SIGNAL(sig_revCommand(char)),this,SLOT(slot_revCommand(char)));
    connect(this,SIGNAL(sig_handleError(ERRCODE)),this,SLOT(slot_handleError(ERRCODE)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void        MainWindow::initCommCombo()
{
    QString str;

    if(!ui->comboBox->isEnabled())
        return ;

    ui->comboBox->clear();

    for(int i=0;i<30;++i)
    {
        str.sprintf("COM%d",i);
        HANDLE  handle=CreateFileA(str.toStdString().c_str(),GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
        if(handle==INVALID_HANDLE_VALUE)
            continue;
        else
        {
            ui->comboBox->addItem(str);
            CloseHandle(handle);
        }
    }
}

/* connect the comm */
void MainWindow::on_pushButton_3_clicked()
{
    MSG_SIGN::CxGlobalVariable  gl;
    QString str=ui->comboBox->currentText();

    if(gl.getPort()->isOpen())
        gl.getPort()->close();

    if(gl.getPort()->open(str.toStdString()))
    {
        setSerialOption();
        ui->frame->sig_light();
        ui->comboBox->setEnabled(false);
    }
}

/* break the comm */
void MainWindow::on_pushButton_4_clicked()
{
    MSG_SIGN::CxGlobalVariable  gl;
    gl.getPort()->close();
    ui->frame->sig_extinguish();
    ui->comboBox->setEnabled(true);
    sig_pushInfo("Break success..");
}


void        MainWindow::setSerialOption()
{
    MSG_SIGN::CxGlobalVariable  gl;
    CxSerialPort::t_parity              parity=(CxSerialPort::t_parity)m_serialsetdlg.parity;
    CxSerialPort::t_stop_bits           stop=(CxSerialPort::t_stop_bits)m_serialsetdlg.stopbits;
    CxSerialPort::t_baud_rate           rate=(CxSerialPort::t_baud_rate)m_serialsetdlg.baud_rate;
    CxSerialPort::t_char_size           charsize=(CxSerialPort::t_char_size)m_serialsetdlg.charsize;

    gl.getPort()->setOption(parity);
    gl.getPort()->setOption(rate);
    gl.getPort()->setOption(charsize);
    gl.getPort()->setOption(stop);

    sig_pushInfo("Connect success....");
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/*  the  button  function     */
void MainWindow::on_pushButton_8_clicked()
{
    sig_screenPower(true);
    sig_powerSet(true,false,0);
}

void MainWindow::on_pushButton_7_clicked()
{
    sig_screenPower(false);
    sig_powerSet(false,false,0);
}


void MainWindow::on_pushButton_5_clicked()
{
    m_serialsetdlg.exec();
}
////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////
/*      the slot function   */
void    MainWindow::slot_shotTime(const _Time &t,int index)
{
    switch(index)

    {
        case 0:
        {
            QDate   _d(t.date().year(),t.date().month(),t.date().day());
            QTime   _t(t.time_of_day().hours(),t.time_of_day().minutes(),t.time_of_day().seconds(),t.date().day_of_week());
            ui->dateTimeEdit->setDate(_d);
            ui->dateTimeEdit->setTime(_t);
            break;
        }
        case 1:
        {
            QTime   _t(t.time_of_day().hours(),t.time_of_day().minutes(),t.time_of_day().seconds(),t.date().day_of_week());
            ui->timeEdit->setTime(_t);
        }
        case 2:
        {
            QTime   _t(t.time_of_day().hours(),t.time_of_day().minutes(),t.time_of_day().seconds(),t.date().day_of_week());
            ui->timeEdit_2->setTime(_t);
        }
    }
}



void MainWindow::slot_pushInfo(const std::string &str)
{
    ui->listWidget->insertItem(0,QString::fromLocal8Bit(str.c_str()));
}

void    MainWindow::slot_setledBug(int x, int y)
{
    sig_setLedBug(x,y);
}


void    MainWindow::slot_revCommand(char c)
{
    QString str;
    str.sprintf("Now rev a %c command",c);
    sig_pushInfo(str.toStdString());
}


void    MainWindow::slot_handleError(const ERRCODE &ec)
{
    QString str=QString::fromLocal8Bit(ec.message().c_str());
    ui->listWidget->insertItem(0,str);
}
/*      the slot function   */
////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*  menu  acction  */
void MainWindow::on_AccSerial_triggered()
{
    m_serialsetdlg.exec();
}

void MainWindow::on_AccNet_triggered()
{
    m_setnetdlg.exec();
}

void MainWindow::on_AccExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_AccEnvSet_triggered()
{
    CxSetEnvDialog dlg;
    dlg.exec();
    initTempLCD();
}

void MainWindow::on_AccSetledBug_triggered()
{
    CxledBugDialog dlg;
    dlg.exec();
    sig_setLedBug(dlg.x,dlg.y);
}

void MainWindow::on_action_triggered()
{
   initCommCombo();
}

/*          */
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

