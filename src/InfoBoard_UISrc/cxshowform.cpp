#include "cxshowform.h"
#include "ui_cxshowform.h"
#include <QPainter>


CxShowForm::CxShowForm(QWidget *parent,size_t   cx,size_t   cy) :
    QWidget(parent),
    ui(new Ui::CxShowForm),
    m_ppre(new QImage(cx,cy,QImage::Format_ARGB32)),
    m_pcurrent(new QImage(cx,cy,QImage::Format_ARGB32)),
    m_offImage(":/Ico/MyIco/_Resource/Ico/png-0652.png"),
    m_cx(cx),m_cy(cy),m_light(180),b_ispowerOn(true)
{
    ui->setupUi(this);
    buildPart();
    setMaximumSize(m_cx,m_cy);
    setMinimumSize(m_cx,m_cy);
}

CxShowForm::~CxShowForm()
{
    delete ui;
}

void            CxShowForm::paintEvent(QPaintEvent *)
{ 
    QPainter paint(this);

    if(b_ispowerOn)
        paint.drawImage(this->rect(),*m_pcurrent);

    else
    {
        QRect rt(192,0,128,128);
        paint.drawImage(rt,m_offImage);
    }
}

void            CxShowForm::pushImageData(const _ImageData &data)
{
    if(data.size()!=m_cy)return ;

    for(int i=0;i<m_cy;++i)
        for(int j=0;j<m_cx;++j)
            m_bufferData[j][i]=data[i][j];

    generateImg();
}

void            CxShowForm::generateImg()
{
    QImage *pImage=new QImage(m_cx,m_cy,QImage::Format_ARGB32);

    QPainter paint(pImage);

    paint.fillRect(pImage->rect(),QColor(127,127,127,255));

    int sum=0;

    for(int i=0;i<m_bufferData.size();++i)
    {
        for(int j=0;j<m_bufferData[i].size();++j)
        {
            ++sum;
            drawPoint(paint,i,j,m_bufferData[i][j]);
        }
    }
    delete m_ppre;
    m_ppre=m_pcurrent;
    m_pcurrent=pImage;

    running(m_mode.m_showmodeIn);
}


void            CxShowForm::drawPoint(QPainter &paint, int x, int y, unsigned char data)
{
    QPen        *pen;
    switch(data)
    {
        case 0:
        pen= new QPen(QColor(0,0,0,m_light));
        break;
        case 1:
        pen=new QPen(QColor(255,0,0,m_light));
        break;
        case 2:
        pen=new QPen(QColor(0,255,0,m_light));
        break;
        case 3:
        pen=new QPen(QColor(255,255,0,m_light));
        break;
        case 4:
        pen=new QPen(QColor(0,0,255,m_light));
        break;
        case 5:
        pen=new QPen(QColor(255,0,255,m_light));
        break;
        case 6:
        pen =new QPen(QColor(0,255,255,m_light));
        break;
        case 7:
        pen =new QPen(QColor(255,255,255,m_light));
        break;
    default:
        pen=new QPen(QColor(0,0,0,m_light));
        break;
    }
    paint.setPen(*pen);
    paint.drawPoint(x,y);
    delete pen;
}

void        CxShowForm::buildPart()
{
    m_bufferData.resize(m_cx);

    for(int i=0;i<m_cx;++i)
        m_bufferData[i].resize(m_cy);

}

void            CxShowForm::slot_setLight(int light)
{
    m_light=light;
    sig_setLight(light);
    update();
}

