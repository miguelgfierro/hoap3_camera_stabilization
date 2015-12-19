#include "maingui.h"

#include <stdio.h>
#include "vget.h"
#include "command.h"

MainGui::MainGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainGui)
{
    ui->setupUi(this);
    /* vget_init */
    vget_init();
    count = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_update()));
    lScene = new QGraphicsScene(this);
    rScene = new QGraphicsScene(this);
    lScene->setBackgroundBrush(Qt::black);
    rScene->setBackgroundBrush(Qt::black);
    //*Im = new QImage(V4L_WIDTH, V4L_HEIGHT, QImage::Format_RGB888);
    //Im.fill(0);
    Im = next_frame(1);
    
}

MainGui::~MainGui()
{
    delete ui;
}

void MainGui::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainGui::start_timer()
{
    timer->start(10);
}

void MainGui::timer_update()
{
    count++;
//    printf("%d", count);

    ui->lcdNumber->display(count);
    //updatescreen_left();
    updatescreen_right();
}     

int MainGui::updatescreen_left()
{
    //printf("left");
    Im2 = next_frame(0);
    render_screenleft();

    return(0);
}

int MainGui::updatescreen_right()
{
    printf("\n::right::\n");
    Im = next_frame(1, Im);
    //render_screenright();
    printf("\ngettingframes\n");
    return(0);
}

int MainGui::render_screenleft()
{
    lScene->clear();
    pixm2 = new QPixmap(Im2.size());
    pixm2->fromImage(Im2);

    lScene->addPixmap(*pixm2);

    ui->leftcamera->setScene(lScene);
    ui->leftcamera->show();

    return(0);
}

int MainGui::render_screenright()
{
    rScene->clear();
    pixm = new QPixmap(Im.size());
    pixm->fromImage(Im);

    rScene->addPixmap(*pixm);

    ui->rightcamera->setScene(rScene);
    ui->rightcamera->show();

    return(0);
}
