#include "frame.h"
#include "cmath"

int Frame::totalFrame = 0;

Frame::Frame(Frame *prev)
{
    firstLine = nullptr;
    firstPoint = nullptr;
    nextFrame = nullptr;
    prevFrame = prev;
    totalLines = 0;
    totalPoints = 0;
    totalFrame++;
    frameNum = totalFrame;
}
// удаление списков вершин и граней
Frame::~Frame()
{
    if (firstPoint != nullptr)
    {
        do
        {
            Point *tPoint = firstPoint;
            firstPoint = tPoint->next;
            delete tPoint;
        } while (firstPoint != nullptr);
    }
    if (firstLine != nullptr)
    {
        do
        {
            Line *tLine = firstLine;
            firstLine = tLine->next;
            delete tLine;
        } while (firstLine != nullptr);
    }
    totalFrame--;
}
//проверяет наличие точки в списке с определенной точностью
//добавляет в конец списка, если таковой нет
void Frame::addPoint(int tX, int tY)
{
    Point *temp;
    if (!pointExist(tX, tY, 18, temp))
    {
        Point *tPoint = firstPoint;
        Point *newPoint = new Point;
        newPoint->x = tX;
        newPoint->y = tY;
        newPoint->next = nullptr;

        if (firstPoint == nullptr) firstPoint = newPoint;
        else
        {
            tPoint = firstPoint;
            while (tPoint->next != nullptr)
            {
                tPoint = tPoint->next;
            }
            tPoint->next = newPoint;
        }
        totalPoints++;
     }
}
//проверяет наличие вершины в списке с определенной точностью
//и выполняет удаление её из списка
void Frame::deletePoint(int tX, int tY)
{
    if (firstPoint != nullptr)
    {

        int x, y;
        bool end = false;
        Point *tPoint = firstPoint;
        Point *prevPoint;
        do
        {
            x = tPoint->x;
            y = tPoint->y;
            if (((tX-x)*(tX-x)+(tY-y)*(tY-y)) <= 8)
            {
                    if (tPoint == firstPoint)
                    {
                        firstPoint = tPoint->next;
                        delete tPoint;
                        end = true;
                    }
                    else if (tPoint->next == nullptr)
                    {
                        delete tPoint;
                        prevPoint->next = nullptr;
                        end = true;
                    }
                    else
                    {
                        prevPoint->next = tPoint->next;
                        delete tPoint;
                        end = true;
                    }
                    totalPoints--;
            }
            prevPoint = tPoint;
            tPoint = tPoint->next;
        } while ((tPoint != nullptr) && !end);
    }
}

Point *Frame::getFirstPoint()
{
    return firstPoint;
}

Line *Frame::getFirstLine()
{
    return firstLine;
}

Frame *Frame::getNextFrame()
{
    return nextFrame;
}

Frame *Frame::getPrevFrame()
{
    return prevFrame;
}

void  Frame::setPrevFrame(Frame *prev)
{
    prevFrame = prev;
}

void  Frame::setNextFrame(Frame *prev)
{
    nextFrame = prev;
}
//проверяет наличие вершины в списке с точностью accu, temp - указатель этой вершины
bool Frame::pointExist(int tX, int tY, int accu, Point*& temp)
{
    bool res=false;
    int x, y;
    Point *tPoint = firstPoint;
    while ((tPoint != nullptr) && !res)
    {
        x = tPoint->x;
        y = tPoint->y;
        if (((tX-x)*(tX-x)+(tY-y)*(tY-y)) <= accu) res = true;
        temp = tPoint;
        tPoint = tPoint->next;
    }
    return res;
}

int Frame::getFrameNum()
{
    return frameNum;
}
//добавляет грань между вершинами (tsX, tsY) и (teX, teY)
//в конец списка, если таковой нет
void Frame::addLine(int tsX, int tsY, int teX, int teY)
{
    bool exists = false;
    Line *tLine = firstLine;
    Line *temp;

    if (tLine != nullptr)
    {
       int sx = tLine->sx;
       int sy = tLine->sy;
       int ex = tLine->ex;
       int ey = tLine->ey;
       do
        {
            if ((sx == tsX)&&(sy == tsY)&&(ex == teX)&&(ey == teY))
            {
                exists = true;
            }
            temp = tLine;
            tLine = tLine->next;
        } while ((tLine != nullptr)&&!exists);
        if (!exists)
        {
            tLine = new Line;
            tLine->sx = tsX;
            tLine->sy = tsY;
            tLine->ex = teX;
            tLine->ey = teY;
            tLine->next = nullptr;
            temp->next = tLine;
            totalLines++;
        }
    }
    else
    {
        tLine = new Line;
        tLine->sx = tsX;
        tLine->sy = tsY;
        tLine->ex = teX;
        tLine->ey = teY;
        tLine->next = nullptr;
        firstLine = tLine;
        totalLines++;
    }



}
//проверяет ( ф-ия lineExists)наличие грани в точке (tX, tY) с определенной точностью,
//и удаляет ее из списка, если такая имеется
void Frame::deleteLine(int tX, int tY)
{
    bool end = false;
    Line *temp = lineExists(tX, tY);
    if (temp != nullptr)
    {
        Line *tLine = firstLine->next;
        Line *prevLine = firstLine;
        if (temp == firstLine)
        {
            firstLine = temp->next;
            delete temp;
            end = true;
        }
        while (!end)
        {
            if (tLine == temp)
            {
                if (tLine->next == nullptr)
                {
                    delete tLine;
                    prevLine->next = nullptr;
                }
                else
                {
                    prevLine->next = tLine->next;
                    delete tLine;
                }
                end = true;
            }
            prevLine = tLine;
            tLine = tLine->next;
        }
        totalLines--;
    }
}

Line *Frame::lineExists(int tX, int tY)
{
    bool exists = false;
    Line *temp;
    if (firstLine != nullptr)
    {
        Line *tLine = firstLine;
        do
        {
            int sx = tLine->sx;
            int sy = tLine->sy;
            int ex = tLine->ex;
            int ey = tLine->ey;
            if (abs((tX-sx)*(ey-sy)-(tY-sy)*(ex-sx)) <= 500) exists = true;
            temp = tLine;
            tLine = tLine->next;
        } while((tLine != nullptr) && !exists);
    }
    if (exists) return temp;
    else return nullptr;
}
//проверяет наличие граней у вершины, возвращает 1, если вершина - начальная точка
//2 - вершина конечная точка, и 0 если граней нет.
int Frame::pointInLine(int tX, int tY, Line*& temp)
{
    int part = 0;
    Line *tLine = firstLine;
    if (firstLine == nullptr)
    {
        return part;
    }
    else
    {
        do
        {
            if((tLine->ex == tX)&&(tLine->ey == tY)) part = 2;
            else if((tLine->sx == tX)&&(tLine->sy == tY)) part = 1;
            temp = tLine;
            tLine = tLine->next;
        }   while((tLine!=nullptr)&&(part == 0));
    }
    return part;
}

int Frame::getTotalFrame()
{
    return totalFrame;
}

int Frame::getTotalPoints()
{
    return totalPoints;
}

int Frame::getTotalLines()
{
    return totalLines;
}