void          CxShowForm::slot_pushImage(const _ImageData& data,const showMode& mode,int light)
{
     m_light=((float)light/32.0)*255;
     m_mode=mode;
     pushImageData(data);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void        CxShowForm::running(int mode)
{
    switch(mode)
    {
    case 0:
        running(boost::bind(&CxShowForm::directdraw,this));
        break;
    case 1:
        running(boost::bind(&CxShowForm::blinkdraw,this));
        break;
    case 2:
        running(boost::bind(&CxShowForm::rightdraw,this));
        break;
    case 3:
        running(boost::bind(&CxShowForm::leftdraw,this));
        break;
    case 8:
        running(boost::bind(&CxShowForm::updraw,this));
        break;
    case 9:
        running(boost::bind(&CxShowForm::downdraw,this));
        break;
    default:
        running(boost::bind(&CxShowForm::directdraw,this));
        break;
    }
}

void        CxShowForm::directdraw()
{
    update();
}


void        CxShowForm::leftdraw()
{
    QImage *pcurrent=m_pcurrent;

    long  lt=(m_mode.m_staytime*1000 < 1536)?(m_mode.m_staytime*1000-25):1536;
    lt/=128;




    boost::posix_time::milliseconds mill(lt);

    QImage *p=new QImage(512,128,QImage::Format_ARGB32);
    m_pcurrent=p;

    for(int i=0;i<=128;++i)
    {
        QRect rtleft,rtright;
        rtleft.setRect(4*i,0,512-4*i,128);
        rtright.setRect(0,0,4*i,128);
        boost::this_thread::sleep(mill);
        QPainter paint(p);

        paint.drawImage(QRect(0,0,512-4*i,128),*m_ppre,rtleft);
        paint.drawImage(QRect(512-4*i,0,4*i,128),*pcurrent,rtright);

        update();
    }
    delete pcurrent;
}


void        CxShowForm::rightdraw()
{
    QImage *pcurrent=m_pcurrent;

    long  lt=(m_mode.m_staytime*1000 < 1536)?(m_mode.m_staytime*1000-25):1536;
    lt/=128;
    boost::posix_time::milliseconds mill(lt);

    QImage *p=new QImage(512,128,QImage::Format_ARGB32);
    m_pcurrent=p;

    for(int i=0;i<=128;++i)
    {
        QRect rtleft(512-4*i,0,4*i,128),rtright(0,0,512-4*i,128);

        boost::this_thread::sleep(mill);
        QPainter paint(p);

        paint.drawImage(QRect(0,0,4*i,128),*pcurrent,rtleft);
        paint.drawImage(QRect(4*i,0,512-4*i,128),*m_ppre,rtright);

        update();
    }
    delete pcurrent;
}


void             CxShowForm::updraw()
{
    QImage *pcurrent=m_pcurrent;

    long  lt=(m_mode.m_staytime*1000 < 1024)?(m_mode.m_staytime*1000-25):1024;

    lt/=64;

    boost::posix_time::milliseconds mill(lt);

    QImage *p=new QImage(512,128,QImage::Format_ARGB32);
    m_pcurrent=p;

    for(int i=0;i<=64;++i)
    {
        QRect rtup(0,2*i,512,128-2*i),rtdown(0,0,512,2*i);

        boost::this_thread::sleep(mill);
        QPainter paint(p);

        paint.drawImage(QRect(0,0,512,128-2*i),*m_ppre,rtup);
        paint.drawImage(QRect(0,128-2*i,512,2*i),*pcurrent,rtdown);

        update();
    }

    delete pcurrent;
}


void             CxShowForm::downdraw()
{
    QImage *pcurrent=m_pcurrent;

    long  lt=(m_mode.m_staytime*1000 < 1024)?(m_mode.m_staytime*1000-25):1024;

    lt/=64;

    boost::posix_time::milliseconds mill(lt);

    QImage *p=new QImage(512,128,QImage::Format_ARGB32);
    m_pcurrent=p;

    for(int i=0;i<=64;++i)
    {
        QRect rtup(0,128-2*i,512,2*i),rtdown(0,0,512,128-2*i);

        boost::this_thread::sleep(mill);
        QPainter paint(p);

        paint.drawImage(QRect(0,0,512,2*i),*pcurrent,rtup);
        paint.drawImage(QRect(0,2*i,512,128-2*i),*m_ppre,rtdown);

        update();
    }

    delete pcurrent;
}


void             CxShowForm::blinkdraw()
{
    QImage *pcurrent=m_pcurrent;

    long  lt=(m_mode.m_staytime*1000 < 1500)?(m_mode.m_staytime*1000-25):1500;

    lt/=3;

    boost::posix_time::milliseconds mill(lt);

    QImage *p=new QImage(512,128,QImage::Format_ARGB32);

    QPainter paint(p);
    paint.fillRect(p->rect(),Qt::black);
    paint.end();

    QImage *ptemp=p;

    for(int i=0;i<3;++i)
    {
        boost::this_thread::sleep(mill/2);
        m_pcurrent=ptemp;
        update();
        boost::this_thread::sleep(mill/2);
        m_pcurrent=m_ppre;
        update();
    }

    delete p;
    m_pcurrent=pcurrent;
    update();
}


