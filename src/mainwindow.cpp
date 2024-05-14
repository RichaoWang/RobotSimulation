#include "mainwindow.h"
#include "../froms/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initMainWindow();
    connectSigSlots();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initMainWindow() {
    this->setWindowTitle("RobotSimulation");
    this->setWindowIcon(QIcon(":/icon/icon.ico"));

    controlWidget = new RobotControlWidget(this);
    ddr6RobotWidget = new DDR6RobotWidget(this);
    fpsGetter = new FPSGetter();

    ui->groupBox->layout()->addWidget(controlWidget);
    ui->centerGridLayout->addWidget(ddr6RobotWidget);

    auto fpsLabel = new QLabel("FPS: ", this);
    fpsVarLabel = new QLabel(this);
    ui->statusbar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    ui->statusbar->addWidget(fpsVarLabel);
    ui->statusbar->addWidget(fpsLabel);

    moveToCenter();
}

void MainWindow::moveToCenter() {
    int screenWidth = QApplication::desktop()->width();
    int screenHeight = QApplication::desktop()->height();
    int windowWidth = width();
    int windowHeight = height();
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;
    move(x, y);
}

void MainWindow::connectSigSlots() {
    connect(fpsGetter, &FPSGetter::updateFps, [=](double fps) {
        fpsVarLabel->setText(QString::number(fps));
    });

    connect(ui->actionUse, &QAction::triggered, [=] {
        showUseMessageBox();
    });

    connect(controlWidget, &RobotControlWidget::sigJoinValueChanged, ddr6RobotWidget, [=](int idx, int var) {
        ddr6RobotWidget->mRobotConfig.JVars[idx] = var;
        ddr6RobotWidget->update();
    });

    connect(controlWidget, &RobotControlWidget::sigCheckOptChanged, ddr6RobotWidget, [=](QString key, bool arg) {
        if (key == "grid") {
            ddr6RobotWidget->mGlobalConfig.isDrawGrid = arg;
        } else if (key == "baseCoord") {
            ddr6RobotWidget->mGlobalConfig.isDrawWorldCoord = arg;
        } else if (key == "jCoord") {
            ddr6RobotWidget->mGlobalConfig.isDrawJoint1Coord = arg;
            ddr6RobotWidget->mGlobalConfig.isDrawJoint2Coord = arg;
            ddr6RobotWidget->mGlobalConfig.isDrawJoint3Coord = arg;
            ddr6RobotWidget->mGlobalConfig.isDrawJoint4Coord = arg;
            ddr6RobotWidget->mGlobalConfig.isDrawJoint5Coord = arg;
            ddr6RobotWidget->mGlobalConfig.isDrawJoint6Coord = arg;
        } else if (key == "desk") {
            ddr6RobotWidget->mGlobalConfig.isDrawDesk = arg;
        } else {
            return;
        }
        ddr6RobotWidget->update();
    });
}

void MainWindow::showUseMessageBox() {
    QMessageBox::information(this, "Use", "In the robot rendering window: \n"
                                          "1.left mouse button to rotate the perspective,\n"
                                          "2.right mouse button zooms the angle of view,\n"
                                          "3.middle mouse button to move the perspective.\n\n"
                                          "In the control window: \n"
                                          "1.checkbox enable different options,\n"
                                          "2.slider controlling robot joint angles.");
}


