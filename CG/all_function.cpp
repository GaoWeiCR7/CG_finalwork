#include "mainwindow.h"

void MainWindow::f_resetCanvas(int w, int h)
{
    my_width = w;
    my_height = h;
}

void MainWindow::f_saveCanvas(QString name)
{
    QPixmap pixmap(my_width,my_height+55);
    QPainter p(&pixmap);
    p.fillRect(QRect(0, 0, my_width, my_height+55), Qt::white);
    this->render(&p);
    QString res = contentpath+name+".bmp";
    pixmap.copy(QRect(0,55,my_width,my_height)).save(res);
}

void MainWindow::f_setColor(int r, int g, int b)
{
    my_r = r;
    my_g = g;
    my_b = b;
}

void MainWindow::f_drawlineDDA(int x1, int y1, int x2, int y2,QPainter &painter)
{
    int length = 0;
    if(abs(x2-x1) >= abs(y2-y1))
        length = abs(x2-x1);
    else
        length = abs(y2-y1);

    float dx = (float)(x2-x1)/length;
    float dy = (float)(y2-y1)/length;

    float x = x1;
    float y = y1;

    for(int i = 0; i <= length; ++i)
    {
        painter.drawPoint((int)round(x),(int)round(y));
        x = x + dx;
        y = y + dy;
    }
}

void MainWindow::f_drawlinebresenham(int x1, int y1, int x2, int y2,QPainter &painter)
{
    int x = x1;
    int y = y1;
    int dx = abs(x2-x1);
    int dy = abs(y2-y1);
    int signx = 0;
    int signy = 0;

    if(x2 > x1)
        signx = 1;
    else
        signx = -1;

    if(y2 > y1)
        signy = 1;
    else
        signy = -1;

    if(dx >= dy)
    {
        int p = 2*dy-dx;
        for(int i = 0; i <= dx; ++i)
        {
            painter.drawPoint(x,y);
            x = x + signx;
            if(p >= 0)
            {
                y = y + signy;
                p = p -2*dx;
            }
            p = p + 2*dy;
        }
    }
    else
    {
        int p = 2*dx-dy;
        for(int i = 0; i < dy; ++i)
        {
            painter.drawPoint(x,y);
            y = y+signy;
            if(p >= 0)
            {
                x = x + signx;
                p = p - 2*dy;
            }
            p = p + 2*dx;
        }
    }
}

void MainWindow::f_drawellipse(int x0, int y0, int rx, int ry,QPainter &painter)
{
    int rx2 = rx*rx;
    int ry2 = ry*ry;
    float d = ry2 + (float)rx2*(0.25-ry);

    int x = 0;
    int y = ry;

    painter.drawPoint(x0,y0+ry);
    painter.drawPoint(x0,y0-ry);

    while(ry2*x <rx2*y)
    {
        if(d < 0)
            d = d + ry2*(2*x + 3);
        else
        {
            d = d + ry2*(2*x + 3)+rx2*(2-2*y);
            y--;
        }
        x++;
        painter.drawPoint(x0+x,y0+y);
        painter.drawPoint(x0+x,y0-y);
        painter.drawPoint(x0-x,y0+y);
        painter.drawPoint(x0-x,y0-y);
    }
    d = (float)ry2*(x+0.5)*(x+0.5)+rx2*(y-1)*(y-1)-rx2*ry2;
    while(y > 0)
    {
        if(d > 0)
            d = d + rx2*(3-2*y);
        else
        {
            d = d + rx2*(3-2*y) + ry2*(2*x+2);
            x++;
        }
        y--;
        painter.drawPoint(x0+x,y0+y);
        painter.drawPoint(x0+x,y0-y);
        painter.drawPoint(x0-x,y0+y);
        painter.drawPoint(x0-x,y0-y);
    }
}

