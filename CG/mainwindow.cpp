#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    translate_in_rect = false;
    contentpath = "../";
    leftpress = false;
    presstype = 0;
    my_width = 600;
    my_height = 400;
    polygonfirst = true;
    polygonmovefirst = true;
    curvefirst = true;
    my_r = 0;
    my_g = 0;
    my_b = 0;
    ui->setupUi(this);
    this->setWindowTitle("画板");
    this->setWindowIcon(QIcon(":/myicon/images/painter.PNG"));
    this->setGeometry(350,200,my_width,my_height+55);
    setMouseTracking(true);
    QPalette palt(this->palette());
    palt.setColor(QPalette::Background, QColor(196,214,255));
    this->setAutoFillBackground(true);
    this->setPalette(palt);
    QToolBar *tbar = addToolBar(tr("工具栏"));
    tbar->setMovable(false);
    tbar->setIconSize(QSize(16, 16));
    tbar->setStyleSheet("background-color:rgb(200,240,200)");
    connect(ui->action_o,&QAction::triggered,this,&MainWindow::fileopen);
    connect(ui->action_s,&QAction::triggered,this,&MainWindow::Save);
    connect(ui->action_r,&QAction::triggered,this,&MainWindow::Reset);


    QAction *lineAction = new QAction(tr("&直线"), this);
    lineAction->setIcon(QIcon(":/myicon/images/line.png"));
    tbar->addAction(lineAction);

    QAction *rectAction = new QAction(tr("&矩形"), this);
    rectAction->setIcon(QIcon(":/myicon/images/rect.png"));
    tbar->addAction(rectAction);

    QAction *ellipseAction = new QAction(tr("&椭圆"), this);
    ellipseAction->setIcon(QIcon(":/myicon/images/ellipse.png"));
    tbar->addAction(ellipseAction);

    QAction *polygonAction = new QAction(tr("&多边形"), this);
    polygonAction->setIcon(QIcon(":/myicon/images/polygon.PNG"));
    tbar->addAction(polygonAction);

    QAction *curveAction = new QAction(tr("&曲线"), this);
    curveAction->setIcon(QIcon(":/myicon/images/curve.PNG"));
    tbar->addAction(curveAction);

    QAction *changecolorAction = new QAction(tr("&颜色"),this);
    changecolorAction->setIcon(QIcon(":/myicon/images/color.png"));
    tbar->addAction(changecolorAction);

    QAction *translateAction = new QAction(tr("&平移"),this);
    translateAction->setIcon(QIcon(":/myicon/images/t.PNG"));
    tbar->addAction(translateAction);

    QAction *rotateAction = new QAction(tr("&旋转"),this);
    rotateAction->setIcon(QIcon(":/myicon/images/r.PNG"));
    tbar->addAction(rotateAction);

    QAction *scaleAction = new QAction(tr("&缩放"),this);
    scaleAction->setIcon(QIcon(":/myicon/images/s.PNG"));
    tbar->addAction(scaleAction);

    QAction *clipAction = new QAction(tr("&裁剪"),this);
    clipAction->setIcon(QIcon(":/myicon/images/c.PNG"));
    tbar->addAction(clipAction);

    connect(lineAction,&QAction::triggered,this,&MainWindow::pressline);
    connect(rectAction,&QAction::triggered,this,&MainWindow::pressrect);
    connect(ellipseAction,&QAction::triggered,this,&MainWindow::pressellipse);
    connect(polygonAction,&QAction::triggered,this,&MainWindow::presspolygon);
    connect(changecolorAction,&QAction::triggered,this,&MainWindow::presscolor);
    connect(curveAction,&QAction::triggered,this,&MainWindow::presscurve);
    connect(translateAction,&QAction::triggered,this,&MainWindow::presstranslate);
    connect(rotateAction,&QAction::triggered,this,&MainWindow::pressrotate);
    connect(scaleAction,&QAction::triggered,this,&MainWindow::pressscale);
    connect(clipAction,&QAction::triggered,this,&MainWindow::pressclip);

    mymap.setX(0);
    mymap.setY(0);
    mymap.setHeight(my_height);
    mymap.setWidth(my_width);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    pixmap = QPixmap(my_width,my_height);
    pixmap.fill(Qt::white);
    QPixmap canvaspix = pixmap;
    QPainter p(&canvaspix);

    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    int n4 = 0;
    int n5 = 0;
    for(int i = 0; i < all_ass.size();++i)
    {
        my_pen.setColor(id_color_ass[i].second);
        my_pen.setStyle(Qt::SolidLine);
        p.setPen(my_pen);
        if(all_ass.at(i) == 1)
        {
            if(clip_unshow[n1] == false)
            {
                if(DDAline[n1])
                    f_drawlineDDA(line_ass.at(n1).topLeft().x(),line_ass.at(n1).topLeft().y(),line_ass.at(n1).bottomRight().x(),line_ass.at(n1).bottomRight().y(),p);
                else
                    f_drawlinebresenham(line_ass.at(n1).topLeft().x(),line_ass.at(n1).topLeft().y(),line_ass.at(n1).bottomRight().x(),line_ass.at(n1).bottomRight().y(),p);
            }
            n1++;
        }
        else if(all_ass.at(i) == 2)
        {
            f_drawrect(rect_ass.at(n2).topLeft().x(),rect_ass.at(n2).topLeft().y(),rect_ass.at(n2).bottomRight().x(),rect_ass.at(n2).bottomRight().y(),p);
            n2++;
        }
        else if(all_ass.at(i) == 3)
        {
            int x0 = round((ellipse_ass.at(n3).topLeft().x() + ellipse_ass.at(n3).bottomRight().x())/2.0);
            int y0 = round((ellipse_ass.at(n3).topLeft().y() + ellipse_ass.at(n3).bottomRight().y())/2.0);

            int rx = abs((int)round((ellipse_ass.at(n3).topLeft().x() - ellipse_ass.at(n3).bottomRight().x())/2.0));
            int ry = abs((int)round((ellipse_ass.at(n3).topLeft().y() - ellipse_ass.at(n3).bottomRight().y())/2.0));
            f_drawellipse(x0,y0,rx,ry,p);
            n3++;
        }
        else if(all_ass.at(i) == 4)
        {
            int polygonsize = polygon_ass.at(n4).size();
            int *x = new int[polygonsize];
            int *y = new int[polygonsize];
            for(int i = 0; i < polygonsize; ++i)
            {
                x[i] = polygon_ass.at(n4).at(i).x();
                y[i] = polygon_ass.at(n4).at(i).y();
            }
            if(DDApolygon[n4])
                f_drawpolygonDDA(polygonsize,x,y,p);
            else
                f_drawpolygonbresenham(polygonsize,x,y,p);
            delete [] x;
            delete [] y;
            n4++;
        }
        else if(all_ass.at(i) == 5)
        {
            int curvesize = curve_ass.at(n5).size();
            int *x = new int[curvesize];
            int *y = new int[curvesize];
            for(int i = 0; i < curvesize; ++i)
            {
                x[i] = curve_ass.at(n5).at(i).x();
                y[i] = curve_ass.at(n5).at(i).y();
            }
            if(Beziercurve[n5])
                f_drawcurvebezier(curvesize,x,y,p);
            else
                f_drawcurvebspline(curvesize,x,y,p);
            delete []x;
            delete []y;
            n5++;
        }
    }
    if(presstype == 6)
    {
        my_pen.setStyle(Qt::DashDotLine);
        my_pen.setColor(Qt::black);
        p.setPen(my_pen);
        p.drawRect(translaterect);
    }
    else if(presstype == 7)
    {
        my_pen.setWidth(5);
        my_pen.setColor(Qt::red);
        p.setPen(my_pen);
        p.drawPoint(rotatepoint);
        my_pen.setWidth(1);
    }
    else if(presstype == 8)
    {
        my_pen.setWidth(5);
        my_pen.setColor(Qt::blue);
        p.setPen(my_pen);
        p.drawPoint(scalepoint);
        my_pen.setWidth(1);
    }
    else if(presstype == 9)
    {
        my_pen.setStyle(Qt::DashDotLine);
        my_pen.setColor(Qt::black);
        p.setPen(my_pen);
        p.drawRect(cliprect);
    }
    p.end();
    p.begin(this);
    p.drawPixmap(0,55,canvaspix);
}

