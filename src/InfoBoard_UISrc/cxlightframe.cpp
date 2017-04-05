#include "cxlightframe.h"
#include <QPainter>
#include "ui_cxlightframe.h"

CxLightFrame::CxLightFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CxLightFrame),m_pixlight(":/Ico/MyIco/_Resource/Ico/png-0044.png"),
    m_pixexting(":/Ico/MyIco/_Resource/Ico/png-0652.png")
{
    ui->setupUi(this);
    m_current=&m_pixexting;
    connect(this,SIGNAL(sig_light()),this,SLOT(slot_light()));
    connect(this,SIGNAL(sig_extinguish()),this,SLOT(slot_extinguish()));
}

CxLightFrame::~CxLightFrame()
{
    delete ui;
}

void        CxLightFrame::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.fillRect(this->rect(),Qt::yellow);
    paint.drawPixmap(this->rect(),*m_current);
}

void                    CxLightFrame::slot_light()
{
    m_current=&m_pixlight;
    update();
}
void                    CxLightFrame::slot_extinguish()
{
    m_current=&m_pixexting;
    update();
}