void MainWindow::f_drawrect(int x1, int y1, int x2, int y2, QPainter &painter)
{
    int lengthx = abs(x2 - x1);
    int lengthy = abs(y2 - y1);

    int signx = -1;
    int signy = -1;

    if(x2 - x1 >= 0)
        signx = 1;
    if(y2 - y1 >= 0)
        signy = 1;

    for(int i = 0; i <= lengthx; ++i)
    {
        painter.drawPoint(x1+i*signx,y1);
        painter.drawPoint(x1+i*signx,y2);
    }
    for(int i = 0; i <= lengthy; ++i)
    {
        painter.drawPoint(x1,y1+i*signy);
        painter.drawPoint(x2,y1+i*signy);
    }
}

void MainWindow::f_drawpolygonDDA(int n, int x[], int y[],QPainter &painter)
{
    for(int i = 0; i < n - 1; ++i)
        f_drawlineDDA(x[i],y[i],x[i+1],y[i+1],painter);
}

void MainWindow::f_drawpolygonbresenham(int n, int x[], int y[],QPainter &painter)
{
    for(int i = 0; i < n - 1; ++i)
        f_drawlinebresenham(x[i],y[i],x[i+1],y[i+1],painter);
}

QRect& MainWindow::changepoint(int id, int &flag, int &count)
{
    int c = 0;
    if(id == -1)
        c = id_color_ass.size() - 1;
    else
    {
        for(;c <id_color_ass.size(); ++c)
            if(id_color_ass[c].first == id)
                break;
    }
    for(int i = 0; i < c; ++i)
        if(all_ass[i] == all_ass[c])
            count++;
    if(all_ass[c] == 1)
    {
        flag = 1;
        return line_ass[count];
    }
    else if (all_ass[c] == 2)
    {
        flag = 2;
        return rect_ass[count];
    }
    else if (all_ass[c] == 3)
    {
        flag = 3;
        return ellipse_ass[count];
    }
    else if (all_ass[c] == 4)
    {
        flag = 4;
        return mymap;
    }
    else if(all_ass[c] == 5)
    {
        flag = 5;
        return mymap;
    }
}

void MainWindow::f_translate(int id, int dx, int dy)
{
    int flag = 0;
    int count = 0;
    QRect &p = changepoint(id,flag,count);
    if(flag == 1||flag == 2||flag == 3)
    {
        int x0 = p.topLeft().x();
        int y0 = p.topLeft().y();
        int x1 = p.bottomRight().x();
        int y1 = p.bottomRight().y();
        p.setTopLeft(QPoint(x0+dx,y0+dy));
        p.setBottomRight(QPoint(x1+dx,y1+dy));
    }
    else if (flag == 4)
        for(int i = 0; i < polygon_ass[count].size(); ++i)
        {
            int x0 = polygon_ass[count][i].x();
            int y0 = polygon_ass[count][i].y();
            polygon_ass[count][i].setX(x0+dx);
            polygon_ass[count][i].setY(y0+dy);
        }
    else if(flag == 5)
        for(int i = 0; i < curve_ass[count].size(); ++i)
        {
            int x0 = curve_ass[count][i].x();
            int y0 = curve_ass[count][i].y();
            curve_ass[count][i].setX(x0+dx);
            curve_ass[count][i].setY(y0+dy);
        }
}

