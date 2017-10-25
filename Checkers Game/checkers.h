#ifndef CHECKERS_H
#define CHECKERS_H


#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <time.h>

const int W= 80*8, H=80*8;


class Piece
{
public:
    Piece(bool is);
    void draw(QPainter* paint, int x, int y);
    int x,y;
    bool isRed, isKing;
    bool canMove(int startx, int starty, int endx, int endy);
    bool jump(int start, int end);
    bool jumpagain(int endlocx,int endlocy);
};

class Checkers : public QMainWindow
{
    Q_OBJECT

public:
    Checkers(QWidget *parent = 0);
    ~Checkers();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void randomAI();

};


#endif // CHECKERS_H
