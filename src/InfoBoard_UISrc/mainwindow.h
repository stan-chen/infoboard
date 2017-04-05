#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <vector>
#include "cxserialsetdialog.h"
#include "cxsetnetdialog.h"

#ifndef Q_MOC_RUN
    #include "CxComDef.h"
    #include <boost/signals2.hpp>
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
     typedef         std::vector<std::vector< unsigned char>  >        _ImageData;
     typedef         MSG_SIGN::MachineComponent::showMode                showMode;
     typedef         boost::posix_time::ptime                           _Time;
     typedef         boost::signals2::signal<void(bool,bool,unsigned char)>		_SigPowerSet;
     typedef         boost::signals2::signal<void(int ,int )>           _SigLedBug;
     typedef         boost::system::error_code                          ERRCODE;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:

    _SigPowerSet            sig_powerSet;
    _SigLedBug              sig_setLedBug;

signals:

    void        sig_pushImage(const _ImageData&,const showMode&,int);
    void        sig_showTime(const _Time &,int );
    void        sig_screenPower(bool    );
    void        sig_pushInfo(const std::string &);
    void        sig_setledBug(int ,int );
    void        sig_revCommand(char );
    void        sig_handleError(const ERRCODE &);

private slots:

    void        slot_shotTime(const _Time &,int);
    void        slot_pushInfo(const std::string &);
    void        slot_setledBug(int ,int );
    void        slot_revCommand(char );
    void        slot_handleError(const ERRCODE &);

private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_AccSerial_triggered();

    void on_AccNet_triggered();

    void on_AccExit_triggered();

    void on_AccEnvSet_triggered();

    void on_AccSetledBug_triggered();

    void on_action_triggered();

private:

    void        initCommCombo();
    void        initTempLCD();
    void        setSerialOption();
    void        connectAllsignals();

private:
    CxSerialSetDialog       m_serialsetdlg;
    CxSetNetDialog          m_setnetdlg;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