void MainWindow::f_rotate(int id, int xr, int yr, int r)
{
    int flag = 0;
    int count = 0;
    QRect &p = changepoint(id,flag,count);
    if(flag == 1)
    {
        int x0 = p.topLeft().x();
        int y0 = p.topLeft().y();
        int x1 = p.bottomRight().x();
        int y1 = p.bottomRight().y();
        int final_x0 = round(xr + (x0-xr)*cos(r*M_PI/180.0)-(y0-yr)*sin(r*M_PI/180.0));
        int final_y0 = round(yr + (x0-xr)*sin(r*M_PI/180.0)+(y0-yr)*cos(r*M_PI/180.0));
        int final_x1 = round(xr + (x1-xr)*cos(r*M_PI/180.0)-(y1-yr)*sin(r*M_PI/180.0));
        int final_y1 = round(yr + (x1-xr)*sin(r*M_PI/180.0)+(y1-yr)*cos(r*M_PI/180.0));
        p.setTopLeft(QPoint(final_x0,final_y0));
        p.setBottomRight(QPoint(final_x1,final_y1));
    }
    else if (flag == 4)
    {
        for(int i = 0; i < polygon_ass[count].size(); ++i)
        {
            int x0 = polygon_ass[count][i].x();
            int y0 = polygon_ass[count][i].y();
            int final_x0 = round(xr + (x0-xr)*cos(r*M_PI/180)-(y0-yr)*sin(r*M_PI/180));
            int final_y0 = round(yr + (x0-xr)*sin(r*M_PI/180)+(y0-yr)*cos(r*M_PI/180));
            polygon_ass[count][i].setX(final_x0);
            polygon_ass[count][i].setY(final_y0);
        }
    }
    else if(flag == 5)
    {
        for(int i = 0; i < curve_ass[count].size(); ++i)
        {
            int x0 = curve_ass[count][i].x();
            int y0 = curve_ass[count][i].y();
            int final_x0 = round(xr + (x0-xr)*cos(r*M_PI/180)-(y0-yr)*sin(r*M_PI/180));
            int final_y0 = round(yr + (x0-xr)*sin(r*M_PI/180)+(y0-yr)*cos(r*M_PI/180));
            curve_ass[count][i].setX(final_x0);
            curve_ass[count][i].setY(final_y0);
        }
    }
}

void MainWindow::f_scale(int id, int xf, int yf, float s)
{
    int flag = 0;
    int count = 0;
    QRect &p = changepoint(id,flag,count);
    if(flag == 1||flag == 2||flag == 3)
    {
        int x0 = p.topLeft().x();
        int y0 = p.topLeft().y();
        int x1 = p.bottomRight().x();
        int y1 = p.bottomRight().y();
        x0 = round(xf +(x0-xf)*s);
        y0 = round(yf +(y0-yf)*s);
        x1 = round(xf +(x1-xf)*s);
        y1 = round(yf +(y1-yf)*s);
        p.setTopLeft(QPoint(x0,y0));
        p.setBottomRight(QPoint(x1,y1));
    }
    else if(flag == 4)
    {
        for(int i = 0; i < polygon_ass[count].size(); ++i)
        {
            int x0 = polygon_ass[count][i].x();
            int y0 = polygon_ass[count][i].y();
            x0 = round(xf +(x0-xf)*s);
            y0 = round(yf +(y0-yf)*s);
            polygon_ass[count][i].setX(x0);
            polygon_ass[count][i].setY(y0);
        }
    }
    else if(flag == 5)
    {
        for(int i = 0; i < curve_ass[count].size(); ++i)
        {
            int x0 = curve_ass[count][i].x();
            int y0 = curve_ass[count][i].y();
            x0 = round(xf +(x0-xf)*s);
            y0 = round(yf +(y0-yf)*s);
            curve_ass[count][i].setX(x0);
            curve_ass[count][i].setY(y0);
        }
    }
}

int MainWindow::cs_judgebit(int x, int y, int x1, int y1, int x2, int y2)
{
    int res = 0;
    if(x < x1)
        res = res|1;
    if(x > x2)
        res = res|2;
    if(y > y2)
        res = res|4;
    if(y < y1)
        res = res|8;
    return res;
}

