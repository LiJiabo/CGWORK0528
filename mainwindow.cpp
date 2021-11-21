#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //img.load("/Users/lijiabo/Documents/GitHub/CGWORK0528/0528.png");

    //connect menu actions
    connect(ui->actionDrawRect,&QAction::triggered,this,[=](){//绘制矩形
        this->state=RECT;
    });
    connect(ui->actionDrawCircle,&QAction::triggered,this,[=](){//绘制圆形
        this->state=CIRCLE;
    });
    connect(ui->actionSetColor,&QAction::triggered,this,[=](){//设置颜色
        penColor=QColorDialog::getColor(penColor,this,"设置颜色");
        update(rect());
    });
    connect(ui->actionDrawPolygon,&QAction::triggered,this,[=](){//绘制多边形
        this->state=POLYGON;
    });
    connect(ui->actionSetColor_2,&QAction::triggered,this,[=](){//设置颜色
        penColor=QColorDialog::getColor(penColor,this,"设置颜色");
        update(rect());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(penColor);
    //DEBUG
    painter.drawText(rect(),QString("Width: "+QString::number(rect().width())+" Height: "+QString::number(rect().height())));//显示画面大小
    //if(ifPaintMouseClickText)
        //painter.drawText(rect(),Qt::AlignCenter,QString("Mouse Pressed!"));
    //painter.drawText(rect(),Qt::AlignCenter,QString("x:"+QString::number(mousePos.x())+" y:"+QString::number(mousePos.y())));
    //
    switch(state)
    {
    case RECT:
    case CIRCLE:
        for(QRect rect:rects)
        {
            DDALine(&painter,rect.topLeft().x(),rect.topLeft().y(),rect.topRight().x(),rect.topRight().y());//top
            DDALine(&painter,rect.bottomLeft().x(),rect.bottomLeft().y(),rect.bottomRight().x(),rect.bottomRight().y());//bottom
            DDALine(&painter,rect.topLeft().x(),rect.topLeft().y(),rect.bottomLeft().x(),rect.bottomLeft().y());//left
            DDALine(&painter,rect.topRight().x(),rect.topRight().y(),rect.bottomRight().x(),rect.bottomRight().y());//right
        }
        for(Circle c:circles)
            BresenhamCircle(&painter,c.x,c.y,(int)c.r);
        break;
    case POLYGON:
        if(currentPolygon!=nullptr&&movePoint!=nullptr)
        {
            curPaintPolygon=*currentPolygon;
            curPaintPolygon.append(*movePoint);
            Polygon(&painter,&curPaintPolygon);
        }
        if(polygons.size()>0)
        {
            for(const QPolygon* polygon:polygons)
            {
                Polygon(&painter,polygon);
            }
        }
        break;
    case CUBE:
        break;
    case BEZIER:
        break;
    default:
        ;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //ifPaintMouseClickText=true;
    switch(state)
    {
    case RECT:
        rectX1=event->pos().x();
        rectY1=event->pos().y();
        rects.push_back(QRect(rectX1,rectY1,0,0));
        break;
    case CIRCLE:
        circleX1=event->pos().x();
        circleY1=event->pos().y();
        circles.push_back({circleX1,circleY1,1});//r=0可能会有除0问题
        break;
    case POLYGON:
        if(currentPolygon==nullptr)
        {
            currentPolygon=new QPolygon;
            currentPolygon->append(QPoint(event->pos().x(),event->pos().y()));
        }
        else
            currentPolygon->append(QPoint(event->pos().x(),event->pos().y()));
        break;
    case CUBE:
        break;
    case BEZIER:
        break;
    default:
        ;
    }

    update(rect());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //ifPaintMouseClickText=false;
    switch(state)
    {
    case RECT:
        break;
    case CIRCLE:
        break;
    case POLYGON:
        if(event->button()==Qt::LeftButton)
        {
            if(currentPolygon!=nullptr)
                polygons.push_back(currentPolygon);//?????????????????????
            currentPolygon=nullptr;
            delete movePoint;
            movePoint=nullptr;
        }
        break;
    case CUBE:
        break;
    case BEZIER:
        break;
    default:
        ;
    }
    update(rect());
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    mousePos=event->position();
    switch(state)
    {
    case RECT:
        rects.back().setCoords(rectX1,rectY1,event->pos().x(),event->pos().y());
        break;
    case CIRCLE:
        circles.back().r=sqrt((event->pos().x()-circleX1)*(event->pos().x()-circleX1)+(event->pos().y()-circleY1)*(event->pos().y()-circleY1));
        break;
    case POLYGON:
        if(movePoint==nullptr)
            movePoint=new QPoint(event->pos().x(),event->pos().y());
        else
            *movePoint=event->pos();
        break;
    case CUBE:
        break;
    case BEZIER:
        break;
    default:
        ;
    }
    update(rect());
}

void MainWindow::DDALine(QPainter* painter, int x1, int y1, int x2, int y2)
{
    double dx, dy, e, x, y;
    dx = x2 - x1;
    dy = y2 - y1;
    e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
    dx /= e;
    dy /= e;
    x = x1;
    y = y1;
    for (int i = 1; i <= e; i++)
    {
        painter->drawPoint((int)(x + 0.5), (int)(y + 0.5));
        x += dx;
        y += dy;
    }
}

void MainWindow::BresenhamCircle(QPainter* painter, int x, int y, int r)//有问题
{
    int edgeX,edgeY,p;

    edgeX=0;
    edgeY=r;
    p=3-2*r;
    for(;edgeX<=edgeY;edgeX++)
    {
        painter->drawPoint(x+edgeX,y+edgeY);//上
        double dT=0;
        for(int dTNC=1;dTNC<8;dTNC++)//画剩下7个点
        {
            dT=dTNC*0.25*Pi;
            painter->drawPoint( x + r*cos(acos(edgeX/double(r))+dT), y + r*sin(asin(edgeY/double(r))+dT) );
        }
        if(p>=0)
        {
            p+=4*(edgeX-edgeY)+10;
            edgeY--;
        }
        else
            p+=4*edgeX+6;
    }


}

void MainWindow::MidpointCircle2(QPainter *painter, double x0, double y0, double r)
{
    double x,y,dx,dy,d;
    x=0;
    y=r;
    d=1-r;
    dx=3;
    dy=2-r-r;
    painter->drawPoint(x0+x,y0+y);

    double dT=0;
    for(int dTNC=1;dTNC<8;dTNC++)//画剩下7个点
    {
        dT=dTNC*0.25*Pi;
        painter->drawPoint( x0 + r*cos(acos(x/r)+dT), y0 + r*sin(asin(y/r)+dT) );
    }

    while(x<y)
    {
        if(d<0)
        {
            d+=dx;
            dx+=2;
            x++;
        }
        else
        {
            d+=(dx+dy);
            dx+=2;
            dy+=2;
            x++;
            y--;
        }
        painter->drawPoint(x0+x,y0+y);

        for(int dTNC=1;dTNC<8;dTNC++)//画剩下7个点
        {
            dT=dTNC*0.25*Pi;
            painter->drawPoint( x0 + r*cos(acos(x/r)+dT), y0 + r*sin(asin(y/r)+dT) );
        }

    }
}

void MainWindow::Polygon(QPainter* painter, const QPolygon* cpolygon)//附带填充功能
{
    QPolygon* polygon=new QPolygon(*cpolygon);
    if(polygon==nullptr||polygon->size()<=1)
    {
        //qDebug(QString("Polygon() failed because polygon->size()=="+QString::number(polygon->size())).toStdString().c_str());
        return;
    }
    QPoint prevPoint = polygon->front();
    QPoint startPoint = polygon->front();
    polygon->erase(polygon->cbegin());
    for(QPoint point:*polygon)
    {
        DDALine(painter,prevPoint.x(),prevPoint.y(),point.x(),point.y());
        prevPoint=point;
        polygon->erase(polygon->cbegin());
    }
    DDALine(painter,prevPoint.x(),prevPoint.y(),startPoint.x(),startPoint.y());
    //填充
    //QImage img("https://oss.ljbmedia.top/uPic/2021/11/18/0528.png");
    /*
    int imgWidth=img.width();
    int imgHeight=img.height();
    int x1=rect().left();
    int x2=rect().right();
    int y1=rect().bottom();
    int y2=rect().top();
    bool mask[MAXHEIGHT][MAXWIDTH];
    for(int y=y1;y<=y2;y++)
        for(int x=x1;x<=x2;x++)
            mask[y][x]=false;
    for(QPolygon::const_iterator it=polygon->cbegin();it!=polygon->cend();it++)
    {
        int xs=it->x();
        int dxs=((it+1)->x()-it->x())/((it+1)->y()/it->y());
        int dys=abs((it+1)->y()-it->y())/((it+1)->y()-it->y());
        for(int ys=it->y();ys!=(it+1)->y();ys+=dys)
        {
            int Ixs=int(xs+0.5);
            mask[ys][Ixs]=!mask[ys][Ixs];
            xs+=dys*dxs;
        }
    }
    QPen initialPen = painter->pen();
    for(int y=y1;y<=y2;y++)
    {
        bool inside=false;
        for(int x=x1;x<=x2;x++)
        {
            if(mask[y][x])
                inside=!inside;
            if(inside)
            {
                painter->setPen(img.pixel(x%imgWidth,y%imgHeight));
                painter->drawPoint(x,y);
            }
        }
    }
    painter->setPen(initialPen);
    */
}
