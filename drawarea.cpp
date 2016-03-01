#include "drawarea.h"
#include <QWidget>


DrawArea::DrawArea(QWidget *parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(256, 256);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Window, Qt::white);
    setAutoFillBackground(true);
    setPalette(Pal);
    setFrameStyle(Box | Sunken);
    currentFrame = new Frame;
    firstFrame = currentFrame;
    moving = false;

    act = addPoint;
}

DrawArea::~DrawArea()
{
    if (currentFrame != nullptr)
    {
        while (currentFrame->getPrevFrame() != nullptr)
        {
            currentFrame = currentFrame->getPrevFrame();
        }
        do
        {
            Frame *temp = currentFrame;
            currentFrame = currentFrame->getNextFrame();
            delete temp;
        } while (currentFrame != nullptr);
    }
}
//в зависимости от типа действия (определяемого кнопками) выполняет:
//режим добавления/удаления вершин - левый клик добавляет вершину
//правый удаляет
//----
//режим перетаскивания - при нажатии левой кнопки мыши запоминается точка
//(если она етсь в списке), при отпускании левой кнопки точка удаляется
//и добавляется новая, при этом меняются и грани
//----
//режим добавления/удаления граней - левый клик обозначает первую вершину
//второй клик левой кнопкой добавляет грань, а правой сбрасывает первую точку
//правый клик по грани удаляет её
void DrawArea::mousePressEvent(QMouseEvent *event)
{
    switch (act)
    {
    case addPoint:
        if (event->button() == Qt::LeftButton)
        {
            currentFrame->addPoint(event->x(), event->y());
            this->update();
        }
        if (event->button() == Qt::RightButton)
        {
            currentFrame->deletePoint(event->x(), event->y());
            this->update();
        }
        break;
    case movePoint:
        if (event->button() == Qt::LeftButton)
        {
            Point *temp;
            if (currentFrame->pointExist(event->x(), event->y(), 8, temp))
            {
                startPoint = temp;
                QPixmap pixmap (5, 5);
                pixmap.fill(Qt::blue);
                QCursor cursor(pixmap);
                this->setCursor(cursor);
                this->update();
                moving = true;
            }
        }
        break;
    case addLine:
        if (event->button() == Qt::LeftButton)
        {
            if (moving)
            {
                Point *secondPoint;
                if (currentFrame->pointExist(event->x(), event->y(), 8, secondPoint))
                {
                    if (startPoint != secondPoint)
                    {
                        int sx = startPoint->x;
                        int sy = startPoint->y;
                        int ex = secondPoint->x;
                        int ey = secondPoint->y;
                        currentFrame->addLine(sx, sy, ex, ey);
                        moving = false;
                        this->update();
                        startPoint = nullptr;
                    }
                }
            }
            else
            {
                if (currentFrame->pointExist(event->x(), event->y(), 8, startPoint))
                {
                    moving = true;
                    this->update();
                }
            }
        }
        else if (event->button() == Qt::RightButton)
        {
            if (moving)
            {
                moving =  false;
                startPoint = nullptr;
                this->update();
            }
            else
            {
                currentFrame->deleteLine(event->x(), event->y());
                this->update();
            }
        }
    }

}

void DrawArea::mouseReleaseEvent(QMouseEvent *event)
{

    if ((event->button() == Qt::LeftButton) && (act == movePoint) && moving)
        {
            Point *tPoint;
            if (!(currentFrame->pointExist(event->x(), event->y(), 18, tPoint)))
            {
                bool end = false;
                int t;
                Line *temp;
                while (!end)
                {
                    end = true;
                    t = currentFrame->pointInLine(startPoint->x, startPoint->y, temp);
                    if (t == 1)
                    {
                        temp->sx = event->x();
                        temp->sy = event->y();
                        end = false;
                    }
                    else if (t == 2)
                    {
                        temp->ex = event->x();
                        temp->ey = event->y();
                        end = false;
                    }
                }
                currentFrame->deletePoint(startPoint->x, startPoint->y);
                currentFrame->addPoint(event->x(), event->y());
                this->update();
                this->unsetCursor();
                moving = false;
                startPoint = nullptr;
            }
        }
}

//рисует на виджете, проходя по спискам граней и вершин.
void DrawArea::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black, 5);
    QPen penl(Qt::black, 2);
    Point *tPoint = currentFrame->getFirstPoint();
    Line *tLine = currentFrame->getFirstLine();
    if (tLine != nullptr)
    {
        painter.setPen(penl);
        do
        {

            int sx = tLine->sx;
            int sy = tLine->sy;
            int ex = tLine->ex;
            int ey = tLine->ey;
            painter.drawLine(sx, sy, ex, ey);
            tLine = tLine->next;
        } while (tLine != nullptr);
    }
    if (tPoint != nullptr)
    {
        painter.setPen(pen);
        do
        {
             if (tPoint == startPoint)
             {
                 pen.setColor(Qt::red);
                 painter.setPen(pen);
             }
             painter.drawPoint(tPoint->x,tPoint->y);
             if (tPoint == startPoint)
             {
                 pen.setColor(Qt::black);
                 painter.setPen(pen);
             }
             tPoint = tPoint->next;
        } while (tPoint != nullptr);
    }
}

//меняет фрейм на следующий (при необходимости создает новый, посылает сигнал об изменении фрейма
void DrawArea::setNextFrame()
{
    if (currentFrame != nullptr)
    {
        if (currentFrame->getNextFrame() == nullptr)
        {
            currentFrame = new Frame(currentFrame);
            (currentFrame->getPrevFrame())->setNextFrame(currentFrame);
        }
        else
        {
            currentFrame = currentFrame->getNextFrame();
        }
    }
    else
    {
        currentFrame = new Frame;
    }
    emit frameChanged();
    this->update();
}

//меняет фрейм на предыдущий
void DrawArea::setPrevFrame()
{
    if (currentFrame->getPrevFrame() != nullptr)
    {
        currentFrame = currentFrame->getPrevFrame();
        emit frameChanged();
    }
    this->update();
}


Frame *DrawArea::getCurrentFrame()
{
    return currentFrame;
}

int DrawArea::getCurFrameNum()
{
    return currentFrame->getFrameNum();
}

void DrawArea::addPoints()
{
   act = addPoint;
}

void DrawArea::movePoints()
{
   act = movePoint;
   startPoint = nullptr;
}

void DrawArea::addLines()
{
   act = addLine;
}

Frame *DrawArea::getFirstFrame()
{
    return firstFrame;
}