void MainWindow::f_clipcs(int id, int cx1, int cy1, int cx2, int cy2)
{
    int left = qMin(cx1,cx2);
    int right = qMax(cx1,cx2);
    int top = qMin(cy1,cy2);
    int bottom = qMax(cy1,cy2);
    int flag = 0;
    int count = 0;
    QRect& p = changepoint(id,flag,count);
    if(flag == 1)
    {
        int outres = 0;//裁剪区域外的点
        int xs = 0, ys = 0;//交点
        int x1 = p.topLeft().x();
        int y1 = p.topLeft().y();
        int x2 = p.bottomRight().x();
        int y2 = p.bottomRight().y();
        //cout << x1 << ' ' << y1  << ' ' << x2 << ' '<< y2 << endl;
        int res1 = cs_judgebit(x1,y1,left,top,right,bottom);
        int res2 = cs_judgebit(x2,y2,left,top,right,bottom);
        while(res1!=0||res2!=0)
        {
            if((res1&res2)!=0)
            {
                clip_unshow[count] = true;
                break;
            }
            if(res1!=0)
                outres = res1;
            else
                outres = res2;
            if((outres&1)!= 0)
            {
                xs = left;
                ys = y1 + (y2-y1)*(left-x1)/(x2-x1);
            }
            else if ((outres&2)!=0)
            {
                xs = right;
                ys = y1 + (y2-y1)*(right-x1)/(x2-x1);
            }
            else if((outres&4)!=0)
            {
                ys = bottom;
                xs = x1 + (x2-x1)*(bottom-y1)/(y2-y1);
            }
            else if((outres&8)!=0)
            {
                ys = top;
                xs = x1 + (x2-x1)*(top-y1)/(y2-y1);
            }
            if(outres == res1)
            {
                x1 = xs;
                y1 = ys;
                res1 = cs_judgebit(x1,y1,left,top,right,bottom);
            }
            else
            {
                x2 = xs;
                y2 = ys;
                res2 = cs_judgebit(x2,y2,left,top,right,bottom);
            }
        }
        p.setTopLeft(QPoint(x1,y1));
        p.setBottomRight(QPoint(x2,y2));
    }
}

void MainWindow::f_cliplb(int id, int cx1, int cy1, int cx2, int cy2)
{
    int left = qMin(cx1,cx2);
    int right = qMax(cx1,cx2);
    int top = qMin(cy1,cy2);
    int bottom = qMax(cy1,cy2);
    int flag = 0;
    int count = 0;
    QRect &p = changepoint(id,flag,count);

    if(flag == 1)
    {
        int x1 = p.topLeft().x();
        int y1 = p.topLeft().y();
        int x2 = p.bottomRight().x();
        int y2 = p.bottomRight().y();
        float u1 = 0;
        float u2 = 1;
        int final_x1 = x1;
        int final_y1 = y1;
        int final_x2 = x2;
        int final_y2 = y2;

        if(x1 == x2)
        {
            if(x1<left||x1>right)
                clip_unshow[count] = true;
            else
            {
                final_x1 = x1;
                final_x2 = x1;
                final_y1 = qMax(top,qMin(y1,y2));
                final_y2 = qMin(bottom,qMax(y1,y2));
            }
        }
        else if (y1 == y2)
        {
            if(y1<top||y1>bottom)
                clip_unshow[count] = true;
            else
            {
                final_y1 = y1;
                final_y2 = y1;
                final_x1 = qMax(left,qMin(x1,x2));
                final_x2 = qMin(right,qMax(x1,x2));
            }
        }
        else
        {
            float p[4];
            float q[4];
            float r[4];
            p[0] = x1 - x2;
            p[1] = x2 - x1;
            p[2] = y1 - y2;
            p[3] = y2 - y1;
            q[0] = x1 - left;
            q[1] = right - x1;
            q[2] = y1 - top;
            q[3] = bottom - y1;
            r[0] = q[0]/p[0];
            r[1] = q[1]/p[1];
            r[2] = q[2]/p[2];
            r[3] = q[3]/p[3];
            if(p[0] < 0)
            {
                if(p[2] < 0)
                {
                    u1 = qMax(float(0),qMax(r[0],r[2]));
                    u2 = qMin(float(1),qMin(r[1],r[3]));
                }
                else
                {
                    u1 = qMax(float(0),qMax(r[0],r[3]));
                    u2 = qMin(float(1),qMin(r[1],r[2]));
                }
            }
            else
            {
                if(p[2] < 0)
                {
                    u1 = qMax(float(0),qMax(r[1],r[2]));
                    u2 = qMin(float(1),qMin(r[0],r[3]));
                }
                else
                {
                    u1 = qMax(float(0),qMax(r[1],r[3]));
                    u2 = qMin(float(1),qMin(r[0],r[2]));
                }
            }
            if(u1 > u2)
                clip_unshow[count] = true;
            else
            {
                final_x1 = round(x1 + u1*(x2-x1));
                final_y1 = round(y1 + u1*(y2-y1));
                final_x2 = round(x1 + u2*(x2-x1));
                final_y2 = round(y1 + u2*(y2-y1));
            }
        }
    p.setTopLeft(QPoint(final_x1,final_y1));
    p.setBottomRight(QPoint(final_x2,final_y2));
    }
}

