#ifndef MAINGUI_H
#define MAINGUI_H

#include "ui_maingui.h"
#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QColor>


namespace Ui {
    class MainGui;
}

class MainGui : public QMainWindow {
    Q_OBJECT
public:
    QGraphicsScene *lScene;
    QGraphicsScene *rScene;
    QTimer *timer;
    QImage Im;
    QImage *Imr;
    QImage Im2;
    QPixmap *pixm;
    QPixmap *pixm2;
    QGraphicsItem *item;
    QGraphicsPixmapItem *mPix;
    int count;

    MainGui(QWidget *parent = 0);
    ~MainGui();

    void start_timer();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainGui *ui;

private slots:

    void timer_update();

    int updatescreen_left();
    int updatescreen_right();

    int render_screenleft();
    int render_screenright();

};

#endif // MAINGUI_H
