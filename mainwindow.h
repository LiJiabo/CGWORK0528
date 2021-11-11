#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include <math.h>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum State{NONE,RECT,CIRCLE,POLYGON,CUBE,BEZIER};

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
    bool ifPaintMouseClickText=false;
    State state=NONE;
    vector<QRect> rects;
    int rectX1;
    int rectY1;
};
#endif // MAINWINDOW_H
