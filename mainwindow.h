#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QPolygon>
#include <vector>
#include <math.h>
#define Pi 3.1415926535897932
#define MAXHEIGHT 4320
#define MAXWIDTH 7680
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum State{NONE,RECT,CIRCLE,POLYGON,CUBE,BEZIER};

struct Circle
{
    int x;
    int y;
    double r;
};

struct Polygon
{

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

    void DDALine(QPainter* painter, int x1, int y1, int x2, int y2);
    void BresenhamCircle(QPainter* painter, int x, int y, int r);
    void MidpointCircle2(QPainter* painter, double x0, double y0, double r);
    void Polygon(QPainter* painter, QPolygon cpolygon);

    bool ifPaintMouseClickText=false;
    QPointF mousePos;
    State state=NONE;
    vector<QRect> rects;
    vector<Circle> circles;
    vector<QPolygon> polygons;
    QPolygon currentPolygon;
    QPoint* movePoint=nullptr;
    QPolygon curPaintPolygon;
    //bool polygonMove=false;
    int rectX1;
    int rectY1;
    int circleX1;
    int circleY1;
    QColor penColor=Qt::black;
    QImage img;
};
#endif // MAINWINDOW_H
