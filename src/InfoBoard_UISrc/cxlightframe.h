#ifndef CXLIGHTFRAME_H
#define CXLIGHTFRAME_H

#include <QFrame>

namespace Ui {
class CxLightFrame;
}

class CxLightFrame : public QFrame
{
    Q_OBJECT
    
public:
    explicit CxLightFrame(QWidget *parent = 0);
    ~CxLightFrame();
protected:

    void            paintEvent(QPaintEvent *);

signals:
    void                    sig_light();
    void                    sig_extinguish();

private slots:
    void                    slot_light();
    void                    slot_extinguish();
private:
    QPixmap                 m_pixlight;
    QPixmap                 m_pixexting;
    QPixmap                 *m_current;
    Ui::CxLightFrame *ui;
};

#endif // CXLIGHTFRAME_H
