#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QLabel>
#include "robotcontrolwidget.h"
#include "ddr6robotwidget.h"
#include "fpsgetter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void showUseMessageBox();

private:
    void initMainWindow();

    void moveToCenter();

    void connectSigSlots();


private slots:

//    void on_actionHide_Control_toggled(bool arg);

private:
    Ui::MainWindow *ui;
    RobotControlWidget *controlWidget;
    DDR6RobotWidget *ddr6RobotWidget;
    FPSGetter *fpsGetter;
    QLabel *fpsVarLabel;
};

#endif // MAINWINDOW_H