void MainWindow::fileopen()
{
    QString path = QFileDialog::getOpenFileName(this,"open","../","TXT(*.txt)");
    if(path.isEmpty() == false)
    {
        QFile file(path);
        filedeal(file);
    }
}

void MainWindow::Save()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("保存"),"../","BMP(*.bmp)");
    QPixmap pixmap(my_width,my_height+55);
    QPainter p(&pixmap);
    p.fillRect(QRect(0, 0, my_width, my_height+55), Qt::white);
    this->render(&p);
    pixmap.copy(QRect(0,55,my_width,my_height)).save(filename);
}

void MainWindow::Reset()
{
    myrest r;
    if(r.exec() == QDialog::Accepted)
    {
        my_height = r.height;
        my_width = r.width;
        this->setGeometry(350,200,my_width,my_height+55);
        line_ass.clear();
        clip_unshow.clear();
        rect_ass.clear();
        ellipse_ass.clear();
        polygon_ass.clear();
        curve_ass.clear();
        all_ass.clear();
        id_color_ass.clear();

        DDAline.clear();
        DDApolygon.clear();
        Beziercurve.clear();
    }
}

void MainWindow::pressline()
{
    presstype = 1;
}

void MainWindow::pressrect()
{
    presstype = 2;
}

void MainWindow::pressellipse()
{
    presstype = 3;
}

