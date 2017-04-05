#include "mainwindow.h"
#include <QApplication>
#include "cxshowform.h"


#ifndef Q_MOC_RUN
    #include "CxComBuilder.hpp"
    #include "CxExplainThread.h"
    #include "CxRevThread.h"
    #include "CxGlobalVariable.h"
    #include "CxComPlay.hpp"
    #include "CxComTime.hpp"
    #include "CxComDetect.hpp"
    #include "CxComExport.hpp"
    #include "CxComImport.hpp"
#endif



using namespace MSG_SIGN;
using namespace MSG_SIGN::MachineComponent;


/*      Init    Machine  code      */
void        InitMachine(MainWindow &w)
{
    CxGlobalVariable    gl;

    CxComBuilder_1      *pbuilder=new CxComBuilder_1;
    CxComBuilder_1      &builder=*pbuilder;
    gl.fromBuilder(pbuilder,512,128);



    /*  Init  the  outlink  signals */
    builder.getComPlay()->sig_show.connect(boost::bind(&MainWindow::sig_pushImage,boost::ref(w),_1,_2,_3));

    builder.getComTime()->sig_showTime.connect(boost::bind(&MainWindow::sig_showTime,boost::ref(w),_1,0));
    builder.getComTime()->sig_showOpenTime.connect(boost::bind(&MainWindow::sig_showTime,boost::ref(w),_1,1));
    builder.getComTime()->sig_showCloseTime.connect(boost::bind(&MainWindow::sig_showTime,boost::ref(w),_1,2));

    builder.getComPower()->sig_powerOn.connect(boost::bind(&MainWindow::sig_screenPower,boost::ref(w),true));
    builder.getComPower()->sig_powerOff.connect(boost::bind(&MainWindow::sig_screenPower,boost::ref(w),false));
    builder.getComExport()->sig_pushStr.connect(boost::bind(&MainWindow::sig_pushInfo,boost::ref(w),_1));


    /*  Init  the   Inlink  signals */
    w.sig_powerSet.connect(builder.getComPower()->sig_setPower);
    w.sig_setLedBug.connect(boost::bind(&CxComImport::setBulbBug,builder.getComImport(),_1,_2));
}



////////////////////////////////////////////////////
/*      Init  the  tuple  type     */
void        InitType()
{
    typedef boost::posix_time::ptime                        _Time;
    typedef std::vector <std::vector<unsigned char>  >      _ImageData;
    typedef boost::system::error_code                       ERRCODE;
    qRegisterMetaType<showMode>("showMode");
    qRegisterMetaType<_ImageData>("_ImageData");
    qRegisterMetaType<_Time>("_Time");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::string>("string");
    qRegisterMetaType<ERRCODE>("ERRCODE");
}




/*  Main function Initialize */
int main(int argc, char *argv[])
{
    InitType();

    QApplication a(argc, argv);

    MainWindow w;
    /* The  global  variable */
    CxGlobalVariable    gl;

    InitMachine(boost::ref(w));

    /* Init  the  thread handle */
    CxMachine           *machine=gl.getMachine();
    CxRevThread         *revT=new CxRevThread(gl.getPort());
    CxExplainThread     *exT=new CxExplainThread(machine,&revT->getBuffer());

    /* Init  the output  signals*/
    exT->sig_msg.connect(boost::bind(&MainWindow::sig_revCommand,boost::ref(w),_1));
    gl.getPort()->sig_errorsignal.connect(boost::bind(&MainWindow::sig_handleError,boost::ref(w),_1));

    w.show();
    int result = a.exec();

    /*  destory   all   handle */
    gl.getPort()->close();

    delete revT;
    delete exT;
    gl.destoryAll();

    return result;
}
