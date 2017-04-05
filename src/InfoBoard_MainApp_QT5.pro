#-------------------------------------------------
#
# Project created by QtCreator 2013-04-07T14:37:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InfoBoard
TEMPLATE = app


SOURCES += InfoBoard_UISrc/main.cpp\
        InfoBoard_UISrc/mainwindow.cpp \
    InfoBoard_UISrc/cxshowform.cpp \
    InfoBoard_MainSrc/CxTransaction.cpp \
    InfoBoard_MainSrc/CxRevThread.cpp \
    InfoBoard_MainSrc/CxProtocolHandle.cpp \
    InfoBoard_MainSrc/CxGlobalVariable.cpp \
    InfoBoard_MainSrc/CxExplainThread.cpp \
    InfoBoard_MainSrc/CxCommandHandle.cpp \
    InfoBoard_MainSrc/CxMachineCom/CxComScene.cpp \
    InfoBoard_MainSrc/CxMachineCom/CxComPlay.cpp \
    InfoBoard_MainSrc/CxMachineCom/CxComImport.cpp \
    InfoBoard_MainSrc/CxMachineCom/CxComExport.cpp \
    InfoBoard_MainSrc/CxMachineCom/CxComDetect.cpp \
    InfoBoard_MainSrc/CxPort/CxTcpService.cpp \
    InfoBoard_MainSrc/CxPort/CxTcpClient.cpp \
    InfoBoard_MainSrc/CxPort/CxSerialPort.cpp \
    InfoBoard_MainSrc/CxPort/CxIoService.cpp \
    InfoBoard_UISrc/cxserialsetdialog.cpp \
    InfoBoard_UISrc/cxsetnetdialog.cpp \
    InfoBoard_UISrc/cxsetenvdialog.cpp \
    InfoBoard_UISrc/cxledbugdialog.cpp \
    InfoBoard_MainSrc/CxMachineCom/CxMachine.cpp \
    InfoBoard_MainSrc/CxMachineCom/CxComBuilder.cpp \
    InfoBoard_UISrc/cxlightframe.cpp

HEADERS  += InfoBoard_UISrc/mainwindow.h \
    InfoBoard_UISrc/cxshowform.h \
    InfoBoard_MainSrc/CxType.h \
    InfoBoard_MainSrc/CxTransaction.h \
    InfoBoard_MainSrc/CxThread.hpp \
    InfoBoard_MainSrc/CxRevThread.h \
    InfoBoard_MainSrc/CxProtocolHandle.h \
    InfoBoard_MainSrc/CxMachineCom/CxMachine.hpp \
    InfoBoard_MainSrc/CxGlobalVariable.h \
    InfoBoard_MainSrc/CxExplainThread.h \
    InfoBoard_MainSrc/CxMachineCom/CxComTime.hpp \
    InfoBoard_MainSrc/CxMachineCom/CxComStorage.hpp \
    InfoBoard_MainSrc/CxMachineCom/CxComScene.hpp \
    InfoBoard_MainSrc/CxMachineCom/CxComPlay.hpp \
    InfoBoard_MainSrc/CxMachineCom/CxComImport.hpp \
    InfoBoard_MainSrc/CxMachineCom/CxComExport.hpp \
    InfoBoard_MainSrc/CxMachineCom/CxComDetect.hpp \
    InfoBoard_MainSrc/CxMachineCom/CxComDef.h \
    InfoBoard_MainSrc/CxMachineCom/CxComBuilder.hpp \
    InfoBoard_MainSrc/CxPort/CxTcpService.h \
    InfoBoard_MainSrc/CxPort/CxTcpClient.h \
    InfoBoard_MainSrc/CxPort/CxTcp.h \
    InfoBoard_MainSrc/CxPort/CxSerialPort.h \
    InfoBoard_MainSrc/CxPort/CxIoSevice.h \
    InfoBoard_MainSrc/CxPort/CxClientList.hpp \
    InfoBoard_MainSrc/CxPort/CxBuffer.hpp \
    InfoBoard_UISrc/cxserialsetdialog.h \
    InfoBoard_UISrc/cxsetnetdialog.h \
    InfoBoard_UISrc/cxsetenvdialog.h \
    InfoBoard_UISrc/cxledbugdialog.h \
    InfoBoard_UISrc/cxlightframe.h

FORMS    += InfoBoard_UISrc/mainwindow.ui \
    InfoBoard_UISrc/cxshowform.ui \
    InfoBoard_UISrc/cxserialsetdialog.ui \
    InfoBoard_UISrc/cxsetnetdialog.ui \
    InfoBoard_UISrc/cxsetenvdialog.ui \
    InfoBoard_UISrc/cxledbugdialog.ui \
    InfoBoard_UISrc/cxlightframe.ui

	
LIBS +=  -L$(BOOST_LIB)

DEPENDPATH  +=  $(BOOST_INC)
INCLUDEPATH +=  $(BOOST_INC)

DEPENDPATH  += ./InfoBoard_MainSrc
INCLUDEPATH += ./InfoBoard_MainSrc

DEPENDPATH  += ./InfoBoard_MainSrc/CxMachineCom
INCLUDEPATH += ./InfoBoard_MainSrc/CxMachineCom

RESOURCES += \
    InfoBoard_UISrc/CxResource.qrc
