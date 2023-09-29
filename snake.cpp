#include<iostream>
#include<QPainter>
#include<QTime>
#include "snake.h"

Snake::Snake(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color:black;");
    leftDirection=false;
    rightDirection=true;
    upDirection=false;
    downDirection=false;
    inGame=true;
    resize(B_WIDTH,B_HEIGHT);
    loadImages();
    initGame();

}


void Snake::loadImages()
{
    head=QImage("D:\\my_project\\qt\\Snake\\img\\head.gif");

    if(head.isNull())
        exit(1);
    head=head.scaledToWidth(10);

    dot=QImage("D:\\my_project\\qt\\Snake\\img\\dot.gif");
    dot=dot.scaledToWidth(10);

    apple=QImage("D:\\my_project\\qt\\Snake\\img\\apple.gif");
    apple=apple.scaledToWidth(10);
}

void Snake::initGame()
{
    dots=3;
    for(int z=0;z<dots;z++){
        x[z]=50-z*10;
        y[z]=50;
    }
    locateAplle();

    timerId=startTimer(DELAY);

}


void Snake::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    doDrawing();
}

void Snake::doDrawing()
{
    QPainter qp(this);

    if(inGame){
        qp.drawImage(apple_x,apple_y,apple);

        for(int z=0;z<dots;z++){
            if(z==0)
                qp.drawImage(x[z],y[z],head);
            else
                qp.drawImage(x[z],y[z],dot);
        }
    }
    else
        gameOver(qp);

}

void Snake::gameOver(QPainter & qp)
{
    QString message="Game Over, man!";
    QFont font("Courier",15,QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth=fm.horizontalAdvance(message);

    qp.setPen(QColor(Qt::white));
    qp.setFont(font);

    int h= height();
    int w= width();

    qp.translate(QPoint(w/2,h/2));
    qp.drawText(-textWidth/2,0,message);

}

void Snake::checkApple()
{
    if((x[0]==apple_x)&&(y[0]==apple_y)){

        dots++;
        locateAplle();
    }

}


void Snake::move()
{
    for(int z=dots;z>0;z--){
        x[z]=x[z-1];
        y[z]=y[z-1];
    }

    if(leftDirection){
        x[0]-=DOT_SZIE;
    }
    if(rightDirection){
        x[0]+=DOT_SZIE;
    }
    if(upDirection){
        y[0]-=DOT_SZIE;
    }
    if(downDirection){
        y[0]+=DOT_SZIE;
    }
}

void Snake::checkCollision()
{
    for(int z=dots;z>0;z--){
        if((z>4)&&(x[0]==x[z])&&(y[0]==y[z]))
            inGame=false;
    }

    if(y[0]>=B_HEIGHT)
        inGame=false;
    if(y[0]<0)
        inGame=false;
    if(x[0]>=B_WIDTH)
        inGame=false;
    if(x[0]<0)
        inGame=false;

    if(!inGame){
        killTimer(timerId);
    }

}

void Snake::locateAplle()
{
    QTime time=QTime::currentTime();
    srand((uint)time.msec());

    int r=rand()%RAND_POS;
    apple_x=(r*DOT_SZIE);

    r=rand()%RAND_POS;
    apple_y=(r*DOT_SZIE);
}

void Snake::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);

    if(inGame){
        checkApple();
        checkCollision();
        move();
    }
    repaint();
}

void Snake::keyPressEvent(QKeyEvent *e)
{
    int key=e->key();

    if((key==Qt::Key_Left)&&(!rightDirection)){
        leftDirection=true;
        upDirection=false;
        downDirection=false;
    }

    if((key==Qt::Key_Right)&&(!leftDirection)){
        rightDirection=true;
        upDirection=false;
        downDirection=false;
    }

    if((key==Qt::Key_Up)&&(!downDirection)){
        upDirection=true;
        leftDirection=false;
        rightDirection=false;
    }

    if((key==Qt::Key_Down)&&(!upDirection)){
        downDirection=true;
        leftDirection=false;
        rightDirection=false;
    }

    QWidget::keyPressEvent(e);
}



