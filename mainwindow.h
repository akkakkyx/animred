#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "drawarea.h"

// класс основного окна

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void open();
    void save();
    void changeLabel();     //смена номера фрейма
private:
    QHBoxLayout *HLayout;
    DrawArea *drawArea;
    QLabel *frameLabel;     //вмджет для отображения кол-ва фреймов
    QMenu *fileMenu;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionClose;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QShortcut *nextShort;
    QShortcut *prevShort;
    QRadioButton *addPointButton;
    QRadioButton *movePointButton;
    QRadioButton *addLineButton;
    void createMenu();
    void connection();              //связывание слотов и сигналов
};

#endif // MAINWINDOW_H
