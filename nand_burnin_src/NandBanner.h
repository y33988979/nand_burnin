#ifndef NANDBANNER_H
#define NANDBANNER_H

#include <common.h>
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QList>
#include <QWidget>

class NandBanner : public QWidget
{
    Q_OBJECT
public:
    NandBanner(QWidget *parent = 0);
    ~NandBanner();

    QPainter     *painter;
    QList<int>    badblock_list;
    QList<int>    percent_list;

    int           startPos;
    int           endPos;
    int           length;

    void reset_background();
    int  setRange(int start, int len);
protected:
    void paintEvent(QPaintEvent *);

};

#endif // NANDBANNER_H
