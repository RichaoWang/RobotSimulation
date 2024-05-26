#include "ddr6robotwidget.h"
#include <QDebug>
#include <QApplication>

DDR6RobotWidget::DDR6RobotWidget(QWidget *parent)
        : RRGLWidget(parent) {
    loadRobotModelSTLFile();
    configureModelParams();
}

DDR6RobotWidget::~DDR6RobotWidget() {

}

void DDR6RobotWidget::loadRobotModelSTLFile() {
    //模型由7个小部件组成
    mRobotModel.link0 = new STLFileLoader(":/stl/us10base.STL", 1000);
    mRobotModel.link1 = new STLFileLoader(":stl/us10shoulder.STL", 1000);
    mRobotModel.link2 = new STLFileLoader(":stl/us10upperarm.STL", 1000);
    mRobotModel.link3 = new STLFileLoader(":stl/us10forearm.STL", 1000);
    mRobotModel.link4 = new STLFileLoader(":stl/us10wrist1.STL", 1000);
    mRobotModel.link5 = new STLFileLoader(":stl/us10wrist2.STL", 1000);
    mRobotModel.link6 = new STLFileLoader(":stl/us10wrist3.STL", 1000);

    mDeskModel.link0 = new STLFileLoader(":/stl/desk.STL", 1);
}

void DDR6RobotWidget::configureModelParams() {
    //沿z轴平移
    mRobotConfig.d = {0, 127.3, 0, 0, 163.941, 115.7, 92.2};
    //绕z轴旋转角度
    mRobotConfig.JVars = {0, 0, 0, 0, 0, 0, 0};
    //沿x轴平移
    mRobotConfig.a = {0, 0, -612, -572.3, 0, 0, 0};
    //沿x轴旋转
    mRobotConfig.alpha = {0, 90, 0, 0, 90, -90, 0};

    // 默认开启网格
    mGlobalConfig = {false, false, false, false, false, false, false, false, false};
//    mGlobalConfig = {true, true, true, true, true, true, true, true, true,true};
}

void DDR6RobotWidget::drawGL() {
    /// TODO 坐标写死了 （当前ur10）

    glPushMatrix();

    if (mGlobalConfig.isDrawGrid) drawGrid();
    if (mGlobalConfig.isDrawWorldCoord) drawCoordinates();
    if (mGlobalConfig.isDrawDesk) drawGLForDesk();



    glRotatef(180, 0.0, 0.0, 1.0);

    // 基座
    setupColor(160, 160, 160);
    mRobotModel.link0->draw();

    glRotatef(180, 0.0, 0.0, 1.0);

    // 一关节
    if (mGlobalConfig.isDrawJoint1Coord) {
        drawSTLCoordinates(255, 0, 255, "J1");
    }

    setupColor(50, 50, 50);
    glTranslatef(0.0, 0.0, mRobotConfig.d[1]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[1], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[1], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[1], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link1->draw();

    glRotatef(180, 0.0, 0.0, 1.0);
    // 二关节  修改2关节的Z轴
    if (mGlobalConfig.isDrawJoint2Coord) {
        drawSTLCoordinates(0, 255, 0, "J2");
    }
    setupColor(160, 160, 160);
    glTranslatef(0.0, 0.0, mRobotConfig.d[2]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[2], 0.0, 0.0, 1.0);       // 绕z轴旋转
    glTranslatef(mRobotConfig.a[2], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[2], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link2->draw();

    // 三关节
    if (mGlobalConfig.isDrawJoint3Coord) {
        drawSTLCoordinates(0, 0, 255, "J3");
    }
    setupColor(50, 50, 50);
    glTranslatef(0.0, 0.0, mRobotConfig.d[3]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[3], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[3], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[3], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link3->draw();

    // 四关节
    if (mGlobalConfig.isDrawJoint4Coord) {
        drawSTLCoordinates(255, 255, 0, "J4");
    }
    setupColor(160, 160, 160);
    glTranslatef(0.0, 0.0, mRobotConfig.d[4]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[4], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[4], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[4], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link4->draw();

    glRotatef(180, 0.0, 1.0, 0.0);
    glTranslatef(0.0, -328, 0);
    // 五关节
    if (mGlobalConfig.isDrawJoint5Coord) {
        drawSTLCoordinates(0, 255, 255, "J5");
    }
    setupColor(50, 50, 50);
    glTranslatef(0.0, 0.0, mRobotConfig.d[5]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[5], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[5], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[5], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link5->draw();

    glRotatef(180, 0.0, 1.0, 0.0);
    // 六关节
    if (mGlobalConfig.isDrawJoint6Coord) {
        drawSTLCoordinates(255, 0, 255, "J6");
    }
    setupColor(160, 160, 160);
    glTranslatef(0.0, 0.0, mRobotConfig.d[6]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[6], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[6], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[6], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link6->draw();

//     末端坐标系
    if (mGlobalConfig.isDrawEnd) {
        drawEndCoordinates();
    }

    if (mGlobalConfig.isDrawPoseText) {
        drawPoseText();
    }

    glPopMatrix();
}

void DDR6RobotWidget::drawGLForDesk() {
    glPushMatrix();
    setupColor(105, 105, 105);
    glTranslatef(0.0, 0.0, 490); //桌子高度
    mDeskModel.link0->draw();
    glPopMatrix();
    glTranslatef(0.0, 0.0, 500); // glPushMatrix\glPopMatrix
}

//用来绘制OpenGL的窗口，只要有更新发生，这个函数就会被调用
void DDR6RobotWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         //清除屏幕和深度缓存
    glPushMatrix();
    glTranslated(0, 0, z_zoom);
    glTranslated(xTran, yTran, 0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0); //绕x轴旋转
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0); //绕y轴旋转
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0); //绕z轴旋转
    glRotated(+90.0, 1.0, 0.0, 0.0);
    drawGL();
    glPopMatrix();
}

void DDR6RobotWidget::drawEndCoordinates() {
    glPushMatrix();
    glLineWidth(1.5f);
//    setupColor(205 ,133, 0	);
    glBegin(GL_LINES);

    setupColor(255, 0, 0);
    glVertex3f(-100, 0, 0);
    glVertex3f(100, 0, 0);
    glVertex3f(90, 10, 0);
    glVertex3f(100, 0, 0);
    glVertex3f(90, -10, 0);
    glVertex3f(100, 0, 0);

    setupColor(0, 255, 0);
    glVertex3f(0, -100, 0);
    glVertex3f(0, 100, 0);
    glVertex3f(10, 90, 0);
    glVertex3f(0, 100, 0);
    glVertex3f(-10, 90, 0);
    glVertex3f(0, 100, 0);

#ifdef _DEBUG
    setupColor(0, 0, 255);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1000);
    glEnd();
#else
    setupColor(0, 0, 255);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);
    glVertex3f(10, 0, 90);
    glVertex3f(0, 0, 100);
    glVertex3f(-10, 0, 90);
    glVertex3f(0, 0, 100);
    glEnd();
#endif
    // 标签
//    qglColor(QColor::fromRgbF(0.2, 0.2, 0.2));
    renderText(100, 0, 0, "+X", QFont("helvetica", 10, QFont::Bold, true),QColor(255*0.2,255*0.2,255*0.2));
    renderText(0, 100, 0, "+Y", QFont("helvetica", 10, QFont::Bold, true),QColor(255*0.2,255*0.2,255*0.2));
    renderText(0, 0, 100, "+Z", QFont("helvetica", 10, QFont::Bold, true),QColor(255*0.2,255*0.2,255*0.2));
    glLineWidth(1.0f);
    glPopMatrix();
}