void MainWindow::presspolygon()
{
    presstype = 4;
    if(!polygonfirst)
        polygonfirst = true;
    if(!polygonmovefirst)
        polygonmovefirst = true;
}

void MainWindow::presscurve()
{
    presstype = 5;
    polygonfirst = true;
    polygonmovefirst = true;
    if(!curvefirst)
        curvefirst = true;
}

void MainWindow::presscolor()
{
    presstype = -1;
    QColor color = QColorDialog::getColor();
    if(color.isValid())
    {
        my_r = color.red();
        my_g = color.green();
        my_b = color.blue();
    }
}

void MainWindow::presstranslate()
{
    presstype = 6;
    int type = all_ass.last();
    if(type == 1)
        translaterect = line_ass.last();
    else if(type == 2)
        translaterect = rect_ass.last();
    else if(type == 3)
        translaterect = ellipse_ass.last();
    else if(type == 4)
    {
        int minx = 100000;
        int miny = 100000;
        int maxx = -1;
        int maxy = -1;
        for(int i = 0; i < polygon_ass.last().size(); ++i)
        {
            if(minx > polygon_ass.last()[i].x())
                minx = polygon_ass.last()[i].x();
            if(miny > polygon_ass.last()[i].y())
                miny = polygon_ass.last()[i].y();
            if(maxx < polygon_ass.last()[i].x())
                maxx = polygon_ass.last()[i].x();
            if(maxy < polygon_ass.last()[i].y())
                maxy = polygon_ass.last()[i].y();
        }
        translaterect.setTopLeft(QPoint(minx,miny));
        translaterect.setBottomRight(QPoint(maxx,maxy));
    }
    else if(type == 5)
    {
        int minx = 100000;
        int miny = 100000;
        int maxx = -1;
        int maxy = -1;
        for(int i = 0; i < curve_ass.last().size(); ++i)
        {
            if(minx > curve_ass.last()[i].x())
                minx = curve_ass.last()[i].x();
            if(miny > curve_ass.last()[i].y())
                miny = curve_ass.last()[i].y();
            if(maxx < curve_ass.last()[i].x())
                maxx = curve_ass.last()[i].x();
            if(maxy < curve_ass.last()[i].y())
                maxy = curve_ass.last()[i].y();
        }
        translaterect.setTopLeft(QPoint(minx,miny));
        translaterect.setBottomRight(QPoint(maxx,maxy));
    }
    update();
}

void MainWindow::pressrotate()
{
    presstype = 7;
    int type = all_ass.last();
    if(type == 1)
        rotatepoint = line_ass.last().bottomRight();
    else if(type == 2)
        rotatepoint = rect_ass.last().bottomRight();
    else if(type == 3)
        rotatepoint = ellipse_ass.last().bottomRight();
    else if(type == 4)
        rotatepoint = polygon_ass.last()[0];
    else if(type == 5)
        rotatepoint = curve_ass.last()[0];
    update();
}

