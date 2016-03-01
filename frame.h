#ifndef FRAME_H
#define FRAME_H

struct Point            //представляет вершину в списке
{
    int x;
    int y;
    Point *next;
};

struct Line             //представляет грань в списке
{
    int sx;
    int sy;
    int ex;
    int ey;
    Line *next;
};

//класс фрейма

class Frame
{
public:
    Frame(Frame *prev = nullptr);
    ~Frame();
    void addPoint(int tX, int tY);
    void addLine(int tsX, int tsY, int teX, int teY);
    void deleteLine(int tX, int tY);
    void deletePoint(int tX, int tY);
    Point *getFirstPoint();
    Line *getFirstLine();
    Frame *getNextFrame();
    Frame *getPrevFrame();
    void setPrevFrame(Frame *prev);
    void setNextFrame(Frame *prev);
    int getFrameNum();
    bool pointExist(int tX, int tY, int accu, Point*& temp);//проверяет наличие вершины в списке с точностью accu, temp - указатель на предыдущую вершину
    Line *lineExists(int tX, int tY);                       //проверяет наличие грани в списке
    int pointInLine(int tX, int tY, Line*& temp);
    int getTotalFrame();
    int getTotalPoints();
    int getTotalLines();
private:
    static int totalFrame;
    int totalPoints;
    int totalLines;
    int frameNum;
    Point *firstPoint;
    Line *firstLine;
    Frame *nextFrame;
    Frame *prevFrame;
};

#endif // FRAME_H
