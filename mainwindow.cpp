#include "mainwindow.h"
#include <QtWidgets>


//создание графического интерфейса
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);

    createMenu();

    nextButton = new QPushButton("Next (D)", this);
    prevButton = new QPushButton("Prev (A)", this);
    nextShort = new QShortcut(QKeySequence("D"), this);
    prevShort = new QShortcut(QKeySequence("A"), this);

    addPointButton = new QRadioButton("add Points", this);
    movePointButton = new QRadioButton("move Points", this);
    addLineButton = new QRadioButton("add Lines", this);
    addPointButton->setChecked(true);

    QLabel *Label = new QLabel("frame:", this);
    frameLabel = new QLabel("test", this);

    drawArea = new DrawArea(this);

    QVBoxLayout *Vlayout = new QVBoxLayout;
    Vlayout->addWidget(nextButton);
    Vlayout->addWidget(prevButton);
    Vlayout->addWidget(addPointButton);
    Vlayout->addWidget(movePointButton);
    Vlayout->addWidget(addLineButton);
    Vlayout->addStretch();
    Vlayout->addWidget(Label);
    Vlayout->addWidget(frameLabel);


    HLayout = new QHBoxLayout;
    HLayout->addWidget(drawArea);
    HLayout->addLayout(Vlayout);

    widget->setLayout(HLayout);
    frameLabel->setNum(drawArea->getCurFrameNum());
    frameLabel->setAlignment(Qt::AlignCenter);

    connection();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenu()
{
    actionClose = new QAction("Close", this);
    connect(actionClose, SIGNAL(triggered()), this, SLOT(close()));

    actionOpen = new QAction("&Open", this);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));

    actionSave = new QAction("&Save", this);
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));

    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(actionOpen);
    fileMenu->addAction(actionSave);
    fileMenu->addSeparator();
    fileMenu->addAction(actionClose);

}
//сохраняет фреймы в бинарный файл (формат:
//кол-во фреймов,[кол-во вершин, координаты вершин,кол-во граней, координаты граней](для каждого фрейма)
void MainWindow::save()
{
    QString filename = QFileDialog::getSaveFileName(this, "save", QString(), "animation (*.ani)");
    if (!filename.isEmpty())
    {
        QFile file(filename);
        if (!file.open(QFile::WriteOnly))
        {
            QMessageBox::critical(this, "error", "could not save file");
            return;
        }
        else
        {
            QDataStream stream(&file);
            int frameNum = drawArea->getCurrentFrame()->getTotalFrame();
            stream << frameNum;
            Frame *frame = drawArea->getFirstFrame();
            for(int i=0; i<frameNum; i++)
            {
                int pointNum = frame->getTotalPoints();
                stream << pointNum;
                Point *tPoint = frame->getFirstPoint();
                for (int i=0; i<pointNum; i++)
                {
                    stream << tPoint->x << tPoint->y;
                    tPoint = tPoint->next;
                }
                int lineNum = frame->getTotalLines();
                stream << lineNum;
                Line *tLine = frame->getFirstLine();
                for (int i=0; i<lineNum; i++)
                {
                    stream << tLine->sx << tLine->sy << tLine->ex << tLine->ey;
                    tLine = tLine->next;
                }
                frame = frame->getNextFrame();
            }

            file.close();
        }
    }
}
// удаляет текущие фреймы и инициализирует фреймы по бинарному файлу, если файл не пустой
void MainWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", QString(), "animation (*.ani)");
    if (!filename.isEmpty())
    {
        QFile file(filename);
        if (!file.open(QFile::ReadOnly))
        {
            QMessageBox::critical(this, "error", "could not open file");
            return;
        }
        else
        {
            QDataStream stream(&file);
            int frameNum;
            stream >> frameNum;
            if (frameNum == 0)
            {
                QMessageBox::critical(this, "error", "empty file");
                return;
            }
            else
            {
                delete drawArea;
                drawArea = new DrawArea(this);
                HLayout->insertWidget(0, drawArea);
                connection();
                for(int i=0; i<frameNum; i++)
                {
                    int pointNum;
                    stream >> pointNum;
                    for (int i=0; i<pointNum; i++)
                    {
                        int x, y;
                        stream >> x >> y;
                        drawArea->getCurrentFrame()->addPoint(x, y);
                    }
                    int lineNum;
                    stream >> lineNum;
                    for (int i=0; i<lineNum; i++)
                    {
                        int sx , sy, ex, ey;
                        stream >> sx >> sy >> ex >> ey;
                        drawArea->getCurrentFrame()->addLine(sx, sy, ex ,ey);
                    }
                    if (i != (frameNum-1)) drawArea->setNextFrame();
                }
            }

            file.close();
        }
    }
}

void MainWindow::changeLabel()
{
    frameLabel->setNum(drawArea->getCurFrameNum());
}

void MainWindow::connection()
{
    connect(nextButton, SIGNAL(clicked()), drawArea, SLOT(setNextFrame()));
    connect(prevButton, SIGNAL(clicked()), drawArea, SLOT(setPrevFrame()));
    connect(prevShort, SIGNAL(activated()), drawArea, SLOT(setPrevFrame()));
    connect(nextShort, SIGNAL(activated()), drawArea, SLOT(setNextFrame()));
    connect(drawArea, SIGNAL(frameChanged()), this, SLOT(changeLabel()));
    connect(addPointButton, SIGNAL(pressed()), drawArea, SLOT(addPoints()));
    connect(movePointButton, SIGNAL(pressed()), drawArea, SLOT(movePoints()));
    connect(addLineButton, SIGNAL(pressed()), drawArea, SLOT(addLines()));
}