void MainWindow::pressscale()
{
    presstype = 8;
    int type = all_ass.last();
    if(type == 1)
        scalepoint = line_ass.last().bottomRight();
    else if(type == 2)
        scalepoint = rect_ass.last().bottomRight();
    else if(type == 3)
        scalepoint = ellipse_ass.last().bottomRight();
    else if(type == 4)
        scalepoint = polygon_ass.last()[0];
    else if(type == 5)
        scalepoint = curve_ass.last()[0];
    update();
}

void MainWindow::pressclip()
{
    presstype = 9;
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(presstype == 1)
        {
            leftpress = true;
            QRect temp;
            line_ass.append(temp);
            DDAline.append(false);
            QRect &templine = line_ass.last();
            templine.setTopLeft(QPoint(event->pos().x(),event->pos().y()-55));
            clip_unshow.append(false);
            all_ass.append(1);
            id_color_ass.append(qMakePair<int,QColor>(-1,QColor(my_r,my_g,my_b)));
        }
        else if(presstype == 2)
        {
            leftpress = true;
            QRect temp;
            rect_ass.append(temp);
            QRect &temprect = rect_ass.last();
            temprect.setTopLeft(QPoint(event->pos().x(),event->pos().y()-55));
            all_ass.append(2);
            id_color_ass.append(qMakePair<int,QColor>(-1,QColor(my_r,my_g,my_b)));
        }
        else if(presstype == 3)
        {
            leftpress = true;
            QRect temp;
            ellipse_ass.append(temp);
            QRect &tempellipse = ellipse_ass.last();
            tempellipse.setTopLeft(QPoint(event->pos().x(),event->pos().y()-55));
            all_ass.append(3);
            id_color_ass.append(qMakePair<int,QColor>(-1,QColor(my_r,my_g,my_b)));
        }
        else if(presstype == 4)
        {
            leftpress = true;
            if(polygonfirst)
            {
                QVector<QPoint> temp;
                polygon_ass.append(temp);
                DDApolygon.append(false);
                all_ass.append(4);
                id_color_ass.append(qMakePair<int,QColor>(-1,QColor(my_r,my_g,my_b)));
            }
            QVector<QPoint> &temppolygon = polygon_ass.last();
            temppolygon.append(QPoint(event->pos().x(),event->pos().y()-55));
            if(polygonmovefirst == false)
            {
                int poldx = abs(QPoint(event->pos().x(),event->pos().y()-55).x()-temppolygon.first().x());
                int poldy = abs(QPoint(event->pos().x(),event->pos().y()-55).y()-temppolygon.first().y());
                if(poldx<=2&&poldy<=2)
                {
                    leftpress = false;
                    polygonfirst = true;
                    polygonmovefirst = true;
                }
            }
            update();
        }
        else if(presstype == 5)
        {
            leftpress = true;
            if(curvefirst)
            {
                QVector<QPoint> temp;
                curve_ass.append(temp);
                Beziercurve.append(true);
                all_ass.append(5);
                id_color_ass.append(qMakePair<int,QColor>(-1,QColor(my_r,my_g,my_b)));
                curvefirst = false;
            }
            QVector<QPoint> &tempcurve = curve_ass.last();
            tempcurve.append(QPoint(event->pos().x(),event->pos().y()-55));
            update();
        }
        else if(presstype == 6)
        {
            leftpress = true;
            QPoint tpoint(event->pos().x(),event->pos().y()-55);
            int minx = qMin(translaterect.topLeft().x(),translaterect.bottomRight().x());
            int maxx = qMax(translaterect.topLeft().x(),translaterect.bottomRight().x());
            int miny = qMin(translaterect.topLeft().y(),translaterect.bottomRight().y());
            int maxy = qMax(translaterect.topLeft().y(),translaterect.bottomRight().y());
            if(tpoint.x() >= minx
                    &&tpoint.x() <= maxx
                    &&tpoint.y() >= miny
                    &&tpoint.y() <= maxy)
            {
                translate_in_rect = true;
                translate_move_rect.setTopLeft(QPoint(event->pos().x(),event->pos().y()-55));
            }
        }
        else if(presstype == 7)
        {
            leftpress = true;
            rotatepoint = QPoint(event->pos().x(),event->pos().y()-55);
            update();
        }
        else if(presstype == 8)
        {
            leftpress = true;
            scalepoint = QPoint(event->pos().x(),event->pos().y()-55);
            update();
        }
        else if(presstype == 9)
        {
            leftpress = true;
            cliprect.setTopLeft(QPoint(event->pos().x(),event->pos().y()-55));
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(leftpress)
    {
        if(presstype == 1)
        {
            QRect &templine = line_ass.last();
            templine.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
            update();
        }
        else if(presstype == 2)
        {
            QRect &temprect = rect_ass.last();
            temprect.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
            update();
        }
        else if(presstype == 3)
        {
            QRect &tempellipse = ellipse_ass.last();
            tempellipse.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
            update();
        }
        else if(presstype == 4)
        {
            if(polygonfirst)
            {
                QVector<QPoint> &temppolygon = polygon_ass.last();
                if(polygonmovefirst)
                {
                    temppolygon.append(QPoint(event->pos().x(),event->pos().y()-55));
                    polygonmovefirst = false;
                }
                else
                {
                    QPoint &temppoint = temppolygon.last();
                    temppoint = QPoint(event->pos().x(),event->pos().y()-55);
                }
                update();
            }
            else
            {
                QVector<QPoint> &temppolygon = polygon_ass.last();
                QPoint &temppoint = temppolygon.last();
                temppoint = QPoint(event->pos().x(),event->pos().y()-55);
                update();
            }
        }
        else if(presstype == 5)
        {
            QVector<QPoint> &tempcurve = curve_ass.last();
            QPoint &temppoint = tempcurve.last();
            temppoint = QPoint(event->pos().x(),event->pos().y()-55);
            update();
        }
        else if(presstype == 6)
        {
            if(translate_in_rect)
            {
                translate_move_rect.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
                f_translate(-1,translate_move_rect.bottomRight().x()-translate_move_rect.topLeft().x(),translate_move_rect.bottomRight().y()-translate_move_rect.topLeft().y());
                translate_move_rect.setTopLeft(QPoint(event->pos().x(),event->pos().y()-55));
                update();
                presstranslate();
            }
        }
        else if(presstype == 7)
        {
            rotatepoint = QPoint(event->pos().x(),event->pos().y()-55);
            update();
        }
        else if(presstype == 8)
        {
            scalepoint = QPoint(event->pos().x(),event->pos().y()-55);
            update();
        }
        else if(presstype == 9)
        {
            cliprect.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
            update();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(leftpress)
    {
        if(presstype == 1)
        {
            QRect &templine = line_ass.last();
            templine.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
            leftpress = false;
        }
        else if(presstype == 2)
        {
            QRect &temprect = rect_ass.last();
            temprect.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
            leftpress = false;
        }
        else if(presstype == 3)
        {
            QRect &tempellipse = ellipse_ass.last();
            tempellipse.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
            leftpress = false;
        }
        else if(presstype == 4)
        {
            polygonfirst = false;
            QVector<QPoint> &temppolygon = polygon_ass.last();
            QPoint &temppoint = temppolygon.last();
            temppoint = QPoint(event->pos().x(),event->pos().y()-55);
            leftpress = false;
        }
        else if(presstype == 5)
        {
            QVector<QPoint> &tempcurve = curve_ass.last();
            QPoint &temppoint = tempcurve.last();
            temppoint = QPoint(event->pos().x(),event->pos().y()-55);
            leftpress = false;
        }
        else if(presstype == 6)
        {
            if(translate_in_rect)
            {
                translate_move_rect.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
                leftpress = false;
                translate_in_rect = false;
            }
        }
        else if(presstype == 7)
        {
            rotatepoint = QPoint(event->pos().x(),event->pos().y()-55);
            leftpress = false;
        }
        else if(presstype == 8)
        {
            scalepoint = QPoint(event->pos().x(),event->pos().y()-55);
            leftpress = false;
        }
        else if(presstype == 9)
        {
            cliprect.setBottomRight(QPoint(event->pos().x(),event->pos().y()-55));
            f_cliplb(-1,cliprect.topLeft().x(),cliprect.topLeft().y(),cliprect.bottomRight().x(),cliprect.bottomRight().y());
            update();
            leftpress = false;
        }
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(presstype == 7)
    {
        rotatedegrees = event->delta()/120;
        f_rotate(-1,rotatepoint.x(),rotatepoint.y(),rotatedegrees);
        update();
    }
    else if(presstype == 8)
    {
        scaletimes = 1.0+event->delta()/1200.0;
        f_scale(-1,scalepoint.x(),scalepoint.y(),scaletimes);
        update();
    }
}
