#ifndef CXSHOWFORM_H
#define CXSHOWFORM_H

#include <QWidget>
#include <vector>
#include <QPainter>
#include <QImage>

#ifndef Q_MOC_RUN
    #include "CxComDef.h"
    #include <boost/thread.hpp>
#endif

namespace Ui {
class CxShowForm;
}

using std::vector;
using MSG_SIGN::MachineComponent::showMode;

class CxShowForm : public QWidget
{
    Q_OBJECT
    
protected:

    typedef         vector<vector< unsigned char>  >        _ImageData;
    typedef         MSG_SIGN::MachineComponent::showMode    showMode;
public:

    CxShowForm(QWidget *parent = 0,size_t  cx=512,size_t   cy=128);
    ~CxShowForm();

public:

    void            setImageSize(int cx,int cy){m_cx=cx;m_cy=cy;}
    void            pushImageData(const _ImageData &);

protected:

    void            paintEvent(QPaintEvent *);
    void            generateImg();
    void            drawPoint(QPainter&,int x,int y,unsigned char );
    void            buildPart();

private:

    Ui::CxShowForm *ui;

private:
    size_t              m_cx;
    size_t              m_cy;

    volatile        unsigned char           m_light;

signals:

    void            sig_setLight(int );

private slots:

    void            slot_pushImage(const _ImageData&,const showMode&,int);

private slots:

    void            slot_setLight(int );

    void            slot_setPower(bool  bpower){b_ispowerOn=bpower;update();}
protected:
    void             running(boost::function<void()> func)
    {
        boost::thread(func).detach();
    }

    void             running(int );

    void             leftdraw();
    void             rightdraw();
    void             directdraw();
    void             updraw();
    void             downdraw();
    void             blinkdraw();


private:

    QImage              *m_pcurrent;
    QImage              *m_ppre;
    QImage              m_offImage;


    showMode            m_mode;
private:

    bool                b_ispowerOn;
    _ImageData          m_bufferData;

};

#endif // CXSHOWFORM_H
