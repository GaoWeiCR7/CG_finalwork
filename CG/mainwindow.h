#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define cout qDebug() << "[" <<__FILE__ <<":"<<__FUNCTION__<<":"<<__LINE__ <<"]"

#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QAction>
#include <QMouseEvent>
#include <QVector>
#include <QColor>
#include <QMenu>
#include <QPushButton>
#include <QTextStream>
#include <QRegExp>
#include <QRegExpValidator>
#include <cmath>
#include <QMessageBox>
#include <QPair>
#include <algorithm>
#include <QColorDialog>
#include "myrest.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void f_resetCanvas(int w, int h);
    void f_saveCanvas(QString name);
    void f_setColor(int r, int g, int b);
    void f_drawlineDDA(int x1, int y1, int x2, int y2,QPainter &painter);
    void f_drawlinebresenham(int x1, int y1, int x2, int y2,QPainter &painter);
    void f_drawrect(int x1, int y1, int x2, int y2, QPainter &painter);
    void f_drawpolygonDDA(int n, int x[], int y[],QPainter &painter);
    void f_drawpolygonbresenham(int n, int x[], int y[],QPainter &painter);
    void f_drawellipse(int x0, int y0, int rx, int ry,QPainter &painter);
    void f_drawcurvebezier(int n, int x[], int y[],QPainter &painter);
    void f_drawcurvebspline(int n, int x[], int y[],QPainter &painter);
    void f_translate(int id, int dx, int dy);
    void f_rotate(int id, int xr, int yr, int r);
    void f_scale(int id, int xf, int yf, float s);
    int cs_judgebit(int x, int y, int left, int top, int rignt, int bottom);
    void f_clipcs(int id, int left, int top, int rignt, int bottom);
    void f_cliplb(int id, int left, int top, int rignt, int bottom);
    void filedeal(QFile& file);
    QRect& changepoint(int id, int &flag, int &count);
    void getcontentpath(QString x);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

    void fileopen();
    void Save();
    void Reset();
    void pressline();
    void pressrect();
    void pressellipse();
    void presspolygon();
    void presscolor();
    void presscurve();
    void presstranslate();
    void pressrotate();
    void pressscale();
    void pressclip();

private:
    Ui::MainWindow *ui;
    bool leftpress;
    QPixmap pixmap;
    int presstype;
    QVector<QRect> line_ass;
    QVector<bool> clip_unshow;
    QVector<QRect> rect_ass;
    QVector<QRect> ellipse_ass;
    QVector<QVector<QPoint>> polygon_ass;
    QVector<QVector<QPoint>> curve_ass;
    QVector<int> all_ass;
    QVector<QPair<int,QColor>> id_color_ass;

    int my_width;
    int my_height;
    int my_r;
    int my_g;
    int my_b;
    QPen my_pen;
    QRect mymap;

    QRect translaterect;
    bool translate_in_rect;
    QRect translate_move_rect;

    QPoint rotatepoint;
    int rotatedegrees;

    QPoint scalepoint;
    float scaletimes;

    QRect cliprect;

    QVector<bool> DDAline;
    QVector<bool> DDApolygon;
    QVector<bool> Beziercurve;
    bool polygonfirst;
    bool polygonmovefirst;

    bool curvefirst;

    QString contentpath;
};

#endif // MAINWINDOW_H
