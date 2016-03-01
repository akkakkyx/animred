#ifndef DRAWAREA_H
#define DRAWAREA_H\

#include <QtWidgets>
#include "frame.h"

//Класс для рисования

enum action {addPoint, movePoint, addLine}; // тип действия addPoint - добавление/удаление вершин
                                            //movePoint - перетаскивание вершины мышкой
                                            //addLine соединение вершин гранями/удаление граней

class DrawArea : public QFrame
{
    Q_OBJECT
public:
    explicit DrawArea(QWidget *parent);
    ~DrawArea();
    Frame *getCurrentFrame();
    int getCurFrameNum();
    Frame *getFirstFrame();
public slots:
    void setNextFrame();     //делает активным следующий фрейм
    void setPrevFrame();     //делает активным предыдущий фрейм
    void addPoints();        //------
    void movePoints();       //устанавливают тип действия action
    void addLines();         //------
signals:
    void frameChanged();
protected:
    void paintEvent(QPaintEvent *event);         //переопределенная функция отрисовки виджета
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    Frame *currentFrame;
    Frame *firstFrame;
    action act;
    bool moving;             //используется для отслеживания передвижения вершины
    Point *startPoint;       //используется для хранения первой точки грани
};

#endif // DRAWAREA_H