void MainWindow::filedeal(QFile& file)
{
    bool isok = file.open(QIODevice::ReadOnly);
    if(isok == true)
    {
        QTextStream f(&file);
        QString tempstr;
        QRegExp rx[15];

        rx[0].setPattern("resetCanvas\\s\\d{3,4}\\s\\d{3,4}");
        rx[1].setPattern("saveCanvas\\s\\S+");
        rx[2].setPattern("setColor\\s\\d{1,3}\\s\\d{1,3}\\s\\d{1,3}");
        rx[3].setPattern("drawLine\\s\\d{1,}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}\\sDDA");
        rx[4].setPattern("drawLine\\s\\d{1,}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}\\sBresenham");
        rx[5].setPattern("drawPolygon\\s\\d{1,}\\s\\d{1,4}\\sDDA(\\s\\d{1,4})+");
        rx[6].setPattern("drawPolygon\\s\\d{1,}\\s\\d{1,4}\\sBresenham(\\s\\d{1,4})+");
        rx[7].setPattern("drawEllipse\\s\\d{1,}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}");
        rx[8].setPattern("drawCurve\\s\\d{1,}\\s\\d{1,4}\\sBezier(\\s\\d{1,4})+");
        rx[9].setPattern("drawCurve\\s\\d{1,}\\s\\d{1,4}\\sB-spline(\\s\\d{1,4})+");
        rx[10].setPattern("translate\\s\\d{1,}\\s-?\\d{1,4}\\s-?\\d{1,4}");
        rx[11].setPattern("rotate\\s\\d{1,}\\s\\d{1,4}\\s\\d{1,4}\\s-?\\d{1,}");
        rx[12].setPattern("scale\\s\\d{1,}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,}.\\d*");
        rx[13].setPattern("clip\\s\\d{1,}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}\\sCohen-Sutherland");
        rx[14].setPattern("clip\\s\\d{1,}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}\\s\\d{1,4}\\sLiang-Barsky");

        QRegExpValidator v[15];
        for(int i = 0; i < 15; ++i)
            v[i].setRegExp(rx[i]);

        int pos = 0;
        int t = -1;

        while(!f.atEnd())
        {
            tempstr = f.readLine();
            if(tempstr.mid(0,11) == "drawPolygon"||tempstr.mid(0,9) == "drawCurve")
            {
                QString tempstr2 = f.readLine();
                tempstr = tempstr + ' ' + tempstr2;
            }
            int c = 0;
            for(c = 0; c < 15; ++c)
            {
                t = v[c].validate(tempstr,pos);
                if(t == 2)
                    break;
            }
            QStringList list = tempstr.split(" ");
            if(c == 0)
            {
                f_resetCanvas(list[1].toInt(),list[2].toInt());
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
            else  if(c == 1)
            {
                f_saveCanvas(list[1]);
            }
            else if(c == 2)
            {
                f_setColor(list[1].toInt(),list[2].toInt(),list[3].toInt());
            }
            else if(c == 3)
            {
                QRect temp;
                line_ass.append(temp);
                QRect &templine = line_ass.last();
                templine.setTopLeft(QPoint(list[2].toInt(),list[3].toInt()));
                templine.setBottomRight(QPoint(list[4].toInt(),list[5].toInt()));
                clip_unshow.append(false);
                all_ass.append(1);
                id_color_ass.append(qMakePair<int,QColor>(list[1].toInt(),QColor(my_r,my_g,my_b)));
                DDAline.append(true);
                update();
            }
            else if(c == 4)
            {
                QRect temp;
                line_ass.append(temp);
                QRect &templine = line_ass.last();
                templine.setTopLeft(QPoint(list[2].toInt(),list[3].toInt()));
                templine.setBottomRight(QPoint(list[4].toInt(),list[5].toInt()));
                clip_unshow.append(false);
                all_ass.append(1);
                id_color_ass.append(qMakePair<int,QColor>(list[1].toInt(),QColor(my_r,my_g,my_b)));
                DDAline.append(false);
                update();
            }
            else if(c == 5||c == 6)
            {
                int n = list[2].toInt();
                int *x = new int[n + 1];
                int *y = new int[n + 1];
                for(int i = 0; i < n; ++i)
                {
                    x[i] = list[2*i+4].toInt();
                    y[i] = list[2*i+5].toInt();
                }
                x[n] = list[4].toInt();
                y[n] = list[5].toInt();
                QVector<QPoint> temp;
                polygon_ass.append(temp);
                if(c == 5)
                    DDApolygon.append(true);
                else
                    DDApolygon.append(false);
                QVector<QPoint> &temppolygon = polygon_ass.last();
                for(int i = 0; i < n + 1; ++i)
                    temppolygon.append(QPoint(x[i],y[i]));
                delete [] x;
                delete [] y;
                all_ass.append(4);
                id_color_ass.append(qMakePair<int,QColor>(list[1].toInt(),QColor(my_r,my_g,my_b)));
                update();
            }
            else if(c == 7)
            {
                QRect temp;
                ellipse_ass.append(temp);
                QRect &tempellipse = ellipse_ass.last();
                tempellipse.setTopLeft(QPoint(list[2].toInt()-list[4].toInt(),list[3].toInt()-list[5].toInt()));
                tempellipse.setBottomRight(QPoint(list[2].toInt()+list[4].toInt(),list[3].toInt()+list[5].toInt()));
                all_ass.append(3);
                id_color_ass.append(qMakePair<int,QColor>(list[1].toInt(),QColor(my_r,my_g,my_b)));
                update();
            }
            else if(c == 8||c == 9)
            {
                int n = list[2].toInt();
                int *x = new int[n];
                int *y = new int[n];
                for(int i = 0; i < n; ++i)
                {
                    x[i] = list[2*i+4].toInt();
                    y[i] = list[2*i+5].toInt();
                }
                QVector<QPoint> temp;
                curve_ass.append(temp);
                if(c == 8)
                    Beziercurve.append(true);
                else
                    Beziercurve.append(false);
                QVector<QPoint> &tempcurve = curve_ass.last();
                for(int i = 0; i < n; ++i)
                    tempcurve.append(QPoint(x[i],y[i]));
                delete [] x;
                delete [] y;
                all_ass.append(5);
                id_color_ass.append(qMakePair<int,QColor>(list[1].toInt(),QColor(my_r,my_g,my_b)));
                update();
            }
            else if(c == 10)
            {
                int id = list[1].toInt();
                int dx = list[2].toInt();
                int dy = list[3].toInt();
                f_translate(id,dx,dy);
                update();
            }
            else if(c == 11)
            {
                int id = list[1].toInt();
                int x = list[2].toInt();
                int y = list[3].toInt();
                int r = list[4].toInt();
                f_rotate(id,x,y,r);
                update();
            }
            else if(c == 12)
            {
                int id = list[1].toInt();
                int x = list[2].toInt();
                int y = list[3].toInt();
                float s = list[4].toFloat();
                f_scale(id,x,y,s);
                update();
            }
            else if(c == 13)
            {
                int id = list[1].toInt();
                int x1 = list[2].toInt();
                int y1 = list[3].toInt();
                int x2 = list[4].toInt();
                int y2 = list[5].toInt();
                f_clipcs(id,x1,y1,x2,y2);
                update();
            }
            else if(c == 14)
            {
                int id = list[1].toInt();
                int x1 = list[2].toInt();
                int y1 = list[3].toInt();
                int x2 = list[4].toInt();
                int y2 = list[5].toInt();
                f_cliplb(id,x1,y1,x2,y2);
                update();
            }
        }
    }
}

void MainWindow::getcontentpath(QString x)
{
    contentpath = x + '\\';
}

void MainWindow::f_drawcurvebezier(int n, int x[], int y[],QPainter &painter)
{
    int **px = new int*[n];
    int **py = new int*[n];
    for(int i = 0; i < n; ++i)
    {
        px[i] = new int[n];
        py[i] = new int[n];
    }
    for(int i = 0; i < n; ++i)
        for(int j = 0; j <n; ++j)
        {
            px[i][j] = 0;
            py[i][j] = 0;
        }
    for(int i = 0; i < n; ++i)
    {
        px[0][i] = x[i];
        py[0][i] = y[i];
    }
    double t = 0;
    int lastx = -1;
    int lasty = -1;
    int currentx = -1;
    int currenty = -1;
    while(t <= 1)
    {
        for(int k = 1; k < n; ++k)
            for(int i = 0; i < n - k; ++i)
            {
                px[k][i] = round((1-t)*px[k-1][i] + t*px[k-1][i+1]);
                py[k][i] = round((1-t)*py[k-1][i] + t*py[k-1][i+1]);
            }
        if(lastx == -1&&lasty == -1)
        {
            lastx = px[n-1][0];
            lasty = py[n-1][0];
        }
        currentx = px[n-1][0];
        currenty = py[n-1][0];
        f_drawlinebresenham(lastx,lasty,currentx,currenty,painter);
        lastx = currentx;
        lasty = currenty;
        t += 0.004;
    }
    for(int i = 0; i < n; ++i)
    {
        delete []px[i];
        delete []py[i];
    }
    delete []px;
    delete []py;
}

void MainWindow::f_drawcurvebspline(int n, int x[], int y[],QPainter &painter)
{
    int num_u = n + 4;
    double *u = new double[num_u];
    u[0] = 0;
    u[1] = 0;
    u[2] = 0;
    u[num_u-1] = 1;
    u[num_u-2] = 1;
    u[num_u-3] = 1;
    double du = 0.5;
    if(num_u!=7)
        du = 1.0/(num_u-7);
    for(int i = 3; i < num_u-3; ++i)
        u[i] = (i-3)*du;
    double **b = new double*[4];
    for(int i = 0; i < 4; ++i)
        b[i] = new double[num_u - 1];
    double t = 0;
    int lastx = -1;
    int lasty = -1;
    int currentx = -1;
    int currenty = -1;
    while(t <= 1)
    {
        for(int i = 0; i < num_u - 1; ++i)
        {
            if(t>=u[i]&&t<u[i+1])
                b[0][i] = 1;
            else
                b[0][i] = 0;
        }
        for(int k = 1; k < 4; ++k)
            for(int i = 0; i < num_u - 1; ++i)
            {
                double t1 = 0;
                double t2 = 0;
                if(u[i+k]-u[i] != 0)
                    t1 = (t-u[i])*b[k-1][i]/(u[i+k]-u[i]);
                if(u[i+k+1]-u[i+1] != 0)
                    t2 = (u[i+k+1]-t)*b[k-1][i+1]/(u[i+k+1]-u[i+1]);
                b[k][i] = t1 + t2;
            }
        int px = 0;
        int py = 0;
        for(int i = 0; i < n; ++i)
        {
            px += round(x[i]*b[3][i]);
            py += round(y[i]*b[3][i]);
        }
        if(lastx == -1&&lasty == -1)
        {
            lastx = px;
            lasty = py;
        }
        currentx = px;
        currenty = py;
        f_drawlinebresenham(lastx,lasty,currentx,currenty,painter);
        lastx = currentx;
        lasty = currenty;
        t += 0.004;
    }
    for(int i = 0; i < 4; ++i)
        delete []b[i];
    delete []b;
    delete []u;
}
