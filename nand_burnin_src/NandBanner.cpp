#include "NandBanner.h"

NandBanner::NandBanner(QWidget *parent):
    QWidget(parent)
{
    painter = new QPainter(this);

    badblock_list.clear();
    percent_list.clear();
    startPos = 0;
    endPos = 0;
    length = 0;
}

NandBanner::~NandBanner()
{
    delete painter;
}

int NandBanner::setRange(int startPercent, int endPercent)
{
    startPos = (width()*startPercent) / 1000;
    endPos = (width()*endPercent) / 1000;
    length = endPos - startPos;
    return 0;
}

void NandBanner::paintEvent(QPaintEvent *event)
{
    int i , percent;
    int start = 0, end = width();
    painter->begin(this);
    QPen pen;
    pen.setColor(Qt::red);
    painter->setPen(pen);
    // 画一个实心矩形
    QBrush bruch(Qt::SolidPattern);     // 画刷
    bruch.setColor(Qt::green);
    painter->setBrush(bruch);            // 设置画刷
    painter->drawRect(QRect(1, 1, width()-4, height()-2));
    //painter->fillRect(QRect(1, 1, 100, 100), bruch);

//    QList<int>::iterator i;

    for(i=0; i<badblock_list.size(); ++i)
    {
        percent = (int)badblock_list.at(i);
        painter->drawLine((width()*percent)/1000+1, 1, (width()*percent)/1000+1, height()-2);
    }
    badblock_list.clear();

    /* 擦写进度条 */
    for(i=0; i<percent_list.size(); ++i)
    {
        percent = (int)percent_list.at(i);
        if(percent==0)
            percent = (int)percent_list.at(i?i-1:i);
        //painter->fillRect(2, 2, (width()*percent/100)+1, height()-3, Qt::blue);
        painter->fillRect(startPos+2, 2, (length*percent/100)+1, height()-3, Qt::blue);

    }
    percent_list.clear();
    painter->end();
}

void NandBanner::reset_background()
{
    badblock_list.clear();
    percent_list.clear();
    update();
}
